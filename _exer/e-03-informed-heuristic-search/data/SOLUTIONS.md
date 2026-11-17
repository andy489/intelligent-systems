# E03 — Hard Puzzle Solutions

Optimal move sequences for the two manually added hard 4×4 puzzles.  
All solutions produced by C++ IDA* (Manhattan Distance + Linear Conflict).

---

## puzzle161 — HARD

**File:** [`puzzles/puzzle161-4x4-HARD.txt`](puzzles/puzzle161-4x4-HARD.txt)

**Initial state:**
```
 9  5  1 12
10  _  11 13
 3  7 14  6
 2  8 15  4
```

**Optimal moves: 56**

```
left up right right down right down down left up up up right down down down
left up left down right right up left up left down left down right right up
up up right down left left down left up up right down down right right up
left up left down right down down right
```

| Language | Moves | Time |
|---|---|---|
| C++ | 56 | 3.7 s |
| Java | 56 | 4.9 s |
| Python | 56 | ~116 s |

---

## puzzle162 — MEGA-HARD

**File:** [`puzzles/puzzle162-4x4-MEGA-HARD.txt`](puzzles/puzzle162-4x4-MEGA-HARD.txt)

**Initial state:**
```
14 15  8 12
10 11  9 13
 2  6  5  1
 3  7  4  _
```

**Optimal moves: 66**

```
up up up left down left down right down right up up left down down left up
up up right down left down right down right up left up left down left up up
right down down down left up up up right down down down right up left left
down right up left up right right down right up up left down down right down
```

| Language | Moves | Time |
|---|---|---|
| C++ | 66 | 51.5 s |
| Java | 66 | ~68 s |
| Python | — | > 600 s (timeout) |
