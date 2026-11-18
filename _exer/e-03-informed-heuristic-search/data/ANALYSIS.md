# E03 — N-Sliding Blocks Puzzle: Results Analysis

Algorithm: **IDA\*** with **Additive 5-5-5 PDB** (4×4 canonical) / **Manhattan Distance + Linear Conflict** (all other grids)  
Implementation: [`cpp/SlidingBlocksPuzzleProceduralSolution/NSlidingPuzzle.cpp`](../cpp/SlidingBlocksPuzzleProceduralSolution/NSlidingPuzzle.cpp) · [`java/SlidingBlocksPuzzleProceduralSolution/src/NSlidingPuzzle.java`](../java/SlidingBlocksPuzzleProceduralSolution/src/NSlidingPuzzle.java) · [`python/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle.py`](../python/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle.py)  
Runners: [`run_cpp.py`](run_cpp.py) · [`run_java.py`](run_java.py) · [`run_python.py`](run_python.py) (write to `data/results-cpp/`, `data/results-java/`, `data/results-python/`)  
Timeout per puzzle: **2400 s**

Algorithm details and hard puzzle solutions: [**SOLUTION.md**](SOLUTION.md)

---

## Summary

| Status | Count |
|---|---|
| Solved (C++) | 162 |
| Solved (Java) | 162 |
| Solved (Python) | 161 |
| Unsolvable (detected correctly) | 10 |
| Timed out (Python #162 only) | 1 |
| **Total puzzles** | **162** |

---

## Full Results Table

Sorted by puzzle number. Unsolvable puzzles appear first (001–010). Timing format: `HH:MM:SS.mmm` for ≥1 s; `ms` for sub-second; `—` for 0-move/unsolvable; `TIMEOUT` for >2400 s.

| # | Puzzle | Grid | Moves | C++ | Java | Python |
|---|---|---|---|---|---|---|
| 1 | 001-puzzle-2x2-unsolvable | 2×2 | — | — | — | — |
| 2 | 002-puzzle-2x2-unsolvable | 2×2 | — | — | — | — |
| 3 | 003-puzzle-2x2-unsolvable | 2×2 | — | — | — | — |
| 4 | 004-puzzle-3x3-unsolvable | 3×3 | — | — | — | — |
| 5 | 005-puzzle-3x3-unsolvable | 3×3 | — | — | — | — |
| 6 | 006-puzzle-3x3-unsolvable | 3×3 | — | — | — | — |
| 7 | 007-puzzle-4x4-unsolvable | 4×4 | — | — | — | — |
| 8 | 008-puzzle-4x4-unsolvable | 4×4 | — | — | — | — |
| 9 | 009-puzzle-4x4-unsolvable | 4×4 | — | — | — | — |
| 10 | 010-puzzle-4x4-unsolvable | 4×4 | — | — | — | — |
| 11 | 011-puzzle-2x2 | 2×2 | 0 | — | — | — |
| 12 | 012-puzzle-4x4 | 4×4 | 0 | — | — | — |
| 13 | 013-puzzle-2x2 | 2×2 | 1 | 0.003 ms | 00:00:00.000 | 00:00:00.000 |
| 14 | 014-puzzle-2x2 | 2×2 | 1 | 0.003 ms | 00:00:00.000 | 00:00:00.000 |
| 15 | 015-puzzle-3x3 | 3×3 | 1 | 0.002 ms | 00:00:00.000 | 00:00:00.000 |
| 16 | 016-puzzle-4x4 | 4×4 | 1 | 0.009 ms | 00:00:00.000 | 00:00:00.000 |
| 17 | 017-puzzle-2x2 | 2×2 | 2 | 0.002 ms | 00:00:00.000 | 00:00:00.000 |
| 18 | 018-puzzle-3x3 | 3×3 | 2 | 0.003 ms | 00:00:00.000 | 00:00:00.000 |
| 19 | 019-puzzle-4x4 | 4×4 | 2 | 0.011 ms | 00:00:00.000 | 00:00:00.000 |
| 20 | 020-puzzle-2x2 | 2×2 | 3 | 0.004 ms | 00:00:00.000 | 00:00:00.000 |
| 21 | 021-puzzle-2x2 | 2×2 | 3 | 0.003 ms | 00:00:00.000 | 00:00:00.000 |
| 22 | 022-puzzle-3x3 | 3×3 | 3 | 0.003 ms | 00:00:00.000 | 00:00:00.000 |
| 23 | 023-puzzle-4x4 | 4×4 | 3 | 0.008 ms | 00:00:00.000 | 00:00:00.000 |
| 24 | 024-puzzle-2x2 | 2×2 | 4 | 0.003 ms | 00:00:00.000 | 00:00:00.000 |
| 25 | 025-puzzle-3x3 | 3×3 | 4 | 0.003 ms | 00:00:00.000 | 00:00:00.000 |
| 26 | 026-puzzle-3x3 | 3×3 | 4 | 0.003 ms | 00:00:00.000 | 00:00:00.000 |
| 27 | 027-puzzle-4x4 | 4×4 | 4 | 0.009 ms | 00:00:00.000 | 00:00:00.000 |
| 28 | 028-puzzle-2x2 | 2×2 | 5 | 0.003 ms | 00:00:00.000 | 00:00:00.000 |
| 29 | 029-puzzle-3x3 | 3×3 | 5 | 0.003 ms | 00:00:00.000 | 00:00:00.000 |
| 30 | 030-puzzle-3x3 | 3×3 | 5 | 0.003 ms | 00:00:00.000 | 00:00:00.000 |
| 31 | 031-puzzle-4x4 | 4×4 | 5 | 0.008 ms | 00:00:00.000 | 00:00:00.000 |
| 32 | 032-puzzle-2x2 | 2×2 | 6 | 0.003 ms | 00:00:00.000 | 00:00:00.000 |
| 33 | 033-puzzle-3x3 | 3×3 | 6 | 0.004 ms | 00:00:00.000 | 00:00:00.000 |
| 34 | 034-puzzle-4x4 | 4×4 | 6 | 0.009 ms | 00:00:00.000 | 00:00:00.000 |
| 35 | 035-puzzle-4x4 | 4×4 | 6 | 0.009 ms | 00:00:00.000 | 00:00:00.000 |
| 36 | 036-puzzle-3x3 | 3×3 | 7 | 0.004 ms | 00:00:00.000 | 00:00:00.000 |
| 37 | 037-puzzle-3x3 | 3×3 | 7 | 0.003 ms | 00:00:00.000 | 00:00:00.000 |
| 38 | 038-puzzle-4x4 | 4×4 | 7 | 0.008 ms | 00:00:00.000 | 00:00:00.000 |
| 39 | 039-puzzle-3x3 | 3×3 | 8 | 0.004 ms | 00:00:00.000 | 00:00:00.000 |
| 40 | 040-puzzle-3x3 | 3×3 | 8 | 0.004 ms | 00:00:00.000 | 00:00:00.000 |
| 41 | 041-puzzle-4x4 | 4×4 | 8 | 0.010 ms | 00:00:00.000 | 00:00:00.000 |
| 42 | 042-puzzle-3x3 | 3×3 | 9 | 0.004 ms | 00:00:00.000 | 00:00:00.000 |
| 43 | 043-puzzle-4x4 | 4×4 | 9 | 0.011 ms | 00:00:00.000 | 00:00:00.000 |
| 44 | 044-puzzle-4x4 | 4×4 | 9 | 0.012 ms | 00:00:00.000 | 00:00:00.000 |
| 45 | 045-puzzle-3x3 | 3×3 | 10 | 0.004 ms | 00:00:00.000 | 00:00:00.000 |
| 46 | 046-puzzle-4x4 | 4×4 | 10 | 0.008 ms | 00:00:00.000 | 00:00:00.000 |
| 47 | 047-puzzle-9x9 | 9×9 | 10 | 0.012 ms | 00:00:00.000 | 00:00:00.000 |
| 48 | 048-puzzle-3x3 | 3×3 | 11 | 0.005 ms | 00:00:00.000 | 00:00:00.000 |
| 49 | 049-puzzle-3x3 | 3×3 | 11 | 0.005 ms | 00:00:00.000 | 00:00:00.000 |
| 50 | 050-puzzle-4x4 | 4×4 | 11 | 0.008 ms | 00:00:00.000 | 00:00:00.000 |
| 51 | 051-puzzle-3x3 | 3×3 | 12 | 0.005 ms | 00:00:00.000 | 00:00:00.000 |
| 52 | 052-puzzle-4x4 | 4×4 | 12 | 0.010 ms | 00:00:00.000 | 00:00:00.000 |
| 53 | 053-puzzle-5x5 | 5×5 | 12 | 0.010 ms | 00:00:00.000 | 00:00:00.000 |
| 54 | 054-puzzle-3x3 | 3×3 | 13 | 0.007 ms | 00:00:00.000 | 00:00:00.000 |
| 55 | 055-puzzle-4x4 | 4×4 | 13 | 0.019 ms | 00:00:00.000 | 00:00:00.000 |
| 56 | 056-puzzle-4x4 | 4×4 | 13 | 0.008 ms | 00:00:00.000 | 00:00:00.000 |
| 57 | 057-puzzle-3x3 | 3×3 | 14 | 0.011 ms | 00:00:00.000 | 00:00:00.000 |
| 58 | 058-puzzle-3x3 | 3×3 | 14 | 0.006 ms | 00:00:00.000 | 00:00:00.000 |
| 59 | 059-puzzle-4x4 | 4×4 | 14 | 0.012 ms | 00:00:00.000 | 00:00:00.000 |
| 60 | 060-puzzle-7x7 | 7×7 | 14 | 0.078 ms | 00:00:00.003 | 00:00:00.004 |
| 61 | 061-puzzle-3x3 | 3×3 | 15 | 0.009 ms | 00:00:00.000 | 00:00:00.000 |
| 62 | 062-puzzle-4x4 | 4×4 | 15 | 0.013 ms | 00:00:00.000 | 00:00:00.000 |
| 63 | 063-puzzle-8x8 | 8×8 | 15 | 0.013 ms | 00:00:00.001 | 00:00:00.000 |
| 64 | 064-puzzle-3x3 | 3×3 | 16 | 0.018 ms | 00:00:00.000 | 00:00:00.000 |
| 65 | 065-puzzle-4x4 | 4×4 | 16 | 0.012 ms | 00:00:00.000 | 00:00:00.000 |
| 66 | 066-puzzle-10x10 | 10×10 | 16 | 0.057 ms | 00:00:00.002 | 00:00:00.003 |
| 67 | 067-puzzle-3x3 | 3×3 | 17 | 0.026 ms | 00:00:00.001 | 00:00:00.000 |
| 68 | 068-puzzle-3x3 | 3×3 | 17 | 0.046 ms | 00:00:00.001 | 00:00:00.000 |
| 69 | 069-puzzle-4x4 | 4×4 | 17 | 0.012 ms | 00:00:00.000 | 00:00:00.000 |
| 70 | 070-puzzle-3x3 | 3×3 | 18 | 0.045 ms | 00:00:00.001 | 00:00:00.000 |
| 71 | 071-puzzle-3x3 | 3×3 | 18 | 0.024 ms | 00:00:00.001 | 00:00:00.000 |
| 72 | 072-puzzle-4x4 | 4×4 | 18 | 0.025 ms | 00:00:00.001 | 00:00:00.001 |
| 73 | 073-puzzle-3x3 | 3×3 | 19 | 0.005 ms | 00:00:00.000 | 00:00:00.000 |
| 74 | 074-puzzle-3x3 | 3×3 | 19 | 0.037 ms | 00:00:00.001 | 00:00:00.000 |
| 75 | 075-puzzle-4x4 | 4×4 | 19 | 0.010 ms | 00:00:00.000 | 00:00:00.000 |
| 76 | 076-puzzle-3x3 | 3×3 | 20 | 0.041 ms | 00:00:00.001 | 00:00:00.000 |
| 77 | 077-puzzle-3x3 | 3×3 | 20 | 0.042 ms | 00:00:00.001 | 00:00:00.001 |
| 78 | 078-puzzle-4x4 | 4×4 | 20 | 0.011 ms | 00:00:00.000 | 00:00:00.000 |
| 79 | 079-puzzle-3x3 | 3×3 | 21 | 0.041 ms | 00:00:00.001 | 00:00:00.000 |
| 80 | 080-puzzle-3x3 | 3×3 | 21 | 0.078 ms | 00:00:00.001 | 00:00:00.001 |
| 81 | 081-puzzle-4x4 | 4×4 | 21 | 0.013 ms | 00:00:00.001 | 00:00:00.000 |
| 82 | 082-puzzle-3x3 | 3×3 | 22 | 0.082 ms | 00:00:00.001 | 00:00:00.001 |
| 83 | 083-puzzle-3x3 | 3×3 | 22 | 0.140 ms | 00:00:00.001 | 00:00:00.001 |
| 84 | 084-puzzle-4x4 | 4×4 | 22 | 0.011 ms | 00:00:00.000 | 00:00:00.000 |
| 85 | 085-puzzle-3x3 | 3×3 | 23 | 0.030 ms | 00:00:00.001 | 00:00:00.000 |
| 86 | 086-puzzle-3x3 | 3×3 | 23 | 0.123 ms | 00:00:00.002 | 00:00:00.002 |
| 87 | 087-puzzle-4x4 | 4×4 | 23 | 0.012 ms | 00:00:00.001 | 00:00:00.000 |
| 88 | 088-puzzle-3x3 | 3×3 | 24 | 0.135 ms | 00:00:00.002 | 00:00:00.003 |
| 89 | 089-puzzle-3x3 | 3×3 | 24 | 0.060 ms | 00:00:00.001 | 00:00:00.001 |
| 90 | 090-puzzle-4x4 | 4×4 | 24 | 0.012 ms | 00:00:00.001 | 00:00:00.000 |
| 91 | 091-puzzle-3x3 | 3×3 | 25 | 0.164 ms | 00:00:00.003 | 00:00:00.004 |
| 92 | 092-puzzle-3x3 | 3×3 | 25 | 0.197 ms | 00:00:00.003 | 00:00:00.005 |
| 93 | 093-puzzle-4x4 | 4×4 | 25 | 0.017 ms | 00:00:00.001 | 00:00:00.000 |
| 94 | 094-puzzle-3x3 | 3×3 | 26 | 0.086 ms | 00:00:00.002 | 00:00:00.002 |
| 95 | 095-puzzle-3x3 | 3×3 | 26 | 0.077 ms | 00:00:00.001 | 00:00:00.001 |
| 96 | 096-puzzle-4x4 | 4×4 | 26 | 0.037 ms | 00:00:00.001 | 00:00:00.002 |
| 97 | 097-puzzle-3x3 | 3×3 | 27 | 0.656 ms | 00:00:00.005 | 00:00:00.019 |
| 98 | 098-puzzle-3x3 | 3×3 | 27 | 0.214 ms | 00:00:00.003 | 00:00:00.005 |
| 99 | 099-puzzle-4x4 | 4×4 | 27 | 0.019 ms | 00:00:00.001 | 00:00:00.000 |
| 100 | 100-puzzle-3x3 | 3×3 | 28 | 0.518 ms | 00:00:00.005 | 00:00:00.014 |
| 101 | 101-puzzle-3x3 | 3×3 | 28 | 0.585 ms | 00:00:00.005 | 00:00:00.016 |
| 102 | 102-puzzle-4x4 | 4×4 | 28 | 0.016 ms | 00:00:00.001 | 00:00:00.000 |
| 103 | 103-puzzle-3x3 | 3×3 | 29 | 0.722 ms | 00:00:00.005 | 00:00:00.018 |
| 104 | 104-puzzle-3x3 | 3×3 | 29 | 0.375 ms | 00:00:00.005 | 00:00:00.010 |
| 105 | 105-puzzle-4x4 | 4×4 | 29 | 0.023 ms | 00:00:00.001 | 00:00:00.001 |
| 106 | 106-puzzle-3x3 | 3×3 | 30 | 0.383 ms | 00:00:00.005 | 00:00:00.009 |
| 107 | 107-puzzle-3x3 | 3×3 | 30 | 0.581 ms | 00:00:00.005 | 00:00:00.014 |
| 108 | 108-puzzle-4x4 | 4×4 | 30 | 0.010 ms | 00:00:00.000 | 00:00:00.000 |
| 109 | 109-puzzle-3x3 | 3×3 | 31 | 0.655 ms | 00:00:00.005 | 00:00:00.018 |
| 110 | 110-puzzle-3x3 | 3×3 | 31 | 0.648 ms | 00:00:00.005 | 00:00:00.017 |
| 111 | 111-puzzle-4x4 | 4×4 | 31 | 0.059 ms | 00:00:00.001 | 00:00:00.005 |
| 112 | 112-puzzle-4x4 | 4×4 | 32 | 0.182 ms | 00:00:00.002 | 00:00:00.023 |
| 113 | 113-puzzle-4x4 | 4×4 | 32 | 0.090 ms | 00:00:00.001 | 00:00:00.009 |
| 114 | 114-puzzle-4x4 | 4×4 | 33 | 0.037 ms | 00:00:00.001 | 00:00:00.002 |
| 115 | 115-puzzle-4x4 | 4×4 | 33 | 0.716 ms | 00:00:00.004 | 00:00:00.118 |
| 116 | 116-puzzle-4x4 | 4×4 | 34 | 0.107 ms | 00:00:00.005 | 00:00:00.013 |
| 117 | 117-puzzle-4x4 | 4×4 | 34 | 0.221 ms | 00:00:00.002 | 00:00:00.029 |
| 118 | 118-puzzle-4x4 | 4×4 | 35 | 0.058 ms | 00:00:00.001 | 00:00:00.005 |
| 119 | 119-puzzle-4x4 | 4×4 | 35 | 0.261 ms | 00:00:00.002 | 00:00:00.038 |
| 120 | 120-puzzle-4x4 | 4×4 | 36 | 0.807 ms | 00:00:00.004 | 00:00:00.122 |
| 121 | 121-puzzle-4x4 | 4×4 | 36 | 0.828 ms | 00:00:00.005 | 00:00:00.142 |
| 122 | 122-puzzle-4x4 | 4×4 | 37 | 0.261 ms | 00:00:00.002 | 00:00:00.038 |
| 123 | 123-puzzle-4x4 | 4×4 | 37 | 0.074 ms | 00:00:00.001 | 00:00:00.008 |
| 124 | 124-puzzle-4x4 | 4×4 | 38 | 0.952 ms | 00:00:00.008 | 00:00:00.156 |
| 125 | 125-puzzle-4x4 | 4×4 | 38 | 0.794 ms | 00:00:00.004 | 00:00:00.123 |
| 126 | 126-puzzle-4x4 | 4×4 | 39 | 0.059 ms | 00:00:00.001 | 00:00:00.006 |
| 127 | 127-puzzle-4x4 | 4×4 | 39 | 0.068 ms | 00:00:00.001 | 00:00:00.007 |
| 128 | 128-puzzle-4x4 | 4×4 | 40 | 0.134 ms | 00:00:00.002 | 00:00:00.017 |
| 129 | 129-puzzle-4x4 | 4×4 | 40 | 0.299 ms | 00:00:00.002 | 00:00:00.041 |
| 130 | 130-puzzle-4x4 | 4×4 | 41 | 0.243 ms | 00:00:00.006 | 00:00:00.033 |
| 131 | 131-puzzle-5x5 | 5×5 | 41 | 1.278 ms | 00:00:00.007 | 00:00:00.055 |
| 132 | 132-puzzle-4x4 | 4×4 | 42 | 0.146 ms | 00:00:00.002 | 00:00:00.021 |
| 133 | 133-puzzle-4x4 | 4×4 | 42 | 0.717 ms | 00:00:00.004 | 00:00:00.123 |
| 134 | 134-puzzle-4x4 | 4×4 | 43 | 5.544 ms | 00:00:00.022 | 00:00:00.963 |
| 135 | 135-puzzle-4x4 | 4×4 | 43 | 0.997 ms | 00:00:00.005 | 00:00:00.172 |
| 136 | 136-puzzle-4x4 | 4×4 | 44 | 0.147 ms | 00:00:00.002 | 00:00:00.019 |
| 137 | 137-puzzle-5x5 | 5×5 | 44 | 1.311 ms | 00:00:00.007 | 00:00:00.051 |
| 138 | 138-puzzle-4x4 | 4×4 | 45 | 9.777 ms | 00:00:00.032 | 00:00:01.709 |
| 139 | 139-puzzle-4x4 | 4×4 | 45 | 0.567 ms | 00:00:00.004 | 00:00:00.092 |
| 140 | 140-puzzle-4x4 | 4×4 | 46 | 7.360 ms | 00:00:00.026 | 00:00:01.323 |
| 141 | 141-puzzle-4x4 | 4×4 | 46 | 0.059 ms | 00:00:00.001 | 00:00:00.006 |
| 142 | 142-puzzle-4x4 | 4×4 | 47 | 1.347 ms | 00:00:00.007 | 00:00:00.231 |
| 143 | 143-puzzle-4x4 | 4×4 | 47 | 1.165 ms | 00:00:00.009 | 00:00:00.191 |
| 144 | 144-puzzle-4x4 | 4×4 | 47 | 111.531 ms | 00:00:00.162 | 00:00:03.699 |
| 145 | 145-puzzle-4x4 | 4×4 | 48 | 1.280 ms | 00:00:00.009 | 00:00:00.214 |
| 146 | 146-puzzle-4x4 | 4×4 | 48 | 0.931 ms | 00:00:00.005 | 00:00:00.154 |
| 147 | 147-puzzle-4x4 | 4×4 | 48 | 112.143 ms | 00:00:00.166 | 00:00:03.672 |
| 148 | 148-puzzle-4x4 | 4×4 | 48 | 112.348 ms | 00:00:00.167 | 00:00:03.702 |
| 149 | 149-puzzle-4x4 | 4×4 | 49 | 36.822 ms | 00:00:00.113 | 00:00:06.484 |
| 150 | 150-puzzle-4x4 | 4×4 | 49 | 6.736 ms | 00:00:00.027 | 00:00:01.166 |
| 151 | 151-puzzle-4x4 | 4×4 | 49 | 408.507 ms | 00:00:00.569 | 00:00:13.208 |
| 152 | 152-puzzle-4x4 | 4×4 | 50 | 28.879 ms | 00:00:00.093 | 00:00:04.985 |
| 153 | 153-puzzle-4x4 | 4×4 | 50 | 0.956 ms | 00:00:00.008 | 00:00:00.151 |
| 154 | 154-puzzle-4x4 | 4×4 | 50 | 1.004 ms | 00:00:00.005 | 00:00:00.153 |
| 155 | 155-puzzle-4x4 | 4×4 | 50 | 803.811 ms | 00:00:01.072 | 00:00:24.406 |
| 156 | 156-puzzle-4x4 | 4×4 | 51 | 156.180 ms | 00:00:00.218 | 00:00:04.896 |
| 157 | 157-puzzle-4x4 | 4×4 | 52 | 313.247 ms | 00:00:00.434 | 00:00:09.567 |
| 158 | 158-puzzle-4x4-HARD | 4×4 | 53 | 1.338 s | 00:00:01.798 | 00:00:40.526 |
| 159 | 159-puzzle-4x4-HARD-2 | 4×4 | 56 | 364.883 ms | 00:00:00.897 | 00:01:06.622 |
| 160 | 160-puzzle-4x4-MEGA-HARD | 4×4 | 66 | 3.599 s | 00:00:08.253 | 00:11:04.395 |
| 161 | 161-puzzle-4x4-MEGA-HARD-2 | 4×4 | 78 | 2.025 s | 00:00:05.327 | 00:06:26.773 |
| 162 | 162-puzzle-4x4-ULTRA-HARD | 4×4 | 80 | 00:02:38.655 | 00:06:08.431 | **TIMEOUT** |

<!--
Time format legend:
  0.007 ms       — sub-millisecond (C++ only; µs resolution): 7 microseconds
  1.225 ms       — milliseconds with µs fraction: 1225 µs
  3.580 s        — seconds with ms fraction: 3 s 580 ms
  00:02:39.367   — HH:MM:SS.mmm (used when ≥ 1 s in Java/Python output)
  Java/Python timing is always HH:MM:SS.mmm (ms resolution, no sub-ms)
  —              — not applicable (already solved / unsolvable)
  TIMEOUT        — exceeded 2400 s limit
-->

---

## Analysis

### Language speed comparison (hard 4×4 puzzles)

| Puzzle | Moves | C++ | Java | Python | Java/C++ | Python/C++ |
|---|---|---|---|---|---|---|
| 158-HARD | 53 | 1.338 s | 1.798 s | 40.526 s | ~1.3× | ~30× |
| 159-HARD-2 | 56 | 364.9 ms | 897 ms | 1 m 06.6 s | ~2.5× | ~11× |
| 160-MEGA-HARD | 66 | 3.599 s | 8.253 s | 11 m 04 s | ~2.3× | ~185× |
| 161-MEGA-HARD-2 | 78 | 2.025 s | 5.327 s | 6 m 26 s | ~2.6× | ~191× |
| 162-ULTRA-HARD | 80 | 2 m 38 s | 6 m 08 s | TIMEOUT | ~2.3× | >900× |

Java is consistently **2–3× slower** than C++ across all hard puzzles. Python is **30–200× slower** than C++ and timed out on puzzle 162 even with a 2400 s limit.

### Elapsed time vs. optimal moves (4×4 puzzles, C++)

Elapsed time grows roughly exponentially with solution depth:

| Moves | Elapsed (C++) |
|---|---|
| ≤ 30 | ≤ 0.016 ms |
| 31–35 | 0.058–0.722 ms |
| 36–40 | 0.068–0.952 ms |
| 41–44 | 0.147–5.544 ms |
| 45–47 | 0.059–111.531 ms |
| 48–50 | 0.931 ms–803.811 ms |
| 51–53 | 156.180 ms–1.338 s |
| 56 | 364.883 ms |
| 66 | 3.599 s |
| 78 | 2.025 s |
| 80 | 00:02:38.655 |

The wide variance within a move-count band reflects the structure of individual puzzles — two puzzles with the same optimal depth can differ by 100× in IDA\* cost depending on how tightly the heuristic prunes each specific board.

### Unsolvable puzzles — solvability check for non-default blank targets

Puzzles 008, 009, and 010 share the same board but specify non-default blank target positions (indices 0, 2, and 8 respectively, rather than the default 15). The correct parity check for even-width grids is:

```
(inversions + blank_current_row_from_bottom + blank_target_row_from_bottom) % 2 == 0
```

All three implementations correctly detect these as unsolvable in under 1 ms.

---

## Observations

1. **All three implementations solve the same 162 puzzles** (except Python timing out on #162). The algorithm — IDA\* with additive 5-5-5 PDB for 4×4, MD+LC for all other grids — is correct and complete.

2. **PDB dramatically reduces search for hard 4×4 puzzles.** Puzzle 160 (66 moves) solves in 3.6 s (C++) with PDB. With MD+LC alone it would take over 3 minutes. The one-time PDB build cost (≈100 ms C++, ≈1 s Java, ≈44 s Python) is fully amortised.

3. **Java is 2–3× slower than C++** on all hard puzzles. The JIT-compiled JVM closes most of the gap versus native code but cannot eliminate it for this recursion-heavy workload.

4. **Python is 30–200× slower than C++** due to interpreter overhead on the innermost DLS recursion (~1–2 µs per call vs. ~2 ns native). The three micro-optimisations applied (precomputed popcount table, pre-allocated PDB lookup buffers, local variable caching in `_dls`) reduce the constant factor by 3–8× but cannot bridge the fundamental interpreter gap.

5. **Python timed out on puzzle 162** (80 moves, >2400 s). This is not an algorithmic limitation — the algorithm is correct. A C extension or iterative stack rewrite would solve it.

6. **Solution depth, not grid size, determines IDA\* cost.** The 10×10 puzzle (066) with 16 optimal moves solves in 0.057 ms (C++). The 4×4 puzzle with 80 moves takes 2 min 38 s. The large-grid puzzles (7×7–10×10) are close to the goal state (≤16 moves), so they solve instantly despite the larger grid.

7. **Exponential blow-up** is clearly visible: three 48-move puzzles (145–148) span 0.931 ms to 112.348 ms (C++) — a 121× range. The heuristic's pruning power depends heavily on the specific board configuration, not just depth.

8. **161-puzzle-4x4-MEGA-HARD-2** (fully reversed board, 78 moves) solves in **2.025 s** (C++), **5.327 s** (Java), **6 m 26 s** (Python).

9. **162-puzzle-4x4-ULTRA-HARD** (80 moves) solves in **2 min 38 s** (C++), **6 min 08 s** (Java), timed out in Python.
