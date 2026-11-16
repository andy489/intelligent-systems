#include <cstdio>
#include <chrono>
#include <vector>
#include <list>

using namespace std;
using namespace std::chrono;

typedef pair<int, int> pii;

// pair = {key=RIGHT_FROG/LEFT_FROG, value=SINGLE_JUMP/DOUBLE_JUMP}
static const vector<pii> JUMP_TYPES = {{-1, 1},
                                       {-1, 2},
                                       {1,  1},
                                       {1,  2}};

time_point<steady_clock> start;

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

bool is_goal_state(vector<char> &board, int leaf_pos) {
    int mid_ind = (int) board.size() / 2;

    if (leaf_pos != mid_ind) {
        return false;
    }

    for (int i = 0; i < mid_ind; ++i) {
        if (board[i] != 'R') {
            return false;
        }
    }

    for (int i = mid_ind + 1; i < (int) board.size(); ++i) {
        if (board[i] != 'L') {
            return false;
        }
    }

    return true;
}

bool in_range(vector<char> &board, int pos) {
    return 0 <= pos && pos < (int) board.size();
}

bool move(vector<char> &, int, int, list<int> &, int);

bool shift_leaf(vector<char> &board, int leaf_pos, pii jump_type, list<int> &jump_sequence, int depth) {
    int frog_pos = leaf_pos + jump_type.first * jump_type.second;

    if (in_range(board, frog_pos) && board[frog_pos] == (jump_type.first == -1 ? 'L' : 'R')) {
        return move(board, leaf_pos, frog_pos, jump_sequence, depth);
    }

    return false;
}

auto get_time() {
    return steady_clock::now();
}

void print(vector<char> &board) {
    int SIZE = (int) board.size();

    for (int i = 0; i < SIZE; ++i) {
        printf("%c", board[i]);
    }

    printf("\n");
}

void print(list<int>& ll) {
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

bool dfs(vector<char> &board, int leaf_pos, list<int> &jump_sequence, int depth) {
    if (is_goal_state(board, leaf_pos)) {
        printf("Goal State found at depth: %d\n", depth);

        auto end = get_time();
        printf("Solving puzzle took: %lld ms ≈ %lld s.\n", duration_cast<milliseconds>(end - start).count(),
               (end - start) / 1s);

        print(board);
        return true;
    }

    for (const auto& jump_type : JUMP_TYPES) {
        if (shift_leaf(board, leaf_pos, jump_type, jump_sequence, depth)) {
            return true;
        }
    }

    return false;
}

bool move(vector<char> &board, int leaf_pos, int frog_pos, list<int> &jump_sequence, int depth) {
    ++depth;
    swap(board[leaf_pos], board[frog_pos]);

    if (dfs(board, frog_pos, jump_sequence, depth)) {
        swap(board[leaf_pos], board[frog_pos]);
        print(board);
        jump_sequence.push_front(frog_pos + 1);
        return true;
    }

    swap(board[leaf_pos], board[frog_pos]);

    return false;
}

void dfs(int N) {
    vector<char> board;
    init_board(board, N);
    list<int> linked_list;

    int depth(0);
    start = get_time();
    dfs(board, N, linked_list, depth);
    printf("Jump Sequence: ");
    print(linked_list);
    printf("Total jumps: %zu\nSuccess!\n", linked_list.size());
}

int main() {
    int N;
    printf("N: ");
    scanf("%d", &N);
    if (N <= 0) {
        printf("N must be a positive integer.\n");
        return 1;
    }
    dfs(N);
    return 0;
}