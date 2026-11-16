# Intelligent Systems

> Course exercises covering search algorithms, clustering, classification, and neural networks.

---

## Exercises

| # | Topic | Exercise Sheet | Solutions |
|---|---|---|---|
| E01 | Uninformed (Blind) Search | [is-e-01 (2021-10-10)](_exer/e-01/is-e-01-(2021-10-10).pdf) | [C++](_exer/e-01/cpp/) · [Java](_exer/e-01/java/) · [Python](_exer/e-01/python/) |
| E02 | Informed (Heuristic) Search | [is-e-02 (2021-10-17)](_assets/IS%20E02%20(2021-10-17).pages) | — |
| E03 | Informed (Heuristic) Search | [is-e-03 (2021-10-24)](_assets/IS%20E03%20(2021-10-24).pages) | [C++](_exer/e-03/cpp/) · [Java](_exer/e-03/java/) · [Python](_exer/e-03/python/) |

---

## E01 — Frog Leap Puzzle

**Topic:** Uninformed (Blind) Search — DFS  
**Docs:** [Problem](_exer/e-01/PROBLEM.md) · [Solution](_exer/e-01/SOLUTION.md) · [Exercise Sheet](_exer/e-01/is-e-01-(2021-10-10).pdf)

Classic puzzle on a `2N+1` board: swap N yellow (left-side) and N green (right-side) frogs using steps and jumps.  
Two approaches — DFS search and an O(N²) analytical linear solution.

| Language | Linear | DFS |
|---|---|---|
| C++ | [`FrogLeapPuzzleLinear/main.cpp`](_exer/e-01/cpp/FrogLeapPuzzleLinear/main.cpp) | [`LeapFrogPuzzleDFS/main.cpp`](_exer/e-01/cpp/LeapFrogPuzzleDFS/main.cpp) |
| Java | [`FrogLeapPuzzleLinear.java`](_exer/e-01/java/FrogLeapPuzzleLinear/src/FrogLeapPuzzleLinear.java) | [`DFSSolution.java`](_exer/e-01/java/FrogLeapPuzzleDFS/src/DFSSolution.java) |
| Python | [`linear_solution.py`](_exer/e-01/python/FrogLeapPuzzleLinear/linear_solution.py) | [`dfs_solution.py`](_exer/e-01/python/FrogLeapPuzzleDFS/dfs_solution.py) |

---

## E03 — N-Sliding Blocks Puzzle

**Topic:** Informed (Heuristic) Search — IDA\*  
**Docs:** [Exercise Sheet](_assets/IS%20E03%20(2021-10-24).pages)

The classic N-puzzle (8-puzzle, 15-puzzle, …): slide tiles on an N×N grid to reach the goal configuration.  
Solved with IDA\* using the Manhattan distance heuristic.

| Language | IDA\* |
|---|---|
| C++ | [`NSlidingPuzzle.cpp`](_exer/e-03/cpp/SlidingBlocksPuzzleProceduralSolution/NSlidingPuzzle.cpp) |
| Java | [`NSlidingPuzzle.java`](_exer/e-03/java/SlidingBlocksPuzzleProceduralSolution/src/NSlidingPuzzle.java) |
| Python | [`n_sliding_puzzle.py`](_exer/e-03/python/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle.py) |

Sample puzzles: [`_exer/e-03/puzzles/`](_exer/e-03/puzzles/)
