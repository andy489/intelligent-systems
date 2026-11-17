# E03 — N-Sliding Blocks Puzzle: Algorithm & Solutions

Implementation: C++ · Java · Python  
Algorithm: **IDA\*** with **Manhattan Distance + Linear Conflict**

---

## Algorithm: IDA\* (Iterative Deepening A\*)

IDA\* runs repeated depth-limited searches, each time with a rising f-cost threshold where **f = depth + h**. When a node's f-cost exceeds the current threshold, the search backtracks immediately. The minimum f-cost that exceeded the threshold becomes the next threshold. This continues until the goal is found.

```
function IDA*(start):
    threshold ← h(start)
    loop:
        result ← DLS(start, depth=0, threshold)
        if result == FOUND: return solution
        threshold ← result          # smallest exceeded f-cost

function DLS(node, depth, threshold):
    f ← depth + h(node)
    if f > threshold: return f      # prune — return as next-threshold hint
    if goal(node): return FOUND
    min_exceeded ← ∞
    for each move from node:
        child ← apply(move)
        result ← DLS(child, depth+1, threshold)
        if result == FOUND: return FOUND
        min_exceeded ← min(min_exceeded, result)
        undo(move)
    return min_exceeded
```

**Why IDA\*?**
- **Memory-efficient**: uses O(solution depth) stack space — no closed set, no priority queue.
- **Optimal**: finds the shortest solution, because the threshold rises monotonically and the heuristic is admissible.
- **Complete**: terminates for any solvable puzzle; correctly detects unsolvable ones via the parity check before searching.

---

## Heuristic: Manhattan Distance + Linear Conflict

### Manhattan Distance (MD)

For each tile, count the sum of its row-distance and column-distance to its goal position:

```
MD = Σ (|row(tile) − goal_row(tile)| + |col(tile) − goal_col(tile)|)
```

This is admissible: every tile needs at least that many moves to reach its goal, because each move shifts one tile by one cell.

### Linear Conflict (LC)

Two tiles are in **linear conflict** when:
1. Both are in their goal row (or both in their goal column), **and**
2. Their goal positions are reversed relative to their current positions.

Such a pair must bypass each other, requiring at least **2 extra moves** that MD does not account for. LC counts all such pairs and adds 2 per conflict:

```
LC = 2 × (number of conflicting pairs in rows) + 2 × (number of conflicting pairs in columns)
```

**Combined heuristic: h = MD + LC**

Still admissible (never overestimates), but strictly tighter than MD alone. For the hardest 4×4 puzzles LC can add 10–20 to the root heuristic, dramatically pruning the search tree.

---

## Key Optimisations

### 1. LC recomputed at every node inside DLS

The naive approach is to compute LC only once at the root of each IDA\* iteration and use only MD inside the DLS. This makes the heuristic weaker for all nodes below the root and misses most of the pruning benefit.

The fix: recompute h = MD + LC at **every node** inside DLS:

```
new_md = update_md(blank_pos, tile_pos, old_md)   // O(1) — only one tile changed
board ← swap
new_h  = new_md + calc_linear_conflict()           // O(N²) on the new board state
recurse(new_h, new_md, ...)
board ← swap back
```

For a 4×4 board, `calc_linear_conflict` touches 16 tiles — negligible overhead. The tighter h prunes enormous subtrees that MD alone would explore. This single change reduced the ULTRA-HARD puzzle from >300 s timeout to 2 min 37 s.

### 2. Incremental MD update — O(1) per move

Only one tile moves per step (the tile sliding into the blank). Its old and new distances are cheap to compute:

```cpp
int update_md(int blank_pos, int from_pos, int md) {
    int t = board[from_pos];
    int old_dist = |row(from_pos) − goal_row[t]| + |col(from_pos) − goal_col[t]|;
    int new_dist = |row(blank_pos) − goal_row[t]| + |col(blank_pos) − goal_col[t]|;
    return md − old_dist + new_dist;
}
```

This is called **before** the swap so `from_pos` still holds the tile.

### 3. Initial threshold = MD + LC (not just MD)

Starting the first IDA\* iteration at the full h = MD + LC avoids wasted iterations at lower thresholds. For hard puzzles this can skip many early iterations entirely.

### 4. Goal check: `md == 0`, not `h == 0`

With h = MD + LC, it is possible for LC = 0 at a non-goal state (all tiles in their goal row/column, but not yet at their exact position). If the goal check were `h == 0`, the search would never terminate correctly. The correct check is:

```
if (md == 0):  // all tiles are at their goal positions
    return FOUND
```

This requires passing `md` separately from `h` through the recursion.

### 5. Separate `md` argument prevents wrong delta computation

`update_md` computes a delta and adds it to the current MD. If the wrong base is passed (e.g. `h = MD + LC` instead of `md`), the result is corrupted. Keeping `md` as a dedicated parameter ensures correctness.

---

## Solvability Check

Before searching, the puzzle is checked for solvability using the inversion-count parity rule:

- **Odd grid** (3×3, 5×5, …): solvable iff the number of inversions is **even**.
- **Even grid** (4×4, …): solvable iff `(inversions + blank_row_from_bottom + blank_target_row_from_bottom)` is **even**.

Both the blank's current row from the bottom and the blank's **target** row from the bottom must be included. A bug that omitted `blank_target_row_from_bottom` caused three puzzles (009, 010, 011) with non-default blank targets to be misclassified as solvable, sending IDA\* into an infinite search.

---

## Direction Semantics

Internally the algorithm tracks where the **blank** moves. The output convention is where the **tile** moves — the opposite direction:

| Blank moves | Tile moves (output) |
|---|---|
| up    | down  |
| down  | up    |
| left  | right |
| right | left  |

All three implementations (C++, Java, Python) follow this convention.

---

## Hard Puzzle Solutions

The five hardest puzzles, ordered by number of optimal moves.

---

### 158-puzzle-4x4-HARD — 53 moves

**Initial state:**
```
 2  9  3  5
 8 11 12  7
15  4  _  13
 6  1 10 14
```

| Language | Moves | Time |
|---|---|---|
| C++ | 53 | 1.326 s |
| Java | 53 | ~12 s |
| Python | 53 | ~5 min |

**Solution (53 moves):**
right, right, down, left, left, up, right, up, left, down, left, down, right, right, up, left, up, right, down, right, down, left, down, left, left, up, up, right, right, right, up, left, left, left, down, right, down, down, right, right, up, left, left, left, up, right, right, right, down, left, up, up, right

---

### 159-puzzle-4x4-HARD-2 — 56 moves

**Initial state:**
```
 9  5  1 12
10  _  11 13
 3  7 14  6
 2  8 15  4
```

| Language | Moves | Time |
|---|---|---|
| C++ | 56 | 361 ms |
| Java | 56 | ~4.9 s |
| Python | 56 | ~116 s |

**Solution (56 moves):**
right, down, left, left, up, left, up, up, right, down, down, down, left, up, up, up, right, down, right, up, left, left, down, right, down, right, up, right, up, left, left, down, down, down, left, up, right, right, up, right, down, down, left, up, up, left, left, down, right, down, right, up, left, up, up, left

---

### 160-puzzle-4x4-MEGA-HARD — 66 moves

**Initial state:**
```
14 15  8 12
10 11  9 13
 2  6  5  1
 3  7  4  _
```

| Language | Moves | Time |
|---|---|---|
| C++ | 66 | 3.535 s |
| Java | 66 | ~68 s |
| Python | — | >600 s (timeout) |

**Solution (66 moves):**
down, down, down, right, up, right, up, left, up, left, down, down, right, up, up, right, down, down, down, left, up, right, up, left, up, left, down, right, down, right, up, right, down, down, left, up, up, up, right, down, down, down, left, up, up, up, left, down, right, right, up, left, down, right, down, left, left, up, left, down, down, right, up, up, left, up

---

### 161-puzzle-4x4-MEGA-HARD-2 — 78 moves

**Initial state:**
```
 _  15 14 13
12  11 10  9
 8   7  6  5
 4   3  2  1
```

The fully reversed board (tiles 15→1 read left-to-right, top-to-bottom, blank at top-left).

| Language | Moves | Time |
|---|---|---|
| C++ | 78 | 2.004 s |
| Java | — | >600 s (timeout) |
| Python | — | >600 s (timeout) |

**Solution (78 moves):**
left, left, left, up, right, right, up, left, down, left, up, right, down, right, up, left, left, up, right, right, right, down, down, down, left, left, left, up, up, up, right, right, right, down, down, down, left, left, left, up, up, up, right, right, right, down, down, down, left, left, left, up, up, up, right, right, right, down, down, down, left, left, left, up, up, up, right, right, right, down, down, down, left, left, left, up, up, up

---

### 162-puzzle-4x4-ULTRA-HARD — 80 moves

**Initial state:**
```
 _  12  9 13
15  11 10 14
 3   7  5  6
 4   8  2  1
```

Previously timed out at 300 s with a weaker heuristic setup. Solved after enabling full LC recomputation at every DLS node.

| Language | Moves | Time |
|---|---|---|
| C++ | 80 | 2 min 37 s |
| Java | — | >600 s (timeout) |
| Python | — | >600 s (timeout) |

**Solution (80 moves):**
left, left, left, up, right, up, right, right, down, left, left, down, left, up, up, up, right, right, right, down, down, down, left, up, left, left, up, up, right, right, right, down, down, left, left, up, right, right, down, down, left, left, left, up, up, right, up, right, right, down, down, left, down, left, left, up, up, right, right, down, down, left, left, up, up, up, right, right, down, down, right, down, left, left, left, up, up, right, up, left

---

## Per-Language Notes

- **C++** (`-O2`, clang++): solves all 162 puzzles including ULTRA-HARD (80 moves, 2:37).
- **Java**: solves puzzles up to ~57 moves within 300 s; MEGA-HARD-2 (78 moves) and ULTRA-HARD (80 moves) time out.
- **Python**: correct algorithm, but recursion overhead makes depths >~57 impractical (>600 s). This is a language speed limitation, not an algorithmic one.
