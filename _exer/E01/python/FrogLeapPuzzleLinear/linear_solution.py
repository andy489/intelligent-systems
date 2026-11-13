def init_board(n):
    board = []
    for _ in range(n):
        board.append("L")
    board.append("_")
    for _ in range(n):
        board.append("R")
    return board


def print_state(arr):
    print("".join(arr))


def swap(board, from_ind, to_ind):
    board[from_ind], board[to_ind] = board[to_ind], board[from_ind]


def jump(board, leaf_pos, direction):
    swap(board, leaf_pos, leaf_pos + direction)
    return leaf_pos + direction


def jumps_first(board, leaf_pos, jump_sequence, i, double_dir, single_dir):
    for _ in range(i - 1):
        leaf_pos = jump(board, leaf_pos, double_dir)
        print_state(board)
        jump_sequence.append(leaf_pos + 1)
    leaf_pos = jump(board, leaf_pos, single_dir)
    print_state(board)
    jump_sequence.append(leaf_pos + 1)
    return leaf_pos


def jumps_second(board, leaf_pos, jump_sequence, i, single_dir, double_dir):
    leaf_pos = jump(board, leaf_pos, single_dir)
    print_state(board)
    jump_sequence.append(leaf_pos + 1)
    for _ in range(i - 1):
        leaf_pos = jump(board, leaf_pos, double_dir)
        print_state(board)
        jump_sequence.append(leaf_pos + 1)
    return leaf_pos


def process_first_half(board, n, leaf_pos, jump_sequence):
    for i in range(1, n + 1):
        if i % 2 == 0:
            leaf_pos = jumps_first(board, leaf_pos, jump_sequence, i, -2, -1)
        else:
            leaf_pos = jumps_first(board, leaf_pos, jump_sequence, i, 2, 1)
    return leaf_pos


def process_middle(board, n, leaf_pos, jump_sequence):
    direction = 2 if n % 2 == 0 else -2
    for _ in range(n):
        leaf_pos = jump(board, leaf_pos, direction)
        print_state(board)
        jump_sequence.append(leaf_pos + 1)
    return leaf_pos


def process_second_half(board, n, leaf_pos, jump_sequence):
    for i in range(n, 0, -1):
        if i % 2 == 0:
            leaf_pos = jumps_second(board, leaf_pos, jump_sequence, i, -1, -2)
        else:
            leaf_pos = jumps_second(board, leaf_pos, jump_sequence, i, 1, 2)
    return leaf_pos


def linear_solution(board, n, jump_sequence):
    print_state(board)
    leaf_pos = process_first_half(board, n, n, jump_sequence)
    leaf_pos = process_middle(board, n, leaf_pos, jump_sequence)
    process_second_half(board, n, leaf_pos, jump_sequence)


n = int(input("N: "))
board = init_board(n)
jump_sequence = []
linear_solution(board, n, jump_sequence)
print("Jump sequence: " + ", ".join(str(j) for j in jump_sequence))
print(f"Total jumps: {len(jump_sequence)}")
print("Success!")
