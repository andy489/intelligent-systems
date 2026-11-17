# E03 — N-Sliding Blocks Puzzle: Results Analysis

Algorithm: **IDA\*** with **Additive 5-5-5 PDB** (4×4 canonical) / **Manhattan Distance + Linear Conflict** (all other grids)  
Implementation: [`cpp/SlidingBlocksPuzzleProceduralSolution/NSlidingPuzzle.cpp`](../cpp/SlidingBlocksPuzzleProceduralSolution/NSlidingPuzzle.cpp) · [`java/SlidingBlocksPuzzleProceduralSolution/src/NSlidingPuzzle.java`](../java/SlidingBlocksPuzzleProceduralSolution/src/NSlidingPuzzle.java) · [`python/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle.py`](../python/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle.py)  
Runners: [`run_cpp.py`](run_cpp.py) · [`run_java.py`](run_java.py) · [`run_python.py`](run_python.py) (write to `data/results-cpp/`, `data/results-java/`, `data/results-python/`)  
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

Sorted by optimal move count (ascending). Unsolvable puzzles appear first. Within the same move count, puzzles are ordered by puzzle number.

**Time format legend:**
- `0.007 ms` — sub-millisecond (C++ only; µs resolution): 7 microseconds
- `1.225 ms` — milliseconds with µs fraction: 1225 µs
- `3.580 s` — seconds with ms fraction: 3 s 580 ms
- `00:02:39.367` — HH:MM:SS.mmm (used when ≥ 60 s)
- Java timing is always HH:MM:SS.mmm (ms resolution only, no sub-ms)
- `—` — not applicable (already solved / unsolvable)

| # | Puzzle | Grid | Status | Optimal Moves | Elapsed (C++) | Elapsed (Java) |
|---|---|---|---|---|---|---|
| 1 | 001-puzzle-2x2-unsolvable | 2×2 | Unsolvable | — | — | — |
| 2 | 002-puzzle-2x2-unsolvable | 2×2 | Unsolvable | — | — | — |
| 3 | 003-puzzle-2x2-unsolvable | 2×2 | Unsolvable | — | — | — |
| 4 | 004-puzzle-3x3-unsolvable | 3×3 | Unsolvable | — | — | — |
| 5 | 005-puzzle-3x3-unsolvable | 3×3 | Unsolvable | — | — | — |
| 6 | 006-puzzle-3x3-unsolvable | 3×3 | Unsolvable | — | — | — |
| 7 | 007-puzzle-4x4-unsolvable | 4×4 | Unsolvable | — | — | — |
| 8 | 008-puzzle-4x4-unsolvable | 4×4 | Unsolvable | — | — | — |
| 9 | 009-puzzle-4x4-unsolvable | 4×4 | Unsolvable | — | — | — |
| 10 | 010-puzzle-4x4-unsolvable | 4×4 | Unsolvable | — | — | — |
| 11 | 011-puzzle-2x2 | 2×2 | Solved | 0 | — | — |
| 12 | 012-puzzle-4x4 | 4×4 | Solved | 0 | — | — |
| 13 | 013-puzzle-2x2 | 2×2 | Solved | 1 | 0.007 ms | |
| 14 | 014-puzzle-2x2 | 2×2 | Solved | 1 | 0.032 ms | |
| 15 | 015-puzzle-3x3 | 3×3 | Solved | 1 | 0.007 ms | |
| 16 | 016-puzzle-4x4 | 4×4 | Solved | 1 | 0.010 ms | |
| 17 | 017-puzzle-2x2 | 2×2 | Solved | 2 | 0.025 ms | |
| 18 | 018-puzzle-3x3 | 3×3 | Solved | 2 | 0.007 ms | |
| 19 | 019-puzzle-4x4 | 4×4 | Solved | 2 | 0.010 ms | |
| 20 | 020-puzzle-2x2 | 2×2 | Solved | 3 | 0.006 ms | |
| 21 | 021-puzzle-2x2 | 2×2 | Solved | 3 | 0.006 ms | |
| 22 | 022-puzzle-3x3 | 3×3 | Solved | 3 | 0.007 ms | |
| 23 | 023-puzzle-4x4 | 4×4 | Solved | 3 | 0.010 ms | |
| 24 | 024-puzzle-2x2 | 2×2 | Solved | 4 | 0.008 ms | |
| 25 | 025-puzzle-3x3 | 3×3 | Solved | 4 | 0.007 ms | |
| 26 | 026-puzzle-3x3 | 3×3 | Solved | 4 | 0.007 ms | |
| 27 | 027-puzzle-4x4 | 4×4 | Solved | 4 | 0.011 ms | |
| 28 | 028-puzzle-2x2 | 2×2 | Solved | 5 | 0.007 ms | |
| 29 | 029-puzzle-3x3 | 3×3 | Solved | 5 | 0.037 ms | |
| 30 | 030-puzzle-3x3 | 3×3 | Solved | 5 | 0.009 ms | |
| 31 | 031-puzzle-4x4 | 4×4 | Solved | 5 | 0.011 ms | |
| 32 | 032-puzzle-2x2 | 2×2 | Solved | 6 | 0.007 ms | |
| 33 | 033-puzzle-3x3 | 3×3 | Solved | 6 | 0.011 ms | |
| 34 | 034-puzzle-4x4 | 4×4 | Solved | 6 | 0.012 ms | |
| 35 | 035-puzzle-4x4 | 4×4 | Solved | 6 | 0.010 ms | |
| 36 | 036-puzzle-3x3 | 3×3 | Solved | 7 | 0.012 ms | |
| 37 | 037-puzzle-3x3 | 3×3 | Solved | 7 | 0.012 ms | |
| 38 | 038-puzzle-4x4 | 4×4 | Solved | 7 | 0.013 ms | |
| 39 | 039-puzzle-3x3 | 3×3 | Solved | 8 | 0.012 ms | |
| 40 | 040-puzzle-3x3 | 3×3 | Solved | 8 | 0.008 ms | |
| 41 | 041-puzzle-4x4 | 4×4 | Solved | 8 | 0.014 ms | |
| 42 | 042-puzzle-3x3 | 3×3 | Solved | 9 | 0.012 ms | |
| 43 | 043-puzzle-4x4 | 4×4 | Solved | 9 | 0.011 ms | |
| 44 | 044-puzzle-4x4 | 4×4 | Solved | 9 | 0.013 ms | |
| 45 | 045-puzzle-3x3 | 3×3 | Solved | 10 | 0.012 ms | |
| 46 | 046-puzzle-4x4 | 4×4 | Solved | 10 | 0.012 ms | |
| 47 | 047-puzzle-9x9 | 9×9 | Solved | 10 | 0.010 ms | |
| 48 | 048-puzzle-3x3 | 3×3 | Solved | 11 | 0.017 ms | |
| 49 | 049-puzzle-3x3 | 3×3 | Solved | 11 | 0.012 ms | |
| 50 | 050-puzzle-4x4 | 4×4 | Solved | 11 | 0.010 ms | |
| 51 | 051-puzzle-3x3 | 3×3 | Solved | 12 | 0.012 ms | |
| 52 | 052-puzzle-4x4 | 4×4 | Solved | 12 | 0.012 ms | |
| 53 | 053-puzzle-5x5 | 5×5 | Solved | 12 | 0.010 ms | |
| 54 | 054-puzzle-3x3 | 3×3 | Solved | 13 | 0.021 ms | |
| 55 | 055-puzzle-4x4 | 4×4 | Solved | 13 | 0.010 ms | |
| 56 | 056-puzzle-4x4 | 4×4 | Solved | 13 | 0.011 ms | |
| 57 | 057-puzzle-3x3 | 3×3 | Solved | 14 | 0.027 ms | |
| 58 | 058-puzzle-3x3 | 3×3 | Solved | 14 | 0.012 ms | |
| 59 | 059-puzzle-4x4 | 4×4 | Solved | 14 | 0.011 ms | |
| 60 | 060-puzzle-7x7 | 7×7 | Solved | 14 | 0.075 ms | |
| 61 | 061-puzzle-3x3 | 3×3 | Solved | 15 | 0.022 ms | |
| 62 | 062-puzzle-4x4 | 4×4 | Solved | 15 | 0.013 ms | |
| 63 | 063-puzzle-8x8 | 8×8 | Solved | 15 | 0.012 ms | |
| 64 | 064-puzzle-3x3 | 3×3 | Solved | 16 | 0.034 ms | |
| 65 | 065-puzzle-4x4 | 4×4 | Solved | 16 | 0.013 ms | |
| 66 | 066-puzzle-10x10 | 10×10 | Solved | 16 | 0.055 ms | |
| 67 | 067-puzzle-3x3 | 3×3 | Solved | 17 | 0.060 ms | |
| 68 | 068-puzzle-3x3 | 3×3 | Solved | 17 | 0.042 ms | |
| 69 | 069-puzzle-4x4 | 4×4 | Solved | 17 | 0.011 ms | |
| 70 | 070-puzzle-3x3 | 3×3 | Solved | 18 | 0.089 ms | |
| 71 | 071-puzzle-3x3 | 3×3 | Solved | 18 | 0.048 ms | |
| 72 | 072-puzzle-4x4 | 4×4 | Solved | 18 | 0.029 ms | |
| 73 | 073-puzzle-3x3 | 3×3 | Solved | 19 | 0.022 ms | |
| 74 | 074-puzzle-3x3 | 3×3 | Solved | 19 | 0.082 ms | |
| 75 | 075-puzzle-4x4 | 4×4 | Solved | 19 | 0.012 ms | |
| 76 | 076-puzzle-3x3 | 3×3 | Solved | 20 | 0.089 ms | |
| 77 | 077-puzzle-3x3 | 3×3 | Solved | 20 | 0.091 ms | |
| 78 | 078-puzzle-4x4 | 4×4 | Solved | 20 | 0.011 ms | |
| 79 | 079-puzzle-3x3 | 3×3 | Solved | 21 | 0.086 ms | |
| 80 | 080-puzzle-3x3 | 3×3 | Solved | 21 | 0.165 ms | |
| 81 | 081-puzzle-4x4 | 4×4 | Solved | 21 | 0.015 ms | |
| 82 | 082-puzzle-3x3 | 3×3 | Solved | 22 | 0.169 ms | |
| 83 | 083-puzzle-3x3 | 3×3 | Solved | 22 | 0.149 ms | |
| 84 | 084-puzzle-4x4 | 4×4 | Solved | 22 | 0.012 ms | |
| 85 | 085-puzzle-3x3 | 3×3 | Solved | 23 | 0.070 ms | |
| 86 | 086-puzzle-3x3 | 3×3 | Solved | 23 | 0.261 ms | |
| 87 | 087-puzzle-4x4 | 4×4 | Solved | 23 | 0.015 ms | |
| 88 | 088-puzzle-3x3 | 3×3 | Solved | 24 | 0.288 ms | |
| 89 | 089-puzzle-3x3 | 3×3 | Solved | 24 | 0.104 ms | |
| 90 | 090-puzzle-4x4 | 4×4 | Solved | 24 | 0.014 ms | |
| 91 | 091-puzzle-3x3 | 3×3 | Solved | 25 | 0.316 ms | |
| 92 | 092-puzzle-3x3 | 3×3 | Solved | 25 | 0.359 ms | |
| 93 | 093-puzzle-4x4 | 4×4 | Solved | 25 | 0.017 ms | |
| 94 | 094-puzzle-3x3 | 3×3 | Solved | 26 | 0.174 ms | |
| 95 | 095-puzzle-3x3 | 3×3 | Solved | 26 | 0.134 ms | |
| 96 | 096-puzzle-4x4 | 4×4 | Solved | 26 | 0.039 ms | |
| 97 | 097-puzzle-3x3 | 3×3 | Solved | 27 | 1.225 ms | |
| 98 | 098-puzzle-3x3 | 3×3 | Solved | 27 | 0.400 ms | |
| 99 | 099-puzzle-4x4 | 4×4 | Solved | 27 | 0.028 ms | |
| 100 | 100-puzzle-3x3 | 3×3 | Solved | 28 | 0.944 ms | |
| 101 | 101-puzzle-3x3 | 3×3 | Solved | 28 | 1.053 ms | |
| 102 | 102-puzzle-4x4 | 4×4 | Solved | 28 | 0.015 ms | |
| 103 | 103-puzzle-3x3 | 3×3 | Solved | 29 | 1.198 ms | |
| 104 | 104-puzzle-3x3 | 3×3 | Solved | 29 | 0.700 ms | |
| 105 | 105-puzzle-4x4 | 4×4 | Solved | 29 | 0.027 ms | |
| 106 | 106-puzzle-3x3 | 3×3 | Solved | 30 | 0.640 ms | |
| 107 | 107-puzzle-3x3 | 3×3 | Solved | 30 | 0.979 ms | |
| 108 | 108-puzzle-4x4 | 4×4 | Solved | 30 | 0.013 ms | |
| 109 | 109-puzzle-3x3 | 3×3 | Solved | 31 | 1.131 ms | |
| 110 | 110-puzzle-3x3 | 3×3 | Solved | 31 | 1.098 ms | |
| 111 | 111-puzzle-4x4 | 4×4 | Solved | 31 | 0.064 ms | |
| 112 | 112-puzzle-4x4 | 4×4 | Solved | 32 | 0.189 ms | |
| 113 | 113-puzzle-4x4 | 4×4 | Solved | 32 | 0.087 ms | |
| 114 | 114-puzzle-4x4 | 4×4 | Solved | 33 | 0.035 ms | |
| 115 | 115-puzzle-4x4 | 4×4 | Solved | 33 | 0.722 ms | |
| 116 | 116-puzzle-4x4 | 4×4 | Solved | 34 | 0.112 ms | |
| 117 | 117-puzzle-4x4 | 4×4 | Solved | 34 | 0.221 ms | |
| 118 | 118-puzzle-4x4 | 4×4 | Solved | 35 | 0.058 ms | |
| 119 | 119-puzzle-4x4 | 4×4 | Solved | 35 | 0.273 ms | |
| 120 | 120-puzzle-4x4 | 4×4 | Solved | 36 | 0.804 ms | |
| 121 | 121-puzzle-4x4 | 4×4 | Solved | 36 | 0.841 ms | |
| 122 | 122-puzzle-4x4 | 4×4 | Solved | 37 | 0.261 ms | |
| 123 | 123-puzzle-4x4 | 4×4 | Solved | 37 | 0.080 ms | |
| 124 | 124-puzzle-4x4 | 4×4 | Solved | 38 | 0.960 ms | |
| 125 | 125-puzzle-4x4 | 4×4 | Solved | 38 | 0.784 ms | |
| 126 | 126-puzzle-4x4 | 4×4 | Solved | 39 | 0.062 ms | |
| 127 | 127-puzzle-4x4 | 4×4 | Solved | 39 | 0.076 ms | |
| 128 | 128-puzzle-4x4 | 4×4 | Solved | 40 | 0.146 ms | |
| 129 | 129-puzzle-4x4 | 4×4 | Solved | 40 | 0.313 ms | |
| 130 | 130-puzzle-4x4 | 4×4 | Solved | 41 | 0.250 ms | |
| 131 | 131-puzzle-5x5 | 5×5 | Solved | 41 | 1.263 ms | |
| 132 | 132-puzzle-4x4 | 4×4 | Solved | 42 | 0.154 ms | |
| 133 | 133-puzzle-4x4 | 4×4 | Solved | 42 | 0.706 ms | |
| 134 | 134-puzzle-4x4 | 4×4 | Solved | 43 | 5.576 ms | |
| 135 | 135-puzzle-4x4 | 4×4 | Solved | 43 | 1.021 ms | |
| 136 | 136-puzzle-4x4 | 4×4 | Solved | 44 | 0.158 ms | |
| 137 | 137-puzzle-5x5 | 5×5 | Solved | 44 | 1.282 ms | |
| 138 | 138-puzzle-4x4 | 4×4 | Solved | 45 | 10.169 ms | |
| 139 | 139-puzzle-4x4 | 4×4 | Solved | 45 | 0.585 ms | |
| 140 | 140-puzzle-4x4 | 4×4 | Solved | 46 | 7.751 ms | |
| 141 | 141-puzzle-4x4 | 4×4 | Solved | 46 | 0.065 ms | |
| 142 | 142-puzzle-4x4 | 4×4 | Solved | 47 | 1.369 ms | |
| 143 | 143-puzzle-4x4 | 4×4 | Solved | 47 | 1.211 ms | |
| 144 | 144-puzzle-4x4 | 4×4 | Solved | 47 | 116.174 ms | |
| 145 | 145-puzzle-4x4 | 4×4 | Solved | 48 | 1.344 ms | |
| 146 | 146-puzzle-4x4 | 4×4 | Solved | 48 | 0.932 ms | |
| 147 | 147-puzzle-4x4 | 4×4 | Solved | 48 | 112.822 ms | |
| 148 | 148-puzzle-4x4 | 4×4 | Solved | 48 | 112.276 ms | |
| 149 | 149-puzzle-4x4 | 4×4 | Solved | 49 | 37.096 ms | |
| 150 | 150-puzzle-4x4 | 4×4 | Solved | 49 | 6.801 ms | |
| 151 | 151-puzzle-4x4 | 4×4 | Solved | 49 | 408.938 ms | |
| 152 | 152-puzzle-4x4 | 4×4 | Solved | 50 | 29.084 ms | |
| 153 | 153-puzzle-4x4 | 4×4 | Solved | 50 | 0.927 ms | |
| 154 | 154-puzzle-4x4 | 4×4 | Solved | 50 | 0.955 ms | |
| 155 | 155-puzzle-4x4 | 4×4 | Solved | 50 | 767.166 ms | |
| 156 | 156-puzzle-4x4 | 4×4 | Solved | 51 | 156.187 ms | |
| 157 | 157-puzzle-4x4 | 4×4 | Solved | 52 | 310.373 ms | |
| 158 | 158-puzzle-4x4-HARD | 4×4 | Solved | 53 | 1.327 s | |
| 159 | 159-puzzle-4x4-HARD-2 | 4×4 | Solved | 56 | 365.801 ms | |
| 160 | 160-puzzle-4x4-MEGA-HARD | 4×4 | Solved | 66 | 3.580 s | |
| 161 | 161-puzzle-4x4-MEGA-HARD-2 | 4×4 | Solved | 78 | 2.028 s | |
| 162 | 162-puzzle-4x4-ULTRA-HARD | 4×4 | Solved | 80 | 00:02:39.367 | |

---

## Analysis

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

Puzzles 008, 009, and 010 share the same board but specify non-default blank target positions (indices 0, 2, and 8 respectively, rather than the default 15). A solvability-check bug originally caused them to be misclassified as solvable, sending IDA\* into an infinite search.

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

1. **All three implementations now use the same heuristic.** For the standard 4×4 puzzle (blank at bottom-right), all use additive 5-5-5 PDB; for all other grids, MD + LC. PDB solves puzzle 160 in ~3.5 s (C++), ~10 s (Java) instead of 3 min 18 s with MD+LC. The PDB build cost (≈100 ms C++, ≈1 s Java, ≈44 s Python) is amortised over hard puzzles.

2. **For non-4×4 grids, recomputing LC at every DLS node is the key optimisation.** Without it the heuristic degrades to pure MD inside DLS, and hard puzzles time out. With it, each node gets a tighter bound that prunes branches MD alone would explore.

3. **Solution depth, not grid size, determines IDA\* cost.** A 10×10 puzzle with 16 optimal moves runs in 0.055 ms. A 4×4 puzzle with 80 moves takes 2 min 39 s. The large-grid puzzles (7×7–10×10) are experimental and were placed close to the goal state (≤16 moves), so they solve instantly despite the larger grid.

4. **Exponential blow-up** is clearly visible in the 4×4 results: 47 moves can take anywhere from 1.211 ms to 116.174 ms depending on heuristic quality for that specific board; 80 moves takes >100× longer than 78 moves.

5. **161-puzzle-4x4-MEGA-HARD-2** (fully reversed board, 78 moves) solves in **2.028 s** with PDB (C++).

6. **162-puzzle-4x4-ULTRA-HARD** (80 moves) solves in **2 min 39 s** with PDB (C++).

7. **Python is algorithmically correct** but the recursion overhead makes depths >~57 impractical. This is not an algorithmic limitation — it is a language speed issue. C++ solves all 162 puzzles; Python would need a native extension or iterative stack rewrite for the very hardest ones.

8. **Variance within a move-count band** is large and unpredictable. Two 48-move puzzles (145 vs 147) differ by a factor of 84× (1.344 ms vs 112.822 ms). The heuristic's ability to prune a particular board state is highly problem-specific.

9. **122-puzzle-4x4** (37 moves, 0.261 ms) was previously misreported as 49 moves due to a missing `target_index` line in the puzzle file causing the parser to consume the first board tile as the target. With the corrected input the board solves in 37 optimal moves instantly.
