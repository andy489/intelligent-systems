# NSlidingPuzzle — Python Solution

**Algorithm:** IDA\* with Manhattan Distance + Linear Conflict  
**Source:** [`n_sliding_puzzle.py`](n_sliding_puzzle.py)

---

## What the program does

`n_sliding_puzzle.py` is a single-file procedural Python script that reads an N-puzzle from a file (or stdin) and solves it optimally using IDA\*.

Key steps:

1. **Read input** — grid size, blank target index, and the initial board using a token iterator over the file contents.
2. **Solvability check** — counts inversions with merge sort; for even-width grids also checks both blank rows from the bottom. Exits immediately with `"Puzzle cannot be solved!"` if unsolvable.
3. **Compute heuristic** — Manhattan Distance + Linear Conflict at the root.
4. **IDA\* search** — iterative deepening over f-cost thresholds. Each depth-limited call (`dls`) passes pure Manhattan as the running heuristic and updates it in O(1) per move. Linear Conflict is recomputed at the start of each outer iteration to raise the threshold without inflating the inner search.
5. **Reconstruct path** — on success each stack frame appends its own `prev_move` to a `deque`, building the path in reverse; the list is reversed before output and the `NEUTRAL` root sentinel is skipped.
6. **Output** — number of moves, one move name per line (`up`/`down`/`left`/`right`), elapsed time, `Success!`.

---

## Run

```bash
# from a file
python3 n_sliding_puzzle.py path/to/puzzle.txt

# from stdin
echo "3 -1
1 2 3
4 5 6
7 8 0" | python3 n_sliding_puzzle.py
```

No dependencies beyond the standard library.

---

## Notes

- `sys.setrecursionlimit(1 << 20)` raises Python's default recursion limit to handle deep IDA\* call stacks.
- Module-level globals are used throughout (procedural style matching the C++ and Java versions).
- `REVERSE = [RIGHT, LEFT, UP, DOWN, NEUTRAL]` — O(1) opposite-direction lookup; index with the move constant directly.
- The swap in `dls` uses tuple unpacking: `board[blank_pos], board[np] = board[np], 0` — the right-hand side is fully evaluated before assignment, so no temporary variable is needed.
- `MAX_H = 1 << 15` (32 768) is the sentinel for "no solution found at this depth".
