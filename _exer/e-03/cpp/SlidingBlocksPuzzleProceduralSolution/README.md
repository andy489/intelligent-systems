# NSlidingPuzzle — C++ Solution

**Algorithm:** IDA\* with Manhattan Distance + Linear Conflict  
**Source:** [`NSlidingPuzzle.cpp`](NSlidingPuzzle.cpp)

---

## What the program does

`NSlidingPuzzle.cpp` is a single-file procedural C++ program that reads an N-puzzle from a file (or stdin) and solves it optimally using IDA\*.

Key steps:

1. **Read input** — grid size, blank target index, and the initial board.
2. **Solvability check** — counts inversions with merge sort; for even-width grids also checks both blank rows from the bottom. Exits immediately with `"Puzzle cannot be solved!"` if unsolvable.
3. **Compute heuristic** — Manhattan Distance + Linear Conflict at the root.
4. **IDA\* search** — iterative deepening over f-cost thresholds. Each depth-limited call (`dls`) passes pure Manhattan as the running heuristic and updates it in O(1) per move. Linear Conflict is recomputed at the start of each outer iteration to raise the threshold without inflating the inner search.
5. **Reconstruct path** — on success each stack frame pushes its own `prev_move`, building the move list in forward order on the call stack.
6. **Output** — number of moves, one move name per line (`up`/`down`/`left`/`right`), elapsed time, `Success!`.

---

## Build

**Direct (recommended for the batch runner):**

```bash
clang++ -O2 -std=c++17 -o n_sliding_puzzle NSlidingPuzzle.cpp
```

**CMake:**

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

---

## Run

```bash
# from a file
./n_sliding_puzzle path/to/puzzle.txt

# from stdin
echo "3 -1
1 2 3
4 5 6
7 8 0" | ./n_sliding_puzzle
```

---

## Notes

- Maximum supported grid: 10×10 (`MAX_DIM = 10`, `MAX_TILES = 100`).
- The compiled binary `n_sliding_puzzle` is used by [`../../run_all_puzzles.py`](../../run_all_puzzles.py) to batch-run all 160 puzzles.
- `MAX_HEURISTIC = 1 << 15` (32 768) is the sentinel for "no solution found at this depth"; well above any real puzzle cost.
