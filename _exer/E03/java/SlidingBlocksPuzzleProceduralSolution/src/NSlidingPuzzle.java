import java.io.*;
import java.util.*;

public class NSlidingPuzzle {

    static final int MAX_TILES = 100;
    static final int BLANK = 0;
    static final int NEUTRAL = 4;
    static final int MAX_H = 1 << 15;

    // Direction constants: LEFT=0, RIGHT=1, DOWN=2, UP=3
    static final int LEFT = 0, RIGHT = 1, DOWN = 2, UP = 3;

    static int gridSize;
    static int tilesCount;
    static int[] board = new int[MAX_TILES];
    static int[] goalRow = new int[MAX_TILES];
    static int[] goalCol = new int[MAX_TILES];
    static int blankCurr;
    static int blankTarget;
    static int[] offset = new int[5];

    // Solution path built in reverse during DLS, reversed at output
    static Deque<Integer> history = new ArrayDeque<>();

    static void initOffsets() {
        offset[LEFT]    = -1;
        offset[RIGHT]   = +1;
        offset[DOWN]    = +gridSize;
        offset[UP]      = -gridSize;
        offset[NEUTRAL] = 0;
    }

    static void setGoalState(int blankTargetIndex) {
        blankTarget = blankTargetIndex;
        for (int t = 1; t <= tilesCount; ++t) {
            int pos = (t <= blankTarget) ? (t - 1) : t;
            goalRow[t] = pos / gridSize;
            goalCol[t] = pos % gridSize;
        }
        goalRow[BLANK] = blankTarget / gridSize;
        goalCol[BLANK] = blankTarget % gridSize;
    }

    // ---------------------------------------------------------------------------
    // Input
    // ---------------------------------------------------------------------------

    static void readInput(Scanner sc) {
        gridSize = sc.nextInt();
        int targetIndex = sc.nextInt();

        tilesCount = gridSize * gridSize - 1;
        int blankTargetIndex = (targetIndex < 0) ? tilesCount : targetIndex;

        for (int i = 0; i < gridSize * gridSize; ++i) {
            board[i] = sc.nextInt();
            if (board[i] == BLANK) blankCurr = i;
        }

        initOffsets();
        setGoalState(blankTargetIndex);
    }

    // ---------------------------------------------------------------------------
    // Solvability check
    // ---------------------------------------------------------------------------

    static long mergeCount(int[] arr, int[] tmp, int lo, int hi) {
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

    static boolean checkSolvable() {
        int[] flat = new int[tilesCount];
        int p = 0;
        int blankRow = blankCurr / gridSize;

        for (int i = 0; i < gridSize * gridSize; ++i) {
            if (board[i] != BLANK) flat[p++] = board[i];
        }

        long inv = mergeCount(flat, new int[tilesCount], 0, tilesCount);

        if (gridSize % 2 == 1) return inv % 2 == 0;
        return (blankRow + inv) % 2 == 1;
    }

    // ---------------------------------------------------------------------------
    // Heuristic
    // ---------------------------------------------------------------------------

    static int calcManhattan() {
        int cost = 0;
        for (int i = 0; i < gridSize * gridSize; ++i) {
            int t = board[i];
            if (t == BLANK) continue;
            cost += Math.abs(i / gridSize - goalRow[t]) + Math.abs(i % gridSize - goalCol[t]);
        }
        return cost;
    }

    static int calcLinearConflict() {
        int conflict = 0;

        for (int row = 0; row < gridSize; ++row) {
            for (int c1 = 0; c1 < gridSize - 1; ++c1) {
                int t1 = board[row * gridSize + c1];
                if (t1 == BLANK || goalRow[t1] != row) continue;
                for (int c2 = c1 + 1; c2 < gridSize; ++c2) {
                    int t2 = board[row * gridSize + c2];
                    if (t2 == BLANK || goalRow[t2] != row) continue;
                    if (goalCol[t1] > goalCol[t2]) conflict += 2;
                }
            }
        }

        for (int col = 0; col < gridSize; ++col) {
            for (int r1 = 0; r1 < gridSize - 1; ++r1) {
                int t1 = board[r1 * gridSize + col];
                if (t1 == BLANK || goalCol[t1] != col) continue;
                for (int r2 = r1 + 1; r2 < gridSize; ++r2) {
                    int t2 = board[r2 * gridSize + col];
                    if (t2 == BLANK || goalCol[t2] != col) continue;
                    if (goalRow[t1] > goalRow[t2]) conflict += 2;
                }
            }
        }

        return conflict;
    }

    static int updateHeuristic(int blankPos, int fromPos, int h) {
        int t = board[fromPos];
        int gr = goalRow[t], gc = goalCol[t];
        int oldDist = Math.abs(fromPos / gridSize - gr) + Math.abs(fromPos % gridSize - gc);
        int newDist = Math.abs(blankPos / gridSize - gr) + Math.abs(blankPos % gridSize - gc);
        return h - oldDist + newDist;
    }

    // ---------------------------------------------------------------------------
    // Move validation
    // ---------------------------------------------------------------------------

    static int neighbourPos(int blankPos, int move) {
        int np = blankPos + offset[move];
        if (np < 0 || np >= gridSize * gridSize) return -1;
        if (move == LEFT  && blankPos % gridSize == 0)             return -1;
        if (move == RIGHT && blankPos % gridSize == gridSize - 1)  return -1;
        return np;
    }

    static int reverseMove(int m) {
        if (m == UP)    return DOWN;
        if (m == DOWN)  return UP;
        if (m == LEFT)  return RIGHT;
        if (m == RIGHT) return LEFT;
        return NEUTRAL;
    }

    // ---------------------------------------------------------------------------
    // IDA* search
    // ---------------------------------------------------------------------------

    static int dls(int blankPos, int limit, int prevMove, int h, int depth) {
        int cost = h + depth;
        if (cost > limit) return cost;

        if (h == 0) {
            history.push(prevMove);
            System.out.println(depth);
            return 0;
        }

        int opposite = reverseMove(prevMove);
        int minCost = MAX_H;

        for (int move = 0; move < 4; ++move) {
            if (move == opposite) continue;

            int np = neighbourPos(blankPos, move);
            if (np < 0) continue;

            int newH = updateHeuristic(blankPos, np, h);

            board[blankPos] = board[np];
            board[np] = BLANK;

            int result = dls(np, limit, move, newH, depth + 1);

            board[np] = board[blankPos];
            board[blankPos] = BLANK;

            if (result == 0) {
                history.push(prevMove);
                return 0;
            }
            if (result < minCost) minCost = result;
        }

        return minCost;
    }

    static void idaStar(int mdInit, int lcInit) {
        int threshold = mdInit + lcInit;
        int result;

        do {
            result = dls(blankCurr, threshold, NEUTRAL, mdInit, 0);
            if (result == 0) return;
            threshold = result;
            mdInit = calcManhattan();
            lcInit = calcLinearConflict();
            threshold = Math.max(threshold, mdInit + lcInit);
        } while (result != 0);
    }

    // ---------------------------------------------------------------------------
    // Output
    // ---------------------------------------------------------------------------

    static String moveName(int m) {
        if (m == UP)    return "up";
        if (m == DOWN)  return "down";
        if (m == LEFT)  return "left";
        if (m == RIGHT) return "right";
        return "";
    }

    static String formatDuration(long ms) {
        long s   = ms / 1000; ms %= 1000;
        long min = s  / 60;   s  %= 60;
        long h   = min / 60;  min %= 60;
        return String.format("Elapsed: %02d:%02d:%02d.%03d", h, min, s, ms);
    }

    // ---------------------------------------------------------------------------
    // Main
    // ---------------------------------------------------------------------------

    public static void main(String[] args) throws IOException {
        Scanner sc;
        if (args.length > 0) {
            sc = new Scanner(new File(args[0]));
        } else {
            sc = new Scanner(System.in);
        }

        readInput(sc);
        sc.close();

        int md = calcManhattan();
        int lc = calcLinearConflict();

        if (md + lc == 0) {
            System.out.println(0);
            System.out.println("\nSuccess!");
            return;
        }

        if (!checkSolvable()) {
            System.out.println("Puzzle cannot be solved!");
            return;
        }

        long t0 = System.currentTimeMillis();
        idaStar(md, lc);
        long elapsed = System.currentTimeMillis() - t0;

        // history: front=NEUTRAL (root sentinel), then move1..moveN in order
        history.pop(); // discard NEUTRAL
        while (!history.isEmpty()) {
            System.out.println(moveName(history.pop()));
        }

        System.out.println();
        System.out.println(formatDuration(elapsed));
        System.out.println("\nSuccess!");
    }
}
