import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;

public class DFSSolution {

    // pair = {key=RIGHT_FROG/LEFT_FROG, value=SINGLE_JUMP/DOUBLE_JUMP}
    // -1 = right frog jumps left, 1 = left frog jumps right
    // 1 = single jump, 2 = double jump
    private static final List<Pair<Integer, Integer>> JUMP_TYPES = new ArrayList<>();

    {
        JUMP_TYPES.add(new Pair<>(-1, 1));
        JUMP_TYPES.add(new Pair<>(-1, 2));
        JUMP_TYPES.add(new Pair<>(1, 1));
        JUMP_TYPES.add(new Pair<>(1, 2));
    }

    private long startTime;

    public void solve() {

        char[] board = initBoard();

        LinkedList<Integer> jumpSequence = new LinkedList<>();

        int depth = 0;
        startTime = System.nanoTime();
        dfs(board, board.length / 2, jumpSequence, depth);
        System.out.print("Jump Sequence: ");
        print(jumpSequence);
        System.out.println("Total jumps: " + jumpSequence.size());
        System.out.println("Success!");
    }

    private char[] initBoard() {

        Scanner sc = new Scanner(System.in);
        System.out.print("N: ");


        int N = sc.nextInt();
        int SIZE = N * 2 + 1;

        char[] board = new char[SIZE];

        int i = 0;

        while (i < N) {
            board[i++] = 'L';
        }

        board[i++] = '_';

        while (i < SIZE) {
            board[i++] = 'R';
        }

        return board;
    }

    private boolean dfs(char[] board, int leafPos, LinkedList<Integer> jumpSequence, int depth) {

        if (isGoalState(board, leafPos)) {
            long endTime = System.nanoTime();
            System.out.printf("Goal State found at depth: %d%n", depth);

            long ms = (endTime - startTime) / 1000000;
            long s = ms / 1000;
            System.out.printf("Solving puzzle took: %d µs ≈ %d s.%n", ms, s);

            print(board);
            return true;
        }

        for (Pair<Integer, Integer> jumpType : JUMP_TYPES) {
            if (shiftLeaf(board, leafPos, jumpType, jumpSequence, depth)) {
                return true;
            }
        }

        return false;
    }

    private boolean isGoalState(char[] board, int leafPos) {

        int midIndex = board.length / 2;

        if (midIndex != leafPos) {
            return false;
        }

        for (int i = 0; i < midIndex; i++) {
            if (board[i] != 'R') {
                return false;
            }
        }

        return true;
    }

    boolean shiftLeaf(char[] board, int leafPos, Pair<Integer, Integer> jumpType, LinkedList<Integer> jumpSequence,
                      int depth) {
        int frogPos = leafPos + jumpType.getKey() * jumpType.getValue();

        if (inRange(board, frogPos) && board[frogPos] == (jumpType.getKey() == -1 ? 'L' : 'R')) {
            return move(board, leafPos, frogPos, jumpSequence, depth);
        }

        return false;
    }

    boolean inRange(char[] board, int pos) {
        return 0 <= pos && pos < board.length;
    }

    boolean move(char[] board, int leafPos, int frogPos, LinkedList<Integer> jumpSequence, int depth) {
        ++depth;
        swap(board, leafPos, frogPos);

        if (dfs(board, frogPos, jumpSequence, depth)) {
            swap(board, leafPos, frogPos);
            print(board);
            jumpSequence.add(frogPos + 1);
            return true;
        }

        --depth;

        swap(board, leafPos, frogPos);

        return false;
    }

    private void swap(char[] arr, int ind1, int ind2) {
        char temp = arr[ind1];
        arr[ind1] = arr[ind2];
        arr[ind2] = temp;
    }

    private void print(char[] board) {
        for (char c : board) {
            System.out.print(c);
        }

        System.out.println();
    }

    private static void print(LinkedList<Integer> ll) {
        int SIZE = ll.size();

        for (int i = 0; i < SIZE - 1; i++) {
            System.out.print(ll.get(i) + ", ");
        }

        if (!ll.isEmpty()) {
            System.out.println(ll.get(SIZE - 1));
        } else {
            System.out.println();
        }
    }
}
