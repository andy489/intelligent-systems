import java.util.LinkedList;
import java.util.Scanner;

public class FrogLeapPuzzleLinear {

    private static final int LEFT_SINGLE = 1, RIGHT_SINGLE = -1, LEFT_DOUBLE = 2, RIGHT_DOUBLE = -2;

    private static char[] initBoard() {

        Scanner sc = new Scanner(System.in);
        System.out.print("N: ");

        int N = sc.nextInt();
        sc.close();
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

    private static void printState(char[] board) {
        for (char c : board) {
            System.out.print(c);
        }

        System.out.println();
    }

    private static void printJumpSequence(LinkedList<Integer> ll) {
        java.util.Iterator<Integer> it = ll.iterator();
        while (it.hasNext()) {
            Integer val = it.next();
            if (it.hasNext()) {
                System.out.print(val + ", ");
            } else {
                System.out.println(val);
            }
        }
        if (ll.isEmpty()) {
            System.out.println();
        }
    }

    private static void swap(char[] board, int fromIndex, int toIndex) {
        char temp = board[fromIndex];
        board[fromIndex] = board[toIndex];
        board[toIndex] = temp;
    }

    private static int jump(char[] board, int leafPos, int direction) {
        swap(board, leafPos, leafPos + direction);
        leafPos += direction;
        return leafPos;
    }

    private static int jumpsFirst(char[] board, int leafPos, LinkedList<Integer> jumpSequence, int i,
                                  int rightDouble, int rightSingle) {

        for (int j = 0; j < i - 1; j++) {
            leafPos = jump(board, leafPos, rightDouble);
            printState(board);
            jumpSequence.addLast(leafPos + 1);
        }

        leafPos = jump(board, leafPos, rightSingle);
        printState(board);
        jumpSequence.addLast(leafPos + 1);
        return leafPos;
    }

    private static int jumpsSecond(char[] board, int leafPos, LinkedList<Integer> jumpSequence, int i,
                                   int leftSingle, int leftDouble) {
        leafPos = jump(board, leafPos, leftSingle);
        printState(board);
        jumpSequence.addLast(leafPos + 1);

        for (int j = 1; j < i; j++) {
            leafPos = jump(board, leafPos, leftDouble);
            printState(board);
            jumpSequence.addLast(leafPos + 1);
        }
        return leafPos;
    }

    private static int processFirstHalf(char[] board, int N, int leafPos, LinkedList<Integer> jumpSequence) {
        for (int i = 1; i <= N; i++) {
            if (i % 2 == 0) {
                leafPos = jumpsFirst(board, leafPos, jumpSequence, i, LEFT_DOUBLE, LEFT_SINGLE);
            } else {
                leafPos = jumpsFirst(board, leafPos, jumpSequence, i, RIGHT_DOUBLE, RIGHT_SINGLE);
            }
        }
        return leafPos;
    }

    private static int processMiddle(char[] board, int N, int leafPos, LinkedList<Integer> jumpSequence) {
        if (N % 2 == 0) {
            for (int j = 0; j < N; j++) {
                leafPos = jump(board, leafPos, RIGHT_DOUBLE);
                printState(board);
                jumpSequence.addLast(leafPos + 1);
            }
        } else {
            for (int j = 0; j < N; j++) {
                leafPos = jump(board, leafPos, LEFT_DOUBLE);
                printState(board);
                jumpSequence.addLast(leafPos + 1);
            }
        }

        return leafPos;
    }

    private static void processSecondHalf(char[] board, int N, int leafPos, LinkedList<Integer> jumpSequence) {
        for (int i = N; i > 0; i--) {
            if (i % 2 == 0) {
                leafPos = jumpsSecond(board, leafPos, jumpSequence, i, LEFT_SINGLE, LEFT_DOUBLE);
            } else {
                leafPos = jumpsSecond(board, leafPos, jumpSequence, i, RIGHT_SINGLE, RIGHT_DOUBLE);
            }
        }
    }

    private static void linearSolution(char[] board, int N, LinkedList<Integer> jumpSequence) {
        printState(board);

        int leafPos = processFirstHalf(board, N, N, jumpSequence);
        leafPos = processMiddle(board, N, leafPos, jumpSequence);
        processSecondHalf(board, N, leafPos, jumpSequence);
    }

    public static void main(String[] args) {

        char[] board = initBoard();

        LinkedList<Integer> jumpSequence = new LinkedList<>();

        linearSolution(board, board.length / 2, jumpSequence);
        System.out.print("Jump sequence: ");
        printJumpSequence(jumpSequence);
        System.out.println("Total jumps: " + jumpSequence.size());
        System.out.println("Success!");
    }
}