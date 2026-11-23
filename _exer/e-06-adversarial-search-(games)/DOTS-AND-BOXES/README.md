# Dots and Boxes — Minimax with Alpha-Beta Pruning

Play against a computer opponent using the **minimax algorithm with alpha-beta pruning**.

See [PROBLEM.md](PROBLEM.md) for the problem statement and
[SOLUTION.md](SOLUTION.md) for the algorithm description.

---

## Implementations

| Language | Entry point |
|----------|-------------|
| Python | `PYTHON/dots_and_boxes.py` |

---

## Run

```bash
cd PYTHON
python3 dots_and_boxes.py
```

Requires Python 3.10+.

---

## Input format

```
Enter N, M:
3 3

Computer(1) or Player(2) is first?
2
```

On each turn:

```
Please enter your move:
  Format: <1=vertical/2=horizontal> <col> <row>  (1-indexed)
1 1 1
```

- `1` = vertical edge, `2` = horizontal edge
- col and row are 1-indexed, referring to the top-left dot of the edge

---

## Example session

```
Enter N, M:
3 3

Computer(1) or Player(2) is first?
2

o   o   o   o

o   o   o   o

o   o   o   o

o   o   o   o
Score — Computer: 0  Player: 0

Please enter your move:
  Format: <1=vertical/2=horizontal> <col> <row>  (1-indexed)
1 1 1

o   o   o   o
|
o   o   o   o

o   o   o   o

o   o   o   o
Score — Computer: 0  Player: 0

Computer plays:

o - o   o   o
|
o   o   o   o

...

Computer wins!
```

---

## Notes

- **Board size**: N×M boxes → (N+1)×(M+1) dots. Default is 3×3 (4×4 dots, 24 edges).
- **2×2 board**: exhaustive minimax — computer is provably optimal (never scores fewer boxes than the opponent with best play).
- **3×3 board**: depth-7 minimax with heuristic — full search is infeasible (24! positions).
- **Extra-turn rule**: closing a box grants another turn; minimax handles this by keeping the same player's perspective without decrementing depth.
- **First-mover advantage**: on 2×2, the first player wins 3-1 with perfect play. The computer cannot guarantee a win when going second — the best it achieves is matching optimal play.
