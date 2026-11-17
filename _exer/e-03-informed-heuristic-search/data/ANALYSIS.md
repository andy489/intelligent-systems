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

| # | Puzzle | Grid | Status | Optimal Moves | Elapsed (C++) |
|---|---|---|---|---|---|
| 001 | 001-puzzle-2x2-unsolvable | 2×2 | Unsolvable | — | — |
| 002 | 002-puzzle-2x2-unsolvable | 2×2 | Unsolvable | — | — |
| 003 | 003-puzzle-2x2-unsolvable | 2×2 | Unsolvable | — | — |
| 004 | 004-puzzle-3x3-unsolvable | 3×3 | Unsolvable | — | — |
| 005 | 005-puzzle-3x3-unsolvable | 3×3 | Unsolvable | — | — |
| 006 | 006-puzzle-3x3-unsolvable | 3×3 | Unsolvable | — | — |
| 007 | 007-puzzle-4x4-unsolvable | 4×4 | Unsolvable | — | — |
| 008 | 008-puzzle-4x4 | 4×4 | Solved | 49 | 1.062 s |
| 009 | 009-puzzle-4x4-unsolvable | 4×4 | Unsolvable | — | — |
| 010 | 010-puzzle-4x4-unsolvable | 4×4 | Unsolvable | — | — |
| 011 | 011-puzzle-4x4-unsolvable | 4×4 | Unsolvable | — | — |
| 012 | 012-puzzle-2x2 | 2×2 | Solved | 0 | — (already solved) |
| 013 | 013-puzzle-4x4 | 4×4 | Solved | 0 | — (already solved) |
| 014 | 014-puzzle-2x2 | 2×2 | Solved | 1 | <1 ms |
| 015 | 015-puzzle-2x2 | 2×2 | Solved | 1 | <1 ms |
| 016 | 016-puzzle-3x3 | 3×3 | Solved | 1 | <1 ms |
| 017 | 017-puzzle-4x4 | 4×4 | Solved | 1 | <1 ms |
| 018 | 018-puzzle-2x2 | 2×2 | Solved | 2 | <1 ms |
| 019 | 019-puzzle-3x3 | 3×3 | Solved | 2 | <1 ms |
| 020 | 020-puzzle-4x4 | 4×4 | Solved | 2 | <1 ms |
| 021 | 021-puzzle-2x2 | 2×2 | Solved | 3 | <1 ms |
| 022 | 022-puzzle-2x2 | 2×2 | Solved | 3 | <1 ms |
| 023 | 023-puzzle-3x3 | 3×3 | Solved | 3 | <1 ms |
| 024 | 024-puzzle-4x4 | 4×4 | Solved | 3 | <1 ms |
| 025 | 025-puzzle-2x2 | 2×2 | Solved | 4 | <1 ms |
| 026 | 026-puzzle-3x3 | 3×3 | Solved | 4 | <1 ms |
| 027 | 027-puzzle-3x3 | 3×3 | Solved | 4 | <1 ms |
| 028 | 028-puzzle-4x4 | 4×4 | Solved | 4 | <1 ms |
| 029 | 029-puzzle-2x2 | 2×2 | Solved | 5 | <1 ms |
| 030 | 030-puzzle-3x3 | 3×3 | Solved | 5 | <1 ms |
| 031 | 031-puzzle-4x4 | 4×4 | Solved | 5 | <1 ms |
| 032 | 032-puzzle-3x3 | 3×3 | Solved | 5 | <1 ms |
| 033 | 033-puzzle-2x2 | 2×2 | Solved | 6 | <1 ms |
| 034 | 034-puzzle-3x3 | 3×3 | Solved | 6 | <1 ms |
| 035 | 035-puzzle-4x4 | 4×4 | Solved | 6 | <1 ms |
| 036 | 036-puzzle-4x4 | 4×4 | Solved | 6 | <1 ms |
| 037 | 037-puzzle-3x3 | 3×3 | Solved | 7 | <1 ms |
| 038 | 038-puzzle-4x4 | 4×4 | Solved | 7 | <1 ms |
| 039 | 039-puzzle-3x3 | 3×3 | Solved | 7 | <1 ms |
| 040 | 040-puzzle-3x3 | 3×3 | Solved | 8 | <1 ms |
| 041 | 041-puzzle-4x4 | 4×4 | Solved | 8 | <1 ms |
| 042 | 042-puzzle-3x3 | 3×3 | Solved | 8 | <1 ms |
| 043 | 043-puzzle-3x3 | 3×3 | Solved | 9 | <1 ms |
| 044 | 044-puzzle-4x4 | 4×4 | Solved | 9 | <1 ms |
| 045 | 045-puzzle-4x4 | 4×4 | Solved | 9 | <1 ms |
| 046 | 046-puzzle-3x3 | 3×3 | Solved | 10 | <1 ms |
| 047 | 047-puzzle-4x4 | 4×4 | Solved | 10 | <1 ms |
| 048 | 048-puzzle-9x9 | 9×9 | Solved | 10 | <1 ms |
| 049 | 049-puzzle-3x3 | 3×3 | Solved | 11 | <1 ms |
| 050 | 050-puzzle-4x4 | 4×4 | Solved | 11 | <1 ms |
| 051 | 051-puzzle-3x3 | 3×3 | Solved | 11 | <1 ms |
| 052 | 052-puzzle-3x3 | 3×3 | Solved | 12 | <1 ms |
| 053 | 053-puzzle-4x4 | 4×4 | Solved | 12 | <1 ms |
| 054 | 054-puzzle-5x5 | 5×5 | Solved | 12 | <1 ms |
| 055 | 055-puzzle-3x3 | 3×3 | Solved | 13 | <1 ms |
| 056 | 056-puzzle-4x4 | 4×4 | Solved | 13 | <1 ms |
| 057 | 057-puzzle-4x4 | 4×4 | Solved | 13 | <1 ms |
| 058 | 058-puzzle-3x3 | 3×3 | Solved | 14 | <1 ms |
| 059 | 059-puzzle-3x3 | 3×3 | Solved | 14 | <1 ms |
| 060 | 060-puzzle-4x4 | 4×4 | Solved | 14 | <1 ms |
| 061 | 061-puzzle-7x7 | 7×7 | Solved | 14 | <1 ms |
| 062 | 062-puzzle-3x3 | 3×3 | Solved | 15 | <1 ms |
| 063 | 063-puzzle-4x4 | 4×4 | Solved | 15 | <1 ms |
| 064 | 064-puzzle-8x8 | 8×8 | Solved | 15 | <1 ms |
| 065 | 065-puzzle-3x3 | 3×3 | Solved | 16 | <1 ms |
| 066 | 066-puzzle-4x4 | 4×4 | Solved | 16 | <1 ms |
| 067 | 067-puzzle-10x10 | 10×10 | Solved | 16 | <1 ms |
| 068 | 068-puzzle-3x3 | 3×3 | Solved | 17 | <1 ms |
| 069 | 069-puzzle-4x4 | 4×4 | Solved | 17 | <1 ms |
| 070 | 070-puzzle-3x3 | 3×3 | Solved | 17 | <1 ms |
| 071 | 071-puzzle-3x3 | 3×3 | Solved | 18 | <1 ms |
| 072 | 072-puzzle-4x4 | 4×4 | Solved | 18 | <1 ms |
| 073 | 073-puzzle-3x3 | 3×3 | Solved | 18 | <1 ms |
| 074 | 074-puzzle-3x3 | 3×3 | Solved | 19 | <1 ms |
| 075 | 075-puzzle-4x4 | 4×4 | Solved | 19 | <1 ms |
| 076 | 076-puzzle-3x3 | 3×3 | Solved | 19 | <1 ms |
| 077 | 077-puzzle-3x3 | 3×3 | Solved | 20 | <1 ms |
| 078 | 078-puzzle-4x4 | 4×4 | Solved | 20 | <1 ms |
| 079 | 079-puzzle-3x3 | 3×3 | Solved | 20 | <1 ms |
| 080 | 080-puzzle-3x3 | 3×3 | Solved | 21 | <1 ms |
| 081 | 081-puzzle-4x4 | 4×4 | Solved | 21 | <1 ms |
| 082 | 082-puzzle-3x3 | 3×3 | Solved | 21 | <1 ms |
| 083 | 083-puzzle-3x3 | 3×3 | Solved | 22 | <1 ms |
| 084 | 084-puzzle-4x4 | 4×4 | Solved | 22 | <1 ms |
| 085 | 085-puzzle-3x3 | 3×3 | Solved | 22 | <1 ms |
| 086 | 086-puzzle-3x3 | 3×3 | Solved | 23 | <1 ms |
| 087 | 087-puzzle-4x4 | 4×4 | Solved | 23 | <1 ms |
| 088 | 088-puzzle-3x3 | 3×3 | Solved | 23 | <1 ms |
| 089 | 089-puzzle-3x3 | 3×3 | Solved | 24 | <1 ms |
| 090 | 090-puzzle-4x4 | 4×4 | Solved | 24 | <1 ms |
| 091 | 091-puzzle-3x3 | 3×3 | Solved | 24 | <1 ms |
| 092 | 092-puzzle-3x3 | 3×3 | Solved | 25 | <1 ms |
| 093 | 093-puzzle-4x4 | 4×4 | Solved | 25 | <1 ms |
| 094 | 094-puzzle-3x3 | 3×3 | Solved | 25 | <1 ms |
| 095 | 095-puzzle-3x3 | 3×3 | Solved | 26 | <1 ms |
| 096 | 096-puzzle-4x4 | 4×4 | Solved | 26 | <1 ms |
| 097 | 097-puzzle-3x3 | 3×3 | Solved | 26 | <1 ms |
| 098 | 098-puzzle-3x3 | 3×3 | Solved | 27 | <1 ms |
| 099 | 099-puzzle-4x4 | 4×4 | Solved | 27 | <1 ms |
| 100 | 100-puzzle-3x3 | 3×3 | Solved | 27 | <1 ms |
| 101 | 101-puzzle-3x3 | 3×3 | Solved | 28 | <1 ms |
| 102 | 102-puzzle-4x4 | 4×4 | Solved | 28 | <1 ms |
| 103 | 103-puzzle-3x3 | 3×3 | Solved | 28 | <1 ms |
| 104 | 104-puzzle-3x3 | 3×3 | Solved | 29 | <1 ms |
| 105 | 105-puzzle-4x4 | 4×4 | Solved | 29 | <1 ms |
| 106 | 106-puzzle-3x3 | 3×3 | Solved | 29 | <1 ms |
| 107 | 107-puzzle-3x3 | 3×3 | Solved | 30 | <1 ms |
| 108 | 108-puzzle-4x4 | 4×4 | Solved | 30 | <1 ms |
| 109 | 109-puzzle-3x3 | 3×3 | Solved | 30 | <1 ms |
| 110 | 110-puzzle-3x3 | 3×3 | Solved | 31 | <1 ms |
| 111 | 111-puzzle-4x4 | 4×4 | Solved | 31 | <1 ms |
| 112 | 112-puzzle-3x3 | 3×3 | Solved | 31 | <1 ms |
| 113 | 113-puzzle-4x4 | 4×4 | Solved | 32 | <1 ms |
| 114 | 114-puzzle-4x4 | 4×4 | Solved | 32 | <1 ms |
| 115 | 115-puzzle-4x4 | 4×4 | Solved | 33 | <1 ms |
| 116 | 116-puzzle-4x4 | 4×4 | Solved | 33 | <1 ms |
| 117 | 117-puzzle-4x4 | 4×4 | Solved | 34 | <1 ms |
| 118 | 118-puzzle-4x4 | 4×4 | Solved | 34 | <1 ms |
| 119 | 119-puzzle-4x4 | 4×4 | Solved | 35 | <1 ms |
| 120 | 120-puzzle-4x4 | 4×4 | Solved | 35 | <1 ms |
| 121 | 121-puzzle-4x4 | 4×4 | Solved | 36 | <1 ms |
| 122 | 122-puzzle-4x4 | 4×4 | Solved | 36 | <1 ms |
| 123 | 123-puzzle-4x4 | 4×4 | Solved | 37 | <1 ms |
| 124 | 124-puzzle-4x4 | 4×4 | Solved | 38 | <1 ms |
| 125 | 125-puzzle-4x4 | 4×4 | Solved | 38 | <1 ms |
| 126 | 126-puzzle-4x4 | 4×4 | Solved | 39 | <1 ms |
| 127 | 127-puzzle-4x4 | 4×4 | Solved | 39 | <1 ms |
| 128 | 128-puzzle-4x4 | 4×4 | Solved | 40 | <1 ms |
| 129 | 129-puzzle-4x4 | 4×4 | Solved | 40 | <1 ms |
| 130 | 130-puzzle-4x4 | 4×4 | Solved | 41 | <1 ms |
| 131 | 131-puzzle-5x5 | 5×5 | Solved | 41 | 1 ms |
| 132 | 132-puzzle-4x4 | 4×4 | Solved | 42 | <1 ms |
| 133 | 133-puzzle-4x4 | 4×4 | Solved | 42 | <1 ms |
| 134 | 134-puzzle-4x4 | 4×4 | Solved | 43 | 5 ms |
| 135 | 135-puzzle-4x4 | 4×4 | Solved | 43 | 1 ms |
| 136 | 136-puzzle-4x4 | 4×4 | Solved | 44 | <1 ms |
| 137 | 137-puzzle-5x5 | 5×5 | Solved | 44 | 1 ms |
| 138 | 138-puzzle-4x4 | 4×4 | Solved | 45 | 9 ms |
| 139 | 139-puzzle-4x4 | 4×4 | Solved | 45 | <1 ms |
| 140 | 140-puzzle-4x4 | 4×4 | Solved | 46 | 7 ms |
| 141 | 141-puzzle-4x4 | 4×4 | Solved | 46 | <1 ms |
| 142 | 142-puzzle-4x4 | 4×4 | Solved | 47 | 1 ms |
| 143 | 143-puzzle-4x4 | 4×4 | Solved | 47 | 1 ms |
| 144 | 144-puzzle-4x4 | 4×4 | Solved | 47 | 111 ms |
| 145 | 145-puzzle-4x4 | 4×4 | Solved | 48 | 1 ms |
| 146 | 146-puzzle-4x4 | 4×4 | Solved | 48 | <1 ms |
| 147 | 147-puzzle-4x4 | 4×4 | Solved | 48 | 112 ms |
| 148 | 148-puzzle-4x4 | 4×4 | Solved | 48 | 113 ms |
| 149 | 149-puzzle-4x4 | 4×4 | Solved | 49 | 36 ms |
| 150 | 150-puzzle-4x4 | 4×4 | Solved | 49 | 6 ms |
| 151 | 151-puzzle-4x4 | 4×4 | Solved | 49 | 409 ms |
| 152 | 152-puzzle-4x4 | 4×4 | Solved | 50 | 28 ms |
| 153 | 153-puzzle-4x4 | 4×4 | Solved | 50 | <1 ms |
| 154 | 154-puzzle-4x4 | 4×4 | Solved | 50 | <1 ms |
| 155 | 155-puzzle-4x4 | 4×4 | Solved | 50 | 764 ms |
| 156 | 156-puzzle-4x4 | 4×4 | Solved | 51 | 155 ms |
| 157 | 157-puzzle-4x4 | 4×4 | Solved | 52 | 310 ms |
| 158 | 158-puzzle-4x4-HARD | 4×4 | Solved | 53 | 1.326 s |
| 159 | 159-puzzle-4x4-HARD-2 | 4×4 | Solved | 56 | 361 ms |
| 160 | 160-puzzle-4x4-MEGA-HARD | 4×4 | Solved | 66 | 3.535 s |
| 161 | 161-puzzle-4x4-MEGA-HARD-2 | 4×4 | Solved | 78 | 2.004 s |
| 162 | 162-puzzle-4x4-ULTRA-HARD | 4×4 | Solved | 80 | 2 min 37 s |

---

## Analysis

### IDA\* performance by grid size

| Grid | Puzzles in set | Max optimal moves | Max elapsed (C++) |
|---|---|---|---|
| 2×2 | 12 | 6 | <1 ms |
| 3×3 | 46 | 31 | <1 ms |
| 4×4 | 94 | 80 | 2 min 37 s |
| 5×5 | 3 | 44 | 1 ms |
| 7×7 | 1 | 14 | <1 ms |
| 8×8 | 1 | 15 | <1 ms |
| 9×9 | 1 | 10 | <1 ms |
| 10×10 | 1 | 16 | <1 ms |

Note: large grid puzzles (7×7–10×10) solved instantly because they were close to the goal state (≤16 moves). IDA\* difficulty depends on **solution depth**, not grid size alone.

### Elapsed time vs. optimal moves (4×4 puzzles)

Elapsed time grows roughly exponentially with solution depth:

| Moves | Elapsed (C++) |
|---|---|
| ≤ 40 | <1 ms |
| 41–43 | 1–11 ms |
| 44–47 | 1–111 ms |
| 48–50 | <1 ms – 764 ms |
| 51–53 | 155 ms – 1.3 s |
| 56 | 361 ms |
| 66 | 3.5 s |
| 78 | 2.0 s |
| 80 | 2 min 37 s |

The wide variance within a move-count band reflects the structure of individual puzzles — two puzzles with the same optimal depth can differ by 10× in IDA\* cost depending on how tightly the heuristic prunes each one.

### Unsolvable puzzles — solvability check for non-default blank targets

Puzzles 009, 010, and 011 share the same board but specify non-default blank target positions (indices 0, 2, and 8 respectively, rather than the default 15). A solvability-check bug originally caused them to be misclassified as solvable, sending IDA\* into an infinite search.

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

3. **Solution depth, not grid size, determines IDA\* cost.** A 10×10 puzzle with 16 optimal moves runs in <1 ms. A 4×4 puzzle with 80 moves takes 2 min 37 s. Grid size only matters insofar as it affects the branching factor.

4. **Exponential blow-up** is clearly visible in the 4×4 results: 47 moves can take anywhere from 1 ms to 111 ms depending on heuristic quality for that specific board; 80 moves takes >150× longer than 78 moves.

5. **puzzle161-4x4-MEGA-HARD-2** (fully reversed board, 78 moves) solves in **2.0 s** with MD+LC recomputed per node.

6. **puzzle162-4x4-ULTRA-HARD** (80 moves) now solves in **2 min 37 s** — previously timed out at 300 s before the LC-per-node fix was applied.

7. **Python is algorithmically correct** but the recursion overhead makes depths >~57 impractical. This is not an algorithmic limitation — it is a language speed issue. C++ solves all 162 puzzles; Python would need a native extension or iterative stack rewrite for the very hardest ones.

8. **Variance within a move-count band** is large and unpredictable. Two 48-move puzzles (145 vs 147) differ by a factor of 100× (1 ms vs 112 ms). The heuristic's ability to prune a particular board state is highly problem-specific.

9. **puzzle008-4x4** (49 moves, 1.062 s) is a good example of a nominally deep puzzle that is solved quickly — its specific layout allows the heuristic to prune very aggressively.
