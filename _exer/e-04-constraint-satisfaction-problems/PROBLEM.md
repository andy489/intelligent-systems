# Problem — N-Queens as a Constraint Satisfaction Problem

## Statement

Place **N non-attacking queens** on an N×N chessboard.

Two queens attack each other if they share the same row, column, or diagonal. A valid solution assigns exactly one queen to each column such that no two queens threaten each other.

## Formal CSP Definition

| Component | Definition |
|-----------|-----------|
| **Variables** | Q₁, Q₂, …, Qₙ — one per column, Qᵢ is the row of the queen in column i |
| **Domains** | Dᵢ = {0, 1, …, N−1} for all i |
| **Constraints** | ∀ i ≠ j: Qᵢ ≠ Qⱼ (no shared row) and \|Qᵢ − Qⱼ\| ≠ \|i − j\| (no shared diagonal) |

## Input / Output

**Input:** A single integer N read from standard input.

**Output:**
- If a solution exists, print elapsed time to stderr.
- Optionally print the board to stdout (`*` = queen, `_` = empty).

## Special Cases

| N | Expected behaviour |
|---|-------------------|
| 1 | Trivially solved (single queen on 1×1 board) |
| 2 | No solution — program exits with error |
| 3 | No solution — program exits with error |
| N ≥ 4 | Solution always exists |

## Scale Requirement

> **The solution must handle at least N = 10 000 within a reasonable time.**

For N = 10 000 the board has 10⁸ cells. Brute-force backtracking is completely infeasible. The algorithm must be near-linear in N — O(N) or O(N log N) in practice — to meet this requirement.

Typical target: solve N = 10 000 in **under 5 seconds** on a modern laptop.
