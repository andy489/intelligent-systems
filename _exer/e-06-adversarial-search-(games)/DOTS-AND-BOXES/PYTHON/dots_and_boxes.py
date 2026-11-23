"""
Dots and Boxes — minimax with alpha-beta pruning.

Board notation
--------------
An N×M grid has (N+1) rows and (M+1) columns of dots.
Edges:
  h[r][c]  horizontal edge between dot(r,c) and dot(r,c+1)   r in [0..N], c in [0..M-1]
  v[r][c]  vertical   edge between dot(r,c) and dot(r+1,c)   r in [0..N-1], c in [0..M]

Boxes:
  box[r][c] is completed when h[r][c], h[r+1][c], v[r][c], v[r][c+1] are all set.
  r in [0..N-1], c in [0..M-1]

Move encoding: (horiz: bool, col: int, row: int)   (1-indexed input)
"""

import sys
from copy import deepcopy

INF = float('inf')

# Depth limit for minimax on larger boards to keep it playable
DEPTH_LIMIT = {
    (2, 2): INF,   # 12 edges total — exhaustive is fine (~1s)
    (3, 3): 7,     # 24 edges — depth-limited + heuristic (~1-2s per move)
}
DEFAULT_DEPTH = 5


def _depth_limit(n: int, m: int) -> float:
    return DEPTH_LIMIT.get((n, m), DEFAULT_DEPTH)


class DotsAndBoxes:
    # owner codes: 0 = none, 1 = computer, 2 = player
    OWNER_CHAR = {0: ' ', 1: 'C', 2: 'P'}

    def __init__(self, n: int, m: int, computer_first: bool):
        self.N = n          # rows of boxes
        self.M = m          # cols of boxes
        self.computer_first = computer_first

        # edges (bool arrays)
        self.h = [[False] * m for _ in range(n + 1)]   # horizontal
        self.v = [[False] * (m + 1) for _ in range(n)] # vertical

        # box owners (0 = unclaimed)
        self.box = [[0] * m for _ in range(n)]

        # scores
        self.score = [0, 0, 0]  # index 0 unused; 1=computer, 2=player

        # whose turn: 1=computer, 2=player
        self.turn = 1 if computer_first else 2

        self.depth_limit = _depth_limit(n, m)

        if computer_first:
            move = self._best_move()
            self._apply(move, 1)
            print("Computer plays:")
            print(self)

    # ------------------------------------------------------------------ #
    #  State helpers                                                        #
    # ------------------------------------------------------------------ #

    def _get_edge(self, horiz: bool, col: int, row: int) -> bool:
        if horiz:
            return self.h[row][col]
        return self.v[row][col]

    def _set_edge(self, horiz: bool, col: int, row: int, val: bool):
        if horiz:
            self.h[row][col] = val
        else:
            self.v[row][col] = val

    def _boxes_closed_by(self, horiz: bool, col: int, row: int) -> list[tuple[int, int]]:
        """Return list of box (r,c) indices completed by placing edge (horiz,col,row)."""
        closed = []
        if horiz:
            # edge h[row][col] is the top of box(row,col) and bottom of box(row-1,col)
            for br in (row - 1, row):
                if 0 <= br < self.N:
                    if (self.h[br][col] and self.h[br + 1][col] and
                            self.v[br][col] and self.v[br][col + 1]):
                        closed.append((br, col))
        else:
            # edge v[row][col] is left of box(row,col) and right of box(row,col-1)
            for bc in (col - 1, col):
                if 0 <= bc < self.M:
                    if (self.h[row][bc] and self.h[row + 1][bc] and
                            self.v[row][bc] and self.v[row][bc + 1]):
                        closed.append((row, bc))
        return closed

    def _apply(self, move: tuple, owner: int) -> list[tuple[int, int]]:
        """Place edge, claim any completed boxes, return list of newly claimed boxes."""
        horiz, col, row = move
        self._set_edge(horiz, col, row, True)
        newly_closed = []
        for br, bc in self._boxes_closed_by(horiz, col, row):
            if self.box[br][bc] == 0:
                self.box[br][bc] = owner
                self.score[owner] += 1
                newly_closed.append((br, bc))
        return newly_closed

    def _undo(self, move: tuple, newly_closed: list[tuple[int, int]], owner: int):
        horiz, col, row = move
        self._set_edge(horiz, col, row, False)
        for br, bc in newly_closed:
            self.box[br][bc] = 0
            self.score[owner] -= 1

    def _legal_moves(self) -> list[tuple]:
        moves = []
        for r in range(self.N + 1):
            for c in range(self.M):
                if not self.h[r][c]:
                    moves.append((True, c, r))
        for r in range(self.N):
            for c in range(self.M + 1):
                if not self.v[r][c]:
                    moves.append((False, c, r))
        return moves

    def _is_terminal(self) -> bool:
        return all(self.box[r][c] != 0
                   for r in range(self.N) for c in range(self.M))

    def _edge_count_around(self, br: int, bc: int) -> int:
        return (self.h[br][bc] + self.h[br + 1][bc] +
                self.v[br][bc] + self.v[br][bc + 1])

    # ------------------------------------------------------------------ #
    #  Heuristic evaluation (used when depth limit is reached)             #
    # ------------------------------------------------------------------ #

    def _would_close_count(self, horiz: bool, col: int, row: int) -> int:
        """Count how many boxes this move would close (without modifying state)."""
        count = 0
        if horiz:
            for br in (row - 1, row):
                if 0 <= br < self.N and self.box[br][col] == 0:
                    if (self.h[br][col] and self.h[br + 1][col] and
                            self.v[br][col] and self.v[br][col + 1]):
                        count += 1
        else:
            for bc in (col - 1, col):
                if 0 <= bc < self.M and self.box[row][bc] == 0:
                    if (self.h[row][bc] and self.h[row + 1][bc] and
                            self.v[row][bc] and self.v[row][bc + 1]):
                        count += 1
        return count

    def _heuristic(self, is_max: bool) -> float:
        """
        Score = (computer_score - player_score) + strategic bonus.
        3-edge boxes are free captures for whoever moves next; sign depends on is_max.
        2-edge boxes open chains that tend to benefit the opponent of whoever opens them.
        """
        diff = float(self.score[1] - self.score[2])
        bonus = 0.0
        for r in range(self.N):
            for c in range(self.M):
                if self.box[r][c] != 0:
                    continue
                ec = self._edge_count_around(r, c)
                if ec == 3:
                    # Free capture for the side about to move
                    bonus += 0.3 if is_max else -0.3
                elif ec == 2:
                    # Opening a chain tends to give opponent a run; penalise slightly
                    bonus -= 0.1
        return diff + bonus

    # ------------------------------------------------------------------ #
    #  Minimax with alpha-beta                                             #
    # ------------------------------------------------------------------ #

    def _minimax(self, alpha: float, beta: float,
                 is_max: bool, depth: int) -> tuple[tuple | None, float]:
        if self._is_terminal():
            return None, float(self.score[1] - self.score[2])

        if depth == 0:
            return None, self._heuristic(is_max)

        moves = self._legal_moves()
        # Move ordering: box-closing moves first (improves alpha-beta cutoffs).
        moves.sort(key=lambda m: -self._would_close_count(*m))

        best_move = moves[0]
        best_val = -INF if is_max else INF

        for move in moves:
            owner = 1 if is_max else 2
            closed = self._apply(move, owner)

            # Closing a box grants an extra turn — same player moves again.
            # Depth only decrements on a true turn switch.
            # Termination is guaranteed: each call places one more edge and
            # the board has finitely many edges, so _is_terminal() is always reached.
            if closed:
                _, val = self._minimax(alpha, beta, is_max, depth)
            else:
                _, val = self._minimax(alpha, beta, not is_max, depth - 1)

            self._undo(move, closed, owner)

            if is_max:
                if val > best_val:
                    best_val, best_move = val, move
                alpha = max(alpha, best_val)
            else:
                if val < best_val:
                    best_val, best_move = val, move
                beta = min(beta, best_val)

            if alpha >= beta:
                break

        return best_move, best_val

    def _best_move(self) -> tuple:
        move, _ = self._minimax(-INF, INF, True, self.depth_limit)
        return move

    # ------------------------------------------------------------------ #
    #  Public interface                                                     #
    # ------------------------------------------------------------------ #

    def player_move(self, horiz: bool, col: int, row: int) -> bool:
        """Validate and apply player move; then let computer respond."""
        if horiz:
            if not (0 <= row <= self.N and 0 <= col < self.M):
                return False
            if self.h[row][col]:
                return False
        else:
            if not (0 <= row < self.N and 0 <= col <= self.M):
                return False
            if self.v[row][col]:
                return False

        self._apply((horiz, col, row), 2)
        print(self)

        if not self._is_terminal():
            move = self._best_move()
            self._apply(move, 1)
            print("Computer plays:")
            print(self)

        return True

    def winner(self) -> int:
        """Return 1 (computer), 2 (player), or 0 (draw)."""
        if self.score[1] > self.score[2]:
            return 1
        if self.score[2] > self.score[1]:
            return 2
        return 0

    # ------------------------------------------------------------------ #
    #  Display                                                              #
    # ------------------------------------------------------------------ #

    def __str__(self) -> str:
        """
        Render the board.

        Each dot row looks like:
          o - o   o   o        (dots with/without horizontal edges)
        Between dot rows:
          | C |               (vertical edges with box labels)
        Vertical edges use | for the left side and | or space on the right.
        A box label is 'C' (computer), 'P' (player), or ' '.
        Horizontal edges connecting to a box use '-' on top and '_' on bottom.
        """
        lines = []

        for r in range(self.N + 1):
            # --- dot row ---
            dot_row = []
            for c in range(self.M + 1):
                dot_row.append('o')
                if c < self.M:
                    dot_row.append(' - ' if self.h[r][c] else '   ')
            lines.append(''.join(dot_row))

            # --- edge row between dot rows ---
            if r < self.N:
                edge_row = []
                for c in range(self.M + 1):
                    edge_row.append('|' if self.v[r][c] else ' ')
                    if c < self.M:
                        label = self.OWNER_CHAR[self.box[r][c]]
                        edge_row.append(f' {label} ')
                lines.append(''.join(edge_row))

        lines.append(f"Score — Computer: {self.score[1]}  Player: {self.score[2]}")
        lines.append('')
        return '\n'.join(lines)


# ------------------------------------------------------------------ #
#  Main                                                                #
# ------------------------------------------------------------------ #

def main():
    print("Enter N, M:")
    try:
        parts = input().split()
        n, m = int(parts[0]), int(parts[1])
    except (ValueError, IndexError, EOFError):
        print("Invalid input. Using default 3x3.")
        n, m = 3, 3

    if n < 1 or m < 1 or n > 6 or m > 6:
        print("Board size must be between 1 and 6. Using 3x3.")
        n, m = 3, 3

    print("Computer(1) or Player(2) is first?")
    try:
        choice = int(input().strip())
    except (ValueError, EOFError):
        choice = 2

    computer_first = (choice == 1)

    game = DotsAndBoxes(n, m, computer_first)
    print(game)

    total_boxes = n * m

    while game.score[1] + game.score[2] < total_boxes:
        print("Please enter your move:")
        print("  Format: <1=vertical/2=horizontal> <col> <row>  (1-indexed)")
        try:
            parts = input().split()
            if len(parts) < 3:
                raise ValueError
            orientation = int(parts[0])
            col = int(parts[1]) - 1   # convert to 0-indexed
            row = int(parts[2]) - 1
        except (ValueError, IndexError):
            print("Invalid input. Use: <1|0> <col> <row>")
            continue
        except EOFError:
            break

        horiz = (orientation == 2)

        if not game.player_move(horiz, col, row):
            print("Invalid move. Try again.")

    w = game.winner()
    if w == 1:
        print("Computer wins!")
    elif w == 2:
        print("You win!")
    else:
        print("Draw!")


if __name__ == '__main__':
    main()
