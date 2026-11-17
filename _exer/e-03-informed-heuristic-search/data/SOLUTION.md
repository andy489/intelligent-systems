# E03 — N-Sliding Blocks Puzzle: Algorithm & Solutions

Implementation: C++ · Java · Python  
Algorithm: **IDA\*** with **Additive 5-5-5 Pattern Databases** (4×4 canonical) / **Manhattan Distance + Linear Conflict** (all other grids)

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

## Heuristic: Additive Pattern Databases (4×4 canonical goal)

For the standard 15-puzzle (4×4 with blank at bottom-right corner), all three implementations use **additive 5-5-5 disjoint pattern databases (PDB)**.

### What is a Pattern Database?

A PDB for a group of tiles is a precomputed lookup table that stores the **exact minimum number of moves** needed to bring just those tiles to their goal positions, treating all other tiles as wildcards (empty space the blank can pass through freely).

It is built once by running a **BFS backwards from the goal state** in an abstract space where only the group tiles and the blank exist. Every reachable abstract state gets its exact cost recorded.

### The 5-5-5 partition

Tiles 1–15 are split into three disjoint groups of 5:

| Group | Tiles |
|---|---|
| A | 1, 2, 3, 4, 5 |
| B | 6, 7, 8, 9, 10 |
| C | 11, 12, 13, 14, 15 |

Each group has **16P5 = 524,160** distinct arrangements (5 tiles on 16 cells, ordered), so each PDB has 524,160 entries stored as bytes.

### Why additive?

Because the groups are **disjoint** — no tile appears in two groups — the three PDB values can be summed and the result is still admissible. Each move can advance at most one group, so no move is double-counted:

```
h = pdb_A[rank(positions of tiles 1–5)]
  + pdb_B[rank(positions of tiles 6–10)]
  + pdb_C[rank(positions of tiles 11–15)]
```

This heuristic is much tighter than MD+LC. For the hardest 4×4 puzzles it can return values close to the true optimal cost, pruning orders of magnitude more of the search tree.

### Lehmer-code rank (perm_rank_5)

Each group's 5 tile positions are encoded as a unique integer index using the **Lehmer code** (factoradic number system):

```
rank = Σ (pos[i] − count of used positions smaller than pos[i]) × factor[i]
```

where `factors = [15×14×13×12, 14×13×12, 13×12, 12, 1]`. This maps any ordered 5-tuple of distinct positions from 0–15 into `[0, 524160)`.

### 0-1 BFS for PDB construction

The BFS uses a **deque** (0-1 BFS / dial's algorithm):
- Moving the blank through a **non-group tile** costs **0** (the group state doesn't change).
- Moving a **group tile** costs **1**.

Cost-0 edges go to the front of the deque; cost-1 edges go to the back. This gives exact shortest-path costs without Dijkstra's overhead.

After BFS, each abstract state `(rank, blank_pos)` has a cost. The PDB is projected by taking the **minimum cost over all blank positions** for each rank — the blank's location is irrelevant to the group's distance to goal.

### Incremental tile position tracking

During IDA\* search, each group tile's current board position is maintained in a `tile_pos[]` array updated O(1) per move (instead of scanning the board). This makes `pdb_lookup()` O(1) per node.

### PDB build cost

The three PDBs are built once at startup:

| Language | Build time |
|---|---|
| C++ | ~100 ms |
| Java | ~1 s |
| Python | ~44 s |

After the first run they are cached to disk (C++ rebuilds each run; Java saves to `~/.n_sliding_puzzle_pdb.cache`; Python saves to `n_sliding_puzzle.pdb_cache` next to the script), so subsequent runs load in milliseconds.

---

## Heuristic: Manhattan Distance + Linear Conflict (other grids)

For grids other than 4×4 standard, all implementations use **MD + LC**.

### Manhattan Distance (MD)

For each tile, count the sum of its row-distance and column-distance to its goal position:

```
MD = Σ (|row(tile) − goal_row(tile)| + |col(tile) − goal_col(tile)|)
```

Admissible: every tile needs at least that many moves to reach its goal.

### Linear Conflict (LC)

Two tiles are in **linear conflict** when:
1. Both are in their goal row (or both in their goal column), **and**
2. Their goal positions are reversed relative to their current positions.

Such a pair must bypass each other, requiring at least **2 extra moves** that MD does not account for:

```
LC = 2 × (conflicting pairs in rows) + 2 × (conflicting pairs in columns)
h = MD + LC
```

Still admissible, but strictly tighter than MD alone.

---

## Key Optimisations

### 1. LC recomputed at every node inside DLS

The naive approach computes LC only once at the IDA\* root and uses only MD inside DLS. The fix: recompute `h = MD + LC` at **every node**:

```
new_md = update_md(blank_pos, tile_pos, old_md)   // O(1) — only one tile changed
board ← swap
new_h  = new_md + calc_linear_conflict()           // O(N²) on the new board state
recurse(new_h, new_md, ...)
board ← swap back
```

For a 4×4 board `calc_linear_conflict` touches 16 tiles — negligible overhead. This single change reduced the ULTRA-HARD puzzle from >300 s timeout to 2 min 37 s (C++, MD+LC only, before PDB was added).

### 2. Incremental MD update — O(1) per move

Only one tile moves per step. Its old and new distances are cheap to compute:

```cpp
int update_md(int blank_pos, int from_pos, int md) {
    int t = board[from_pos];
    int old_dist = |row(from_pos) − goal_row[t]| + |col(from_pos) − goal_col[t]|;
    int new_dist = |row(blank_pos) − goal_row[t]| + |col(blank_pos) − goal_col[t]|;
    return md − old_dist + new_dist;
}
```

Called **before** the swap so `from_pos` still holds the tile.

### 3. Goal check: `md == 0`, not `h == 0`

With `h = MD + LC`, LC can be 0 at a non-goal state. The correct check is:

```
if (md == 0):  // all tiles at their goal positions
    return FOUND
```

### 4. Separate `md` argument

`update_md` computes a delta added to MD. Passing `h = MD + LC` instead of `md` as the base corrupts the result. `md` is kept as a dedicated parameter throughout the recursion.

---

## Solvability Check

Before searching, the puzzle is checked using the inversion-count parity rule:

- **Odd grid** (3×3, 5×5, …): solvable iff inversions is **even**.
- **Even grid** (4×4, …): solvable iff `(inversions + blank_row_from_bottom + blank_target_row_from_bottom)` is **even**.

Both the blank's current row and its **target** row from the bottom must be included. Omitting `blank_target_row_from_bottom` caused three puzzles with non-default blank targets to be misclassified as solvable.

---

## Direction Semantics

Internally the algorithm tracks where the **blank** moves. The output convention is where the **tile** moves — the opposite direction:

| Blank moves | Tile moves (output) |
|---|---|
| up    | down  |
| down  | up    |
| left  | right |
| right | left  |

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
| C++ | 53 | 1.327 s |
| Java | 53 | ~2 s |
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
| C++ | 56 | 365.801 ms |
| Java | 56 | ~2 s |
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
| C++ | 66 | 3.580 s |
| Java | 66 | ~10 s |
| Python | — | >300 s (timeout) |

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
| C++ | 78 | 2.028 s |
| Java | — | >600 s (timeout) |
| Python | — | >300 s (timeout) |

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

| Language | Moves | Time |
|---|---|---|
| C++ | 80 | 00:02:39.367 |
| Java | — | >600 s (timeout) |
| Python | — | >300 s (timeout) |

**Solution (80 moves):**
left, left, left, up, right, up, right, right, down, left, left, down, left, up, up, up, right, right, right, down, down, down, left, up, left, left, up, up, right, right, right, down, down, left, left, up, right, right, down, down, left, left, left, up, up, right, up, right, right, down, down, left, down, left, left, up, up, right, right, down, down, left, left, up, up, up, right, right, down, down, right, down, left, left, left, up, up, right, up, left

---

## Per-Language Notes

- **C++** (`-O2`, clang++): solves all 162 puzzles. PDB built in ~100 ms each run (no disk cache).
- **Java**: solves puzzles up to ~66 moves within 600 s with PDB. PDB cached to `~/.n_sliding_puzzle_pdb.cache` after first run (~1 s build, ~5 ms load).
- **Python**: correct algorithm; PDB cached to `n_sliding_puzzle.pdb_cache` after first run (~44 s build, ~0.1 s load). Recursion overhead makes depths >~57 impractical regardless of heuristic.
