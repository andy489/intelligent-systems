// N-Queens — Deterministic O(N) Construction
//
// Builds one valid solution in O(N) time using a closed-form placement formula.
// No search, no backtracking, no conflict repair — every queen lands in its
// final position on the first try.
//
// ── Algorithm ────────────────────────────────────────────────────────────────
//
// The construction is based on three cases that depend on N mod 6:
//
//   Case A  N even, N mod 6 ≠ 2  (i.e. N mod 6 ∈ {0, 4})
//     First N/2 rows:   row m       → column 2m
//     Next  N/2 rows:   row N/2 + m → column 2m − 1       (m = 1 … N/2)
//     Produces two interleaved arithmetic sequences: even columns, then odd.
//
//   Case B  N even, N mod 6 = 2
//     First N/2 rows:   row m       → column 1 + (2(m−1) + N/2 − 1) mod N
//     Next  N/2 rows:   row N+1 − m → column N − (2(m−1) + N/2 − 1) mod N
//     A "rotated" variant that avoids the diagonal conflicts Case A creates
//     when N ≡ 2 (mod 6).
//
//   Case C  N odd
//     Solve for N−1 (even) with Case A or B, then place the last queen at
//     row N, column N.  The corner (N, N) never conflicts with the N−1
//     solution because neither Case A nor B ever uses column N.
//
// ── Complexity ───────────────────────────────────────────────────────────────
//   Time  O(N)  — exactly N array writes, two half-size loops
//   Space O(N)  — one vector<int> of size N
//
// ── Reference ────────────────────────────────────────────────────────────────
//   E. Hoffman, J. Loessi, R. Moore (1969). "Constructions for the Solution
//   of the m Queens Problem." Mathematics Magazine 42(2):66–72.

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

// colOfRow[r] = 1-indexed column of the queen in row r (0-indexed)
using Board = std::vector<int>;

// ── Construction ─────────────────────────────────────────────────────────────

// Case A: N even, N mod 6 ≠ 2
// Row m       → col 2m         (even columns, first half)
// Row N/2 + m → col 2m − 1    (odd  columns, second half)
static void caseA(int sz, int offset, Board& b) {
    int half = sz / 2;
    for (int m = 1; m <= half; ++m) {
        b[offset + m - 1]        = 2 * m;
        b[offset + half + m - 1] = 2 * m - 1;
    }
}

// Case B: N even, N mod 6 = 2
// Uses a modular shift to break the diagonals that Case A cannot avoid.
static void caseB(int sz, int offset, Board& b) {
    int half = sz / 2;
    for (int m = 1; m <= half; ++m) {
        // forward pass: rows 1 … N/2
        b[offset + m - 1]  = 1 + (2 * (m - 1) + half - 1) % sz;
        // reverse pass: rows N … N/2+1  (fills second half back-to-front)
        b[offset + sz - m] = sz - (2 * (m - 1) + half - 1) % sz;
    }
}

// Fills b[offset .. offset+sz-1] with a valid sz-queens solution.
static void place(int sz, int offset, Board& b) {
    if (sz % 2 == 0) {
        if (sz % 6 != 2) caseA(sz, offset, b);
        else              caseB(sz, offset, b);
    } else {
        // Odd: solve the even sub-problem, then append queen at (sz, sz).
        place(sz - 1, offset, b);
        b[offset + sz - 1] = sz;
    }
}

// ── Output ───────────────────────────────────────────────────────────────────

static void printBoard(int n, const Board& b) {
    for (int row = 0; row < n; ++row) {
        for (int col = 1; col <= n; ++col)
            std::cout << (col == b[row] ? '*' : '_');
        std::cout << '\n';
    }
}

// ── Entry point ──────────────────────────────────────────────────────────────

int main(int argc, char* argv[]) {
    bool quiet   = false;
    bool verbose = false;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "-q") quiet   = true;
        if (a == "-v") verbose = true;
    }

    int n;
    std::cin >> n;

    if (n == 2 || n == 3) {
        std::cerr << "No solution for N=" << n << '\n';
        return 1;
    }

    auto t0 = std::chrono::steady_clock::now();
    Board b(n);
    place(n, 0, b);
    double elapsed = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - t0).count();

    // Default: print board only when N ≤ 10 (printing is O(N²))
    if ((n <= 10) ? !quiet : verbose)
        printBoard(n, b);

    std::cerr << std::fixed;
    std::cerr.precision(6);
    std::cerr << "elapsed time: " << elapsed << " s\n";

    return 0;
}
