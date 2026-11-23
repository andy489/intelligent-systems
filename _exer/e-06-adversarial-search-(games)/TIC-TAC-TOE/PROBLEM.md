# Problem: Tic-Tac-Toe with Min-Max and Alpha-Beta Pruning

## Description

Implement a game of **Tic-Tac-Toe** against a computer opponent using the **minimax algorithm with alpha-beta pruning**.

## Rules

- Start with an empty 3×3 board.
- On each turn:
  1. The human player enters two numbers `[1, 3]` from the keyboard — row and column of their move.
  2. The computer uses the algorithm to find its optimal move.
  3. The current state of the board is displayed.
- At the end, the winner is announced.

## Requirements

- It must be configurable whether **the computer** or **the player** goes first.
- The algorithm must be **optimal** — the computer never loses.

## Input

```
Do you want to play first? [y/n]
```

Then on each turn:

```
Where do you want to play? (row col):
2 2
```

## Output

```
  1 2 3
1 .|.|.
2 .|X|.
3 .|.|O

...

Draw!
```

or `You win!` / `You lose!`
