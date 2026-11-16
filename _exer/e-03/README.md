# E03 — N-Sliding Blocks Puzzle

**Problem:** N-Sliding Blocks Puzzle  
**Topic:** Informed (Heuristic) Search — IDA\*  
**Exercise Sheet:** [is-e-03-(2021-10-17)-informed-heuristic-search.pdf](is-e-03-(2021-10-17)-informed-heuristic-search.pdf)

---

## Problem

The classic N-puzzle (8-puzzle, 15-puzzle, …): an N×N grid holds tiles numbered 1 to N²−1 and one blank. A move slides an adjacent tile into the blank. The goal is to reach a specified target configuration in the minimum number of moves.

---

## Algorithm

**IDA\*** (Iterative Deepening A\*) with a combined heuristic:

- **Manhattan Distance** — sum of each tile's row + column distance from its goal position; used incrementally (O(1) per move) inside the depth-limited search.
- **Linear Conflict** — adds 2 for every pair of tiles that are in their goal row/column but in reversed order; recomputed at the start of each IDA\* iteration and used only to raise the f-cost threshold.

**Solvability check** — before searching, a merge-sort inversion count decides in O(N² log N) whether the puzzle is solvable:
- Odd grid width: solvable iff the number of inversions is even.
- Even grid width: solvable iff `(inversions + blank_row_from_bottom + blank_target_row_from_bottom)` is even.

---

## Solutions

| Language | Source |
|---|---|
| C++ | [`cpp/SlidingBlocksPuzzleProceduralSolution/`](cpp/SlidingBlocksPuzzleProceduralSolution/) |
| Java | [`java/SlidingBlocksPuzzleProceduralSolution/`](java/SlidingBlocksPuzzleProceduralSolution/) |
| Python | [`python/SlidingBlocksPuzzleProceduralSolution/`](python/SlidingBlocksPuzzleProceduralSolution/) |

---

## Input format

```
N  target_index
t1 t2 ... tN²
```

- `N` — grid dimension.
- `target_index` — linear index (0-based, row-major) where the blank should end up in the goal state; pass `-1` for the default bottom-right corner.
- `t1 … tN²` — the initial board, row by row; `0` is the blank tile.

Example (3×3, blank goes to index 8 = bottom-right):

```
3 -1
1 2 3
4 5 6
7 8 0
```

---

## Output format

```
<number of moves>
<move 1>
<move 2>
...
Elapsed: HH:MM:SS.mmm

Success!
```

Each move names where the **blank** moves: `up`, `down`, `left`, or `right`.  
If the puzzle is already solved, only `0` and `Success!` are printed.  
If unsolvable, only `Puzzle cannot be solved!` is printed.

---

## Data & Runner

| Path | Description |
|---|---|
| [`data/puzzles/`](data/puzzles/) | 160 input puzzles (2×2 through 10×10) |
| [`data/results/`](data/results/) | One `.txt` output file per puzzle |
| [`data/ANALYSIS.md`](data/ANALYSIS.md) | Full results table and performance analysis |
| [`run_all_puzzles.py`](run_all_puzzles.py) | Batch runner — builds C++ binary, runs all 160 puzzles, writes results |

### Running a single puzzle

```bash
# C++
cd cpp/SlidingBlocksPuzzleProceduralSolution
clang++ -O2 -std=c++17 -o n_sliding_puzzle NSlidingPuzzle.cpp
./n_sliding_puzzle ../../data/puzzles/puzzle081-4x4.txt

# Java
cd java/SlidingBlocksPuzzleProceduralSolution
javac -d out src/NSlidingPuzzle.java
java -cp out NSlidingPuzzle ../../data/puzzles/puzzle081-4x4.txt

# Python
python3 python/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle.py \
    data/puzzles/puzzle081-4x4.txt
```

### Running all 160 puzzles

```bash
python3 run_all_puzzles.py
```

Builds the C++ binary automatically if not present, runs each puzzle with a 300 s timeout, and writes results to `data/results/`. Already-existing result files are skipped.
