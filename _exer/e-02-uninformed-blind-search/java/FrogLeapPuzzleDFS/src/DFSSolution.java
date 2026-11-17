import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;

public class DFSSolution {

    // direction: -1 = right frog jumps left, +1 = left frog jumps right
    // distance:   1 = single jump,            2 = double jump
    private static final List<Pair<Integer, Integer>> JUMP_TYPES = new ArrayList<>();

    static {
        JUMP_TYPES.add(new Pair<>(-1, 1));
        JUMP_TYPES.add(new Pair<>(-1, 2));
        JUMP_TYPES.add(new Pair<>(1, 1));
        JUMP_TYPES.add(new Pair<>(1, 2));
    }

    @Timed(label = "DFS Frog Leap Puzzle")
    public void solve() {
        char[] board = initBoard();
        LinkedList<Integer> jumpSequence = new LinkedList<>();

        dfs(board, board.length / 2, jumpSequence, 0);

        System.out.print("Jump Sequence: ");
        printJumpSequence(jumpSequence);
        System.out.println("Total jumps: " + jumpSequence.size());
        System.out.println("Success!");
    }

    private char[] initBoard() {
        Scanner sc = new Scanner(System.in);
        System.out.print("N: ");
        int n = sc.nextInt();
        sc.close();

        int size = n * 2 + 1;
        char[] board = new char[size];
        int i = 0;
        while (i < n)    board[i++] = 'L';
        board[i++] = '_';
        while (i < size) board[i++] = 'R';
        return board;
    }

    private boolean dfs(char[] board, int leafPos, LinkedList<Integer> jumpSequence, int depth) {
        if (isGoalState(board, leafPos)) {
            System.out.printf("Goal state found at depth: %d%n", depth);
            printBoard(board);
            return true;
        }

        for (Pair<Integer, Integer> jumpType : JUMP_TYPES) {
            if (tryJump(board, leafPos, jumpType, jumpSequence, depth)) {
                return true;
            }
        }

        return false;
    }

    private boolean isGoalState(char[] board, int leafPos) {
        int mid = board.length / 2;
        if (leafPos != mid) return false;
        for (int i = 0;       i < mid;          i++) if (board[i] != 'R') return false;
        for (int i = mid + 1; i < board.length; i++) if (board[i] != 'L') return false;
        return true;
    }

    private boolean tryJump(char[] board, int leafPos, Pair<Integer, Integer> jumpType,
                            LinkedList<Integer> jumpSequence, int depth) {
        int direction = jumpType.key();
        int distance  = jumpType.value();
        int frogPos   = leafPos + direction * distance;
        char expected = direction == -1 ? 'L' : 'R';

        if (!inRange(board, frogPos) || board[frogPos] != expected) return false;

        swap(board, leafPos, frogPos);

        if (dfs(board, frogPos, jumpSequence, depth + 1)) {
            swap(board, leafPos, frogPos);
            printBoard(board);
            jumpSequence.addFirst(frogPos + 1);
            return true;
        }

        swap(board, leafPos, frogPos);
        return false;
    }

    private boolean inRange(char[] board, int pos) {
        return pos >= 0 && pos < board.length;
    }

    private void swap(char[] arr, int i, int j) {
        char tmp = arr[i];
        arr[i]   = arr[j];
        arr[j]   = tmp;
    }

    private void printBoard(char[] board) {
        for (char c : board) System.out.print(c);
        System.out.println();
    }

    private void printJumpSequence(LinkedList<Integer> seq) {
        java.util.Iterator<Integer> it = seq.iterator();
        while (it.hasNext()) {
            System.out.print(it.next());
            if (it.hasNext()) System.out.print(", ");
        }
        System.out.println();
    }
}
