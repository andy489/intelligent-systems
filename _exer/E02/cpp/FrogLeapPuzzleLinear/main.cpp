#include <cstdio>
#include <vector>
#include <list>

using namespace std;

void init_board(vector<char> &board, int N) {
    int SIZE = N * 2 + 1;
    board.resize(SIZE);

    int i(0);

    while (i < N) {
        board[i++] = 'L';
    }

    board[i++] = '_';

    while (i < SIZE) {
        board[i++] = 'R';
    }
}

void print(vector<char> &board) {
    int SIZE = (int) board.size();

    for (int i = SIZE - 1; i >= 0; --i) {
        printf("%c", board[i]);
    }

    printf("\n");
}

void print(list<int> ll) {
    int SIZE = ll.size();

    for (int i = 0; i < SIZE - 1; i++) {
        printf("%d, ", ll.front());
        ll.pop_front();
    }

    if (!ll.empty()) {
        printf("%d\n", ll.front());
        ll.pop_front();
    } else {
        printf("\n");
    }
}

void swap(vector<char> &board, int from_ind, int to_ind) {
    char temp = board[from_ind];
    board[from_ind] = board[to_ind];
    board[to_ind] = temp;
}

int jumps(vector<char> &board, int leaf_pos, int i, int j, list<int> &jump_sequence) {
    if (i % 2 == 0) { // jump left
        if (j < i) {
            swap(board, leaf_pos, leaf_pos - 2);
            leaf_pos = leaf_pos - 2;
        } else {
            swap(board, leaf_pos, leaf_pos - 1);
            leaf_pos = leaf_pos - 1;
        }
    } else { // jump right
        if (j < i) {
            swap(board, leaf_pos, leaf_pos + 2);
            leaf_pos = leaf_pos + 2;
        } else {
            swap(board, leaf_pos, leaf_pos + 1);
            leaf_pos = leaf_pos + 1;
        }
    }

    jump_sequence.push_back(leaf_pos + 1);
    print(board);
    return leaf_pos;
}

void linear_solution(vector<char> &board, int N, list<int> &jump_sequence) {
    print(board);

    int leaf_pos = N;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < i + 1; j++) {
            leaf_pos = jumps(board, leaf_pos, i, j, jump_sequence);
        }
    }

    for (int i = 0; i < N; i++) { // jump left or right
        if (N % 2 == 0) { // jump left if n is even
            swap(board, leaf_pos, leaf_pos - 2);
            leaf_pos = leaf_pos - 2;
        } else { // jump right if n is odd
            swap(board, leaf_pos, leaf_pos + 2);
            leaf_pos = leaf_pos + 2;
        }

        jump_sequence.push_back(leaf_pos + 1);
        print(board);
    }

    for (int i = N - 1; i >= 0; i--) {
        for (int j = i; j >= 0; j--) {
            leaf_pos = jumps(board, leaf_pos, i, j, jump_sequence);
        }
    }
}

int main() {

    int N;
    printf("N: ");
    scanf("%d", &N);

    vector<char> board;
    init_board(board, N);

    list<int> jump_sequence;
    linear_solution(board, board.size() / 2, jump_sequence);
    print(jump_sequence);
    printf("Total jumps: %d\nSuccess!", jump_sequence.size());

    return 0;
}
