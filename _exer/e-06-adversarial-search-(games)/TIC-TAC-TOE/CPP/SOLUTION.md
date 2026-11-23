# Solution: Tic-Tac-Toe with Minimax + Alpha-Beta Pruning (C++)

## Approach

The implementation uses a **template class** `TicTacToe<N>` (supports 3×3 and 4×4 boards) with a built-in **minimax algorithm with alpha-beta pruning**.

## Key Design Decisions

### Class Structure

```
TicTacToe<N>
├── board[N][N]   — the board (' ', 'X', 'O')
├── playerFirst   — true if the human moves first
├── moves         — number of moves made so far
└── result        — 0=ongoing, 1=X won, 2=O won, -1=draw
```

### Incremental Winner Detection

Instead of scanning all lines after every move, `hasWinner(lastPlaced, offset)` checks **only the lines passing through the last-placed piece** — row, column, main diagonal, and anti-diagonal. This avoids redundant work on every call.

### Algorithm

```
alphabeta(alpha, beta):
  if result != 0: return utility()
  for each empty cell:
    makeMove(cell)
    score = alphabeta(alpha, beta).score
    undoMove(cell)
    update best, alpha/beta
    if alpha >= beta: prune (cut-off)
  return best
```

### Utility Function

```
utility():
  draw  →  0
  win   → +(10 + N² - depth)   // earlier win = higher score
  loss  → -(10 + N² - depth)
```

The depth adjustment (`N² - depth`) makes the algorithm prefer **faster wins** and **slower losses**.

### `makeMove` / `undoMove`

In-place board mutation is used instead of copying the board state, avoiding heap allocation inside the search tree.

## Complexity

- The game tree has at most **9!** leaves for 3×3.
- Alpha-beta pruning significantly reduces the effective branching factor.
- In practice the computer moves **instantly**.

## Example Game (computer goes first)

```
Do you want to play first? [y/n] n

  1 2 3
1 X|.|.
2 .|.|.
3 .|.|.

Where do you want to play? (row col): 2 2

  1 2 3
1 X|X|.
2 .|O|.
3 .|.|.

...

Draw!
```

## Build & Run

```bash
g++ -std=c++17 -O2 -o tictactoe tictactoe.cpp
./tictactoe
```
