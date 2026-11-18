# Problem — N-Sliding Blocks Puzzle

## Statement

Given an **N×N grid** with N²−1 numbered tiles and one blank, slide tiles into the
blank until the grid reaches the **goal configuration** (tiles 1 … N²−1 in row-major
order, blank at the bottom-right).

```
Initial (3×3):   Goal:
 2  8  3         1  2  3
 1  6  4         4  5  6
 7  _  5         7  8  _
```

## Rules

- A tile may slide into the blank if it is **immediately adjacent** (up, down, left, right).
- Each slide counts as one move.
- Find the **shortest** sequence of moves (minimum move count).

## Input / Output

**Input:** First line = `N` (grid side length). Following N lines = row contents,
blank represented as `0`.

**Output:** Minimum number of moves, then the move sequence (optional), and elapsed time.

## Examples

| Grid | Puzzle | Min moves |
|------|--------|-----------|
| 3×3 | 8-puzzle | 0 – 31 |
| 4×4 | 15-puzzle | 0 – 80 |
| 5×5 | 24-puzzle | can exceed 100 |

## Complexity

The N-sliding puzzle is **PSPACE-complete** in the general case.

For the 4×4 (15-puzzle) the state space is 16!/2 ≈ 10¹³.
Brute-force BFS is infeasible; an admissible heuristic guided search is required.

## Scale Requirement

> **The solution must handle at least the standard 4×4 (15-puzzle) in a reasonable time.**

Target: solve any solvable 4×4 puzzle in **under 60 seconds** on a modern laptop.
Harder instances (deep 5×5, 6×6, …) may take longer or time out.

## Solvability

An N-puzzle instance is **unsolvable** if and only if:
- N is odd and the number of inversions is **odd**, OR
- N is even and (inversions + blank row from bottom) is **odd**.

An unsolvable instance must be detected and reported without searching.

## Test Data

[`data/puzzles/`](data/puzzles/) — 162 benchmark puzzles: 2×2 through 10×10,
including 5 hard 4×4 instances.
