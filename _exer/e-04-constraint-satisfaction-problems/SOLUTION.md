# Solution — N-Queens

Two independent solutions are provided. Both accept the same `-v`/`-q` flags.

---

## Solution 1 — Min-Conflicts (CSP / Local Search)

### Algorithm Overview

The solution uses **Min-Conflicts**, a local search heuristic for CSPs.

**Key Idea:** Instead of building a solution from scratch (backtracking), place all N queens immediately — one per column — then iteratively repair conflicts until none remain.

### Steps

1. **Smart initialisation** — place queens in a conflict-minimal pattern (interleaved even/odd columns: 1, 3, 5, …, 0, 2, 4, …). This ensures every column is occupied from the start and the initial state already has few conflicts.

2. **Repair loop** — repeat up to `2N` steps:
   - Pick the column whose queen has the **most conflicts** (ties broken randomly).
   - Move that queen to the row in that column with the **fewest conflicts** (ties broken randomly).
   - If total conflicts reach zero, the board is solved.

3. **Restart** — if `2N` repair steps did not find a solution, re-initialise and try again. In practice one or two attempts always suffice.

### Conflict Tracking — O(1) per Move

Naively recomputing conflicts after each move would be O(N). Instead, three integer arrays maintain conflict counts incrementally:

| Array | Size | Tracks |
|-------|------|--------|
| `queensInRow[r]` | N | number of queens in row r |
| `queensInMainDiag[d]` | 2N−1 | number of queens on main diagonal d = (N−1) + row − col |
| `queensInAntidiag[d]` | 2N−1 | number of queens on anti-diagonal d = row + col |

**Conflict count for queen at (row, col):**

```
conflicts(row, col) = queensInRow[row]
                    + queensInMainDiag[(N-1) + row - col]
                    + queensInAntidiag[row + col]
                    - 3 * hasQueen(row, col)   // subtract self-counts
```

When a queen moves from `(fromRow, col)` to `(toRow, col)`, all three arrays are updated in O(1).

### Complexity

| Phase | Time | Space |
|-------|------|-------|
| Initialisation | O(N) | O(N) |
| Single repair step | O(N) — scan all rows for min conflict | O(N) — candidate list |
| Total (expected) | **O(N)** — constant expected repair steps | **O(N)** |

### Randomness

The Mersenne Twister (`std::mt19937`) is seeded with the current system time, ensuring different random tie-breaking on each run. This prevents worst-case deterministic cycles.

### Source

- C++:    `cpp/NQueensCSP/NQueens.cpp`
- Java:   `java/NQueensCSP/src/NQueens.java`
- Python: `python/NQueensCSP/n_queens.py`

### Build & Run

**C++**
```bash
cd cpp/NQueensCSP
g++ -O2 -std=c++17 -o n_queens NQueens.cpp

echo 8     | ./n_queens          # prints board (N ≤ 10 default)
echo 8     | ./n_queens -q       # suppresses board
echo 10000 | ./n_queens          # no board (N > 10 default)
echo 10000 | ./n_queens -v       # forces board print
```

**Java**
```bash
cd java/NQueensCSP
mkdir -p out
javac -d out src/NQueens.java

echo 8     | java -cp out NQueens        # prints board
echo 8     | java -cp out NQueens -q     # suppresses board
echo 10000 | java -cp out NQueens        # no board
echo 10000 | java -cp out NQueens -v     # forces board print
```

**Python**
```bash
cd python/NQueensCSP

echo 8     | python3 n_queens.py         # prints board
echo 8     | python3 n_queens.py -q      # suppresses board
echo 10000 | python3 n_queens.py         # no board
echo 10000 | python3 n_queens.py -v      # forces board print
```

---

## Solution 2 — Deterministic O(N) Linear Construction

### Algorithm Overview

Places all N queens in **O(N) time** using a closed-form mathematical formula — no search, no backtracking, no conflict repair. Produces one fixed valid solution directly.

### Three Cases

The construction depends on **N mod 6**:

**Case 1: N even, N mod 6 ≠ 2**

```
Row m      → column 2m
Row N/2+m  → column 2m-1
for m = 1 .. N/2
```

Example (N = 8, mod 6 = 2 → actually Case 2; N = 4, mod 6 = 4 → Case 1):

| Row | Column |
|-----|--------|
| 1 | 2 |
| 2 | 4 |
| 3 | 1 |
| 4 | 3 |

**Case 2: N even, N mod 6 = 2**

```
Row m      → column 1 + (2(m-1) + N/2 - 1) mod N
Row N+1-m  → column N - (2(m-1) + N/2 - 1) mod N
for m = 1 .. N/2
```

This "rotated even" placement avoids diagonal conflicts that Case 1 would create for N ≡ 2 (mod 6).

**Case 3: N odd**

Recursively solve for N−1 (which is even, so Case 1 or 2 applies), then place the last queen at row N, column N. The corner position never conflicts with the N−1 solution because N−1 queens never occupy the last row or last column in their construction.

### Why It Works

The formulas are a well-known result from combinatorics. The even-N constructions guarantee:

- **Rows**: each of 1..N is used exactly once (by construction, we place one queen per row).
- **Columns**: each of 1..N is used exactly once (the sequences 2, 4, …, N, 1, 3, …, N−1 and its variant are permutations).
- **Diagonals**: verified algebraically; the two interleaved arithmetic progressions avoid all diagonal collisions.

### Complexity

| Metric | Value |
|--------|-------|
| Time | **O(N)** — exactly N assignments, no iterations |
| Space | **O(N)** — one array of N integers |
| Restarts | **0** — deterministic, always succeeds |
| Randomness | **none** — same output for same N |

This is strictly better than Min-Conflicts in the constant factor: there is no repair loop, no conflict re-evaluation, and no possibility of a restart.

### Source

- C++:    `cpp/NQueensLinear/NQueensLinear.cpp`
- Java:   `java/NQueensLinear/src/NQueensLinear.java`
- Python: `python/NQueensLinear/n_queens_linear.py`

### Build & Run

**C++**
```bash
cd cpp/NQueensCSP
g++ -O2 -std=c++17 -o n_queens_linear NQueensLinear.cpp

echo 8     | ./n_queens_linear        # prints board (N ≤ 10 default)
echo 8     | ./n_queens_linear -q     # suppresses board
echo 10000 | ./n_queens_linear        # no board (N > 10 default)
echo 10000 | ./n_queens_linear -v     # forces board print
```

**Java**
```bash
cd java/NQueensCSP
mkdir -p out
javac -d out src/NQueensLinear.java

echo 8     | java -cp out NQueensLinear        # prints board
echo 8     | java -cp out NQueensLinear -q     # suppresses board
echo 10000 | java -cp out NQueensLinear        # no board
echo 10000 | java -cp out NQueensLinear -v     # forces board print
```

**Python**
```bash
cd python/NQueensCSP

echo 8     | python3 n_queens_linear.py        # prints board
echo 8     | python3 n_queens_linear.py -q     # suppresses board
echo 10000 | python3 n_queens_linear.py        # no board
echo 10000 | python3 n_queens_linear.py -v     # forces board print
```

---

## Arguments (both solutions)

| Flag | Meaning |
|---|---|
| *(none)* | Default behaviour (see below) |
| `-v` | **Verbose** — always print the board |
| `-q` | **Quiet** — never print the board |

**Default board printing behaviour:**

- N ≤ 10 → board is printed (use `-q` to suppress)
- N > 10 → board is not printed (use `-v` to force printing)

Elapsed time is always printed to **stderr** regardless of flags.

---

## Known Limitations

- N = 2 and N = 3 have no solution; all implementations exit with an error.
- Board printing is O(N²) — only use `-v` for small N.
- Min-Conflicts: randomised, so timing varies between runs.
- Linear Construction: deterministic, but always produces the same one solution (not suitable if varied solutions are needed).
