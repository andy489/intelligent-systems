import java.io.*;
import java.util.*;

public class NSlidingPuzzle {

    // -----------------------------------------------------------------------
    // Constants
    // -----------------------------------------------------------------------

    private static final int BLANK   = 0;
    private static final int NEUTRAL = 4;
    private static final int MAX_H   = 1 << 15;

    private enum Direction {
        LEFT(0), RIGHT(1), DOWN(2), UP(3), NEUTRAL_DIR(4);

        final int id;
        Direction(int id) { this.id = id; }

        static final Direction[] BY_ID = { LEFT, RIGHT, DOWN, UP, NEUTRAL_DIR };
        // Opposite of each direction by id: LEFT<->RIGHT, DOWN<->UP, NEUTRAL<->NEUTRAL
        private static final int[] REVERSE_ID = { RIGHT.id, LEFT.id, UP.id, DOWN.id, NEUTRAL_DIR.id };

        Direction reverse() { return BY_ID[REVERSE_ID[this.id]]; }

        @Override public String toString() {
            if (this == LEFT)  return "left";
            if (this == RIGHT) return "right";
            if (this == DOWN)  return "down";
            if (this == UP)    return "up";
            return "";
        }
    }

    // -----------------------------------------------------------------------
    // State
    // -----------------------------------------------------------------------

    private final int   gridSize;
    private final int   tilesCount;
    private final int[] board;
    private final int[] goalRow;
    private final int[] goalCol;
    private final int[] offset;

    private int blankCurr;
    private int blankTarget;

    private final Deque<Direction> history = new ArrayDeque<>();

    // -----------------------------------------------------------------------
    // Construction / input
    // -----------------------------------------------------------------------

    private NSlidingPuzzle(Scanner sc) {
        gridSize   = sc.nextInt();
        int targetIndex = sc.nextInt();
        tilesCount = gridSize * gridSize - 1;
        blankTarget = (targetIndex < 0) ? tilesCount : targetIndex;

        board   = new int[gridSize * gridSize];
        goalRow = new int[gridSize * gridSize];
        goalCol = new int[gridSize * gridSize];
        offset  = new int[5];

        for (int i = 0; i < board.length; i++) {
            board[i] = sc.nextInt();
            if (board[i] == BLANK) blankCurr = i;
        }

        initOffsets();
        initGoalState();
    }

    private void initOffsets() {
        offset[Direction.LEFT.id]        = -1;
        offset[Direction.RIGHT.id]       = +1;
        offset[Direction.DOWN.id]        = +gridSize;
        offset[Direction.UP.id]          = -gridSize;
        offset[Direction.NEUTRAL_DIR.id] = 0;
    }

    private void initGoalState() {
        for (int t = 1; t <= tilesCount; t++) {
            int pos = (t <= blankTarget) ? (t - 1) : t;
            goalRow[t] = pos / gridSize;
            goalCol[t] = pos % gridSize;
        }
        goalRow[BLANK] = blankTarget / gridSize;
        goalCol[BLANK] = blankTarget % gridSize;
    }

    // -----------------------------------------------------------------------
    // Solvability
    // -----------------------------------------------------------------------

    private long mergeCount(int[] arr, int[] tmp, int lo, int hi) {
        if (hi - lo < 2) return 0;
        int mid = (lo + hi) / 2;
        long inv = mergeCount(arr, tmp, lo, mid) + mergeCount(arr, tmp, mid, hi);

        int i = lo, j = mid, k = lo;
        while (i < mid && j < hi) {
            if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
            else { tmp[k++] = arr[j++]; inv += (mid - i); }
        }
        while (i < mid) tmp[k++] = arr[i++];
        while (j < hi)  tmp[k++] = arr[j++];
        System.arraycopy(tmp, lo, arr, lo, hi - lo);
        return inv;
    }

    // Parity rule:
    //   Odd grid:  solvable iff #inversions is even.
    //   Even grid: solvable iff (inversions + blank_row_from_bottom + target_row_from_bottom) is even.
    private boolean isSolvable() {
        int[] flat = new int[tilesCount];
        int p = 0;
        for (int v : board) if (v != BLANK) flat[p++] = v;

        long inv = mergeCount(flat, new int[tilesCount], 0, tilesCount);

        if (gridSize % 2 == 1) return inv % 2 == 0;

        int blankRowFromBottom  = (gridSize - 1) - (blankCurr   / gridSize);
        int targetRowFromBottom = (gridSize - 1) - (blankTarget / gridSize);
        return (inv + blankRowFromBottom + targetRowFromBottom) % 2 == 0;
    }

    // -----------------------------------------------------------------------
    // Heuristic
    // -----------------------------------------------------------------------

    private int calcManhattan() {
        int cost = 0;
        for (int i = 0; i < board.length; i++) {
            int t = board[i];
            if (t == BLANK) continue;
            cost += Math.abs(i / gridSize - goalRow[t]) + Math.abs(i % gridSize - goalCol[t]);
        }
        return cost;
    }

    private int calcLinearConflict() {
        int conflict = 0;
        for (int row = 0; row < gridSize; row++) {
            for (int c1 = 0; c1 < gridSize - 1; c1++) {
                int t1 = board[row * gridSize + c1];
                if (t1 == BLANK || goalRow[t1] != row) continue;
                for (int c2 = c1 + 1; c2 < gridSize; c2++) {
                    int t2 = board[row * gridSize + c2];
                    if (t2 == BLANK || goalRow[t2] != row) continue;
                    if (goalCol[t1] > goalCol[t2]) conflict += 2;
                }
            }
        }
        for (int col = 0; col < gridSize; col++) {
            for (int r1 = 0; r1 < gridSize - 1; r1++) {
                int t1 = board[r1 * gridSize + col];
                if (t1 == BLANK || goalCol[t1] != col) continue;
                for (int r2 = r1 + 1; r2 < gridSize; r2++) {
                    int t2 = board[r2 * gridSize + col];
                    if (t2 == BLANK || goalCol[t2] != col) continue;
                    if (goalRow[t1] > goalRow[t2]) conflict += 2;
                }
            }
        }
        return conflict;
    }

    private int updateHeuristic(int blankPos, int fromPos, int h) {
        int t = board[fromPos];
        int oldDist = Math.abs(fromPos / gridSize - goalRow[t]) + Math.abs(fromPos % gridSize - goalCol[t]);
        int newDist = Math.abs(blankPos / gridSize - goalRow[t]) + Math.abs(blankPos % gridSize - goalCol[t]);
        return h - oldDist + newDist;
    }

    // -----------------------------------------------------------------------
    // Move validation
    // -----------------------------------------------------------------------

    private int neighbourPos(int blankPos, Direction dir) {
        int np = blankPos + offset[dir.id];
        if (np < 0 || np >= board.length)                              return -1;
        if (dir == Direction.LEFT  && blankPos % gridSize == 0)        return -1;
        if (dir == Direction.RIGHT && blankPos % gridSize == gridSize - 1) return -1;
        return np;
    }

    // -----------------------------------------------------------------------
    // IDA* search
    // -----------------------------------------------------------------------

    // prevDir is the direction that brought the blank TO blankPos.
    // On success each frame pushes its own prevDir, building the path bottom-up.
    private int dls(int blankPos, int limit, Direction prevDir, int h, int depth) {
        int cost = h + depth;
        if (cost > limit) return cost;

        if (h == 0) {
            history.push(prevDir);
            System.out.println(depth);
            return 0;
        }

        Direction opposite = prevDir.reverse();
        int minCost = MAX_H;

        for (int d = 0; d < 4; d++) {
            Direction dir = Direction.BY_ID[d];
            if (dir == opposite) continue;

            int np = neighbourPos(blankPos, dir);
            if (np < 0) continue;

            int newH = updateHeuristic(blankPos, np, h);

            board[blankPos] = board[np];
            board[np] = BLANK;

            int result = dls(np, limit, dir, newH, depth + 1);

            board[np] = board[blankPos];
            board[blankPos] = BLANK;

            if (result == 0) {
                history.push(prevDir);
                return 0;
            }
            if (result < minCost) minCost = result;
        }

        return minCost;
    }

    private void idaStar(int md, int lc) {
        int threshold = md + lc;
        int result;
        do {
            result = dls(blankCurr, threshold, Direction.NEUTRAL_DIR, md, 0);
            if (result == 0) return;
            threshold = result;
            md = calcManhattan();
            lc = calcLinearConflict();
            threshold = Math.max(threshold, md + lc);
        } while (result != 0);
    }

    // -----------------------------------------------------------------------
    // Public entry point
    // -----------------------------------------------------------------------

    @Timed(label = "IDA* Sliding Blocks Puzzle")
    public void solve() {
        int md = calcManhattan();
        int lc = calcLinearConflict();

        if (md + lc == 0) {
            System.out.println(0);
            System.out.println("\nSuccess!");
            return;
        }

        if (!isSolvable()) {
            System.out.println("Puzzle cannot be solved!");
            return;
        }

        idaStar(md, lc);

        // history: front = NEUTRAL sentinel, then move1..moveN in order — pop NEUTRAL first
        history.pop();
        while (!history.isEmpty()) System.out.println(history.pop());

        System.out.println("\nSuccess!");
    }

    // -----------------------------------------------------------------------
    // Main
    // -----------------------------------------------------------------------

    public static void main(String[] args) throws IOException {
        Scanner sc = (args.length > 0)
                ? new Scanner(new File(args[0]))
                : new Scanner(System.in);

        NSlidingPuzzle puzzle = new NSlidingPuzzle(sc);
        sc.close();

        TimingUtil.run(puzzle, "solve");
    }
}
