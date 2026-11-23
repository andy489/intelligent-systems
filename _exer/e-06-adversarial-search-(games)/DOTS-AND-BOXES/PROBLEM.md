# Problem: Dots and Boxes with Min-Max and Alpha-Beta Pruning

## Description

Implement a game of **Dots and Boxes** against a computer opponent using the **minimax algorithm with alpha-beta pruning**.

## Rules

The board starts as an N×M grid of dots (all edges empty).

On each turn:
1. The current player draws one edge between two adjacent dots.
2. If a player completes the fourth side of a box, they claim it and **take another turn**.
3. The player who claims **more boxes** wins.

## Input

```
Enter N, M:
3 3

Computer(1) or Player(2) is first?
2
```

Then on each turn:

```
Please enter your move:
  Format: <1=vertical/2=horizontal> <col> <row>  (1-indexed)
1 1 1
```

- **Orientation**: `1` = vertical edge, `2` = horizontal edge
- **Col**: 1-indexed column of the left/top dot
- **Row**: 1-indexed row of the left/top dot

## Output

The board is rendered after every move:

```
o - o   o   o
|            
o - o   o   o
             
o   o   o   o
             
o   o   o   o
Score — Computer: 1  Player: 0
```

- `o` = dot
- `-` = horizontal edge
- `|` = vertical edge
- `C` = box claimed by computer
- `P` = box claimed by player

At the end: `Computer wins!`, `You win!`, or `Draw!`

## Notes

- **Default board**: 3×3 boxes (4×4 dots, 24 edges). Full search (24!) is infeasible; a **depth-limited minimax with a heuristic** is applied.
- **2×2 board** (3×3 dots, 12 edges, 12! combinations): exhaustive minimax is used.
- The heuristic rewards claiming boxes with 3 existing edges (easy captures) and penalises 2-edge chains that set up the opponent.
