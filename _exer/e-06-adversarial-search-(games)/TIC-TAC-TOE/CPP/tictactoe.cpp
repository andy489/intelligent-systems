#include <iostream>
#include <utility>
#include <algorithm>
#include <limits>

constexpr int INF = std::numeric_limits<int>::max();

struct Offset {
    int dr, dc;
    Offset operator-() const { return {-dr, -dc}; }
};

struct Position {
    unsigned row, col;
};

template <unsigned N>
class TicTacToe {
    static_assert(N >= 3 && N <= 4, "Board size must be 3 or 4");

    static constexpr char symbols[] = {'X', 'O'};
    static constexpr unsigned players = 2;

    char     board[N][N];
    bool     playerFirst; // true  → human is symbols[0] (X), CPU is symbols[1] (O)
    unsigned moves;       // number of moves made so far
    short    result;      // 0=ongoing, 1=symbols[0] won, 2=symbols[1] won, -1=draw

    // ---- winner detection ----

    // Check whether the line through lp along axis o contains N matching symbols.
    bool hasWinner(Position lp, Offset o) const {
        // Walk backward to find the start of the line.
        int r = (int)lp.row, c = (int)lp.col;
        while (r - o.dr >= 0 && r - o.dr < (int)N &&
               c - o.dc >= 0 && c - o.dc < (int)N)
        { r -= o.dr; c -= o.dc; }
        // Now walk forward N steps and verify all match lp's symbol.
        char sym = board[lp.row][lp.col];
        for (unsigned step = 0; step < N; ++step) {
            int nr = r + o.dr * (int)step;
            int nc = c + o.dc * (int)step;
            if (nr < 0 || nr >= (int)N || nc < 0 || nc >= (int)N) return false;
            if (board[nr][nc] != sym) return false;
        }
        return true;
    }

    bool hasWinner(Position lp) const {
        return hasWinner(lp, {0, 1})
            || hasWinner(lp, {1, 0})
            || hasWinner(lp, {1,  1})
            || hasWinner(lp, {1, -1});
    }

    // ---- move / undo ----

    void makeMove(Position p) {
        board[p.row][p.col] = symbols[moves % players];
        ++moves;
        if (hasWinner(p))
            result = (short)(((moves - 1) % players) + 1); // 1 or 2
        else if (moves >= N * N)
            result = -1;
        // else result stays 0
    }

    void undoMove(Position p) {
        --moves;
        board[p.row][p.col] = ' ';
        result = 0;
    }

    // ---- utility ----

    // Called only when result != 0.
    // Scores from the CPU's perspective (CPU is the maximiser).
    // Earlier win → higher absolute score.
    int utility() const {
        if (result < 0) return 0; // draw
        // moves% players at the time the winning move was made = (moves-1)%players
        // (moves was already incremented inside makeMove).
        bool cpuWon = (result == (playerFirst ? 2 : 1));
        int depth = (int)moves;
        return cpuWon ? (10 + (int)N * (int)N - depth)
                      : -(10 + (int)N * (int)N - depth);
    }

    // ---- alpha-beta minimax ----
    // Returns {best position, score}.
    std::pair<Position, int> alphabeta(int alpha, int beta) {
        if (result != 0)
            return {{0, 0}, utility()};

        // CPU maximises; human minimises.
        // moves%players == 0 → symbols[0] to move.
        // CPU is symbols[1] if playerFirst, else symbols[0].
        bool cpuToMove = (moves % players == (playerFirst ? 1u : 0u));

        // Seed with center so equal-scoring moves don't displace it.
        Position best{N / 2, N / 2};
        int value = cpuToMove ? -INF : INF;

        for (unsigned r = 0; r < N && alpha < beta; ++r) {
            for (unsigned c = 0; c < N && alpha < beta; ++c) {
                if (board[r][c] != ' ') continue;
                makeMove({r, c});
                auto [_, newValue] = alphabeta(alpha, beta);
                undoMove({r, c});
                if (cpuToMove) {
                    if (newValue > value) { value = newValue; best = {r, c}; }
                    if (value > alpha) alpha = value;
                } else {
                    if (newValue < value) { value = newValue; best = {r, c}; }
                    if (value < beta) beta = value;
                }
            }
        }
        return {best, value};
    }

    void makeBestMove() {
        makeMove(alphabeta(-INF, INF).first);
    }

public:
    TicTacToe(bool playerFirst = true)
        : playerFirst(playerFirst), moves(0), result(0)
    {
        for (auto& row : board)
            for (auto& cell : row)
                cell = ' ';
        if (!playerFirst) makeBestMove();
    }

    // Returns false if the move is illegal; otherwise executes human move then CPU reply.
    bool play(Position p) {
        if (p.row >= N || p.col >= N || board[p.row][p.col] != ' ')
            return false;
        makeMove(p);
        if (!result) makeBestMove();
        return true;
    }

    short winner() const { return result; }
    bool  playerGoesFirst() const { return playerFirst; }

    friend std::ostream& operator<<(std::ostream& os, const TicTacToe& g) {
        os << "\n  ";
        for (unsigned c = 0; c < N; ++c) os << (c + 1) << ' ';
        os << '\n';
        for (unsigned r = 0; r < N; ++r) {
            os << (r + 1) << ' ';
            for (unsigned c = 0; c < N; ++c)
                os << (g.board[r][c] == ' ' ? '.' : g.board[r][c])
                   << (c + 1 < N ? '|' : '\n');
        }
        os << '\n';
        return os;
    }
};

int main() {
    std::cout << "Do you want to play first? [y/n] ";
    char c;
    std::cin >> c;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    bool playerFirst;
    switch (c) {
        case 'y': case 'Y':
            playerFirst = true;
            break;
        default:
            std::cout << "Bad choice! You will play second!\n";
            [[fallthrough]];
        case 'n': case 'N':
            playerFirst = false;
    }

    TicTacToe<3> game(playerFirst);

    while (!game.winner()) {
        std::cout << game;
        while (true) {
            std::cout << "Where do you want to play? (row col): ";
            unsigned r, col;
            if (!(std::cin >> r >> col)) {
                if (std::cin.eof()) goto done;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Enter two numbers in [1," << 3 << "].\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (r < 1 || r > 3 || col < 1 || col > 3) {
                std::cout << "Values must be in [1,3].\n";
                continue;
            }
            if (!game.play({r - 1, col - 1})) {
                std::cout << "Cell already occupied. Try again.\n";
                continue;
            }
            break;
        }
    }
    done:

    std::cout << game;
    if (game.winner() < 0)
        std::cout << "Draw!\n";
    else
        std::cout << "You " << (game.winner() == (game.playerGoesFirst() ? 2 : 1) ? "lose" : "win") << "!\n";
}
