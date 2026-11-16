# E01 — Frog Leap Puzzle

## Problem Statement

The **Frog Leap Puzzle** is a classic combinatorial puzzle played on a linear board of `2N + 1` cells.

### Setup

- `N` green frogs sit on the left half of the board (positions `0..N-1`), each facing right.
- `N` brown frogs sit on the right half (positions `N+1..2N`), each facing left.
- One empty cell (the "leaf") sits in the middle at position `N`.

```
Initial state (N = 3):   L L L _ R R R
Goal state   (N = 3):    R R R _ L L L
```

### Rules

A frog may make exactly two kinds of moves:

| Move | Condition |
|---|---|
| **Step** — move one cell into the empty space | The frog is directly adjacent to the leaf |
| **Jump** — leap over one frog of the opposite colour | The frog is two cells from the leaf, and the cell between them is occupied by an opponent |

A frog may only move **forward** (green frogs move right, brown frogs move left). No backward moves are allowed.

### Objective

Move all green frogs to the right half and all brown frogs to the left half, ending up in the mirror image of the starting position.

### Constraints

- `1 ≤ N ≤ 25` (practical limit imposed by recursion depth for DFS; linear solution works for any N)
- The minimum number of moves to solve the puzzle for a given `N` is **N² + 2N** (optimal).

### Try It Online

Play the puzzle interactively: [NIEHS Frog Leap Game](https://www.niehs.nih.gov/health/kids/games/brainteasers/frog-leap)
