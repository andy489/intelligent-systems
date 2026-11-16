# NSlidingPuzzle — Python

**Source:** [`n_sliding_puzzle.py`](n_sliding_puzzle.py)  
**Algorithm:** IDA\* with Manhattan Distance + Linear Conflict — see [`../../README.md`](../../README.md)

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

## Implementation notes

- Single-file procedural Python 3. All state is in module-level globals.
- `sys.setrecursionlimit(1 << 20)` — raised to handle deep IDA\* call stacks.
- Input is read with a token iterator (`iter(source.read().split())`) for clean sequential parsing.
- `REVERSE = [RIGHT, LEFT, UP, DOWN, NEUTRAL]` — O(1) opposite-direction lookup; index directly with the move constant.
- Swap in `dls`: `board[blank_pos], board[np] = board[np], 0` — RHS is fully evaluated before assignment; no temporary needed.
- Path reconstruction: on success each `dls` frame appends its own `prev_move` to `history` (a `deque`); the list is reversed before output and the `NEUTRAL` root sentinel is skipped.
- `MAX_H = 1 << 15` (32 768) — sentinel for "threshold not exceeded at this depth".
