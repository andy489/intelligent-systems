# Intelligent Systems

> Course exercises covering search algorithms, clustering, classification, and neural networks.

---

## Exercises

| # | Topic | Exercise Sheet | Solutions |
|---|---|---|---|
| E01 | Uninformed Search — Warm-Up | [is-e-01 (2021-10-10)](_exer/e-01-uninformed-warmup/is-e-01-(2021-10-03).pdf) | [C++](_exer/e-01-uninformed-warmup/cpp/) · [Java](_exer/e-01-uninformed-warmup/java/) · [Python](_exer/e-01-uninformed-warmup/python/) |
| E02 | Uninformed (Blind) Search | [is-e-02 (2021-10-10)](_exer/e-02-uninformed-blind-search/is-e-02-(2021-10-10)-uninformed-blind-search.pdf) | — |
| E03 | Informed (Heuristic) Search — IDA* | [is-e-03 (2021-10-17)](_exer/e-03/is-e-03-(2021-10-17)-informed-heuristic-search.pdf) | [C++](_exer/e-03/cpp/) · [Java](_exer/e-03/java/) · [Python](_exer/e-03/python/) |

---

## E01 — Frog Leap Puzzle *(warm-up)*

**Topic:** Uninformed (Blind) Search — DFS  
**Docs:** [Problem](_exer/e-01-uninformed-warmup/PROBLEM.md) · [Solution](_exer/e-01-uninformed-warmup/SOLUTION.md) · [Exercise Sheet](_exer/e-01-uninformed-warmup/is-e-01-(2021-10-03).pdf)

Classic puzzle on a `2N+1` board: swap N yellow (left-side) and N green (right-side) frogs using steps and jumps.  
Two approaches — DFS search and an O(N²) analytical linear solution.

| Language | Linear | DFS |
|---|---|---|
| C++ | [`FrogLeapPuzzleLinear/main.cpp`](_exer/e-01-uninformed-warmup/cpp/FrogLeapPuzzleLinear/main.cpp) | [`LeapFrogPuzzleDFS/main.cpp`](_exer/e-01-uninformed-warmup/cpp/LeapFrogPuzzleDFS/main.cpp) |
| Java | [`FrogLeapPuzzleLinear.java`](_exer/e-01-uninformed-warmup/java/FrogLeapPuzzleLinear/src/FrogLeapPuzzleLinear.java) | [`DFSSolution.java`](_exer/e-01-uninformed-warmup/java/FrogLeapPuzzleDFS/src/DFSSolution.java) |
| Python | [`linear_solution.py`](_exer/e-01-uninformed-warmup/python/FrogLeapPuzzleLinear/linear_solution.py) | [`dfs_solution.py`](_exer/e-01-uninformed-warmup/python/FrogLeapPuzzleDFS/dfs_solution.py) |

---

## E02 — Uninformed (Blind) Search

**Topic:** Uninformed Search — BFS, DFS, UCS, DLS, IDS  
**Docs:** [Exercise Sheet](_exer/e-02-uninformed-blind-search/is-e-02-(2021-10-10)-uninformed-blind-search.pdf)

No code solutions yet.

---

## E03 — N-Sliding Blocks Puzzle

**Topic:** Informed (Heuristic) Search — IDA\*  
**Docs:** [Exercise Sheet](_exer/e-03/is-e-03-(2021-10-17)-informed-heuristic-search.pdf)

The classic N-puzzle (8-puzzle, 15-puzzle, …): slide tiles on an N×N grid to reach the goal configuration.  
Solved with IDA\* using Manhattan distance + linear conflict heuristic.

| Language | IDA\* |
|---|---|
| C++ | [`NSlidingPuzzle.cpp`](_exer/e-03/cpp/SlidingBlocksPuzzleProceduralSolution/NSlidingPuzzle.cpp) |
| Java | [`NSlidingPuzzle.java`](_exer/e-03/java/SlidingBlocksPuzzleProceduralSolution/src/NSlidingPuzzle.java) |
| Python | [`n_sliding_puzzle.py`](_exer/e-03/python/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle.py) |

Puzzles: [`data/puzzles/`](_exer/e-03/data/puzzles/) (160 puzzles: 2×2 through 10×10)  
Results: [`data/results/`](_exer/e-03/data/results/) (one `.txt` per puzzle — moves + elapsed time)  
Analysis: [`data/ANALYSIS.md`](_exer/e-03/data/ANALYSIS.md) (145 solved, 9 unsolvable, 1 timed out)  
Runner: [`run_all_puzzles.py`](_exer/e-03/run_all_puzzles.py)
