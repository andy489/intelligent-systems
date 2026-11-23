# Tic-Tac-Toe — Minimax with Alpha-Beta Pruning

Play against an unbeatable computer opponent using the **minimax algorithm with alpha-beta pruning**.

See [PROBLEM.md](PROBLEM.md) for the problem statement and
[SOLUTION.md](SOLUTION.md) for the algorithm description.

---

## Implementations

| Language | Entry point |
|----------|-------------|
| C++    | `CPP/tictactoe.cpp` |
| Java   | `JAVA/TicTacToe.java` |
| Python | `PYTHON/tictactoe.py` |

All three implement the **identical algorithm** and produce the same game behavior.

---

## Build

### C++
```bash
cd CPP
g++ -std=c++17 -O2 -o tictactoe tictactoe.cpp
```

### Java
```bash
cd JAVA
javac TicTacToe.java
```

### Python
No build step needed — requires Python 3.6+.

---

## Run

### C++
```bash
cd CPP
./tictactoe
```

### Java
```bash
cd JAVA
java TicTacToe
```

### Python
```bash
cd PYTHON
python3 tictactoe.py
```

---

## Example session

```
Do you want to play first? [y/n] y

  1 2 3
1 .|.|.
2 .|.|.
3 .|.|.

Where do you want to play? (row col): 2 2

  1 2 3
1 .|.|.
2 .|X|.
3 .|.|O

Where do you want to play? (row col): ...

Draw!
```

---

## Notes

- Enter your move as `row col` (both in `[1, 3]`), separated by a space.
- The computer never loses — the best achievable outcome for the human is a draw.
- The computer prefers the center square when multiple moves score equally.
