# E01 — Frog Leap Puzzle

Play the puzzle online: [NIEHS Frog Leap Game](https://www.niehs.nih.gov/health/kids/games/brainteasers/frog-leap)

See [PROBLEM.md](PROBLEM.md) for the full problem statement and rules.  
See [SOLUTION.md](SOLUTION.md) for algorithm explanations and pseudocode.

---

## Complexity

Both solutions produce the **optimal** number of moves: **N² + 2N**.

### Linear Solution

| | Complexity |
|---|---|
| Time | O(N²) — executes exactly N² + 2N moves |
| Space | O(N) — board of size 2N + 1; no call stack growth |
| Move count | N² + 2N (optimal) |

No search, no backtracking. The move sequence is derived analytically from the pattern structure.

### DFS Solution

| | Complexity |
|---|---|
| Time | O(N²) — visits each state at most once on the unique optimal path |
| Space | O(N²) — recursion depth = N² + 2N; each frame is O(1) extra |
| Move count | N² + 2N (optimal) |

The fixed move-ordering in the search (`step-L, jump-L, step-R, jump-R`) guides DFS directly to the optimal path without wasted backtracking.

### Practical Performance (measured wall-clock)

| N | Linear | DFS |
|---|---|---|
| 5 | < 1 ms | < 1 ms |
| 10 | < 1 ms | < 1 ms |
| 20 | < 1 ms | ~1 ms |
| 25 | < 1 ms | ~5 ms |

The DFS limit is ~N = 25 due to recursion depth (N² + 2N ≈ 675 frames). The linear solution has no practical upper bound.

---

## Implementations

| Language | Linear | DFS |
|---|---|---|
| Java | `java/FrogLeapPuzzleLinear/` | `java/FrogLeapPuzzleDFS/` |
| Python | `python/FrogLeapPuzzleLinear/` | `python/FrogLeapPuzzleDFS/` |
| C++ | `cpp/FrogLeapPuzzleLinear/` | `cpp/LeapFrogPuzzleDFS/` |

### Running

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
python python/FrogLeapPuzzleLinear/linear_solution.py

# DFS
python python/FrogLeapPuzzleDFS/main.py
```

**C++**
```bash
# Linear
cmake -S cpp/FrogLeapPuzzleLinear -B cpp/FrogLeapPuzzleLinear/build && cmake --build cpp/FrogLeapPuzzleLinear/build

# DFS
cmake -S cpp/LeapFrogPuzzleDFS -B cpp/LeapFrogPuzzleDFS/build && cmake --build cpp/LeapFrogPuzzleDFS/build
```

All programs read `N` from stdin.

### Example Output (N = 2)

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
RRRLL   ← wrong, shown for illustration; actual goal: RR_LL → RRLL_... 
```

For N = 2 the goal state is `RR_LL` and total moves = 2² + 2×2 = 8.
