// N-Queens — Deterministic O(N) Construction
//
// Builds one valid solution in O(N) time using a closed-form placement formula.
// No search, no backtracking, no conflict repair — every queen lands in its
// final position on the first try.
//
// ── Algorithm ────────────────────────────────────────────────────────────────
//
// The construction is based on three cases that depend on N mod 6:
//
//   Case A  N even, N mod 6 ≠ 2  (i.e. N mod 6 ∈ {0, 4})
//     First N/2 rows:   row m       → column 2m
//     Next  N/2 rows:   row N/2 + m → column 2m − 1       (m = 1 … N/2)
//     Produces two interleaved arithmetic sequences: even columns, then odd.
//
//   Case B  N even, N mod 6 = 2
//     First N/2 rows:   row m       → column 1 + (2(m−1) + N/2 − 1) mod N
//     Next  N/2 rows:   row N+1 − m → column N − (2(m−1) + N/2 − 1) mod N
//     A "rotated" variant that avoids the diagonal conflicts Case A creates
//     when N ≡ 2 (mod 6).
//
//   Case C  N odd
//     Solve for N−1 (even) with Case A or B, then place the last queen at
//     row N, column N.  The corner (N, N) never conflicts with the N−1
//     solution because neither Case A nor B ever uses column N.
//
// ── Complexity ───────────────────────────────────────────────────────────────
//   Time  O(N)  — exactly N array writes, two half-size loops
//   Space O(N)  — one int[] of size N
//
// ── Reference ────────────────────────────────────────────────────────────────
//   E. Hoffman, J. Loessi, R. Moore (1969). "Constructions for the Solution
//   of the m Queens Problem." Mathematics Magazine 42(2):66–72.

import java.util.Scanner;

public class NQueensLinear {

    // colOfRow[r] = 1-indexed column of the queen placed in row r (0-indexed)
    private final int[] colOfRow;
    private final int n;

    public NQueensLinear(int n) {
        if (n == 2 || n == 3) throw new IllegalArgumentException("No solution for N=" + n);
        this.n = n;
        colOfRow = new int[n];
        place(n, 0);
    }

    // ── Construction ─────────────────────────────────────────────────────────

    // Fills colOfRow[offset .. offset+sz-1] with a valid sz-queens solution.
    private void place(int sz, int offset) {
        if (sz % 2 == 0) {
            if (sz % 6 != 2) caseA(sz, offset);
            else              caseB(sz, offset);
        } else {
            // Odd: solve the even sub-problem, then append queen at (sz, sz).
            place(sz - 1, offset);
            colOfRow[offset + sz - 1] = sz;
        }
    }

    // Case A: N even, N mod 6 ≠ 2
    // Row m       → col 2m          (even columns, first half)
    // Row N/2 + m → col 2m − 1     (odd  columns, second half)
    private void caseA(int sz, int offset) {
        int half = sz / 2;
        for (int m = 1; m <= half; m++) {
            colOfRow[offset + m - 1]        = 2 * m;
            colOfRow[offset + half + m - 1] = 2 * m - 1;
        }
    }

    // Case B: N even, N mod 6 = 2
    // Uses a modular shift to break the diagonals that Case A cannot avoid.
    private void caseB(int sz, int offset) {
        int half = sz / 2;
        for (int m = 1; m <= half; m++) {
            // forward pass: rows 1 … N/2
            colOfRow[offset + m - 1]      = 1 + (2 * (m - 1) + half - 1) % sz;
            // reverse pass: rows N … N/2+1  (fills second half back-to-front)
            colOfRow[offset + sz - m]     = sz - (2 * (m - 1) + half - 1) % sz;
        }
    }

    // ── Output ───────────────────────────────────────────────────────────────

    // Returns the full board as a string (O(N²) — only use for small N).
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder(n * (n + 1));
        for (int row = 0; row < n; row++) {
            for (int col = 1; col <= n; col++)
                sb.append(col == colOfRow[row] ? '*' : '_');
            sb.append('\n');
        }
        return sb.toString();
    }

    // ── Entry point ──────────────────────────────────────────────────────────

    public static void main(String[] args) {
        int n = new Scanner(System.in).nextInt();

        boolean quiet   = false;
        boolean verbose = false;
        for (String arg : args) {
            if (arg.equals("-q")) quiet   = true;
            if (arg.equals("-v")) verbose = true;
        }

        long t0 = System.nanoTime();
        NQueensLinear board = new NQueensLinear(n);
        double elapsed = (System.nanoTime() - t0) / 1e9;

        // Default: print board only when N ≤ 10 (printing is O(N²))
        if ((n <= 10) ? !quiet : verbose)
            System.out.print(board);

        System.err.printf("elapsed time: %.6f s%n", elapsed);
    }
}
