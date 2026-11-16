# NSlidingPuzzle — Java Solution

**Algorithm:** IDA\* with Manhattan Distance + Linear Conflict  
**Source:** [`src/NSlidingPuzzle.java`](src/NSlidingPuzzle.java)

---

## What the program does

`NSlidingPuzzle.java` is a single-class procedural Java program that reads an N-puzzle from a file (or stdin) and solves it optimally using IDA\*.

Key steps:

1. **Read input** — grid size, blank target index, and the initial board via `Scanner`.
2. **Solvability check** — counts inversions with merge sort; for even-width grids also checks both blank rows from the bottom. Exits immediately with `"Puzzle cannot be solved!"` if unsolvable.
3. **Compute heuristic** — Manhattan Distance + Linear Conflict at the root.
4. **IDA\* search** — iterative deepening over f-cost thresholds. Each depth-limited call (`dls`) passes pure Manhattan as the running heuristic and updates it in O(1) per move. Linear Conflict is recomputed at the start of each outer iteration to raise the threshold without inflating the inner search.
5. **Reconstruct path** — on success each stack frame pushes its own `prevMove` onto an `ArrayDeque`, building the path in forward pop order. The root's `NEUTRAL` sentinel is discarded before output.
6. **Output** — number of moves, one move name per line (`up`/`down`/`left`/`right`), elapsed time, `Success!`.

---

## Build

**Direct (single file, no Maven):**

```bash
javac -d out src/NSlidingPuzzle.java
```

**Maven:**

```bash
mvn package
```

Produces `target/SlidingBlocksPuzzleProceduralSolution-1.0.jar`.

---

## Run

```bash
# compiled class
java -cp out NSlidingPuzzle path/to/puzzle.txt

# Maven jar
java -jar target/SlidingBlocksPuzzleProceduralSolution-1.0.jar path/to/puzzle.txt

# stdin
echo "3 -1
1 2 3
4 5 6
7 8 0" | java -cp out NSlidingPuzzle
```

---

## Notes

- Static fields are used throughout (procedural style, no instances).
- `MAX_TILES = 100` supports up to 10×10 grids.
- `REVERSE[]` array gives the opposite direction in O(1): `{ RIGHT, LEFT, UP, DOWN, NEUTRAL }`.
- The `history` deque stores `prevMove` values pushed on the way back up the call stack; the front holds `NEUTRAL` (root sentinel), so after search the moves are popped front-to-back in forward order.
