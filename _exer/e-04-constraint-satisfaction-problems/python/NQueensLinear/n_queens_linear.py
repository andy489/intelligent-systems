# N-Queens — Deterministic O(N) Construction
#
# Builds one valid solution in O(N) time using a closed-form placement formula.
# No search, no backtracking, no conflict repair — every queen lands in its
# final position on the first try.
#
# ── Algorithm ────────────────────────────────────────────────────────────────
#
# The construction is based on three cases that depend on N mod 6:
#
#   Case A  N even, N mod 6 ≠ 2  (i.e. N mod 6 ∈ {0, 4})
#     First N/2 rows:   row m       → column 2m
#     Next  N/2 rows:   row N/2 + m → column 2m − 1       (m = 1 … N/2)
#     Produces two interleaved arithmetic sequences: even columns, then odd.
#
#   Case B  N even, N mod 6 = 2
#     First N/2 rows:   row m       → column 1 + (2(m−1) + N/2 − 1) mod N
#     Next  N/2 rows:   row N+1 − m → column N − (2(m−1) + N/2 − 1) mod N
#     A "rotated" variant that avoids the diagonal conflicts Case A creates
#     when N ≡ 2 (mod 6).
#
#   Case C  N odd
#     Solve for N−1 (even) with Case A or B, then place the last queen at
#     row N, column N.  The corner (N, N) never conflicts with the N−1
#     solution because neither Case A nor B ever uses column N.
#
# ── Complexity ───────────────────────────────────────────────────────────────
#   Time  O(N)  — exactly N list writes, two half-size loops
#   Space O(N)  — one list of size N
#
# ── Reference ────────────────────────────────────────────────────────────────
#   E. Hoffman, J. Loessi, R. Moore (1969). "Constructions for the Solution
#   of the m Queens Problem." Mathematics Magazine 42(2):66–72.

import sys
import time


# ── Construction ─────────────────────────────────────────────────────────────

def _case_a(sz, offset, col):
    # Case A: N even, N mod 6 ≠ 2
    # Row m       → col 2m        (even columns, first half)
    # Row N/2 + m → col 2m − 1   (odd  columns, second half)
    half = sz // 2
    for m in range(1, half + 1):
        col[offset + m - 1]        = 2 * m
        col[offset + half + m - 1] = 2 * m - 1


def _case_b(sz, offset, col):
    # Case B: N even, N mod 6 = 2
    # Uses a modular shift to break the diagonals Case A cannot avoid.
    half = sz // 2
    for m in range(1, half + 1):
        col[offset + m - 1]  = 1 + (2 * (m - 1) + half - 1) % sz   # forward
        col[offset + sz - m] = sz - (2 * (m - 1) + half - 1) % sz  # reverse


def _place(sz, offset, col):
    """Fill col[offset : offset+sz] with a valid sz-queens solution."""
    if sz % 2 == 0:
        if sz % 6 != 2:
            _case_a(sz, offset, col)
        else:
            _case_b(sz, offset, col)
    else:
        # Odd: solve the even sub-problem, then append queen at (sz, sz).
        _place(sz - 1, offset, col)
        col[offset + sz - 1] = sz


def solve(n):
    """Return col[row] = 1-indexed column of queen in that row (0-indexed)."""
    if n in (2, 3):
        raise ValueError(f"No solution for N={n}")
    col = [0] * n
    _place(n, 0, col)
    return col


# ── Output ───────────────────────────────────────────────────────────────────

def _board_str(n, col):
    # O(N²) — only call for small N
    rows = []
    for r in range(n):
        rows.append(''.join('*' if c == col[r] else '_' for c in range(1, n + 1)))
    return '\n'.join(rows) + '\n'


# ── Entry point ──────────────────────────────────────────────────────────────

if __name__ == '__main__':
    n = int(input())
    quiet   = '-q' in sys.argv
    verbose = '-v' in sys.argv

    t0  = time.perf_counter()
    col = solve(n)
    elapsed = time.perf_counter() - t0

    # Default: print board only when N ≤ 10 (printing is O(N²))
    if (not quiet if n <= 10 else verbose):
        print(_board_str(n, col), end='')

    print(f"elapsed time: {elapsed:.6f} s", file=sys.stderr)
