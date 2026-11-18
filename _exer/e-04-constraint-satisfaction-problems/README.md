# E04 — Constraint Satisfaction Problems

**Exercise Sheet:** [is-e-04-(2021-10-25)](is-e-04-(2021-10-25).pdf)  
**Slides:** [e-04-constraint-satisfaction-problems-slides](e-04-constraint-satisfaction-problems-slides.pdf)

## Problem

See [PROBLEM.md](PROBLEM.md) for the full problem description.

## Solution

See [SOLUTION.md](SOLUTION.md) for a detailed explanation of the algorithm.  
See [ANALYSIS.md](ANALYSIS.md) for performance results across all three implementations.

## Implementations

| Language | Source |
|---|---|
| C++ | `cpp/NQueens/NQueens.cpp` |
| Java | `java/NQueens/src/NQueens.java` |
| Python | `python/NQueens/n_queens.py` |

All three are algorithmically identical: Min-Conflicts local search with O(1) incremental conflict tracking.

---

## Arguments

All three implementations accept the same flags:

| Flag | Effect |
|---|---|
| *(none)* | Default: print board if N ≤ 10, skip if N > 10 |
| `-v` | **Verbose** — always print the board (even for large N) |
| `-q` | **Quiet** — never print the board |

Elapsed time is always written to **stderr**, regardless of flags.

---

## Build & Run

### C++

```bash
cd cpp/NQueens
g++ -O2 -std=c++17 -o n_queens NQueens.cpp
```

A pre-built binary `n_queens` is already present in `cpp/NQueens/`.

```bash
# N reads from stdin
echo 8     | ./n_queens        # prints board (N ≤ 10)
echo 8     | ./n_queens -q     # suppress board
echo 10000 | ./n_queens        # no board (N > 10)
echo 10000 | ./n_queens -v     # force board print
```

### Java

```bash
cd java/NQueens
mkdir -p out
javac -d out src/NQueens.java
```

```bash
echo 8     | java -cp out NQueens        # prints board
echo 8     | java -cp out NQueens -q     # suppress board
echo 10000 | java -cp out NQueens        # no board
echo 10000 | java -cp out NQueens -v     # force board print
```

### Python

```bash
cd python/NQueens
```

```bash
echo 8      | python3 n_queens.py         # prints board
echo 8      | python3 n_queens.py -q      # suppress board
echo 10000  | python3 n_queens.py         # no board
echo 10000  | python3 n_queens.py -v      # force board print
```

---

## Run All (Analysis)

To benchmark all three implementations across a range of N values and regenerate `ANALYSIS.md`:

```bash
cd _exer/e-04-constraint-satisfaction-problems
python3 run_analysis.py
```

This runs N = 3, 4, 5, 6, 8, 100, 1 000, 10 000, 100 000, 1 000 000, 10 000 000, 100 000 000 for each language and writes the results table to `ANALYSIS.md`.

---

## Notes

- N = 1 is trivially solved (one queen, no conflicts).
- N = 2 and N = 3 have no valid solution; all implementations exit with an error.
- Board printing for large N is O(N²) — only use `-v` for small N.
