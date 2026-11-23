import sys

N = 3
SYMBOLS = ('X', 'O')
INF = float('inf')


class TicTacToe:
    def __init__(self, player_first: bool = True):
        self.board = [[' '] * N for _ in range(N)]
        self.player_first = player_first
        self.moves = 0
        self.result = 0  # 0=ongoing, 1=X won, 2=O won, -1=draw
        if not player_first:
            self._make_best_move()

    # ---- winner detection ----

    def _has_winner(self, lp_row: int, lp_col: int, dr: int, dc: int) -> bool:
        r, c = lp_row, lp_col
        while 0 <= r - dr < N and 0 <= c - dc < N:
            r -= dr
            c -= dc
        sym = self.board[lp_row][lp_col]
        for step in range(N):
            nr, nc = r + dr * step, c + dc * step
            if not (0 <= nr < N and 0 <= nc < N):
                return False
            if self.board[nr][nc] != sym:
                return False
        return True

    def _winner_at(self, row: int, col: int) -> bool:
        return (self._has_winner(row, col, 0,  1) or
                self._has_winner(row, col, 1,  0) or
                self._has_winner(row, col, 1,  1) or
                self._has_winner(row, col, 1, -1))

    # ---- move / undo ----

    def _make_move(self, row: int, col: int):
        self.board[row][col] = SYMBOLS[self.moves % 2]
        self.moves += 1
        if self._winner_at(row, col):
            self.result = (self.moves - 1) % 2 + 1
        elif self.moves >= N * N:
            self.result = -1

    def _undo_move(self, row: int, col: int):
        self.moves -= 1
        self.board[row][col] = ' '
        self.result = 0

    # ---- utility ----

    def _utility(self) -> float:
        if self.result < 0:
            return 0
        cpu_won = (self.result == (2 if self.player_first else 1))
        score = 10 + N * N - self.moves
        return score if cpu_won else -score

    # ---- alpha-beta minimax ----

    def _alphabeta(self, alpha: float, beta: float):
        if self.result != 0:
            return None, None, self._utility()

        cpu_to_move = (self.moves % 2 == (1 if self.player_first else 0))
        best_r, best_c = N // 2, N // 2
        value = -INF if cpu_to_move else INF

        done = False
        for r in range(N):
            if done:
                break
            for c in range(N):
                if self.board[r][c] != ' ':
                    continue
                self._make_move(r, c)
                _, _, new_value = self._alphabeta(alpha, beta)
                self._undo_move(r, c)
                if cpu_to_move:
                    if new_value > value:
                        value, best_r, best_c = new_value, r, c
                    if value > alpha:
                        alpha = value
                else:
                    if new_value < value:
                        value, best_r, best_c = new_value, r, c
                    if value < beta:
                        beta = value
                if alpha >= beta:
                    done = True
                    break

        return best_r, best_c, value

    def _make_best_move(self):
        r, c, _ = self._alphabeta(-INF, INF)
        self._make_move(r, c)

    # ---- public API ----

    def play(self, row: int, col: int) -> bool:
        if not (0 <= row < N and 0 <= col < N) or self.board[row][col] != ' ':
            return False
        self._make_move(row, col)
        if self.result == 0:
            self._make_best_move()
        return True

    def __str__(self) -> str:
        lines = ['\n  ' + ' '.join(str(c + 1) for c in range(N))]
        for r in range(N):
            row_str = str(r + 1) + ' '
            row_str += '|'.join('.' if self.board[r][c] == ' ' else self.board[r][c]
                                for c in range(N))
            lines.append(row_str)
        lines.append('')
        return '\n'.join(lines)


def main():
    print("Do you want to play first? [y/n] ", end='', flush=True)
    line = input().strip()
    c = line[0] if line else 'n'

    if c in ('y', 'Y'):
        player_first = True
    else:
        if c not in ('n', 'N'):
            print("Bad choice! You will play second!")
        player_first = False

    game = TicTacToe(player_first)

    while game.result == 0:
        print(game)
        while True:
            print("Where do you want to play? (row col): ", end='', flush=True)
            try:
                parts = input().split()
                if len(parts) < 2:
                    raise ValueError
                r, col = int(parts[0]), int(parts[1])
            except (ValueError, IndexError):
                print("Invalid input. Enter two numbers in [1,3].")
                continue
            except EOFError:
                return
            if not (1 <= r <= 3 and 1 <= col <= 3):
                print("Values must be in [1,3].")
                continue
            if not game.play(r - 1, col - 1):
                print("Cell already occupied. Try again.")
                continue
            break

    print(game)
    w = game.result
    if w < 0:
        print("Draw!")
    else:
        print("You", "lose!" if w == (2 if game.player_first else 1) else "win!")


if __name__ == '__main__':
    main()
