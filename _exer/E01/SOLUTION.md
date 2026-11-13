# E01 — Solution Approaches

## Overview

Two fundamentally different strategies are implemented:

| | Linear | DFS |
|---|---|---|
| Strategy | Deterministic pattern | Blind search |
| Time complexity | O(N²) | O(N²) |
| Space complexity | O(N) | O(N²) |
| Moves produced | Optimal (N² + 2N) | Optimal (N² + 2N) |
| Scalability | Any N | Practical limit ~N = 25 |

---

## 1. Linear (Deterministic) Solution

### Insight

The optimal move sequence follows a strict pattern that can be derived analytically. When you write out the moves for small N you notice a triangular structure:

```
N = 1:  1         step             (1 move)
N = 2:  1 2 1                     (3 moves for this group)
N = 3:  1 2 1 3 1 2 1             ...
```

The solution has three phases, all driven by `N`:

### Phase 1 — First Half (N² moves total)

For `i = 1` to `N`:
- Make `i - 1` double-jumps in the primary direction.
- Make `1` single step in the primary direction.

The direction alternates: odd `i` → right (green frogs advance), even `i` → left (brown frogs advance).

### Phase 2 — Middle (N moves)

Make `N` double-jumps in the direction determined by the parity of `N`:
- `N` is even → double-jump right.
- `N` is odd → double-jump left.

### Phase 3 — Second Half (N² moves total, mirror of Phase 1)

For `i = N` down to `1`:
- Make `1` single step.
- Make `i - 1` double-jumps.

Direction logic mirrors Phase 1.

### Total moves: N² + N + N² = N² + 2N (optimal)

### Pseudocode

```
leaf ← N
for i in 1..N:
    repeat (i-1) times: double-jump in primary_dir(i)
    single-step in primary_dir(i)

repeat N times: double-jump in middle_dir(N)

for i in N..1:
    single-step in secondary_dir(i)
    repeat (i-1) times: double-jump in secondary_dir(i)
```

---

## 2. DFS (Search) Solution

### Insight

Model the puzzle as a state-space search. The state is the board configuration plus the leaf position. At each step, try all four move types:

| Direction | Distance | Applied to |
|---|---|---|
| Left | 1 (step) | green frog to the right of leaf |
| Left | 2 (jump) | green frog two cells right of leaf |
| Right | 1 (step) | brown frog to the left of leaf |
| Right | 2 (jump) | brown frog two cells left of leaf |

Because the puzzle has a unique solution path from any reachable state, DFS with no revisited-state tracking finds the solution without any backtracking beyond dead ends caused by illegal move directions.

### Goal State Check

The puzzle is solved when:
1. The leaf is back at the centre (position `N`).
2. The first half of the board (`0..N-1`) is all brown frogs (`R`).

(The second half is guaranteed correct if both conditions hold and the total frog count is conserved.)

### Recursive Structure

```
dfs(board, leaf):
    if goal_state(board, leaf): print solution; return true
    for each move_type in [(-1,1), (-1,2), (+1,1), (+1,2)]:
        frog ← leaf + direction * distance
        if frog in bounds and board[frog] == correct_colour:
            swap(leaf, frog)
            if dfs(board, frog): undo swap; record move; return true
            undo swap
    return false
```

The move sequence is recorded **on the way back up** (after a successful sub-call confirms the path), so `addFirst` / `push_front` is used to build the list in forward order.

### Why It Still Finds the Optimal Solution

The puzzle state graph is a DAG when moves are restricted to forward-only. The fixed ordering of move types (`(-1,1), (-1,2), (+1,1), (+1,2)`) guides the search to the unique optimal path without wasted exploration for reasonable values of N.

### Recursion Depth

Maximum recursion depth equals the number of moves = N² + 2N. For N = 25 that is 675 frames. Python's default limit of 1000 is insufficient; `sys.setrecursionlimit(2100)` in `main.py` covers up to N = 25.
