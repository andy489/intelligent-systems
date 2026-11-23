# Solution: Dots and Boxes

## Approach

### State Representation

- `h[r][c]` — boolean: horizontal edge between dot(r,c) and dot(r,c+1)
- `v[r][c]` — boolean: vertical edge between dot(r,c) and dot(r+1,c)
- `box[r][c]` — owner (0=unclaimed, 1=computer, 2=player)

A box `(r,c)` is complete when all four sides are set:
`h[r][c] ∧ h[r+1][c] ∧ v[r][c] ∧ v[r][c+1]`

### Key Minimax Twist

In Dots and Boxes, **completing a box gives you another turn**. The minimax handles this by keeping `is_max` unchanged when the current player closes one or more boxes.

### Move Ordering

Before searching, moves are sorted so that **box-closing moves come first**. This improves alpha-beta pruning efficiency significantly.

### Depth Limit and Heuristic

| Board | Edges | Search strategy |
|-------|-------|----------------|
| 2×2   | 12    | Exhaustive (depth = ∞) |
| 3×3   | 24    | Depth-limited (depth = 8) + heuristic |
| Larger | …    | Depth limited (default = 6) + heuristic |

The heuristic is:
```
score = (computer_boxes - player_boxes) + strategic_bonus
```
where:
- boxes with **3 edges** get `+0.3` (almost closed — good for whoever moves next)
- boxes with **2 edges** get `−0.1` (open chain — tends to benefit the opponent)

## Complexity

For 2×2: tree has at most 12! ≈ 479M leaves, but alpha-beta reduces this drastically to a few thousand nodes.

For 3×3 at depth 8: branching factor ~20, pruned heavily — typically responds in < 1 s.

## Files

- [`PYTHON/dots_and_boxes.py`](PYTHON/dots_and_boxes.py) — Python implementation
