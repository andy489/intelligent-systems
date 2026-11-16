import sys
import os
import time
from collections import deque

sys.setrecursionlimit(1 << 20)

LEFT, RIGHT, DOWN, UP, NEUTRAL = 0, 1, 2, 3, 4
MAX_H = 1 << 15

grid_size = 0
tiles_count = 0
board = []
goal_row = []
goal_col = []
blank_curr = 0
blank_target = 0
offset = [0] * 5
history = deque()


def init_offsets():
    offset[LEFT]    = -1
    offset[RIGHT]   = +1
    offset[DOWN]    = +grid_size
    offset[UP]      = -grid_size
    offset[NEUTRAL] = 0


def set_goal_state(blank_target_index):
    global blank_target
    blank_target = blank_target_index
    for t in range(1, tiles_count + 1):
        pos = (t - 1) if t <= blank_target else t
        goal_row[t] = pos // grid_size
        goal_col[t] = pos % grid_size
    goal_row[0] = blank_target // grid_size
    goal_col[0] = blank_target % grid_size


def read_input(source):
    global grid_size, tiles_count, blank_curr, board, goal_row, goal_col

    it = iter(source.read().split())
    grid_size    = int(next(it))
    target_index = int(next(it))

    tiles_count = grid_size * grid_size - 1
    blank_target_index = tiles_count if target_index < 0 else target_index

    board    = [0] * (grid_size * grid_size)
    goal_row = [0] * (grid_size * grid_size)
    goal_col = [0] * (grid_size * grid_size)

    for i in range(grid_size * grid_size):
        board[i] = int(next(it))
        if board[i] == 0:
            blank_curr = i

    init_offsets()
    set_goal_state(blank_target_index)


# ---------------------------------------------------------------------------
# Solvability check
# ---------------------------------------------------------------------------

def merge_count(arr, lo, hi):
    if hi - lo < 2:
        return 0
    mid = (lo + hi) // 2
    inv = merge_count(arr, lo, mid) + merge_count(arr, mid, hi)

    left  = arr[lo:mid]
    right = arr[mid:hi]
    i = j = 0
    k = lo
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            arr[k] = left[i]; i += 1
        else:
            arr[k] = right[j]; j += 1; inv += len(left) - i
        k += 1
    while i < len(left):
        arr[k] = left[i]; i += 1; k += 1
    while j < len(right):
        arr[k] = right[j]; j += 1; k += 1
    return inv


# Parity rule for the N-puzzle:
#   Odd grid width:  solvable iff #inversions is even.
#   Even grid width: solvable iff
#     (inversions + blank_row_from_bottom + blank_target_row_from_bottom) is even.
# Both the blank's current row AND its goal row must be counted from the bottom.
# Omitting the goal row gives wrong results when blank_target is not the default.
def check_solvable():
    flat = [board[i] for i in range(grid_size * grid_size) if board[i] != 0]
    inv = merge_count(flat, 0, len(flat))

    if grid_size % 2 == 1:
        return inv % 2 == 0

    blank_row_from_bottom  = (grid_size - 1) - (blank_curr   // grid_size)
    target_row_from_bottom = (grid_size - 1) - (blank_target // grid_size)
    return (inv + blank_row_from_bottom + target_row_from_bottom) % 2 == 0


# ---------------------------------------------------------------------------
# Heuristic
# ---------------------------------------------------------------------------

def calc_manhattan():
    cost = 0
    for i in range(grid_size * grid_size):
        t = board[i]
        if t == 0:
            continue
        cost += abs(i // grid_size - goal_row[t]) + abs(i % grid_size - goal_col[t])
    return cost


def calc_linear_conflict():
    conflict = 0

    for row in range(grid_size):
        for c1 in range(grid_size - 1):
            t1 = board[row * grid_size + c1]
            if t1 == 0 or goal_row[t1] != row:
                continue
            for c2 in range(c1 + 1, grid_size):
                t2 = board[row * grid_size + c2]
                if t2 == 0 or goal_row[t2] != row:
                    continue
                if goal_col[t1] > goal_col[t2]:
                    conflict += 2

    for col in range(grid_size):
        for r1 in range(grid_size - 1):
            t1 = board[r1 * grid_size + col]
            if t1 == 0 or goal_col[t1] != col:
                continue
            for r2 in range(r1 + 1, grid_size):
                t2 = board[r2 * grid_size + col]
                if t2 == 0 or goal_col[t2] != col:
                    continue
                if goal_row[t1] > goal_row[t2]:
                    conflict += 2

    return conflict


def update_heuristic(blank_pos, from_pos, h):
    t = board[from_pos]
    gr = goal_row[t]
    gc = goal_col[t]
    old_dist = abs(from_pos // grid_size - gr) + abs(from_pos % grid_size - gc)
    new_dist = abs(blank_pos // grid_size - gr) + abs(blank_pos % grid_size - gc)
    return h - old_dist + new_dist


# ---------------------------------------------------------------------------
# Move validation
# ---------------------------------------------------------------------------

def neighbour_pos(blank_pos, move):
    np = blank_pos + offset[move]
    if np < 0 or np >= grid_size * grid_size:
        return -1
    if move == LEFT  and blank_pos % grid_size == 0:
        return -1
    if move == RIGHT and blank_pos % grid_size == grid_size - 1:
        return -1
    return np


# REVERSE[m] gives the opposite of move m — used to avoid reversing the last move.
REVERSE = [RIGHT, LEFT, UP, DOWN, NEUTRAL]


# ---------------------------------------------------------------------------
# IDA* search
# ---------------------------------------------------------------------------

def dls(blank_pos, limit, prev_move, h, depth):
    cost = h + depth
    if cost > limit:
        return cost

    if h == 0:
        history.append(prev_move)
        print(depth)
        return 0

    opposite = REVERSE[prev_move]
    min_cost = MAX_H

    for move in range(4):
        if move == opposite:
            continue

        np = neighbour_pos(blank_pos, move)
        if np < 0:
            continue

        new_h = update_heuristic(blank_pos, np, h)

        # Swap blank with the tile at np; RHS fully evaluated before assignment.
        board[blank_pos], board[np] = board[np], 0

        result = dls(np, limit, move, new_h, depth + 1)

        board[np], board[blank_pos] = board[blank_pos], 0  # undo

        if result == 0:
            history.append(prev_move)
            return 0
        if result < min_cost:
            min_cost = result

    return min_cost


def ida_star(md_init, lc_init):
    threshold = md_init + lc_init
    while True:
        result = dls(blank_curr, threshold, NEUTRAL, md_init, 0)
        if result == 0:
            return
        threshold = result
        md_init = calc_manhattan()
        lc_init = calc_linear_conflict()
        threshold = max(threshold, md_init + lc_init)


# ---------------------------------------------------------------------------
# Output
# ---------------------------------------------------------------------------

MOVE_NAME = ["left", "right", "down", "up", ""]


def format_duration(ms):
    s   = ms // 1000; ms %= 1000
    mn  = s  // 60;   s  %= 60
    h   = mn // 60;   mn %= 60
    return f"Elapsed: {h:02d}:{mn:02d}:{s:02d}.{ms:03d}"


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    if len(sys.argv) > 1:
        with open(sys.argv[1], "r") as f:
            read_input(f)
    else:
        read_input(sys.stdin)

    md = calc_manhattan()
    lc = calc_linear_conflict()

    if md + lc == 0:
        print(0)
        print("\nSuccess!")
        sys.exit(0)

    if not check_solvable():
        print("Puzzle cannot be solved!")
        sys.exit(0)

    t0 = time.time()
    ida_star(md, lc)
    elapsed_ms = int((time.time() - t0) * 1000)

    # history contains moves appended during recursion (deepest first), ending with
    # the NEUTRAL sentinel at the top. Strip NEUTRAL and reverse to get forward order.
    path = list(history)
    path.reverse()
    for m in path:
        if m != NEUTRAL:
            print(MOVE_NAME[m])

    print()
    print(format_duration(elapsed_ms))
    print("\nSuccess!")
