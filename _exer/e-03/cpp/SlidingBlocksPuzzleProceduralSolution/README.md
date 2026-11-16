# NSlidingPuzzle — C++

**Source:** [`NSlidingPuzzle.cpp`](NSlidingPuzzle.cpp)  
**Algorithm:** IDA\* with Manhattan Distance + Linear Conflict — see [`../../README.md`](../../README.md)

---

## Build

**Direct** (used by the batch runner):
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

## Implementation notes

- Single-file procedural C++17. All state is in global arrays.
- Maximum grid: 10×10 (`MAX_DIM = 10`, `MAX_TILES = 100`).
- `reverse_move()` uses a `switch`; direction constants are an `enum Move`.
- Path reconstruction: on success each `dls` frame pushes its own `prev_move` onto `shifts_history` (a `std::stack`); popping gives forward move order.
- `MAX_HEURISTIC = 1 << 15` (32 768) — sentinel for "threshold not exceeded at this depth"; well above any real puzzle cost.
- The compiled binary `n_sliding_puzzle` is the executable used by [`../../run_all_puzzles.py`](../../run_all_puzzles.py).
