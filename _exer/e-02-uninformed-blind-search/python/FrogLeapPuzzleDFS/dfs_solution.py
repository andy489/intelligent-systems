import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from timed import timed


class DFSSolution:

    def __init__(self, n):
        # direction: -1 = right frog jumps left, +1 = left frog jumps right
        # distance:   1 = single jump,            2 = double jump
        self.jump_sequence = []
        self.jump_types = [(-1, 1), (-1, 2), (1, 1), (1, 2)]
        self.n = n
        self.board_size = self.n * 2 + 1
        self.board = [self._generate_frog(i) for i in range(self.board_size)]

    def _generate_frog(self, position):
        if position < self.n:
            return "L"
        if position > self.n:
            return "R"
        return "_"

    def _print_board(self):
        print("".join(self.board))

    def _is_goal_state(self, leaf_pos):
        mid = self.n
        if leaf_pos != mid:
            return False
        for i in range(mid):
            if self.board[i] != "R":
                return False
        for i in range(mid + 1, self.board_size):
            if self.board[i] != "L":
                return False
        return True

    def _in_range(self, pos):
        return 0 <= pos < self.board_size

    def _swap(self, i, j):
        self.board[i], self.board[j] = self.board[j], self.board[i]

    def _try_jump(self, leaf_pos, jump_type, depth):
        direction, distance = jump_type
        frog_pos = leaf_pos + direction * distance
        expected = "L" if direction == -1 else "R"

        if not self._in_range(frog_pos) or self.board[frog_pos] != expected:
            return False

        self._swap(leaf_pos, frog_pos)

        if self._dfs(frog_pos, depth + 1):
            self._swap(leaf_pos, frog_pos)
            self._print_board()
            self.jump_sequence.append(frog_pos + 1)
            return True

        self._swap(leaf_pos, frog_pos)
        return False

    def _dfs(self, leaf_pos, depth):
        if self._is_goal_state(leaf_pos):
            print(f"Goal state found at depth: {depth}")
            self._print_board()
            return True

        for jump_type in self.jump_types:
            if self._try_jump(leaf_pos, jump_type, depth):
                return True

        return False

    @timed(label="DFS Frog Leap Puzzle")
    def solve(self):
        self._dfs(self.n, 0)
        print("Jump Sequence: " + ", ".join(str(j) for j in self.jump_sequence))
        print(f"Total jumps: {len(self.jump_sequence)}\nSuccess!")
