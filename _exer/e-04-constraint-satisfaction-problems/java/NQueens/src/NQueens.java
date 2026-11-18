// N-Queens — Min-Conflicts local search
//
// Algorithm:
//   1. Initialise: place queens in interleaved even/odd column order (1,3,5,...,0,2,4,...)
//      so each column is occupied and the initial conflict count is low.
//   2. Repair: up to 2N steps — pick the column with the most conflicts (random tie-break),
//      move its queen to the row with the fewest conflicts (random tie-break).
//   3. Restart if 2N steps did not solve; repeat until solved.
//
// Conflict tracking is O(1) per move via three incremental counter arrays:
//   queensInRow[r]          — queens sharing row r
//   queensInMainDiag[d]     — queens on main diagonal d = (N-1) + row - col
//   queensInAntidiag[d]     — queens on anti-diagonal d = row + col
//
// Expected time: O(N) — constant number of repair steps on average.

import java.util.*;

public class NQueens {

    private final int n;
    private final int[] rowOfQueen;        // rowOfQueen[col] = row of queen in that column
    private final int[] queensInRow;
    private final int[] queensInMainDiag;  // size 2N-1
    private final int[] queensInAntidiag; // size 2N-1
    private final Random rng;
    private boolean solved = false;

    public NQueens(int n, long seed) {
        if (n == 2 || n == 3) throw new IllegalArgumentException("No solution for N=" + n);
        this.n = n;
        rowOfQueen       = new int[n];
        queensInRow      = new int[n];
        queensInMainDiag = new int[2 * n - 1];
        queensInAntidiag = new int[2 * n - 1];
        rng = new Random(seed);
        placeQueens();
    }

    // -----------------------------------------------------------------------
    // Initialisation — interleaved column order
    // -----------------------------------------------------------------------

    private void placeQueens() {
        Arrays.fill(queensInRow,      0);
        Arrays.fill(queensInMainDiag, 0);
        Arrays.fill(queensInAntidiag, 0);
        solved = false;
        int col = 1;
        for (int row = 0; row < n; row++) {
            rowOfQueen[col] = row;
            addQueen(row, col, 1);
            col += 2;
            if (col >= n) col = 0;
        }
    }

    // -----------------------------------------------------------------------
    // Conflict tracking
    // -----------------------------------------------------------------------

    private void addQueen(int row, int col, int delta) {
        queensInRow[row]                    += delta;
        queensInMainDiag[n - 1 + row - col] += delta;
        queensInAntidiag[row + col]         += delta;
    }

    private int conflictCount(int row, int col) {
        return queensInRow[row]
             + queensInMainDiag[n - 1 + row - col]
             + queensInAntidiag[row + col]
             - 3 * (rowOfQueen[col] == row ? 1 : 0);
    }

    private int totalConflicts() {
        int total = 0;
        for (int col = 0; col < n; col++)
            total += conflictCount(rowOfQueen[col], col);
        return total;
    }

    // -----------------------------------------------------------------------
    // Min-Conflicts repair
    // -----------------------------------------------------------------------

    private int colWithMaxConflicts() {
        int max = 0;
        int count = 0;
        int chosen = 0;
        for (int col = 0; col < n; col++) {
            int c = conflictCount(rowOfQueen[col], col);
            if (c > max) { max = c; count = 0; }
            if (c == max) {
                count++;
                if (rng.nextInt(count) == 0) chosen = col;
            }
        }
        return chosen;
    }

    private int rowWithMinConflicts(int col) {
        int min = Integer.MAX_VALUE;
        int count = 0;
        int chosen = 0;
        for (int row = 0; row < n; row++) {
            int c = conflictCount(row, col);
            if (c < min) { min = c; count = 0; }
            if (c == min) {
                count++;
                if (rng.nextInt(count) == 0) chosen = row;
            }
        }
        return chosen;
    }

    private void moveQueen(int col, int toRow) {
        addQueen(rowOfQueen[col], col, -1);
        rowOfQueen[col] = toRow;
        addQueen(toRow, col, 1);
        solved = totalConflicts() == 0;
    }

    // -----------------------------------------------------------------------
    // Public API
    // -----------------------------------------------------------------------

    public boolean solve(int maxSteps) {
        while (maxSteps-- > 0) {
            int col = colWithMaxConflicts();
            moveQueen(col, rowWithMinConflicts(col));
            if (solved) return true;
        }
        return false;
    }

    public boolean isSolved() { return solved; }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder(n * (n + 1));
        for (int row = 0; row < n; row++) {
            for (int col = 0; col < n; col++)
                sb.append(rowOfQueen[col] == row ? '*' : '_');
            sb.append('\n');
        }
        return sb.toString();
    }

    // -----------------------------------------------------------------------
    // Main
    // -----------------------------------------------------------------------

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        sc.close();

        boolean quiet = false;
        for (String a : args) if (a.equals("-q")) quiet = true;

        long start = System.nanoTime();
        NQueens board = new NQueens(n, System.nanoTime());
        while (!board.solve(2 * n))
            board.placeQueens();
        long elapsed = System.nanoTime() - start;

        if (!quiet) System.out.print(board);
        System.err.printf("elapsed time: %.6f s%n", elapsed / 1e9);
    }
}
