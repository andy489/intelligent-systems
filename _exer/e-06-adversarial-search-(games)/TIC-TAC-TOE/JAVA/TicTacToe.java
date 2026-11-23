import java.util.Scanner;

public class TicTacToe {

    private static final int N = 3;
    private static final char[] SYMBOLS = {'X', 'O'};
    private static final int INF = Integer.MAX_VALUE / 2;

    private final char[][] board = new char[N][N];
    private final boolean playerFirst;
    private int moves = 0;
    private int result = 0; // 0=ongoing, 1=X won, 2=O won, -1=draw

    // ---- constructor ----

    TicTacToe(boolean playerFirst) {
        this.playerFirst = playerFirst;
        for (char[] row : board)
            java.util.Arrays.fill(row, ' ');
        if (!playerFirst) makeBestMove();
    }

    // ---- winner detection ----

    private boolean hasWinner(int lpRow, int lpCol, int dr, int dc) {
        // Walk backward to line start.
        int r = lpRow, c = lpCol;
        while (r - dr >= 0 && r - dr < N && c - dc >= 0 && c - dc < N) {
            r -= dr; c -= dc;
        }
        char sym = board[lpRow][lpCol];
        for (int step = 0; step < N; step++) {
            int nr = r + dr * step, nc = c + dc * step;
            if (nr < 0 || nr >= N || nc < 0 || nc >= N) return false;
            if (board[nr][nc] != sym) return false;
        }
        return true;
    }

    private boolean hasWinner(int row, int col) {
        return hasWinner(row, col,  0,  1)
            || hasWinner(row, col,  1,  0)
            || hasWinner(row, col,  1,  1)
            || hasWinner(row, col,  1, -1);
    }

    // ---- move / undo ----

    private void makeMove(int row, int col) {
        board[row][col] = SYMBOLS[moves % 2];
        ++moves;
        if (hasWinner(row, col))
            result = (moves - 1) % 2 + 1;
        else if (moves >= N * N)
            result = -1;
    }

    private void undoMove(int row, int col) {
        --moves;
        board[row][col] = ' ';
        result = 0;
    }

    // ---- utility ----

    private int utility() {
        if (result < 0) return 0;
        boolean cpuWon = (result == (playerFirst ? 2 : 1));
        return cpuWon ? (10 + N * N - moves) : -(10 + N * N - moves);
    }

    // ---- alpha-beta minimax ----
    // Returns int[]{bestRow, bestCol, score}.

    private int[] alphabeta(int alpha, int beta) {
        if (result != 0)
            return new int[]{0, 0, utility()};

        boolean cpuToMove = (moves % 2 == (playerFirst ? 1 : 0));
        int bestR = N / 2, bestC = N / 2;
        int value = cpuToMove ? -INF : INF;

        outer:
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (board[r][c] != ' ') continue;
                makeMove(r, c);
                int newValue = alphabeta(alpha, beta)[2];
                undoMove(r, c);
                if (cpuToMove) {
                    if (newValue > value) { value = newValue; bestR = r; bestC = c; }
                    if (value > alpha) alpha = value;
                } else {
                    if (newValue < value) { value = newValue; bestR = r; bestC = c; }
                    if (value < beta) beta = value;
                }
                if (alpha >= beta) break outer;
            }
        }
        return new int[]{bestR, bestC, value};
    }

    private void makeBestMove() {
        int[] move = alphabeta(-INF, INF);
        makeMove(move[0], move[1]);
    }

    // ---- public API ----

    boolean play(int row, int col) {
        if (row < 0 || row >= N || col < 0 || col >= N || board[row][col] != ' ')
            return false;
        makeMove(row, col);
        if (result == 0) makeBestMove();
        return true;
    }

    int winner() { return result; }
    boolean playerGoesFirst() { return playerFirst; }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("\n  ");
        for (int c = 0; c < N; c++) sb.append(c + 1).append(' ');
        sb.append('\n');
        for (int r = 0; r < N; r++) {
            sb.append(r + 1).append(' ');
            for (int c = 0; c < N; c++) {
                sb.append(board[r][c] == ' ' ? '.' : board[r][c]);
                sb.append(c + 1 < N ? '|' : '\n');
            }
        }
        sb.append('\n');
        return sb.toString();
    }

    // ---- main ----

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Do you want to play first? [y/n] ");
        String line = sc.nextLine().trim();
        char c = line.isEmpty() ? 'n' : line.charAt(0);

        boolean playerFirst;
        if (c == 'y' || c == 'Y') {
            playerFirst = true;
        } else {
            if (c != 'n' && c != 'N')
                System.out.println("Bad choice! You will play second!");
            playerFirst = false;
        }

        TicTacToe game = new TicTacToe(playerFirst);

        while (game.winner() == 0) {
            System.out.print(game);
            while (true) {
                System.out.print("Where do you want to play? (row col): ");
                if (!sc.hasNextLine()) return;
                String[] parts = sc.nextLine().trim().split("\\s+");
                if (parts.length < 2) {
                    System.out.println("Invalid input. Enter two numbers in [1,3].");
                    continue;
                }
                int r, col;
                try {
                    r   = Integer.parseInt(parts[0]);
                    col = Integer.parseInt(parts[1]);
                } catch (NumberFormatException e) {
                    System.out.println("Invalid input. Enter two numbers in [1,3].");
                    continue;
                }
                if (r < 1 || r > 3 || col < 1 || col > 3) {
                    System.out.println("Values must be in [1,3].");
                    continue;
                }
                if (!game.play(r - 1, col - 1)) {
                    System.out.println("Cell already occupied. Try again.");
                    continue;
                }
                break;
            }
        }

        System.out.print(game);
        int w = game.winner();
        if (w < 0)
            System.out.println("Draw!");
        else
            System.out.println("You " + (w == (game.playerGoesFirst() ? 2 : 1) ? "lose" : "win") + "!");
    }
}
