// N-Queens — Deterministic O(N) Construction (Explicit Formula)
//
// Places all N queens in O(N) time using a closed-form formula — no search,
// no backtracking, no conflict repair. Produces one valid solution directly.
//
// Three cases based on N mod 6:
//
//   Case 1: N even, N mod 6 != 2
//     Row m    → queen at column 2m
//     Row N/2+m → queen at column 2m-1
//     for m = 1 .. N/2
//
//   Case 2: N even, N mod 6 == 2
//     Row m      → queen at column 1 + (2(m-1) + N/2 - 1) mod N
//     Row N+1-m  → queen at column N - (2(m-1) + N/2 - 1) mod N
//     for m = 1 .. N/2
//
//   Case 3: N odd
//     Solve for N-1 (even) using Case 1 or 2, then add queen at (N, N).

import java.util.Scanner;

public class NQueensLinear {

    private final int n;
    private final int[] colOfRow; // colOfRow[row] = column of queen in that row (1-indexed)

    public NQueensLinear(int n) {
        if (n == 2 || n == 3) throw new IllegalArgumentException("No solution for N=" + n);
        this.n = n;
        colOfRow = new int[n];
        solve(n, 0);
    }

    // Fill colOfRow starting at offset, for a sub-problem of size sz.
    private void solve(int sz, int offset) {
        if (sz % 2 == 0) {
            if (sz % 6 != 2) methodOne(sz, offset);
            else              methodTwo(sz, offset);
        } else {
            solve(sz - 1, offset);
            colOfRow[offset + sz - 1] = sz; // queen at (sz, sz) in sub-problem
        }
    }

    private void methodOne(int sz, int offset) {
        for (int m = 1; m <= sz / 2; ++m) colOfRow[offset + m - 1]          = 2 * m;
        for (int m = 1; m <= sz / 2; ++m) colOfRow[offset + sz / 2 + m - 1] = 2 * m - 1;
    }

    private void methodTwo(int sz, int offset) {
        for (int m = 1; m <= sz / 2; ++m)
            colOfRow[offset + m - 1] = 1 + (2 * (m - 1) + sz / 2 - 1) % sz;
        for (int m = sz / 2; m >= 1; --m)
            colOfRow[offset + sz - m] = sz - (2 * (m - 1) + sz / 2 - 1) % sz;
    }

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

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        sc.close();

        boolean quiet   = false;
        boolean verbose = false;
        for (String a : args) {
            if (a.equals("-q")) quiet   = true;
            if (a.equals("-v")) verbose = true;
        }

        long start = System.nanoTime();
        NQueensLinear board = new NQueensLinear(n);
        long elapsed = System.nanoTime() - start;

        boolean printBoard = (n <= 10) ? !quiet : verbose;
        if (printBoard) System.out.print(board);
        System.err.printf("elapsed time: %.6f s%n", elapsed / 1e9);
    }
}
