import time


class DFSSolution:

    def __init__(self):
        # pair = {key=RIGHT_FROG/LEFT_FROG, value=SINGLE_JUMP/DOUBLE_JUMP}
        # -1 = right frog jumps left, 1 = left frog jumps right
        # 1 = single jump, 2 = double jump
        self.start_time = None
        self.jump_sequence = []
        self.jump_types = [(-1, 1), (-1, 2), (1, 1), (1, 2)]
        self.n = int(input("N: "))
        self.board_size = self.n * 2 + 1
        self.board = [self.generate_frog(i) for i in range(self.board_size)]

    def print_board(self):
        print("".join(self.board))

    def generate_frog(self, position):
        if position < self.n:
            return "L"
        if position > self.n:
            return "R"
        return "_"

    def is_goal_state(self, leaf_pos):
        mid_ind = self.n

        if mid_ind != leaf_pos:
            return False

        for i in range(mid_ind):
            if self.board[i] != "R":
                return False

        return True

    def in_range(self, pos):
        return 0 <= pos < self.board_size

    def swap(self, ind1, ind2):
        temp = self.board[ind1]
        self.board[ind1] = self.board[ind2]
        self.board[ind2] = temp

    def move(self, leaf_pos, frog_pos, depth):
        depth = depth + 1
        self.swap(leaf_pos, frog_pos)

        if self.dfs(frog_pos, depth):
            self.swap(leaf_pos, frog_pos)
            self.print_board()
            self.jump_sequence.append(frog_pos + 1)
            return True

        self.swap(leaf_pos, frog_pos)
        return False

    def shift_leaf(self, leaf_pos, jump_type, depth):

        frog_pos = leaf_pos + jump_type[0] * jump_type[1]

        if self.in_range(frog_pos) and self.board[frog_pos] == ("L" if jump_type[0] == -1 else "R"):
            return self.move(leaf_pos, frog_pos, depth)

        return False

    def dfs(self, leaf_pos, depth):
        if self.is_goal_state(leaf_pos):
            print(f"Goal State found at depth: {depth}")

            elapsed_time_sec = time.time() - self.start_time
            print(f"Solving puzzle took: {"{:.2f}".format(elapsed_time_sec * 1000)} µs ≈ "
                  f"{"{:.2f}".format(elapsed_time_sec)} s.")
            self.print_board()
            return True

        for jump_type in self.jump_types:
            if self.shift_leaf(leaf_pos, jump_type, depth):
                return True

        return False

    def solve(self):
        self.start_time = time.time()
        depth = 0
        self.dfs(self.n, depth)
        print(f"Jump Sequence: {", ".join(str(j) for j in self.jump_sequence)}")
        print(f"Total jumps: {len(self.jump_sequence)}\nSuccess!")
