# N-Sliding Blocks Puzzle — IDA* with Additive Pattern Databases (PDB) / MD+LC
#
# Algorithm: IDA* (Iterative Deepening A*)
#   Repeated depth-limited search with rising f-cost threshold (depth + h).
#   The threshold for each new iteration is the smallest f-cost that exceeded
#   the previous limit. Guarantees optimal solution with O(solution_length) memory.
#
# Heuristic (4×4 canonical goal): Additive 5-5-5 Disjoint Pattern Databases
#   Tiles 1-15 split into three disjoint groups of 5. A PDB for each group stores
#   the exact minimum moves to bring just those 5 tiles to their goal positions
#   (ignoring all other tiles). Because groups are disjoint, PDB_A+PDB_B+PDB_C is
#   admissible (no move counted twice) and much tighter than MD+LC alone.
#   Each PDB has 16P5 = 524160 entries; built by 0-1 BFS from goal at startup.
#
# Heuristic (all other grids / non-default blank target): MD + Linear Conflict
#   MD: sum of per-tile row+col distances to goal — admissible lower bound.
#   LC: two tiles in their goal row/col but in reversed order need ≥2 extra
#       moves to bypass each other. MD updated O(1) per move; LC recomputed
#       O(N²) after every swap — tight enough to solve hard 4×4 in seconds (C++/Java).
#
# Note: Python recursion overhead makes depths >~56 impractical (>600 s). The
#   algorithm is correct; this is a language speed limitation, not an algorithmic one.

import sys
import os
import time
from collections import deque
from enum import IntEnum

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from timed import timed  # ../timed.py (e-03/python/timed.py)

sys.setrecursionlimit(1 << 20)


class Direction(IntEnum):
    LEFT    = 0
    RIGHT   = 1
    DOWN    = 2
    UP      = 3
    NEUTRAL = 4


_REVERSE = [
    Direction.RIGHT,
    Direction.LEFT,
    Direction.UP,
    Direction.DOWN,
    Direction.NEUTRAL,
]

# Internally directions describe where the blank moves.
# _MOVE_NAMES gives where the TILE moves (opposite of blank).
_MOVE_NAMES = {
    Direction.LEFT:    "right",
    Direction.RIGHT:   "left",
    Direction.DOWN:    "up",
    Direction.UP:      "down",
    Direction.NEUTRAL: "",
}

MAX_H = 1 << 15
BLANK = 0

# PDB constants — 5-5-5 additive disjoint partition of tiles 1-15
_PDB_SIZE = 16 * 15 * 14 * 13 * 12  # 524160 — 16P5
_GROUP_A  = (1,  2,  3,  4,  5)
_GROUP_B  = (6,  7,  8,  9, 10)
_GROUP_C  = (11, 12, 13, 14, 15)
_FACTORS  = (15*14*13*12, 14*13*12, 13*12, 12, 1)


def _perm_rank_5(p):
    """Lehmer-code rank for ordered 5-tuple of positions on a 16-cell board."""
    used = 0
    rank = 0
    for i in range(5):
        pos = p[i]
        rank += (pos - bin(used & ((1 << pos) - 1)).count('1')) * _FACTORS[i]
        used |= (1 << pos)
    return rank


def _build_pdb(group):
    """0-1 BFS in abstract state space for one tile group.

    State = (perm_rank_of_5_positions * 16 + blank_pos).
    Cost-0: blank slides through non-group tile.
    Cost-1: group tile moves.
    Returns projected pdb[rank] = min cost over all blank positions.
    """
    BFS_SIZE = _PDB_SIZE * 16
    bfs = bytearray(b'\xff' * BFS_SIZE)

    goal_pos = [t - 1 for t in group]
    start_state = _perm_rank_5(goal_pos) * 16 + 15
    bfs[start_state] = 0

    dq = deque()
    dq.append(start_state)

    off4 = (-1, +1, +4, -4)  # LEFT, RIGHT, DOWN, UP

    while dq:
        state = dq.popleft()
        blank = state & 15
        rank  = state >> 4
        cost  = bfs[state]

        # Decode tile positions from Lehmer rank
        pos   = [0] * 5
        rem   = rank
        avail = 0xFFFF
        for i in range(5):
            quot = rem // _FACTORS[i]
            rem  = rem %  _FACTORS[i]
            cnt  = 0
            bit  = 0
            while bit < 16:
                if avail & (1 << bit):
                    if cnt == quot:
                        break
                    cnt += 1
                bit += 1
            pos[i]  = bit
            avail  &= ~(1 << bit)

        group_at = [-1] * 16
        for i in range(5):
            group_at[pos[i]] = i

        for d in range(4):
            nb = blank + off4[d]
            if nb < 0 or nb >= 16:
                continue
            if d == 0 and blank % 4 == 0:  # LEFT from col 0
                continue
            if d == 1 and blank % 4 == 3:  # RIGHT from col 3
                continue

            gi = group_at[nb]
            if gi >= 0:
                new_pos       = pos[:]
                new_pos[gi]   = blank
                new_rank      = _perm_rank_5(new_pos)
                new_cost      = cost + 1
            else:
                new_rank  = rank
                new_cost  = cost

            new_state = new_rank * 16 + nb
            old = bfs[new_state]
            if old == 0xFF or old > new_cost:
                bfs[new_state] = new_cost
                if new_cost == cost:
                    dq.appendleft(new_state)
                else:
                    dq.append(new_state)

    # Project: pdb[rank] = min cost over all blank positions
    pdb = bytearray(b'\xff' * _PDB_SIZE)
    for rank in range(_PDB_SIZE):
        base = rank * 16
        best = min(bfs[base + b] for b in range(16))
        pdb[rank] = best
    return pdb


class SlidingBlocksPuzzle:

    def __init__(self, source):
        it = iter(source.read().split())
        self._grid_size    = int(next(it))
        target_index       = int(next(it))
        self._tiles_count  = self._grid_size ** 2 - 1
        self._blank_target = self._tiles_count if target_index < 0 else target_index

        size = self._grid_size ** 2
        self._board    = [0] * size
        self._goal_row = [0] * size
        self._goal_col = [0] * size
        self._blank_curr = 0

        for i in range(size):
            self._board[i] = int(next(it))
            if self._board[i] == BLANK:
                self._blank_curr = i

        self._offset = self._build_offsets()
        self._init_goal_state()
        self._history = deque()

        self._use_pdb = (self._grid_size == 4 and self._blank_target == 15)
        self._pdb_a = self._pdb_b = self._pdb_c = None
        self._tile_pos = [0] * 16  # only used when _use_pdb

    # -----------------------------------------------------------------------
    # Initialisation
    # -----------------------------------------------------------------------

    def _build_offsets(self):
        g = self._grid_size
        return {
            Direction.LEFT:    -1,
            Direction.RIGHT:   +1,
            Direction.DOWN:    +g,
            Direction.UP:      -g,
            Direction.NEUTRAL:  0,
        }

    # Tile t occupies reading-order position (t-1) before blank_target, else t.
    def _init_goal_state(self):
        bt = self._blank_target
        g  = self._grid_size
        for t in range(1, self._tiles_count + 1):
            pos = (t - 1) if t <= bt else t
            self._goal_row[t] = pos // g
            self._goal_col[t] = pos % g
        self._goal_row[BLANK] = bt // g
        self._goal_col[BLANK] = bt % g

    # -----------------------------------------------------------------------
    # PDB setup
    # -----------------------------------------------------------------------

    def _build_pdbs(self):
        self._pdb_a = _build_pdb(_GROUP_A)
        self._pdb_b = _build_pdb(_GROUP_B)
        self._pdb_c = _build_pdb(_GROUP_C)

    def _init_tile_pos(self):
        for i, t in enumerate(self._board):
            self._tile_pos[t] = i

    def _pdb_lookup(self):
        tp = self._tile_pos
        pa = [tp[t] for t in _GROUP_A]
        pb = [tp[t] for t in _GROUP_B]
        pc = [tp[t] for t in _GROUP_C]
        return (self._pdb_a[_perm_rank_5(pa)]
              + self._pdb_b[_perm_rank_5(pb)]
              + self._pdb_c[_perm_rank_5(pc)])

    # -----------------------------------------------------------------------
    # Solvability — O(N² log N) merge-sort inversion count
    # -----------------------------------------------------------------------
    # Odd grid:  solvable iff #inversions is even.
    # Even grid: solvable iff (inversions + blank_row_from_bottom
    #                                      + target_row_from_bottom) is even.

    def _merge_count(self, arr, lo, hi):
        if hi - lo < 2:
            return 0
        mid = (lo + hi) // 2
        inv = self._merge_count(arr, lo, mid) + self._merge_count(arr, mid, hi)

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

    def _is_solvable(self):
        flat = [v for v in self._board if v != BLANK]
        inv  = self._merge_count(flat, 0, len(flat))
        g    = self._grid_size

        if g % 2 == 1:
            return inv % 2 == 0

        blank_row_from_bottom  = (g - 1) - (self._blank_curr   // g)
        target_row_from_bottom = (g - 1) - (self._blank_target // g)
        return (inv + blank_row_from_bottom + target_row_from_bottom) % 2 == 0

    # -----------------------------------------------------------------------
    # Heuristic: Manhattan Distance (full recompute — used at IDA* root)
    # -----------------------------------------------------------------------

    def _calc_manhattan(self):
        g = self._grid_size
        cost = 0
        for i, t in enumerate(self._board):
            if t == BLANK:
                continue
            cost += abs(i // g - self._goal_row[t]) + abs(i % g - self._goal_col[t])
        return cost

    # -----------------------------------------------------------------------
    # Heuristic: Linear Conflict (full recompute after each swap inside DLS)
    # -----------------------------------------------------------------------
    # Two tiles in their goal row with reversed goal columns → +2 per pair.
    # Same for columns.

    def _calc_linear_conflict(self):
        g = self._grid_size
        conflict = 0
        for row in range(g):
            for c1 in range(g - 1):
                t1 = self._board[row * g + c1]
                if t1 == BLANK or self._goal_row[t1] != row:
                    continue
                for c2 in range(c1 + 1, g):
                    t2 = self._board[row * g + c2]
                    if t2 == BLANK or self._goal_row[t2] != row:
                        continue
                    if self._goal_col[t1] > self._goal_col[t2]:
                        conflict += 2
        for col in range(g):
            for r1 in range(g - 1):
                t1 = self._board[r1 * g + col]
                if t1 == BLANK or self._goal_col[t1] != col:
                    continue
                for r2 in range(r1 + 1, g):
                    t2 = self._board[r2 * g + col]
                    if t2 == BLANK or self._goal_col[t2] != col:
                        continue
                    if self._goal_row[t1] > self._goal_row[t2]:
                        conflict += 2
        return conflict

    # Incremental MD update — O(1), called BEFORE the swap.
    # Only the tile sliding into blank_pos changes its distance.
    def _update_md(self, blank_pos, from_pos, md):
        t = self._board[from_pos]
        g = self._grid_size
        old_dist = abs(from_pos // g - self._goal_row[t]) + abs(from_pos % g - self._goal_col[t])
        new_dist = abs(blank_pos // g - self._goal_row[t]) + abs(blank_pos % g - self._goal_col[t])
        return md - old_dist + new_dist

    # -----------------------------------------------------------------------
    # Move validation
    # -----------------------------------------------------------------------

    def _neighbour_pos(self, blank_pos, direction):
        g  = self._grid_size
        np = blank_pos + self._offset[direction]
        if np < 0 or np >= g * g:
            return -1
        if direction == Direction.LEFT  and blank_pos % g == 0:
            return -1
        if direction == Direction.RIGHT and blank_pos % g == g - 1:
            return -1
        return np

    # -----------------------------------------------------------------------
    # IDA* search
    # -----------------------------------------------------------------------
    # h        = heuristic at this node (PDB or MD+LC).
    # md       = MD only (separate for O(1) incremental update and goal check).
    # prev_dir = direction that brought blank TO blank_pos (skip its reverse).
    #
    # On success each frame appends its own prev_dir — path is built deepest-first.
    # solve() strips the NEUTRAL sentinel and reverses for forward output.

    def _dls(self, blank_pos, limit, prev_dir, h, md, depth):
        cost = h + depth
        if cost > limit:
            return cost  # over budget: return as next-threshold hint

        if md == 0:  # all tiles at goal positions
            self._history.append(prev_dir)
            print(depth)
            return 0

        opposite = _REVERSE[prev_dir]
        min_cost = MAX_H

        for direction in (Direction.LEFT, Direction.RIGHT, Direction.DOWN, Direction.UP):
            if direction == opposite:
                continue
            np = self._neighbour_pos(blank_pos, direction)
            if np < 0:
                continue

            new_md = self._update_md(blank_pos, np, md)  # O(1) before swap

            t = self._board[np]
            self._board[blank_pos], self._board[np] = t, BLANK

            if self._use_pdb:
                self._tile_pos[t] = blank_pos
                new_h = self._pdb_lookup()
            else:
                new_h = new_md + self._calc_linear_conflict()  # full LC after swap

            result = self._dls(np, limit, direction, new_h, new_md, depth + 1)

            self._board[np], self._board[blank_pos] = self._board[blank_pos], BLANK
            if self._use_pdb:
                self._tile_pos[t] = np  # restore incremental position

            if result == 0:
                self._history.append(prev_dir)
                return 0
            if result < min_cost:
                min_cost = result  # track tightest next threshold

        return min_cost

    # IDA* outer loop: raise threshold until DLS finds the solution.
    def _ida_star(self, md, initial_h):
        threshold = initial_h
        while True:
            result = self._dls(self._blank_curr, threshold, Direction.NEUTRAL, threshold, md, 0)
            if result == 0:
                return
            threshold = result
            md = self._calc_manhattan()
            if self._use_pdb:
                threshold = max(threshold, self._pdb_lookup())
            else:
                lc = self._calc_linear_conflict()
                threshold = max(threshold, md + lc)

    # -----------------------------------------------------------------------
    # Public entry point
    # -----------------------------------------------------------------------

    @timed(label="IDA* Sliding Blocks Puzzle")
    def solve(self):
        md = self._calc_manhattan()

        if md == 0:
            print(0)
            print("\nSuccess!")
            return

        if not self._is_solvable():
            print("Puzzle cannot be solved!")
            return

        if self._use_pdb:
            self._build_pdbs()
            self._init_tile_pos()
            initial_h = self._pdb_lookup()
        else:
            initial_h = md + self._calc_linear_conflict()

        self._ida_star(md, initial_h)

        # history is built deepest-first; strip NEUTRAL sentinel then reverse
        path = [d for d in self._history if d != Direction.NEUTRAL]
        path.reverse()
        for d in path:
            print(_MOVE_NAMES[d])

        print("\nSuccess!")


if __name__ == "__main__":
    source = open(sys.argv[1]) if len(sys.argv) > 1 else sys.stdin
    SlidingBlocksPuzzle(source).solve()
