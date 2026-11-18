# N-Queens — Deterministic O(N) Construction (Explicit Formula)
#
# Places all N queens in O(N) time using a closed-form formula — no search,
# no backtracking, no conflict repair. Produces one valid solution directly.
#
# Three cases based on N mod 6:
#
#   Case 1: N even, N mod 6 != 2
#     Row m    → queen at column 2m
#     Row N/2+m → queen at column 2m-1
#     for m = 1 .. N/2
#
#   Case 2: N even, N mod 6 == 2
#     Row m      → queen at column 1 + (2(m-1) + N/2 - 1) mod N
#     Row N+1-m  → queen at column N - (2(m-1) + N/2 - 1) mod N
#     for m = 1 .. N/2
#
#   Case 3: N odd
#     Solve for N-1 (even) using Case 1 or 2, then add queen at (N, N).

import sys
import time


def _method_one(sz, col_of_row, offset):
    for m in range(1, sz // 2 + 1):
        col_of_row[offset + m - 1]          = 2 * m
        col_of_row[offset + sz // 2 + m - 1] = 2 * m - 1


def _method_two(sz, col_of_row, offset):
    for m in range(1, sz // 2 + 1):
        col_of_row[offset + m - 1] = 1 + (2 * (m - 1) + sz // 2 - 1) % sz
    for m in range(sz // 2, 0, -1):
        col_of_row[offset + sz - m] = sz - (2 * (m - 1) + sz // 2 - 1) % sz


def _solve(sz, col_of_row, offset):
    if sz % 2 == 0:
        if sz % 6 != 2:
            _method_one(sz, col_of_row, offset)
        else:
            _method_two(sz, col_of_row, offset)
    else:
        _solve(sz - 1, col_of_row, offset)
        col_of_row[offset + sz - 1] = sz  # queen at (sz, sz) in sub-problem


def n_queens_linear(n):
    """Return col_of_row[row] = column of queen in that row (1-indexed)."""
    if n in (2, 3):
        raise ValueError(f"No solution for N={n}")
    col_of_row = [0] * n
    _solve(n, col_of_row, 0)
    return col_of_row


def board_str(n, col_of_row):
    lines = []
    for row in range(n):
        lines.append(''.join('*' if col == col_of_row[row] else '_'
                             for col in range(1, n + 1)))
    return '\n'.join(lines) + '\n'


if __name__ == "__main__":
    n = int(input())
    quiet   = '-q' in sys.argv
    verbose = '-v' in sys.argv

    start = time.perf_counter()
    col_of_row = n_queens_linear(n)
    elapsed = time.perf_counter() - start

    print_board = (not quiet) if n <= 10 else verbose
    if print_board:
        print(board_str(n, col_of_row), end='')
    print(f"elapsed time: {elapsed:.6f} s", file=sys.stderr)
