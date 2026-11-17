# E03 — N-Sliding Blocks Puzzle: Results Analysis

Algorithm: **IDA\*** with **Manhattan Distance + Linear Conflict**  
Implementation: [`cpp/SlidingBlocksPuzzleProceduralSolution/NSlidingPuzzle.cpp`](../cpp/SlidingBlocksPuzzleProceduralSolution/NSlidingPuzzle.cpp) · [`java/SlidingBlocksPuzzleProceduralSolution/src/NSlidingPuzzle.java`](../java/SlidingBlocksPuzzleProceduralSolution/src/NSlidingPuzzle.java) · [`python/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle.py`](../python/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle.py)  
Runner: [`run_all_puzzles.py`](../run_all_puzzles.py)  
Timeout per puzzle: **300 s**

Algorithm details and hard puzzle solutions: [**SOLUTION.md**](SOLUTION.md)

---

## Summary

| Status | Count |
|---|---|
| Solved | 152 |
| Unsolvable (detected correctly) | 10 |
| Timed out | 0 |
| **Total** | **162** |

---

## Full Results Table

Puzzles are ordered by grid size (ascending), then by optimal move count (ascending). Unsolvable puzzles come first within each grid size group. Large-grid puzzles (5×5–10×10) appear at the end.

| # | Puzzle | Grid | Status | Optimal Moves | Elapsed (C++) |
|---|---|---|---|---|---|
| 001 | 001-puzzle-2x2-unsolvable | 2×2 | Unsolvable | — | — |
| 002 | 002-puzzle-2x2-unsolvable | 2×2 | Unsolvable | — | — |
| 003 | 003-puzzle-2x2-unsolvable | 2×2 | Unsolvable | — | — |
| 004 | 004-puzzle-2x2 | 2×2 | Solved | 0 | — |
| 005 | 005-puzzle-2x2 | 2×2 | Solved | 1 | 0.007 ms |
| 006 | 006-puzzle-2x2 | 2×2 | Solved | 1 | 0.032 ms |
| 007 | 007-puzzle-2x2 | 2×2 | Solved | 2 | 0.025 ms |
| 008 | 008-puzzle-2x2 | 2×2 | Solved | 3 | 0.006 ms |
| 009 | 009-puzzle-2x2 | 2×2 | Solved | 3 | 0.006 ms |
| 010 | 010-puzzle-2x2 | 2×2 | Solved | 4 | 0.008 ms |
| 011 | 011-puzzle-2x2 | 2×2 | Solved | 5 | 0.007 ms |
| 012 | 012-puzzle-2x2 | 2×2 | Solved | 6 | 0.007 ms |
| 013 | 013-puzzle-3x3-unsolvable | 3×3 | Unsolvable | — | — |
| 014 | 014-puzzle-3x3-unsolvable | 3×3 | Unsolvable | — | — |
| 015 | 015-puzzle-3x3-unsolvable | 3×3 | Unsolvable | — | — |
| 016 | 016-puzzle-3x3 | 3×3 | Solved | 1 | 0.007 ms |
| 017 | 017-puzzle-3x3 | 3×3 | Solved | 2 | 0.007 ms |
| 018 | 018-puzzle-3x3 | 3×3 | Solved | 3 | 0.007 ms |
| 019 | 019-puzzle-3x3 | 3×3 | Solved | 4 | 0.007 ms |
| 020 | 020-puzzle-3x3 | 3×3 | Solved | 4 | 0.007 ms |
| 021 | 021-puzzle-3x3 | 3×3 | Solved | 5 | 0.037 ms |
| 022 | 022-puzzle-3x3 | 3×3 | Solved | 5 | 0.009 ms |
| 023 | 023-puzzle-3x3 | 3×3 | Solved | 6 | 0.011 ms |
| 024 | 024-puzzle-3x3 | 3×3 | Solved | 7 | 0.012 ms |
| 025 | 025-puzzle-3x3 | 3×3 | Solved | 7 | 0.012 ms |
| 026 | 026-puzzle-3x3 | 3×3 | Solved | 8 | 0.012 ms |
| 027 | 027-puzzle-3x3 | 3×3 | Solved | 8 | 0.008 ms |
| 028 | 028-puzzle-3x3 | 3×3 | Solved | 9 | 0.012 ms |
| 029 | 029-puzzle-3x3 | 3×3 | Solved | 10 | 0.012 ms |
| 030 | 030-puzzle-3x3 | 3×3 | Solved | 11 | 0.017 ms |
| 031 | 031-puzzle-3x3 | 3×3 | Solved | 11 | 0.012 ms |
| 032 | 032-puzzle-3x3 | 3×3 | Solved | 12 | 0.012 ms |
| 033 | 033-puzzle-3x3 | 3×3 | Solved | 13 | 0.021 ms |
| 034 | 034-puzzle-3x3 | 3×3 | Solved | 14 | 0.027 ms |
| 035 | 035-puzzle-3x3 | 3×3 | Solved | 14 | 0.012 ms |
| 036 | 036-puzzle-3x3 | 3×3 | Solved | 15 | 0.022 ms |
| 037 | 037-puzzle-3x3 | 3×3 | Solved | 16 | 0.034 ms |
| 038 | 038-puzzle-3x3 | 3×3 | Solved | 17 | 0.060 ms |
| 039 | 039-puzzle-3x3 | 3×3 | Solved | 17 | 0.042 ms |
| 040 | 040-puzzle-3x3 | 3×3 | Solved | 18 | 0.089 ms |
| 041 | 041-puzzle-3x3 | 3×3 | Solved | 18 | 0.048 ms |
| 042 | 042-puzzle-3x3 | 3×3 | Solved | 19 | 0.022 ms |
| 043 | 043-puzzle-3x3 | 3×3 | Solved | 19 | 0.082 ms |
| 044 | 044-puzzle-3x3 | 3×3 | Solved | 20 | 0.089 ms |
| 045 | 045-puzzle-3x3 | 3×3 | Solved | 20 | 0.091 ms |
| 046 | 046-puzzle-3x3 | 3×3 | Solved | 21 | 0.086 ms |
| 047 | 047-puzzle-3x3 | 3×3 | Solved | 21 | 0.165 ms |
| 048 | 048-puzzle-3x3 | 3×3 | Solved | 22 | 0.169 ms |
| 049 | 049-puzzle-3x3 | 3×3 | Solved | 22 | 0.149 ms |
| 050 | 050-puzzle-3x3 | 3×3 | Solved | 23 | 0.070 ms |
| 051 | 051-puzzle-3x3 | 3×3 | Solved | 23 | 0.261 ms |
| 052 | 052-puzzle-3x3 | 3×3 | Solved | 24 | 0.288 ms |
| 053 | 053-puzzle-3x3 | 3×3 | Solved | 24 | 0.104 ms |
| 054 | 054-puzzle-3x3 | 3×3 | Solved | 25 | 0.316 ms |
| 055 | 055-puzzle-3x3 | 3×3 | Solved | 25 | 0.359 ms |
| 056 | 056-puzzle-3x3 | 3×3 | Solved | 26 | 0.174 ms |
| 057 | 057-puzzle-3x3 | 3×3 | Solved | 26 | 0.134 ms |
| 058 | 058-puzzle-3x3 | 3×3 | Solved | 27 | 1.225 ms |
| 059 | 059-puzzle-3x3 | 3×3 | Solved | 27 | 0.400 ms |
| 060 | 060-puzzle-3x3 | 3×3 | Solved | 28 | 0.944 ms |
| 061 | 061-puzzle-3x3 | 3×3 | Solved | 28 | 1.053 ms |
| 062 | 062-puzzle-3x3 | 3×3 | Solved | 29 | 1.198 ms |
| 063 | 063-puzzle-3x3 | 3×3 | Solved | 29 | 0.700 ms |
| 064 | 064-puzzle-3x3 | 3×3 | Solved | 30 | 0.640 ms |
| 065 | 065-puzzle-3x3 | 3×3 | Solved | 30 | 0.979 ms |
| 066 | 066-puzzle-3x3 | 3×3 | Solved | 31 | 1.131 ms |
| 067 | 067-puzzle-3x3 | 3×3 | Solved | 31 | 1.098 ms |
| 068 | 068-puzzle-4x4-unsolvable | 4×4 | Unsolvable | — | — |
| 069 | 069-puzzle-4x4-unsolvable | 4×4 | Unsolvable | — | — |
| 070 | 070-puzzle-4x4-unsolvable | 4×4 | Unsolvable | — | — |
| 071 | 071-puzzle-4x4-unsolvable | 4×4 | Unsolvable | — | — |
| 072 | 072-puzzle-4x4 | 4×4 | Solved | 0 | — |
| 073 | 073-puzzle-4x4 | 4×4 | Solved | 1 | 0.010 ms |
| 074 | 074-puzzle-4x4 | 4×4 | Solved | 2 | 0.010 ms |
| 075 | 075-puzzle-4x4 | 4×4 | Solved | 3 | 0.010 ms |
| 076 | 076-puzzle-4x4 | 4×4 | Solved | 4 | 0.011 ms |
| 077 | 077-puzzle-4x4 | 4×4 | Solved | 5 | 0.011 ms |
| 078 | 078-puzzle-4x4 | 4×4 | Solved | 6 | 0.012 ms |
| 079 | 079-puzzle-4x4 | 4×4 | Solved | 6 | 0.010 ms |
| 080 | 080-puzzle-4x4 | 4×4 | Solved | 7 | 0.013 ms |
| 081 | 081-puzzle-4x4 | 4×4 | Solved | 8 | 0.014 ms |
| 082 | 082-puzzle-4x4 | 4×4 | Solved | 9 | 0.011 ms |
| 083 | 083-puzzle-4x4 | 4×4 | Solved | 9 | 0.013 ms |
| 084 | 084-puzzle-4x4 | 4×4 | Solved | 10 | 0.012 ms |
| 085 | 085-puzzle-4x4 | 4×4 | Solved | 11 | 0.010 ms |
| 086 | 086-puzzle-4x4 | 4×4 | Solved | 12 | 0.012 ms |
| 087 | 087-puzzle-4x4 | 4×4 | Solved | 13 | 0.010 ms |
| 088 | 088-puzzle-4x4 | 4×4 | Solved | 13 | 0.011 ms |
| 089 | 089-puzzle-4x4 | 4×4 | Solved | 14 | 0.011 ms |
| 090 | 090-puzzle-4x4 | 4×4 | Solved | 15 | 0.013 ms |
| 091 | 091-puzzle-4x4 | 4×4 | Solved | 16 | 0.013 ms |
| 092 | 092-puzzle-4x4 | 4×4 | Solved | 17 | 0.011 ms |
| 093 | 093-puzzle-4x4 | 4×4 | Solved | 18 | 0.029 ms |
| 094 | 094-puzzle-4x4 | 4×4 | Solved | 19 | 0.012 ms |
| 095 | 095-puzzle-4x4 | 4×4 | Solved | 20 | 0.011 ms |
| 096 | 096-puzzle-4x4 | 4×4 | Solved | 21 | 0.015 ms |
| 097 | 097-puzzle-4x4 | 4×4 | Solved | 22 | 0.012 ms |
| 098 | 098-puzzle-4x4 | 4×4 | Solved | 23 | 0.015 ms |
| 099 | 099-puzzle-4x4 | 4×4 | Solved | 24 | 0.014 ms |
| 100 | 100-puzzle-4x4 | 4×4 | Solved | 25 | 0.017 ms |
| 101 | 101-puzzle-4x4 | 4×4 | Solved | 26 | 0.039 ms |
| 102 | 102-puzzle-4x4 | 4×4 | Solved | 27 | 0.028 ms |
| 103 | 103-puzzle-4x4 | 4×4 | Solved | 28 | 0.015 ms |
| 104 | 104-puzzle-4x4 | 4×4 | Solved | 29 | 0.027 ms |
| 105 | 105-puzzle-4x4 | 4×4 | Solved | 30 | 0.013 ms |
| 106 | 106-puzzle-4x4 | 4×4 | Solved | 31 | 0.064 ms |
| 107 | 107-puzzle-4x4 | 4×4 | Solved | 32 | 0.189 ms |
| 108 | 108-puzzle-4x4 | 4×4 | Solved | 32 | 0.087 ms |
| 109 | 109-puzzle-4x4 | 4×4 | Solved | 33 | 0.035 ms |
| 110 | 110-puzzle-4x4 | 4×4 | Solved | 33 | 0.722 ms |
| 111 | 111-puzzle-4x4 | 4×4 | Solved | 34 | 0.112 ms |
| 112 | 112-puzzle-4x4 | 4×4 | Solved | 34 | 0.221 ms |
| 113 | 113-puzzle-4x4 | 4×4 | Solved | 35 | 0.058 ms |
| 114 | 114-puzzle-4x4 | 4×4 | Solved | 35 | 0.273 ms |
| 115 | 115-puzzle-4x4 | 4×4 | Solved | 36 | 0.804 ms |
| 116 | 116-puzzle-4x4 | 4×4 | Solved | 36 | 0.841 ms |
| 117 | 117-puzzle-4x4 | 4×4 | Solved | 37 | 0.261 ms |
| 118 | 118-puzzle-4x4 | 4×4 | Solved | 37 | 0.080 ms |
| 119 | 119-puzzle-4x4 | 4×4 | Solved | 38 | 0.960 ms |
| 120 | 120-puzzle-4x4 | 4×4 | Solved | 38 | 0.784 ms |
| 121 | 121-puzzle-4x4 | 4×4 | Solved | 39 | 0.062 ms |
| 122 | 122-puzzle-4x4 | 4×4 | Solved | 39 | 0.076 ms |
| 123 | 123-puzzle-4x4 | 4×4 | Solved | 40 | 0.146 ms |
| 124 | 124-puzzle-4x4 | 4×4 | Solved | 40 | 0.313 ms |
| 125 | 125-puzzle-4x4 | 4×4 | Solved | 41 | 0.250 ms |
| 126 | 126-puzzle-4x4 | 4×4 | Solved | 42 | 0.154 ms |
| 127 | 127-puzzle-4x4 | 4×4 | Solved | 42 | 0.706 ms |
| 128 | 128-puzzle-4x4 | 4×4 | Solved | 43 | 5.576 ms |
| 129 | 129-puzzle-4x4 | 4×4 | Solved | 43 | 1.021 ms |
| 130 | 130-puzzle-4x4 | 4×4 | Solved | 44 | 0.158 ms |
| 131 | 131-puzzle-4x4 | 4×4 | Solved | 45 | 10.169 ms |
| 132 | 132-puzzle-4x4 | 4×4 | Solved | 45 | 0.585 ms |
| 133 | 133-puzzle-4x4 | 4×4 | Solved | 46 | 7.751 ms |
| 134 | 134-puzzle-4x4 | 4×4 | Solved | 46 | 0.065 ms |
| 135 | 135-puzzle-4x4 | 4×4 | Solved | 47 | 1.369 ms |
| 136 | 136-puzzle-4x4 | 4×4 | Solved | 47 | 1.211 ms |
| 137 | 137-puzzle-4x4 | 4×4 | Solved | 47 | 116.174 ms |
| 138 | 138-puzzle-4x4 | 4×4 | Solved | 48 | 1.344 ms |
| 139 | 139-puzzle-4x4 | 4×4 | Solved | 48 | 0.932 ms |
| 140 | 140-puzzle-4x4 | 4×4 | Solved | 48 | 112.822 ms |
| 141 | 141-puzzle-4x4 | 4×4 | Solved | 48 | 112.276 ms |
| 142 | 142-puzzle-4x4 | 4×4 | Solved | 49 | 37.096 ms |
| 143 | 143-puzzle-4x4 | 4×4 | Solved | 49 | 6.801 ms |
| 144 | 144-puzzle-4x4 | 4×4 | Solved | 49 | 408.938 ms |
| 145 | 145-puzzle-4x4 | 4×4 | Solved | 50 | 29.084 ms |
| 146 | 146-puzzle-4x4 | 4×4 | Solved | 50 | 0.927 ms |
| 147 | 147-puzzle-4x4 | 4×4 | Solved | 50 | 0.955 ms |
| 148 | 148-puzzle-4x4 | 4×4 | Solved | 50 | 767.166 ms |
| 149 | 149-puzzle-4x4 | 4×4 | Solved | 51 | 156.187 ms |
| 150 | 150-puzzle-4x4 | 4×4 | Solved | 52 | 310.373 ms |
| 151 | 151-puzzle-4x4-HARD | 4×4 | Solved | 53 | 1.327 s |
| 152 | 152-puzzle-4x4-HARD-2 | 4×4 | Solved | 56 | 365.801 ms |
| 153 | 153-puzzle-4x4-MEGA-HARD | 4×4 | Solved | 66 | 3.580 s |
| 154 | 154-puzzle-4x4-MEGA-HARD-2 | 4×4 | Solved | 78 | 2.028 s |
| 155 | 155-puzzle-4x4-ULTRA-HARD | 4×4 | Solved | 80 | 00:02:39.367 |
| 156 | 156-puzzle-5x5 | 5×5 | Solved | 12 | 0.010 ms |
| 157 | 157-puzzle-5x5 | 5×5 | Solved | 41 | 1.263 ms |
| 158 | 158-puzzle-5x5 | 5×5 | Solved | 44 | 1.282 ms |
| 159 | 159-puzzle-7x7 | 7×7 | Solved | 14 | 0.075 ms |
| 160 | 160-puzzle-8x8 | 8×8 | Solved | 15 | 0.012 ms |
| 161 | 161-puzzle-9x9 | 9×9 | Solved | 10 | 0.010 ms |
| 162 | 162-puzzle-10x10 | 10×10 | Solved | 16 | 0.055 ms |

---

## Analysis

### IDA\* performance by grid size

| Grid | Puzzles in set | Max optimal moves | Max elapsed (C++) |
|---|---|---|---|
| 2×2 | 12 | 6 | 0.032 ms |
| 3×3 | 55 | 31 | 1.225 ms |
| 4×4 | 88 | 80 | 00:02:39.367 |
| 5×5 | 3 | 44 | 1.282 ms |
| 7×7 | 1 | 14 | 0.075 ms |
| 8×8 | 1 | 15 | 0.012 ms |
| 9×9 | 1 | 10 | 0.010 ms |
| 10×10 | 1 | 16 | 0.055 ms |

Note: large grid puzzles (7×7–10×10) solved instantly because they were close to the goal state (≤16 moves). IDA\* difficulty depends on **solution depth**, not grid size alone.

### Elapsed time vs. optimal moves (4×4 puzzles)

Elapsed time grows roughly exponentially with solution depth:

| Moves | Elapsed (C++) |
|---|---|
| ≤ 30 | ≤ 0.064 ms |
| 31–35 | 0.058–0.722 ms |
| 36–40 | 0.076–0.960 ms |
| 41–42 | 0.154–0.706 ms |
| 43–44 | 0.158–5.576 ms |
| 45–47 | 0.065–116.174 ms |
| 48–50 | 0.927 ms–767.166 ms |
| 51–53 | 156.187 ms–1.327 s |
| 56 | 365.801 ms |
| 66 | 3.580 s |
| 78 | 2.028 s |
| 80 | 00:02:39.367 |

The wide variance within a move-count band reflects the structure of individual puzzles — two puzzles with the same optimal depth can differ by 100× in IDA\* cost depending on how tightly the heuristic prunes each one.

### Unsolvable puzzles — solvability check for non-default blank targets

Puzzles 069, 070, and 071 share the same board but specify non-default blank target positions (indices 0, 2, and 8 respectively, rather than the default 15). A solvability-check bug originally caused them to be misclassified as solvable, sending IDA\* into an infinite search.

The board:
```
 2  9  3  5
 8 11 12  7
15  4  _  13
 6  1 10 14
```

The correct parity check for even-width grids is:

```
(inversions + blank_current_row_from_bottom + blank_target_row_from_bottom) % 2 == 0
```

The original check omitted `blank_target_row_from_bottom`. Once both rows are included, all three puzzles are correctly detected as unsolvable in under 1 ms.

---

## Observations

1. **MD + LC is sufficient to solve all 162 puzzles** — including the 80-move ULTRA-HARD puzzle — without any pattern databases. The key is recomputing LC at every DLS node, not just at the IDA\* root.

2. **Recomputing LC per node is the single most important optimisation.** Without it, the heuristic degrades to pure MD inside DLS, and hard puzzles time out. With it, each node gets a tighter h-value that prunes branches MD alone would explore.

3. **Solution depth, not grid size, determines IDA\* cost.** A 10×10 puzzle with 16 optimal moves runs in 0.055 ms. A 4×4 puzzle with 80 moves takes 2 min 39 s. Grid size only matters insofar as it affects the branching factor.

4. **Exponential blow-up** is clearly visible in the 4×4 results: 47 moves can take anywhere from 1.211 ms to 116.174 ms depending on heuristic quality for that specific board; 80 moves takes >100× longer than 78 moves.

5. **154-puzzle-4x4-MEGA-HARD-2** (fully reversed board, 78 moves) solves in **2.028 s** with MD+LC recomputed per node.

6. **155-puzzle-4x4-ULTRA-HARD** (80 moves) now solves in **2 min 39 s** — previously timed out at 300 s before the LC-per-node fix was applied.

7. **Python is algorithmically correct** but the recursion overhead makes depths >~57 impractical. This is not an algorithmic limitation — it is a language speed issue. C++ solves all 162 puzzles; Python would need a native extension or iterative stack rewrite for the very hardest ones.

8. **Variance within a move-count band** is large and unpredictable. Two 48-move puzzles (138 vs 140) differ by a factor of 84× (1.344 ms vs 112.822 ms). The heuristic's ability to prune a particular board state is highly problem-specific.

9. **117-puzzle-4x4** (37 moves, 0.261 ms) was previously misreported as 49 moves due to a missing `target_index` line in the puzzle file causing the parser to consume the first board tile as the target. With the corrected input the board solves in 37 optimal moves instantly.
