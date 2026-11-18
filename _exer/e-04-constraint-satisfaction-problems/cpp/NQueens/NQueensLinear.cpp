// N-Queens — Deterministic O(N) Construction (Explicit Formula)
//
// Places all N queens in O(N) time using a closed-form formula — no search,
// no backtracking, no conflict repair. Produces one valid solution directly.
//
// Three cases based on N mod 6:
//
//   Case 1: N even, N mod 6 != 2
//     Row m    → queen at column 2m
//     Row N/2+m → queen at column 2m-1
//     for m = 1 .. N/2
//
//   Case 2: N even, N mod 6 == 2
//     Row m      → queen at column 1 + (2(m-1) + N/2 - 1) mod N
//     Row N+1-m  → queen at column N - (2(m-1) + N/2 - 1) mod N
//     for m = 1 .. N/2
//
//   Case 3: N odd
//     Solve for N-1 (even) using Case 1 or 2, then add queen at (N, N).
//
// Reference: explicit construction from combinatorics literature.

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

// Build the solution into a vector: result[col] = row (1-indexed)
static void methodOne(int n, std::vector<int>& sol) {
    // first N/2 rows: row m → col 2m
    for (int m = 1; m <= n / 2; ++m) sol.push_back(2 * m);
    // next N/2 rows: row N/2+m → col 2m-1
    for (int m = 1; m <= n / 2; ++m) sol.push_back(2 * m - 1);
}

static void methodTwo(int n, std::vector<int>& sol) {
    for (int m = 1; m <= n / 2; ++m)
        sol.push_back(1 + (2 * (m - 1) + n / 2 - 1) % n);
    for (int m = n / 2; m >= 1; --m)
        sol.push_back(n - (2 * (m - 1) + n / 2 - 1) % n);
}

static void nQueens(int n, std::vector<int>& sol) {
    if (n % 2 == 0) {
        if (n % 6 != 2)      methodOne(n, sol);
        else                  methodTwo(n, sol);
    } else {
        nQueens(n - 1, sol);
        sol.push_back(n);   // queen at (n, n)
    }
}

static void printBoard(int n, const std::vector<int>& colOfRow) {
    for (int row = 0; row < n; ++row) {
        for (int col = 1; col <= n; ++col)
            std::cout << (col == colOfRow[row] ? '*' : '_');
        std::cout << '\n';
    }
}

int main(int argc, char* argv[]) {
    bool quiet   = false;
    bool verbose = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-q") quiet   = true;
        if (std::string(argv[i]) == "-v") verbose = true;
    }

    int n;
    std::cin >> n;

    if (n == 2 || n == 3) {
        std::cerr << "No solution for N=" << n << '\n';
        return 1;
    }

    auto start = std::chrono::steady_clock::now();
    std::vector<int> sol;
    sol.reserve(n);
    nQueens(n, sol);
    auto end = std::chrono::steady_clock::now();

    bool print_board = (n <= 10) ? !quiet : verbose;
    if (print_board) printBoard(n, sol);

    std::cerr.precision(6);
    std::cerr << "elapsed time: " << std::fixed
              << std::chrono::duration<double>(end - start).count() << " s\n";

    return 0;
}
