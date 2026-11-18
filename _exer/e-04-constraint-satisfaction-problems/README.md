# E04 — Constraint Satisfaction Problems

**Exercise Sheet:** [is-e-04-(2021-10-25)](is-e-04-(2021-10-25).pdf)  
**Slides:** [e-04-constraint-satisfaction-problems-slides](e-04-constraint-satisfaction-problems-slides.pdf)

## Problem

See [PROBLEM.md](PROBLEM.md) for the full problem description.

## Solution

See [SOLUTION.md](SOLUTION.md) for a detailed explanation of both algorithms.  
See [ANALYSIS.md](ANALYSIS.md) for performance results (Min-Conflicts / CSP solution).

---

## Two Solutions

### 1. Min-Conflicts (CSP / Local Search) — `NQueens` / `n_queens.py`

Treats N-Queens as a **Constraint Satisfaction Problem**. Places all queens immediately (one per column) then iteratively repairs conflicts using the Min-Conflicts heuristic until no conflicts remain.

- **Expected time: O(N)** — constant number of repair steps on average
- Randomised tie-breaking; may restart
- Handles arbitrarily large N (tested up to N = 100,000,000)

### 2. Deterministic Linear Construction — `NQueensLinear` / `n_queens_linear.py`

Uses a **closed-form mathematical formula** to place queens directly with zero search. Three cases based on N mod 6.

- **Exact time: O(N)** — fills N positions with simple arithmetic, no randomness, never restarts
- Produces one fixed valid solution per N
- Even faster in practice than Min-Conflicts (no repair loop overhead)

> **Note:** [ANALYSIS.md](ANALYSIS.md) benchmarks the **Min-Conflicts** (CSP) solution. The linear construction is deterministic and its runtime is simply proportional to N with negligible constants.

---

## Implementations

| Language | Min-Conflicts (CSP) | Linear Construction |
|---|---|---|
| C++ | `cpp/NQueens/NQueens.cpp` | `cpp/NQueens/NQueensLinear.cpp` |
| Java | `java/NQueens/src/NQueens.java` | `java/NQueens/src/NQueensLinear.java` |
| Python | `python/NQueens/n_queens.py` | `python/NQueens/n_queens_linear.py` |

---

## Arguments

Both solutions accept the same flags:

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
# Min-Conflicts
g++ -O2 -std=c++17 -o n_queens NQueens.cpp
# Linear Construction
g++ -O2 -std=c++17 -o n_queens_linear NQueensLinear.cpp
```

```bash
echo 8      | ./n_queens              # prints board (N ≤ 10)
echo 8      | ./n_queens -q           # suppress board
echo 10000  | ./n_queens              # no board (N > 10)
echo 10000  | ./n_queens -v           # force board print

echo 8      | ./n_queens_linear       # same flags apply
echo 10000  | ./n_queens_linear -v
```

### Java

```bash
cd java/NQueens
mkdir -p out
javac -d out src/NQueens.java src/NQueensLinear.java
```

```bash
echo 8      | java -cp out NQueens              # Min-Conflicts
echo 8      | java -cp out NQueensLinear        # Linear Construction
echo 10000  | java -cp out NQueens -v
echo 10000  | java -cp out NQueensLinear -v
```

### Python

```bash
cd python/NQueens
echo 8      | python3 n_queens.py               # Min-Conflicts
echo 8      | python3 n_queens_linear.py        # Linear Construction
echo 10000  | python3 n_queens.py -v
echo 10000  | python3 n_queens_linear.py -v
```

---

## Run Analysis (Min-Conflicts / CSP)

To benchmark the Min-Conflicts solution across all N values and regenerate `ANALYSIS.md`:

```bash
cd _exer/e-04-constraint-satisfaction-problems
python3 run_analysis.py
```

Runs N = 3, 4, 5, 6, 8, 100, 1K, 10K, 100K, 1M, 10M, 100M for C++, Java, and Python.

---

## Notes

- N = 1 is trivially solved (one queen, no conflicts).
- N = 2 and N = 3 have no valid solution; all implementations exit with an error.
- Board printing for large N is O(N²) — only use `-v` for small N.
