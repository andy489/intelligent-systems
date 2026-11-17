// N-Sliding Blocks Puzzle — IDA* with Additive Pattern Databases (PDB) / MD+LC
//
// Algorithm: IDA* (Iterative Deepening A*)
//   Runs repeated depth-limited searches, each time with a higher f-cost
//   threshold (= depth + h). The threshold for the next iteration is the
//   smallest f-cost that exceeded the current limit. Guarantees optimal solution.
//
// Heuristic (4×4 canonical goal): Additive 5-5-5 Disjoint Pattern Databases
//   Tiles 1-15 split into three disjoint groups of 5. A PDB for each group stores
//   the exact minimum moves to bring just those 5 tiles to their goal positions
//   (ignoring all other tiles). Because groups are disjoint, PDB_A+PDB_B+PDB_C is
//   admissible (no move counted twice) and much tighter than MD+LC alone.
//   Each PDB has 16P5 = 524160 entries; built by BFS from goal at startup.
//
// Heuristic (all other grids / non-default blank target): MD + Linear Conflict
//   MD: sum of per-tile row+col distances to goal — admissible lower bound.
//   LC: two tiles in their goal row/col but in reversed order need ≥2 extra
//       moves to pass each other. MD updated incrementally O(1); LC recomputed
//       O(N²) after every swap — tight enough to solve hard 4×4 in seconds.

import java.io.*;
import java.util.*;

public class NSlidingPuzzle {

    // -----------------------------------------------------------------------
    // Constants
    // -----------------------------------------------------------------------

    private static final int BLANK   = 0;
    private static final int MAX_H   = 1 << 15;

    private static final int PDB_SIZE = 16 * 15 * 14 * 13 * 12; // 524160 — 16P5

    private static final int[] GROUP_A = {1,  2,  3,  4,  5};
    private static final int[] GROUP_B = {6,  7,  8,  9, 10};
    private static final int[] GROUP_C = {11, 12, 13, 14, 15};

    private static final int[] FACTORS = {15*14*13*12, 14*13*12, 13*12, 12, 1};

    private enum Direction {
        LEFT(0), RIGHT(1), DOWN(2), UP(3), NEUTRAL_DIR(4);

        final int id;
        Direction(int id) { this.id = id; }

        static final Direction[] BY_ID = { LEFT, RIGHT, DOWN, UP, NEUTRAL_DIR };
        private static final int[] REVERSE_ID = { RIGHT.id, LEFT.id, UP.id, DOWN.id, NEUTRAL_DIR.id };

        Direction reverse() { return BY_ID[REVERSE_ID[this.id]]; }

        @Override public String toString() {
            if (this == LEFT)  return "right";
            if (this == RIGHT) return "left";
            if (this == DOWN)  return "up";
            if (this == UP)    return "down";
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
    private boolean usePdb;

    // PDB tables (allocated only when usePdb)
    private short[] pdbA;
    private short[] pdbB;
    private short[] pdbC;
    private final int[] tilePos = new int[16]; // tilePos[t] = current linear index of tile t

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

        usePdb = (gridSize == 4 && blankTarget == 15);
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
    // PDB: Lehmer-code rank for a 5-element partial permutation on 16 positions
    // -----------------------------------------------------------------------

    private static int permRank5(int[] p) {
        int used = 0, rank = 0;
        for (int i = 0; i < 5; i++) {
            int pos = p[i];
            rank += (pos - Integer.bitCount(used & ((1 << pos) - 1))) * FACTORS[i];
            used |= (1 << pos);
        }
        return rank;
    }

    // -----------------------------------------------------------------------
    // PDB builder — 0-1 BFS in abstract state space for one tile group
    // -----------------------------------------------------------------------
    // Abstract state: positions of the 5 group tiles + blank position.
    // Encoded as (perm_rank * 16 + blank_pos).
    // Only moves that relocate a group tile cost 1; blank through wildcard costs 0.
    // Project: pdb[rank] = min cost over all blank positions.

    private static short[] buildPdb(int[] group) {
        final int BFS_SIZE = PDB_SIZE * 16;
        short[] bfs = new short[BFS_SIZE];
        Arrays.fill(bfs, (short) 0xFF);

        int[] goalPos = new int[5];
        for (int i = 0; i < 5; i++) goalPos[i] = group[i] - 1;

        int startState = permRank5(goalPos) * 16 + 15;
        bfs[startState] = 0;

        Deque<Integer> dq = new ArrayDeque<>();
        dq.addLast(startState);

        final int[] off4 = {-1, +1, +4, -4}; // LEFT, RIGHT, DOWN, UP

        while (!dq.isEmpty()) {
            int state = dq.pollFirst();
            int blank = state & 15;
            int rank  = state >> 4;
            short cost = bfs[state];

            // Decode tile positions from Lehmer rank
            int[] pos = new int[5];
            {
                int rem = rank;
                int avail = 0xFFFF;
                for (int i = 0; i < 5; i++) {
                    int quot = rem / FACTORS[i];
                    rem %= FACTORS[i];
                    int cnt = 0, bit = 0;
                    for (bit = 0; bit < 16; bit++) {
                        if ((avail & (1 << bit)) != 0) {
                            if (cnt == quot) break;
                            cnt++;
                        }
                    }
                    pos[i] = bit;
                    avail &= ~(1 << bit);
                }
            }

            int[] groupAt = new int[16];
            Arrays.fill(groupAt, -1);
            for (int i = 0; i < 5; i++) groupAt[pos[i]] = i;

            for (int d = 0; d < 4; d++) {
                int nb = blank + off4[d];
                if (nb < 0 || nb >= 16) continue;
                if (d == 0 && blank % 4 == 0) continue; // LEFT from col 0
                if (d == 1 && blank % 4 == 3) continue; // RIGHT from col 3

                int newRank  = rank;
                int newBlank = nb;
                short newCost;
                int gi = groupAt[nb];

                if (gi >= 0) {
                    int[] newPos = pos.clone();
                    newPos[gi] = blank;
                    newRank = permRank5(newPos);
                    newCost = (short)(cost + 1);
                } else {
                    newCost = cost;
                }

                int newState = newRank * 16 + newBlank;
                if ((bfs[newState] & 0xFF) == 0xFF || (bfs[newState] & 0xFF) > (newCost & 0xFF)) {
                    bfs[newState] = newCost;
                    if (newCost == cost) dq.addFirst(newState);
                    else                 dq.addLast(newState);
                }
            }
        }

        // Project: pdb[rank] = min cost over all blank positions
        short[] pdb = new short[PDB_SIZE];
        Arrays.fill(pdb, (short) 0xFF);
        for (int rank = 0; rank < PDB_SIZE; rank++) {
            int best = 0xFF;
            for (int b = 0; b < 16; b++) {
                int v = bfs[rank * 16 + b] & 0xFF;
                if (v < best) best = v;
            }
            pdb[rank] = (short) best;
        }
        return pdb;
    }

    private void buildPdbs() {
        pdbA = buildPdb(GROUP_A);
        pdbB = buildPdb(GROUP_B);
        pdbC = buildPdb(GROUP_C);
    }

    private void initTilePos() {
        for (int i = 0; i < board.length; i++)
            tilePos[board[i]] = i;
    }

    private int pdbLookup() {
        int[] pA = new int[5], pB = new int[5], pC = new int[5];
        for (int i = 0; i < 5; i++) {
            pA[i] = tilePos[GROUP_A[i]];
            pB[i] = tilePos[GROUP_B[i]];
            pC[i] = tilePos[GROUP_C[i]];
        }
        return (pdbA[permRank5(pA)] & 0xFF)
             + (pdbB[permRank5(pB)] & 0xFF)
             + (pdbC[permRank5(pC)] & 0xFF);
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
    // Heuristic: Manhattan Distance
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

    // -----------------------------------------------------------------------
    // Heuristic: Linear Conflict
    // -----------------------------------------------------------------------

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

    private int updateMd(int blankPos, int fromPos, int md) {
        int t = board[fromPos];
        int oldDist = Math.abs(fromPos / gridSize - goalRow[t]) + Math.abs(fromPos % gridSize - goalCol[t]);
        int newDist = Math.abs(blankPos / gridSize - goalRow[t]) + Math.abs(blankPos % gridSize - goalCol[t]);
        return md - oldDist + newDist;
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
    // IDA* — depth-limited search (DLS)
    // -----------------------------------------------------------------------

    private int dls(int blankPos, int limit, Direction prevDir, int h, int md, int depth) {
        int cost = h + depth;
        if (cost > limit) return cost;

        if (md == 0) {
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

            int newMd = updateMd(blankPos, np, md);

            int t = board[np];
            board[blankPos] = t;
            board[np] = BLANK;

            int newH;
            if (usePdb) {
                tilePos[t] = blankPos;
                newH = pdbLookup();
            } else {
                newH = newMd + calcLinearConflict();
            }

            int result = dls(np, limit, dir, newH, newMd, depth + 1);

            board[np] = board[blankPos];
            board[blankPos] = BLANK;
            if (usePdb) tilePos[t] = np;

            if (result == 0) {
                history.push(prevDir);
                return 0;
            }
            if (result < minCost) minCost = result;
        }

        return minCost;
    }

    // -----------------------------------------------------------------------
    // IDA* outer loop
    // -----------------------------------------------------------------------

    private void idaStar(int md, int initialH) {
        int threshold = initialH;
        int result;
        do {
            result = dls(blankCurr, threshold, Direction.NEUTRAL_DIR, threshold, md, 0);
            if (result == 0) return;
            threshold = result;
            md = calcManhattan();
            if (usePdb) {
                threshold = Math.max(threshold, pdbLookup());
            } else {
                int lc = calcLinearConflict();
                threshold = Math.max(threshold, md + lc);
            }
        } while (result != 0);
    }

    // -----------------------------------------------------------------------
    // Public entry point
    // -----------------------------------------------------------------------

    @Timed(label = "IDA* Sliding Blocks Puzzle")
    public void solve() {
        int md = calcManhattan();

        if (md == 0) {
            System.out.println(0);
            System.out.println("\nSuccess!");
            return;
        }

        if (!isSolvable()) {
            System.out.println("Puzzle cannot be solved!");
            return;
        }

        int initialH;
        if (usePdb) {
            buildPdbs();
            initTilePos();
            initialH = pdbLookup();
        } else {
            initialH = md + calcLinearConflict();
        }

        idaStar(md, initialH);

        history.pop(); // remove NEUTRAL sentinel
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
