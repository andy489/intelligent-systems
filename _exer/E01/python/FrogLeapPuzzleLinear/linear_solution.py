def init_board():
    size = n * 2 + 1
    i = 0
    while i < n:
        board.append("L")
        i = i + 1
    board.append("_")
    i = i + 1
    while i < size:
        board.append("R")
        i = i + 1


def print_state(arr):
    print("".join(arr))


def swap(from_ind, to_ind):
    temp = board[from_ind]
    board[from_ind] = board[to_ind]
    board[to_ind] = temp


def jumps(leaf_pos, i, j):
    if i % 2 == 0:
        if j < i:
            swap(leaf_pos, leaf_pos - 2)
            leaf_pos = leaf_pos - 2
        else:
            swap(leaf_pos, leaf_pos - 1)
            leaf_pos = leaf_pos - 1
    else:
        if j < i:
            swap(leaf_pos, leaf_pos + 2)
            leaf_pos = leaf_pos + 2
        else:
            swap(leaf_pos, leaf_pos + 1)
            leaf_pos = leaf_pos + 1
    jump_seq.append(leaf_pos + 1)
    print_state(board)
    return leaf_pos

def linear_solution():
    print_state(board)

    leaf_pos = n

    for i in range(n):
        for j in range(i + 1):
            leaf_pos = jumps(leaf_pos, i, j)

    for i in range(n):
        if n % 2 == 0:
            swap(leaf_pos, leaf_pos - 2)
            leaf_pos = leaf_pos - 2
        else:
            swap(leaf_pos, leaf_pos + 2)
            leaf_pos = leaf_pos + 2
            print_state(board)
        jump_seq.append(leaf_pos + 1)

    for i in range(n - 1, -1, -1):
        for j in range(i, -1, -1):
            leaf_pos = jumps(leaf_pos, i, j)

n = int(input("N: "))
board = []
jump_seq = []
init_board()
linear_solution()
print(f"Jump sequence: {", ".join(str(j) for j in jump_seq)}")
print(f"Total jumps: {len(jump_seq)}")
print("Success!")
