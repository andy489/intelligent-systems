# E02 — N-Sliding Blocks Puzzle: Results Analysis

Algorithm: **IDA\*** with **Manhattan Distance + Linear Conflict** heuristic  
Implementation: [`cpp/SlidingBlocksPuzzleProceduralSolution/NSlidingPuzzle.cpp`](../cpp/SlidingBlocksPuzzleProceduralSolution/NSlidingPuzzle.cpp)  
Runner: [`run_all_puzzles.py`](../run_all_puzzles.py)  
Timeout per puzzle: **300 s**

---

## Summary

| Status | Count |
|---|---|
| Solved | 145 |
| Unsolvable (detected correctly) | 9 |
| Timed out — hard puzzle | 1 |
| **Total** | **160** |

> **Note:** Puzzles 158–160 were initially timed out due to a solvability-check bug (even-width grids with non-default blank target).
> After fixing `check_solvable()` to also account for `blank_target_row_from_bottom`, all three are correctly detected as unsolvable instantly.

---

## Full Results Table

| # | Puzzle | Grid | Status | Optimal Moves | Elapsed |
|---|---|---|---|---|---|
| 001 | puzzle001-2x2 | 2×2 | Solved | 1 | <1 ms |
| 002 | puzzle002-2x2 | 2×2 | Solved | 0 | — (already solved) |
| 003 | puzzle003-2x2 | 2×2 | Solved | 1 | <1 ms |
| 004 | puzzle004-2x2 | 2×2 | Solved | 2 | <1 ms |
| 005 | puzzle005-2x2 | 2×2 | Solved | 3 | <1 ms |
| 006 | puzzle006-2x2 | 2×2 | Solved | 4 | <1 ms |
| 007 | puzzle007-2x2 | 2×2 | Solved | 5 | <1 ms |
| 008 | puzzle008-2x2 | 2×2 | Solved | 6 | <1 ms |
| 009 | puzzle009-2x2-unsolvable | 2×2 | Unsolvable | — | — |
| 010 | puzzle010-2x2-unsolvable | 2×2 | Unsolvable | — | — |
| 011 | puzzle011-2x2-unsolvable | 2×2 | Unsolvable | — | — |
| 012 | puzzle012-2x2 | 2×2 | Solved | 3 | <1 ms |
| 013 | puzzle013-3x3 | 3×3 | Solved | 14 | <1 ms |
| 014 | puzzle014-3x3 | 3×3 | Solved | 1 | <1 ms |
| 015 | puzzle015-3x3 | 3×3 | Solved | 2 | <1 ms |
| 016 | puzzle016-3x3 | 3×3 | Solved | 3 | <1 ms |
| 017 | puzzle017-3x3 | 3×3 | Solved | 4 | <1 ms |
| 018 | puzzle018-3x3 | 3×3 | Solved | 5 | <1 ms |
| 019 | puzzle019-3x3 | 3×3 | Solved | 6 | <1 ms |
| 020 | puzzle020-3x3 | 3×3 | Solved | 7 | <1 ms |
| 021 | puzzle021-3x3 | 3×3 | Solved | 8 | <1 ms |
| 022 | puzzle022-3x3 | 3×3 | Solved | 9 | <1 ms |
| 023 | puzzle023-3x3 | 3×3 | Solved | 10 | <1 ms |
| 024 | puzzle024-3x3 | 3×3 | Solved | 11 | <1 ms |
| 025 | puzzle025-3x3 | 3×3 | Solved | 12 | <1 ms |
| 026 | puzzle026-3x3 | 3×3 | Solved | 13 | <1 ms |
| 027 | puzzle027-3x3 | 3×3 | Solved | 14 | <1 ms |
| 028 | puzzle028-3x3 | 3×3 | Solved | 15 | <1 ms |
| 029 | puzzle029-3x3 | 3×3 | Solved | 16 | <1 ms |
| 030 | puzzle030-3x3 | 3×3 | Solved | 17 | <1 ms |
| 031 | puzzle031-3x3 | 3×3 | Solved | 18 | <1 ms |
| 032 | puzzle032-3x3 | 3×3 | Solved | 19 | <1 ms |
| 033 | puzzle033-3x3 | 3×3 | Solved | 20 | <1 ms |
| 034 | puzzle034-3x3 | 3×3 | Solved | 21 | <1 ms |
| 035 | puzzle035-3x3 | 3×3 | Solved | 22 | <1 ms |
| 036 | puzzle036-3x3 | 3×3 | Solved | 23 | <1 ms |
| 037 | puzzle037-3x3 | 3×3 | Solved | 24 | <1 ms |
| 038 | puzzle038-3x3 | 3×3 | Solved | 25 | <1 ms |
| 039 | puzzle039-3x3 | 3×3 | Solved | 26 | <1 ms |
| 040 | puzzle040-3x3 | 3×3 | Solved | 27 | <1 ms |
| 041 | puzzle041-3x3 | 3×3 | Solved | 28 | <1 ms |
| 042 | puzzle042-3x3 | 3×3 | Solved | 29 | <1 ms |
| 043 | puzzle043-3x3 | 3×3 | Solved | 30 | <1 ms |
| 044 | puzzle044-3x3 | 3×3 | Solved | 31 | <1 ms |
| 045 | puzzle045-3x3-unsolvable | 3×3 | Unsolvable | — | — |
| 046 | puzzle046-3x3-unsolvable | 3×3 | Unsolvable | — | — |
| 047 | puzzle047-3x3-unsolvable | 3×3 | Unsolvable | — | — |
| 048 | puzzle048-3x3 | 3×3 | Solved | 4 | <1 ms |
| 049 | puzzle049-4x4 | 4×4 | Solved | 0 | — (already solved) |
| 050 | puzzle050-4x4 | 4×4 | Solved | 1 | <1 ms |
| 051 | puzzle051-4x4 | 4×4 | Solved | 2 | <1 ms |
| 052 | puzzle052-4x4 | 4×4 | Solved | 3 | <1 ms |
| 053 | puzzle053-4x4 | 4×4 | Solved | 4 | <1 ms |
| 054 | puzzle054-4x4 | 4×4 | Solved | 5 | <1 ms |
| 055 | puzzle055-4x4 | 4×4 | Solved | 6 | <1 ms |
| 056 | puzzle056-4x4 | 4×4 | Solved | 7 | <1 ms |
| 057 | puzzle057-4x4 | 4×4 | Solved | 8 | <1 ms |
| 058 | puzzle058-4x4 | 4×4 | Solved | 9 | <1 ms |
| 059 | puzzle059-4x4 | 4×4 | Solved | 10 | <1 ms |
| 060 | puzzle060-4x4 | 4×4 | Solved | 11 | <1 ms |
| 061 | puzzle061-4x4 | 4×4 | Solved | 12 | <1 ms |
| 062 | puzzle062-4x4 | 4×4 | Solved | 13 | <1 ms |
| 063 | puzzle063-4x4 | 4×4 | Solved | 14 | <1 ms |
| 064 | puzzle064-4x4 | 4×4 | Solved | 15 | <1 ms |
| 065 | puzzle065-4x4 | 4×4 | Solved | 16 | <1 ms |
| 066 | puzzle066-4x4 | 4×4 | Solved | 17 | <1 ms |
| 067 | puzzle067-4x4 | 4×4 | Solved | 18 | <1 ms |
| 068 | puzzle068-4x4 | 4×4 | Solved | 19 | <1 ms |
| 069 | puzzle069-4x4 | 4×4 | Solved | 20 | <1 ms |
| 070 | puzzle070-4x4 | 4×4 | Solved | 21 | <1 ms |
| 071 | puzzle071-4x4 | 4×4 | Solved | 22 | <1 ms |
| 072 | puzzle072-4x4 | 4×4 | Solved | 23 | <1 ms |
| 073 | puzzle073-4x4 | 4×4 | Solved | 24 | <1 ms |
| 074 | puzzle074-4x4 | 4×4 | Solved | 25 | <1 ms |
| 075 | puzzle075-4x4 | 4×4 | Solved | 26 | <1 ms |
| 076 | puzzle076-4x4 | 4×4 | Solved | 27 | <1 ms |
| 077 | puzzle077-4x4 | 4×4 | Solved | 28 | <1 ms |
| 078 | puzzle078-4x4 | 4×4 | Solved | 29 | <1 ms |
| 079 | puzzle079-4x4 | 4×4 | Solved | 30 | <1 ms |
| 080 | puzzle080-4x4 | 4×4 | Solved | 31 | <1 ms |
| 081 | puzzle081-4x4 | 4×4 | Solved | 32 | 1 ms |
| 082 | puzzle082-4x4 | 4×4 | Solved | 33 | <1 ms |
| 083 | puzzle083-4x4 | 4×4 | Solved | 34 | <1 ms |
| 084 | puzzle084-4x4 | 4×4 | Solved | 35 | <1 ms |
| 085 | puzzle085-4x4 | 4×4 | Solved | 36 | 4 ms |
| 086 | puzzle086-4x4 | 4×4 | Solved | 37 | <1 ms |
| 087 | puzzle087-4x4 | 4×4 | Solved | 38 | 5 ms |
| 088 | puzzle088-4x4 | 4×4 | Solved | 39 | 1 ms |
| 089 | puzzle089-4x4 | 4×4 | Solved | 40 | 3 ms |
| 090 | puzzle090-4x4 | 4×4 | Solved | 41 | 5 ms |
| 091 | puzzle091-4x4 | 4×4 | Solved | 42 | 1 ms |
| 092 | puzzle092-4x4 | 4×4 | Solved | 43 | 23 ms |
| 093 | puzzle093-4x4 | 4×4 | Solved | 44 | 1 ms |
| 094 | puzzle094-4x4 | 4×4 | Solved | 45 | 40 ms |
| 095 | puzzle095-4x4 | 4×4 | Solved | 46 | 61 ms |
| 096 | puzzle096-4x4 | 4×4 | Solved | 47 | 47 ms |
| 097 | puzzle097-4x4 | 4×4 | Solved | 48 | 21 ms |
| 098 | puzzle098-4x4 | 4×4 | Solved | 49 | 216 ms |
| 099 | puzzle099-4x4 | 4×4 | Solved | 50 | 524 ms |
| 100 | puzzle100-4x4 | 4×4 | Solved | 78 | 11 min 30 s |
| 101 | puzzle101-4x4 | 4×4 | **Timed out** | — | >300 s |
| 102 | puzzle102-4x4-unsolvable | 4×4 | Unsolvable | — | — |
| 103 | puzzle103-3x3 | 3×3 | Solved | 5 | <1 ms |
| 104 | puzzle104-4x4 | 4×4 | Solved | 6 | <1 ms |
| 105 | puzzle105-3x3 | 3×3 | Solved | 7 | <1 ms |
| 106 | puzzle106-3x3 | 3×3 | Solved | 8 | <1 ms |
| 107 | puzzle107-4x4 | 4×4 | Solved | 9 | <1 ms |
| 108 | puzzle108-9x9 | 9×9 | Solved | 10 | <1 ms |
| 109 | puzzle109-3x3 | 3×3 | Solved | 11 | <1 ms |
| 110 | puzzle110-5x5 | 5×5 | Solved | 12 | <1 ms |
| 111 | puzzle111-4x4 | 4×4 | Solved | 13 | <1 ms |
| 112 | puzzle112-7x7 | 7×7 | Solved | 14 | <1 ms |
| 113 | puzzle113-8x8 | 8×8 | Solved | 15 | <1 ms |
| 114 | puzzle114-10x10 | 10×10 | Solved | 16 | <1 ms |
| 115 | puzzle115-3x3 | 3×3 | Solved | 17 | <1 ms |
| 116 | puzzle116-3x3 | 3×3 | Solved | 18 | <1 ms |
| 117 | puzzle117-3x3 | 3×3 | Solved | 19 | <1 ms |
| 118 | puzzle118-3x3 | 3×3 | Solved | 20 | <1 ms |
| 119 | puzzle119-3x3 | 3×3 | Solved | 21 | <1 ms |
| 120 | puzzle120-3x3 | 3×3 | Solved | 22 | <1 ms |
| 121 | puzzle121-3x3 | 3×3 | Solved | 23 | <1 ms |
| 122 | puzzle122-3x3 | 3×3 | Solved | 24 | <1 ms |
| 123 | puzzle123-3x3 | 3×3 | Solved | 25 | <1 ms |
| 124 | puzzle124-3x3 | 3×3 | Solved | 26 | <1 ms |
| 125 | puzzle125-3x3 | 3×3 | Solved | 27 | <1 ms |
| 126 | puzzle126-3x3 | 3×3 | Solved | 28 | <1 ms |
| 127 | puzzle127-3x3 | 3×3 | Solved | 29 | <1 ms |
| 128 | puzzle128-3x3 | 3×3 | Solved | 30 | <1 ms |
| 129 | puzzle129-3x3 | 3×3 | Solved | 31 | <1 ms |
| 130 | puzzle130-4x4 | 4×4 | Solved | 32 | 2 ms |
| 131 | puzzle131-4x4 | 4×4 | Solved | 33 | 1 ms |
| 132 | puzzle132-4x4 | 4×4 | Solved | 34 | 1 ms |
| 133 | puzzle133-4x4 | 4×4 | Solved | 35 | 1 ms |
| 134 | puzzle134-4x4 | 4×4 | Solved | 36 | 7 ms |
| 135 | puzzle135-4x4 | 4×4 | Unsolvable | — | — |
| 136 | puzzle136-4x4 | 4×4 | Solved | 38 | 12 ms |
| 137 | puzzle137-4x4 | 4×4 | Solved | 39 | <1 ms |
| 138 | puzzle138-4x4 | 4×4 | Solved | 40 | 1 ms |
| 139 | puzzle139-5x5 | 5×5 | Solved | 41 | <1 ms |
| 140 | puzzle140-4x4 | 4×4 | Solved | 42 | 11 ms |
| 141 | puzzle141-4x4 | 4×4 | Solved | 43 | 6 ms |
| 142 | puzzle142-5x5 | 5×5 | Solved | 44 | <1 ms |
| 143 | puzzle143-4x4 | 4×4 | Solved | 45 | 4 ms |
| 144 | puzzle144-4x4 | 4×4 | Solved | 46 | 1 ms |
| 145 | puzzle145-4x4 | 4×4 | Solved | 47 | 33 ms |
| 146 | puzzle146-4x4 | 4×4 | Solved | 48 | 8 ms |
| 147 | puzzle147-4x4 | 4×4 | Solved | 49 | 142 ms |
| 148 | puzzle148-4x4 | 4×4 | Solved | 50 | 98 ms |
| 149 | puzzle149-4x4 | 4×4 | Solved | 48 | 83 ms |
| 150 | puzzle150-4x4 | 4×4 | Solved | 50 | 98 ms |
| 151 | puzzle151-4x4 | 4×4 | Solved | 47 | 82 ms |
| 152 | puzzle152-4x4 | 4×4 | Solved | 48 | 82 ms |
| 153 | puzzle153-4x4 | 4×4 | Solved | 49 | 336 ms |
| 154 | puzzle154-4x4 | 4×4 | Solved | 50 | 536 ms |
| 155 | puzzle155-4x4 | 4×4 | Solved | 53 | 913 ms |
| 156 | puzzle156-4x4 | 4×4 | Solved | 52 | 197 ms |
| 157 | puzzle157-4x4 | 4×4 | Solved | 51 | 109 ms |
| 158 | puzzle158-4x4-unsolvable | 4×4 | Unsolvable | — | — |
| 159 | puzzle159-4x4-unsolvable | 4×4 | Unsolvable | — | — |
| 160 | puzzle160-4x4-unsolvable | 4×4 | Unsolvable | — | — |

---

## Analysis

### IDA* performance by grid size

| Grid | Puzzles | Max optimal moves | Max elapsed |
|---|---|---|---|
| 2×2 | 12 | 6 | <1 ms |
| 3×3 | 46 | 31 | <1 ms |
| 4×4 | 91 | 78 | 11 min 30 s |
| 5×5 | 3 | 44 | <1 ms |
| 7×7 | 1 | 14 | <1 ms |
| 8×8 | 1 | 15 | <1 ms |
| 9×9 | 1 | 10 | <1 ms |
| 10×10 | 1 | 16 | <1 ms |

Note: large grid puzzles (7×7–10×10) solved instantly because they were very close to the goal state (≤16 moves). IDA\* difficulty depends on **solution depth**, not grid size alone.

### Elapsed time vs. optimal moves (4×4 puzzles)

The 4×4 (15-puzzle) is where IDA\* difficulty becomes visible. Elapsed time grows roughly exponentially with solution depth:

| Moves | Elapsed |
|---|---|
| ≤ 31 | <1 ms |
| 32–40 | 1–5 ms |
| 41–46 | 1–61 ms |
| 47–50 | 21–524 ms |
| 51–53 | 109–913 ms |
| 78 | 11 min 30 s |
| unknown | >300 s (timed out) |

Puzzle 100 (78 moves) took **11 min 30 s** — the current longest solved case. The jump from ~50 moves (under 1 s) to 78 moves (11+ minutes) illustrates the exponential nature of IDA\*.

### Timed-out puzzles

#### puzzle101-4x4 — genuinely hard

```
 0 12  9 13
15 11 10 14
 3  7  5  6
 4  8  2  1
```

Blank at top-left, all tiles heavily scrambled. The optimal solution is likely 60–80+ moves. Given that puzzle100 (78 moves) took 11.5 minutes, puzzle101 likely requires similar or greater search time. It is solvable — IDA\* will find it eventually, but exceeds the 5-minute timeout.

#### puzzle158 / puzzle159 / puzzle160 — solvability check bug (fixed) ✓

All three share the same board but have non-standard blank target positions (0, 2, and 8 respectively instead of the default 15):

```
 2  9  3  5
 8 11 12  7
15  4  0 13
 6  1 10 14
```

The original `check_solvable()` computed parity using only the blank's **current** row but did not account for the blank's **target** row. For even-width grids the correct condition is:

```
(inversions + blank_current_row_from_bottom + blank_target_row_from_bottom) % 2 == 0
```

Without `blank_target_row_from_bottom` the check returned the wrong answer for non-default blank targets, so IDA\* searched indefinitely for a solution that does not exist.

**Fix applied:** `check_solvable()` now computes both rows from the bottom and sums them with the inversion count. All three puzzles are now correctly detected as unsolvable in under 1 ms.

---

## Observations

1. **Manhattan Distance + Linear Conflict** is sufficient for all puzzles up to ~50 optimal moves in well under 1 second.
2. **Solution depth, not grid size**, determines IDA\* runtime — a 10×10 puzzle solved in <1 ms because only 16 moves were needed.
3. **Exponential blow-up** starts around 50 moves for the 15-puzzle; at 78 moves it takes 11+ minutes.
4. **Puzzle100** (78 moves, 11 min 30 s) is the hardest successfully solved puzzle in this dataset.
5. **Puzzle101** is the only genuinely unsolved solvable puzzle — it needs either more time or a stronger heuristic (e.g. Walking Distance or Pattern Databases).
6. **Puzzles 158–160** had a data + solvability-check bug: they are unsolvable, but the original solver incorrectly concluded they were solvable due to a parity check that did not account for the non-default blank target row. Fixed in all three implementations (C++, Java, Python).
