# N-Queens — Min-Conflicts local search
#
# Algorithm:
#   1. Initialise: place queens in interleaved even/odd column order (1,3,5,...,0,2,4,...)
#      so each column is occupied and the initial conflict count is low.
#   2. Repair: up to 2N steps — pick the column with the most conflicts (random tie-break),
#      move its queen to the row with the fewest conflicts (random tie-break).
#   3. Restart if 2N steps did not solve; repeat until solved.
#
# Conflict tracking is O(1) per move via three incremental counter arrays:
#   queens_in_row[r]          — queens sharing row r
#   queens_in_main_diag[d]    — queens on main diagonal d = (N-1) + row - col
#   queens_in_antidiag[d]     — queens on anti-diagonal d = row + col
#
# Expected time: O(N) — constant number of repair steps on average.

import sys
import random
import time


class NQueens:

    def __init__(self, n, seed=None):
        if n in (2, 3):
            raise ValueError(f"No solution for N={n}")
        self._n = n
        self._row_of_queen     = [0] * n
        self._queens_in_row    = [0] * n
        self._queens_in_mdiag  = [0] * (2 * n - 1)
        self._queens_in_adiag  = [0] * (2 * n - 1)
        self._rng = random.Random(seed)
        self._solved = False
        self._place_queens()

    # -----------------------------------------------------------------------
    # Initialisation — interleaved column order
    # -----------------------------------------------------------------------

    def _place_queens(self):
        n = self._n
        self._queens_in_row    = [0] * n
        self._queens_in_mdiag  = [0] * (2 * n - 1)
        self._queens_in_adiag  = [0] * (2 * n - 1)
        self._solved = False
        col = 1
        for row in range(n):
            self._row_of_queen[col] = row
            self._add_queen(row, col, 1)
            col += 2
            if col >= n:
                col = 0

    # -----------------------------------------------------------------------
    # Conflict tracking
    # -----------------------------------------------------------------------

    def _add_queen(self, row, col, delta):
        self._queens_in_row[row]               += delta
        self._queens_in_mdiag[self._n - 1 + row - col] += delta
        self._queens_in_adiag[row + col]        += delta

    def _conflict_count(self, row, col):
        return (self._queens_in_row[row]
              + self._queens_in_mdiag[self._n - 1 + row - col]
              + self._queens_in_adiag[row + col]
              - 3 * (1 if self._row_of_queen[col] == row else 0))

    def _total_conflicts(self):
        return sum(self._conflict_count(self._row_of_queen[col], col)
                   for col in range(self._n))

    # -----------------------------------------------------------------------
    # Min-Conflicts repair
    # -----------------------------------------------------------------------

    def _col_with_max_conflicts(self):
        rng = self._rng
        best = -1
        max_c = -1
        count = 0
        for col in range(self._n):
            c = self._conflict_count(self._row_of_queen[col], col)
            if c > max_c:
                max_c = c
                count = 0
            if c == max_c:
                count += 1
                if rng.randint(1, count) == 1:
                    best = col
        return best

    def _row_with_min_conflicts(self, col):
        rng = self._rng
        best = -1
        min_c = float('inf')
        count = 0
        for row in range(self._n):
            c = self._conflict_count(row, col)
            if c < min_c:
                min_c = c
                count = 0
            if c == min_c:
                count += 1
                if rng.randint(1, count) == 1:
                    best = row
        return best

    def _move_queen(self, col, to_row):
        self._add_queen(self._row_of_queen[col], col, -1)
        self._row_of_queen[col] = to_row
        self._add_queen(to_row, col, 1)
        self._solved = self._total_conflicts() == 0

    # -----------------------------------------------------------------------
    # Public API
    # -----------------------------------------------------------------------

    def solve(self, max_steps):
        for _ in range(max_steps):
            col = self._col_with_max_conflicts()
            self._move_queen(col, self._row_with_min_conflicts(col))
            if self._solved:
                return True
        return False

    @property
    def is_solved(self):
        return self._solved

    def __str__(self):
        lines = []
        for row in range(self._n):
            lines.append(''.join('*' if self._row_of_queen[col] == row else '_'
                                 for col in range(self._n)))
        return '\n'.join(lines) + '\n'


if __name__ == "__main__":
    n = int(input())
    quiet   = '-q' in sys.argv
    verbose = '-v' in sys.argv

    start = time.perf_counter()
    board = NQueens(n, seed=None)
    while not board.solve(2 * n):
        board._place_queens()
    elapsed = time.perf_counter() - start

    print_board = (not quiet) if n <= 10 else verbose
    if print_board:
        print(board, end='')
    print(f"elapsed time: {elapsed:.6f} s", file=sys.stderr)
