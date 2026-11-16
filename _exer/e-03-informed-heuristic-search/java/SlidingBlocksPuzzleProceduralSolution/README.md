# NSlidingPuzzle — Java

**Source:** [`src/NSlidingPuzzle.java`](src/NSlidingPuzzle.java)  
**Algorithm:** IDA\* with Manhattan Distance + Linear Conflict — see [`../../README.md`](../../README.md)

---

## Build

**Direct** (no Maven):
```bash
javac -d out src/NSlidingPuzzle.java
```

**Maven:**
```bash
mvn package
# produces target/SlidingBlocksPuzzleProceduralSolution-1.0.jar
```

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

## Implementation notes

- Single-class procedural Java 11. All state is in static fields (no instances).
- Maximum grid: 10×10 (`MAX_TILES = 100`).
- `REVERSE[] = { RIGHT, LEFT, UP, DOWN, NEUTRAL }` — O(1) opposite-direction lookup.
- Path reconstruction: on success each `dls` frame pushes its own `prevMove` onto `history` (an `ArrayDeque` used as a stack); the root's `NEUTRAL` sentinel is discarded with `pop()` before output, then moves are popped in forward order.
- `MAX_H = 1 << 15` (32 768) — sentinel for "threshold not exceeded at this depth".
