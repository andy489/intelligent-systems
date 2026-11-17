# Intelligent Systems

> Course exercises covering search algorithms, constraint satisfaction, genetic algorithms, games, and machine learning.

**Textbook:** Stuart Russell & Peter Norvig — *Artificial Intelligence: A Modern Approach*, Prentice Hall ([PDF](artificial-intelligence-a-modern-approach-3rd-edition.pdf) · [aima.eecs.berkeley.edu](http://aima.eecs.berkeley.edu/))  
**Algorithms:** [algorithms-pseudo-code.pdf](algorithms-pseudo-code.pdf)  
**Figures:** [global-figures.pdf](global-figures.pdf)

---

## Course Topics

### Problem Solving and Search
- Uninformed (Blind) Search
- Informed (Heuristic) Search
- Constraint Satisfaction Problems
- Genetic Algorithms
- Games

### Machine Learning
- k-Nearest Neighbours
- Naive Bayes Classifier
- Decision Tree
- kMeans
- Neural Networks

---

## Exercise Sheets

| # | Topic | Problem | Exercise Sheet | Solutions |
|---|---|---|---|---|
| E12 | Neural Networks | — | — | — |
| E11 | kMeans | — | — | — |
| E10 | Decision Tree | — | — | — |
| E09 | Naive Bayes Classifier | — | — | — |
| E08 | k-Nearest Neighbours | — | — | — |
| E07 | Introduction to Machine Learning | — | — | — |
| E06 | Games | — | — | — |
| E05 | Genetic Algorithms | — | — | — |
| E04 | Constraint Satisfaction Problems | — | [slides-(2021-10-25)](_exer/e-04-constraint-satisfaction-problems/e-04-constraint-satisfaction-problems-slides.pdf)<br>[is-e-04-(2021-10-25)](_exer/e-04-constraint-satisfaction-problems/is-e-04-(2021-10-25).pdf) | — |
| E03 | Informed (Heuristic) Search — IDA* | N-Sliding Blocks Puzzle | [slides-(2021-10-18)](_exer/e-03-informed-heuristic-search/is-e-03-informed-search-slides.pdf)<br>[romania-map](_exer/e-03-informed-heuristic-search/romania-map.pdf)<br>[is-e-03-(2021-10-18)](_exer/e-03-informed-heuristic-search/is-e-03-(2021-10-18).pdf) | [C++](_exer/e-03-informed-heuristic-search/cpp/) · [Java](_exer/e-03-informed-heuristic-search/java/) · [Python](_exer/e-03-informed-heuristic-search/python/) |
| E02 | Uninformed (Blind) Search | Frog Leap Puzzle | [slides-(2021-10-07)](_exer/e-02-uninformed-blind-search/is-e-02-uninformed-search-slides.pdf)<br>[is-e-02-pt-1-(2021-10-07)](_exer/e-02-uninformed-blind-search/is-e-02-pt-1-(2021-10-07).pdf)<br>[is-e-02-pt-2-(2021-10-07)](_exer/e-02-uninformed-blind-search/is-e-02-pt-2-(2021-10-07).pdf) | [C++](_exer/e-02-uninformed-blind-search/cpp/) · [Java](_exer/e-02-uninformed-blind-search/java/) · [Python](_exer/e-02-uninformed-blind-search/python/) |
| E01 | Introduction | — | [slides-(2021-10-07)](_exer/e-01-introduction/is-e-01-intro-slides.pdf) · [is-e-01-(2021-10-07)](_exer/e-01-introduction/is-e-01-(2021-10-07).pdf) | [README](_exer/e-01-introduction/README.md) |

> `is` = Intelligent Systems · `e` = Exercise · `pt` = Part · `(YYYY-MM-DD)` = date of issue · `slides` = lecture slides

---

## E12 — Neural Networks

**Problem:** —  
**Docs:** —

No content yet.

---

## E11 — kMeans

**Problem:** —  
**Docs:** —

No content yet.

---

## E10 — Decision Tree

**Problem:** —  
**Docs:** —

No content yet.

---

## E09 — Naive Bayes Classifier

**Problem:** —  
**Docs:** —

No content yet.

---

## E08 — k-Nearest Neighbours

**Problem:** —  
**Docs:** —

No content yet.

---

## E07 — Introduction to Machine Learning

**Problem:** —  
**Docs:** —

No content yet.

---

## E06 — Games

**Problem:** —  
**Docs:** —

No content yet.

---

## E05 — Genetic Algorithms

**Problem:** —  
**Docs:** —

No content yet.

---

## E04 — Constraint Satisfaction Problems

**Problem:** —  
**Docs:** [Exercise Sheet](_exer/e-04-constraint-satisfaction-problems/is-e-04-(2021-10-25).pdf) · [Slides-(2021-10-25)](_exer/e-04-constraint-satisfaction-problems/e-04-constraint-satisfaction-problems-slides.pdf)

No code solutions yet.

---

## E03 — Informed (Heuristic) Search — IDA*

**Problem:** N-Sliding Blocks Puzzle  
**Docs:** [Exercise Sheet](_exer/e-03-informed-heuristic-search/is-e-03-(2021-10-18).pdf) · [Slides-(2021-10-18)](_exer/e-03-informed-heuristic-search/is-e-03-informed-search-slides.pdf) · [Romania Map](_exer/e-03-informed-heuristic-search/romania-map.pdf) · [README](_exer/e-03-informed-heuristic-search/README.md)

N-puzzle (8-puzzle, 15-puzzle, …): slide tiles on an N×N grid to reach the goal configuration.
Solved with IDA\* using Manhattan Distance + Linear Conflict heuristic.

| Language | Source |
|---|---|
| C++ | [`NSlidingPuzzle.cpp`](_exer/e-03-informed-heuristic-search/cpp/SlidingBlocksPuzzleProceduralSolution/NSlidingPuzzle.cpp) |
| Java | [`NSlidingPuzzle.java`](_exer/e-03-informed-heuristic-search/java/SlidingBlocksPuzzleProceduralSolution/src/NSlidingPuzzle.java) |
| Python | [`n_sliding_puzzle.py`](_exer/e-03-informed-heuristic-search/python/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle.py) |

Puzzles: [`data/puzzles/`](_exer/e-03-informed-heuristic-search/data/puzzles/) (162 puzzles: 2×2 through 10×10 + 2 hard 4×4)  
Results: [`data/results/`](_exer/e-03-informed-heuristic-search/data/results/) (one `.txt` per puzzle — moves + elapsed time)  
Analysis: [`data/ANALYSIS.md`](_exer/e-03-informed-heuristic-search/data/ANALYSIS.md) (147 solved, 9 unsolvable, 1 timed out)  
Solutions: [`data/SOLUTIONS.md`](_exer/e-03-informed-heuristic-search/data/SOLUTIONS.md) (move sequences + per-language timings for hard puzzles 161 & 162)  
Runner: [`run_all_puzzles.py`](_exer/e-03-informed-heuristic-search/run_all_puzzles.py)

---

## E02 — Uninformed Search

**Problem:** Frog Leap Puzzle  
**Docs:** [Part 1 — Blind Search](_exer/e-02-uninformed-blind-search/is-e-02-pt-1-(2021-10-07).pdf) · [Part 2 — Warm-Up](_exer/e-02-uninformed-blind-search/is-e-02-pt-2-(2021-10-07).pdf) · [Slides-(2021-10-07)](_exer/e-02-uninformed-blind-search/is-e-02-uninformed-search-slides.pdf) · [README](_exer/e-02-uninformed-blind-search/README.md) · [Problem](_exer/e-02-uninformed-blind-search/PROBLEM.md) · [Solution](_exer/e-02-uninformed-blind-search/SOLUTION.md)

Classic puzzle on a `2N+1` board: swap N yellow (left-side) and N green (right-side) frogs using steps and jumps.
Two approaches — DFS search and an O(N²) analytical linear solution.

| Language | Linear | DFS |
|---|---|---|
| C++ | [`FrogLeapPuzzleLinear/main.cpp`](_exer/e-02-uninformed-blind-search/cpp/FrogLeapPuzzleLinear/main.cpp) | [`LeapFrogPuzzleDFS/main.cpp`](_exer/e-02-uninformed-blind-search/cpp/LeapFrogPuzzleDFS/main.cpp) |
| Java | [`FrogLeapPuzzleLinear.java`](_exer/e-02-uninformed-blind-search/java/FrogLeapPuzzleLinear/src/FrogLeapPuzzleLinear.java) | [`DFSSolution.java`](_exer/e-02-uninformed-blind-search/java/FrogLeapPuzzleDFS/src/DFSSolution.java) |
| Python | [`linear_solution.py`](_exer/e-02-uninformed-blind-search/python/FrogLeapPuzzleLinear/linear_solution.py) | [`dfs_solution.py`](_exer/e-02-uninformed-blind-search/python/FrogLeapPuzzleDFS/dfs_solution.py) |

---

## E01 — Introduction

**Problem:** —  
**Docs:** [Exercise Sheet](_exer/e-01-introduction/is-e-01-(2021-10-07).pdf) · [Slides-(2021-10-07)](_exer/e-01-introduction/is-e-01-intro-slides.pdf) · [README](_exer/e-01-introduction/README.md)

Course table of contents and overview. No code solutions.
