# E02 — Uninformed Search

**Problem:** Frog Leap Puzzle  
**Topic:** Uninformed Search — DFS  
**Exercise Sheets:** [Part 1](is-e-02-pt-1-(2021-10-10).pdf) · [Part 2](is-e-02-pt-2-(2021-10-10).pdf)

Play the puzzle online: [NIEHS Frog Leap Game](https://www.niehs.nih.gov/health/kids/games/brainteasers/frog-leap)  
Full problem statement and rules: [PROBLEM.md](PROBLEM.md)  
Algorithm explanations and pseudocode: [SOLUTION.md](SOLUTION.md)

---

## Complexity

Both solutions produce the **optimal** number of moves: **N² + 2N**.

| Solution | Time | Space |
|---|---|---|
| Linear | O(N²) — executes N² + 2N moves analytically, no search | O(N) — board of size 2N+1 |
| DFS | O(N²) — visits each state once on the unique optimal path | O(N²) — recursion depth N² + 2N |

The fixed move-ordering (`step-L, jump-L, step-R, jump-R`) guides DFS directly to the optimal path without backtracking.

### Practical performance (measured wall-clock)

| N | Linear | DFS |
|---|---|---|
| 5 | < 1 ms | < 1 ms |
| 10 | < 1 ms | < 1 ms |
| 20 | < 1 ms | ~1 ms |
| 25 | < 1 ms | ~5 ms |

DFS practical limit is ~N = 25 (recursion depth N² + 2N ≈ 675 frames). The linear solution has no practical upper bound.

---

## Implementations

| Language | Linear | DFS |
|---|---|---|
| C++ | `cpp/FrogLeapPuzzleLinear/` | `cpp/LeapFrogPuzzleDFS/` |
| Java | `java/FrogLeapPuzzleLinear/` | `java/FrogLeapPuzzleDFS/` |
| Python | `python/FrogLeapPuzzleLinear/` | `python/FrogLeapPuzzleDFS/` |

### Running

**C++**
```bash
# Linear
cmake -S cpp/FrogLeapPuzzleLinear -B cpp/FrogLeapPuzzleLinear/build && cmake --build cpp/FrogLeapPuzzleLinear/build

# DFS
cmake -S cpp/LeapFrogPuzzleDFS -B cpp/LeapFrogPuzzleDFS/build && cmake --build cpp/LeapFrogPuzzleDFS/build
```

**Java**
```bash
# Linear
cd java/FrogLeapPuzzleLinear
javac src/FrogLeapPuzzleLinear.java -d out && java -cp out FrogLeapPuzzleLinear

# DFS
cd java/FrogLeapPuzzleDFS
javac src/*.java -d out && java -cp out Main
```

**Python**
```bash
# Linear
python3 python/FrogLeapPuzzleLinear/linear_solution.py

# DFS
python3 python/FrogLeapPuzzleDFS/dfs_solution.py
```

All programs read `N` from stdin.

### Example output (N = 2)

```
LL_RR
L_LRR
LRL_R
LRLR_
LR_RL
_RLRL
R_LRL
RRL_L
RR_LL
```

Goal state `RR_LL` reached in 2² + 2×2 = **8 moves**.
