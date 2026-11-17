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

_MOVE_NAMES = {
    Direction.LEFT:    "left",
    Direction.RIGHT:   "right",
    Direction.DOWN:    "down",
    Direction.UP:      "up",
    Direction.NEUTRAL: "",
}

MAX_H = 1 << 15
BLANK = 0


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
    # Solvability
    # -----------------------------------------------------------------------

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
    # Heuristic
    # -----------------------------------------------------------------------

    def _calc_manhattan(self):
        g = self._grid_size
        cost = 0
        for i, t in enumerate(self._board):
            if t == BLANK:
                continue
            cost += abs(i // g - self._goal_row[t]) + abs(i % g - self._goal_col[t])
        return cost

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

    def _update_heuristic(self, blank_pos, from_pos, h):
        t = self._board[from_pos]
        g = self._grid_size
        old_dist = abs(from_pos // g - self._goal_row[t]) + abs(from_pos % g - self._goal_col[t])
        new_dist = abs(blank_pos // g - self._goal_row[t]) + abs(blank_pos % g - self._goal_col[t])
        return h - old_dist + new_dist

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

    def _dls(self, blank_pos, limit, prev_dir, h, depth):
        cost = h + depth
        if cost > limit:
            return cost

        if h == 0:
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

            new_h = self._update_heuristic(blank_pos, np, h)

            self._board[blank_pos], self._board[np] = self._board[np], BLANK
            result = self._dls(np, limit, direction, new_h, depth + 1)
            self._board[np], self._board[blank_pos] = self._board[blank_pos], BLANK

            if result == 0:
                self._history.append(prev_dir)
                return 0
            if result < min_cost:
                min_cost = result

        return min_cost

    def _ida_star(self, md, lc):
        threshold = md + lc
        while True:
            result = self._dls(self._blank_curr, threshold, Direction.NEUTRAL, md, 0)
            if result == 0:
                return
            threshold = result
            md = self._calc_manhattan()
            lc = self._calc_linear_conflict()
            threshold = max(threshold, md + lc)

    # -----------------------------------------------------------------------
    # Public entry point
    # -----------------------------------------------------------------------

    @timed(label="IDA* Sliding Blocks Puzzle")
    def solve(self):
        md = self._calc_manhattan()
        lc = self._calc_linear_conflict()

        if md + lc == 0:
            print(0)
            print("\nSuccess!")
            return

        if not self._is_solvable():
            print("Puzzle cannot be solved!")
            return

        self._ida_star(md, lc)

        # history is built deepest-first; strip NEUTRAL sentinel then reverse
        path = [d for d in self._history if d != Direction.NEUTRAL]
        path.reverse()
        for d in path:
            print(_MOVE_NAMES[d])

        print("\nSuccess!")


if __name__ == "__main__":
    source = open(sys.argv[1]) if len(sys.argv) > 1 else sys.stdin
    SlidingBlocksPuzzle(source).solve()
