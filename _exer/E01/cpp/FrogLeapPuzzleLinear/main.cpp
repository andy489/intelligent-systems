#include <cstdio>
#include <vector>
#include <list>

using namespace std;

static const int LEFT_SINGLE = 1, RIGHT_SINGLE = -1, LEFT_DOUBLE = 2, RIGHT_DOUBLE = -2;

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

void print_state(vector<char> &board) {
    int SIZE = (int) board.size();

    for (int i = 0; i < SIZE; i++) {
        printf("%c", board[i]);
    }

    printf("\n");
}

void print_jump_sequence(const list<int>& ll) {
    for (auto it = ll.begin(); it != ll.end(); ++it) {
        auto next = it;
        ++next;
        if (next != ll.end()) {
            printf("%d, ", *it);
        } else {
            printf("%d\n", *it);
        }
    }

    if (ll.empty()) {
        printf("\n");
    }
}

void jump(vector<char> &board, int &leaf_pos, int direction) {
    swap(board[leaf_pos], board[leaf_pos + direction]);
    leaf_pos += direction;
}

void process_first_half(vector<char> &board, int N, int &leaf_pos, list<int> &jump_sequence) {
    for (int i = 1; i <= N; i++) {
        if (i % 2 == 0) {
            for (int j = 0; j < i - 1; j++) {
                jump(board, leaf_pos, LEFT_DOUBLE);
                print_state(board);
                jump_sequence.push_back(leaf_pos + 1);
            }

            jump(board, leaf_pos, LEFT_SINGLE);
            print_state(board);
            jump_sequence.push_back(leaf_pos + 1);

        } else {
            for (int j = 0; j < i - 1; j++) {
                jump(board, leaf_pos, RIGHT_DOUBLE);
                print_state(board);
                jump_sequence.push_back(leaf_pos + 1);
            }

            jump(board, leaf_pos, RIGHT_SINGLE);
            print_state(board);
            jump_sequence.push_back(leaf_pos + 1);

        }
    }
}

void process_middle(vector<char> &board, int N, int &leaf_pos, list<int> &jump_sequence) {
    if (N % 2 == 0) {
        for (int j = 0; j < N; j++) {
            jump(board, leaf_pos, RIGHT_DOUBLE);
            print_state(board);
            jump_sequence.push_back(leaf_pos + 1);
        }
    } else {
        for (int j = 0; j < N; j++) {
            jump(board, leaf_pos, LEFT_DOUBLE);
            print_state(board);
            jump_sequence.push_back(leaf_pos + 1);
        }
    }
}

void process_second_half(vector<char> &board, int N, int &leaf_pos, list<int> &jump_sequence) {
    for (int i = N; i > 0; i--) {
        if (i % 2 == 0) {
            jump(board, leaf_pos, LEFT_SINGLE);
            print_state(board);
            jump_sequence.push_back(leaf_pos + 1);

            for (int j = 1; j < i; j++) {
                jump(board, leaf_pos, LEFT_DOUBLE);
                print_state(board);
                jump_sequence.push_back(leaf_pos + 1);
            }
        } else {
            jump(board, leaf_pos, RIGHT_SINGLE);
            print_state(board);
            jump_sequence.push_back(leaf_pos + 1);

            for (int j = 1; j < i; j++) {
                jump(board, leaf_pos, RIGHT_DOUBLE);
                print_state(board);
                jump_sequence.push_back(leaf_pos + 1);
            }
        }
    }
}

void linear_solution(vector<char> &board, int N, int &leaf_pos, list<int> &jump_sequence) {
    print_state(board);
    process_first_half(board, N, leaf_pos, jump_sequence);
    process_middle(board, N, leaf_pos, jump_sequence);
    process_second_half(board, N, leaf_pos, jump_sequence);
}

int main() {
    int N;
    printf("N: ");
    scanf("%d", &N);

    vector<char> board;

    init_board(board, N);

    list<int> jump_sequence;
    int leaf_pos = N;
    linear_solution(board, N, leaf_pos, jump_sequence);
    printf("Jump sequence: ");
    print_jump_sequence(jump_sequence);
    printf("Total jumps: %zu\nSuccess!", jump_sequence.size());

    return 0;
}
