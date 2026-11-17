# E03 — Informed (Heuristic) Search — IDA*

**Problem:** N-Sliding Blocks Puzzle  
**Exercise Sheet:** [is-e-03-(2021-10-18)](is-e-03-(2021-10-18).pdf)  
**Slides:** [is-e-03-informed-search-slides](is-e-03-informed-search-slides.pdf)  
**Romania Map:** [romania-map](romania-map.pdf)

An N×N grid holds tiles numbered 1 to N²−1 and one blank. A move slides an adjacent tile into the blank. The goal is to reach a specified target configuration in the **minimum number of moves**.

---

## Algorithm

**IDA\*** (Iterative Deepening A\*) — memory-efficient optimal search. Instead of keeping an open set like A\*, it runs repeated depth-limited searches (DLS), each time raising the cost threshold until the solution is found:

```
threshold = h(start)
loop:
    result = dls(start, threshold, depth=0)
    if solved → done
    threshold = result   ← smallest f that exceeded the limit
```

Each DLS call returns 0 (solved) or the minimum `f = depth + h` that exceeded the threshold — which becomes the next threshold. This guarantees the **optimal** (minimum-move) solution.

---

## Heuristic: Additive Pattern Databases (4×4 canonical goal)

For the standard 4×4 puzzle with blank at bottom-right, all three implementations use **additive 5-5-5 disjoint pattern databases (PDB)**:

Tiles 1–15 are split into three disjoint groups of 5:

| Group | Tiles |
|---|---|
| A | 1, 2, 3, 4, 5 |
| B | 6, 7, 8, 9, 10 |
| C | 11, 12, 13, 14, 15 |

A PDB for each group stores the **exact minimum moves** to place those 5 tiles at their goal positions (treating all other tiles as wildcards). Because the groups are disjoint, summing the three PDB values is admissible — no move is counted twice.

Each PDB has 16P5 = 524,160 entries, built by 0-1 BFS from the goal state at startup (≈100 ms in C++, ≈1 s in Java, ≈44 s in Python). The PDB heuristic is far tighter than MD+LC and prunes orders of magnitude more of the search tree on hard instances.

---

## Heuristic: Manhattan Distance + Linear Conflict (other grids)

For grids other than 4×4 standard, all implementations use **MD + LC**:

**Manhattan Distance (MD):** For each tile, count how many rows + columns it is away from its goal position. Admissible because each tile needs at least that many moves regardless of everything else.

**Linear Conflict (LC):** If two tiles are both in their goal row (or column) but in reversed order relative to their goals, they must each leave and re-enter that row — adding at least 2 moves per conflicting pair. LC adds 2 per such pair on top of MD.

`h = MD + LC` is a strictly tighter lower bound than MD alone, so it prunes more of the search tree while remaining admissible (never overestimates).

---

## Optimisation: LC recomputed at every node

**The naive approach** computed LC only once per IDA\* iteration (at the root), then ran pure MD inside DLS. For hard puzzles this is too weak — the threshold creeps up slowly and the search tree explodes.

**The fix:** recompute `h = MD + LC` at **every node** inside DLS:

```
new_md = update_md(blank, tile_pos, old_md)   // O(1) — only one tile changed
board ← swap
new_h  = new_md + calc_linear_conflict()      // O(N²) on the new board state
recurse(new_h, new_md)
board ← swap back
```

For a 4×4 board `calc_linear_conflict` touches 16 tiles — negligible cost. The tighter `h` prunes branches that MD alone would explore, reducing the search tree by orders of magnitude on hard instances.

**Two correctness bugs fixed in the process:**

| Bug | Wrong | Correct | Why |
|---|---|---|---|
| Goal check | `h == 0` | `md == 0` | With `h = MD + LC`, LC can be 0 at a non-goal state — the condition never triggered, causing an infinite loop |
| MD update | passed `h = MD + LC` as the base | pass MD separately | `update_md` computes a delta — it must start from MD only, not the combined heuristic |

---

## Solvability check

Before searching, verify the puzzle is reachable from the goal using inversion count (O(N² log N) merge sort):

- **Odd grid width:** solvable iff number of inversions is even.
- **Even grid width:** solvable iff `inversions + blank_row_from_bottom + blank_target_row_from_bottom` is even.

Both the blank's current row and its goal row must be included; omitting the goal row gives wrong results when the blank target is not the default position.

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
<move 1>       ← up / down / left / right  (where the tile moves)
...
Elapsed: HH:MM:SS.mmm

Success!
```
Already-solved prints only `0` + `Success!`. Unsolvable prints `Puzzle cannot be solved!`.

---

## Data & Runners

| Path | Description |
|---|---|
| [`data/puzzles/`](data/puzzles/) | 162 input puzzles (2×2 through 10×10, plus five hard 4×4) |
| [`data/results-cpp/`](data/results-cpp/) | One `.txt` result per puzzle (C++) |
| [`data/results-java/`](data/results-java/) | One `.txt` result per puzzle (Java) |
| [`data/results-python/`](data/results-python/) | One `.txt` result per puzzle (Python) |
| [`data/ANALYSIS.md`](data/ANALYSIS.md) | Full results table and performance analysis |
| [`data/SOLUTION.md`](data/SOLUTION.md) | Algorithm explanation, optimisations, move sequences for all 5 hard puzzles |
| [`data/run_cpp.py`](data/run_cpp.py) | Batch runner — builds C++ binary, runs all puzzles, writes to `results-cpp/` |
| [`data/run_java.py`](data/run_java.py) | Batch runner — compiles Java, runs all puzzles, writes to `results-java/` |
| [`data/run_python.py`](data/run_python.py) | Batch runner — runs all puzzles with Python, writes to `results-python/` |

### Run a single puzzle

```bash
# C++
cd cpp/SlidingBlocksPuzzleProceduralSolution
clang++ -O2 -std=c++17 -o n_sliding_puzzle NSlidingPuzzle.cpp
./n_sliding_puzzle ../../data/puzzles/081-puzzle-4x4.txt

# Java
cd java/SlidingBlocksPuzzleProceduralSolution
javac -d out src/*.java
java -cp out NSlidingPuzzle ../../data/puzzles/081-puzzle-4x4.txt

# Python
python3 python/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle.py \
    data/puzzles/081-puzzle-4x4.txt
```

### Run all puzzles

```bash
python3 data/run_cpp.py     # C++    → data/results-cpp/
python3 data/run_java.py    # Java   → data/results-java/
python3 data/run_python.py  # Python → data/results-python/
```

Each runner builds/compiles if needed, runs each puzzle with a 300 s timeout, and skips puzzles whose result file already exists.
