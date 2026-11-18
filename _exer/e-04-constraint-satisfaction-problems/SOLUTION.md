# Solution — N-Queens (Min-Conflicts / Local Search)

## Algorithm Overview

The solution uses **Min-Conflicts**, a local search heuristic for CSPs.

### Key Idea

Instead of building a solution from scratch (backtracking), place all N queens immediately — one per column — and then iteratively repair conflicts until none remain.

### Steps

1. **Smart initialisation** — place queens in a conflict-minimal pattern (interleaved even/odd columns: 1, 3, 5, …, 0, 2, 4, …). This ensures every column is occupied from the start and the initial state already has few conflicts.

2. **Repair loop** — repeat up to `2N` steps:
   - Pick the column whose queen has the **most conflicts** (ties broken randomly).
   - Move that queen to the row in that column with the **fewest conflicts** (ties broken randomly).
   - If total conflicts reach zero, the board is solved.

3. **Restart** — if `2N` repair steps did not find a solution, re-initialise and try again. In practice one or two attempts always suffice.

## Conflict Tracking — O(1) per Move

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

## Complexity

| Phase | Time | Space |
|-------|------|-------|
| Initialisation | O(N) | O(N) |
| Single repair step | O(N) — scan all rows for min conflict | O(N) — candidate list |
| Total (expected) | **O(N)** — constant expected repair steps | **O(N)** |

The O(N) scan per step is the bottleneck. For N = 10 000 this is ~10 000 × a few hundred steps = well within one second.

## Why This Works at N = 10 000

- No recursion, no backtracking — stack depth is O(1).
- All data structures are flat `std::vector<unsigned>` — cache-friendly.
- Randomised tie-breaking avoids cycles.
- The interleaved initialisation keeps the initial conflict density low, so the repair loop converges in very few steps (empirically O(1) on average for large N).

## Randomness

The Mersenne Twister (`std::mt19937`) is seeded with the current system time, ensuring different random tie-breaking on each run. This prevents worst-case deterministic cycles.

## Known Limitations

- N = 2 and N = 3 have no solution; the constructor throws `std::logic_error`.
- Board printing (`std::cout << board`) is O(N²) and is commented out by default — impractical for large N.
- `unsigned` counters limit N to roughly 4 billion, well beyond any practical requirement.

## Source

- C++:    `cpp/NQueens/NQueens.cpp`
- Java:   `java/NQueens/src/NQueens.java`
- Python: `python/NQueens/n_queens.py`

## Build & Run

**C++**
```bash
cd cpp/NQueens
g++ -O2 -std=c++17 -o n_queens NQueens.cpp
echo 10000 | ./n_queens -q
# elapsed time: 0.XXX s
```

**Java**
```bash
cd java/NQueens
mkdir -p out
javac -d out src/NQueens.java
echo 10000 | java -cp out NQueens -q
# elapsed time: 0.XXX s
```

**Python**
```bash
cd python/NQueens
echo 10000 | python3 n_queens.py -q
# elapsed time: X.XXX s
```
