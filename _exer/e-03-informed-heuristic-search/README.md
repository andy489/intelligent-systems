# E03 — Informed (Heuristic) Search — IDA*

**Problem:** N-Sliding Blocks Puzzle  
**Exercise Sheet:** [is-e-03-(2021-10-18)](is-e-03-(2021-10-18).pdf)  
**Slides:** [is-e-03-informed-search-slides](is-e-03-informed-search-slides.pdf)

An N×N grid holds tiles numbered 1 to N²−1 and one blank. A move slides an adjacent tile into the blank. The goal is to reach a specified target configuration in the **minimum number of moves**.

---

## Algorithm

**IDA\*** (Iterative Deepening A\*) with:

- **Manhattan Distance** — sum of each tile's row + column distance from its goal; updated in O(1) per move inside the depth-limited search.
- **Linear Conflict** — adds 2 per reversed pair of tiles already in their goal row/column; recomputed at the start of each IDA\* iteration to raise the f-cost threshold (never used inside the inner DLS, so admissibility is preserved).

**Solvability check** (O(N² log N) merge-sort inversion count):
- Odd grid width: solvable iff inversions is even.
- Even grid width: solvable iff `(inversions + blank_row_from_bottom + blank_target_row_from_bottom)` is even.

---

## Solutions

| Language | Source |
|---|---|
| C++ | [`cpp/SlidingBlocksPuzzleProceduralSolution/`](cpp/SlidingBlocksPuzzleProceduralSolution/) |
| Java | [`java/SlidingBlocksPuzzleProceduralSolution/`](java/SlidingBlocksPuzzleProceduralSolution/) |
| Python | [`python/SlidingBlocksPuzzleProceduralSolution/`](python/SlidingBlocksPuzzleProceduralSolution/) |

---

## Input / Output

**Input:**
```
N  target_index
t1 t2 ... tN²
```
- `N` — grid dimension.
- `target_index` — 0-based linear index of the blank in the goal state; `-1` = bottom-right corner (default).
- `t1 … tN²` — initial board row by row; `0` is the blank.

**Output:**
```
<number of moves>
<move 1>       ← up / down / left / right  (where the blank moves)
...
Elapsed: HH:MM:SS.mmm

Success!
```
Already-solved prints only `0` + `Success!`. Unsolvable prints `Puzzle cannot be solved!`.

---

## Data & Runner

| Path | Description |
|---|---|
| [`data/puzzles/`](data/puzzles/) | 160 input puzzles (2×2 through 10×10) |
| [`data/results/`](data/results/) | One `.txt` result per puzzle |
| [`data/ANALYSIS.md`](data/ANALYSIS.md) | Full results table and performance analysis |
| [`run_all_puzzles.py`](run_all_puzzles.py) | Batch runner — builds C++ binary, runs all 160 puzzles with 300 s timeout |

### Run a single puzzle

```bash
# C++
cd cpp/SlidingBlocksPuzzleProceduralSolution
clang++ -O2 -std=c++17 -o n_sliding_puzzle NSlidingPuzzle.cpp
./n_sliding_puzzle ../../data/puzzles/puzzle081-4x4.txt

# Java
cd java/SlidingBlocksPuzzleProceduralSolution
javac -d out src/*.java
java -cp out NSlidingPuzzle ../../data/puzzles/puzzle081-4x4.txt

# Python
python3 python/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle.py \
    data/puzzles/puzzle081-4x4.txt
```

### Run all 160 puzzles

```bash
python3 run_all_puzzles.py
```

Builds the C++ binary if missing, runs each puzzle, writes results to `data/results/`. Existing result files are skipped.
