import java.util.LinkedList;
import java.util.Scanner;

public class FrogLeapPuzzleLinear {


    public static void main(String[] args) {


        char[] board = initBoard();

        LinkedList<Integer> jumpSequence = new LinkedList<>();

        linearSolution(board, board.length / 2, jumpSequence);
        System.out.print("Jump Sequence: ");
        print(jumpSequence);
        System.out.println("Total jumps: " + jumpSequence.size());
        System.out.println("Success!");
    }

    private static char[] initBoard() {

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

    private static void linearSolution(char[] board, int N, LinkedList<Integer> jumpSequence) {
        print(board);

        int emptyStatePos = N;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < i + 1; j++) {
                emptyStatePos = jumps(board, emptyStatePos, i, j, jumpSequence);
            }
        }

        for (int i = 0; i < N; i++) { // jump left or right
            if (N % 2 == 0) { // jump left if n is even
                swap(board, emptyStatePos, emptyStatePos - 2);
                emptyStatePos = emptyStatePos - 2;
                jumpSequence.add(emptyStatePos + 1);
            } else { // jump right if n is odd
                swap(board, emptyStatePos, emptyStatePos + 2);
                emptyStatePos = emptyStatePos + 2;
                jumpSequence.add(emptyStatePos + 1);
            }
            print(board);
        }

        for (int i = N - 1; i >= 0; i--) {
            for (int j = i; j >= 0; j--) {
                emptyStatePos = jumps(board, emptyStatePos, i, j, jumpSequence);
            }
        }
    }

    private static int jumps(char[] board, int emptyStatePos, int i, int j, LinkedList<Integer> jumpSequence) {
        if (i % 2 == 0) { // jump left
            if (j < i) {
                swap(board, emptyStatePos, emptyStatePos - 2);
                emptyStatePos = emptyStatePos - 2;
                jumpSequence.add(emptyStatePos + 1);
            } else {
                swap(board, emptyStatePos, emptyStatePos - 1);
                emptyStatePos = emptyStatePos - 1;
                jumpSequence.add(emptyStatePos + 1);
            }
        } else { // jump right
            if (j < i) {
                swap(board, emptyStatePos, emptyStatePos + 2);
                emptyStatePos = emptyStatePos + 2;
                jumpSequence.add(emptyStatePos + 1);
            } else {
                swap(board, emptyStatePos, emptyStatePos + 1);
                emptyStatePos = emptyStatePos + 1;
                jumpSequence.add(emptyStatePos + 1);
            }
        }
        print(board);
        return emptyStatePos;
    }

    private static void swap(char[] board, int fromIndex, int toIndex) {
        char temp = board[fromIndex];
        board[fromIndex] = board[toIndex];
        board[toIndex] = temp;
    }

    private static void print(char[] board) {
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