#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#include <stack>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

static const int MAX_DIM   = 10;
static const int MAX_TILES = MAX_DIM * MAX_DIM;

int grid_size;
int tiles_count;
int blank_tile_target_index;
int blank_tile_target_pos_linear;

int board[MAX_TILES];
int goal_row[MAX_TILES];
int goal_col[MAX_TILES];

int blank_curr;
int blank_target;

const int BLANK_TILE_ID = 0;

enum Move { LEFT = 0, RIGHT, DOWN, UP, NEUTRAL };

int offset[5];

void init_offsets() {
    offset[LEFT]    = -1;
    offset[RIGHT]   = +1;
    offset[DOWN]    = +grid_size;
    offset[UP]      = -grid_size;
    offset[NEUTRAL] = 0;
}

const int NEIGHBOURS_COUNT = 4;
const int MAX_HEURISTIC = 1 << 15;

stack<Move, vector<Move>> shifts_history;

// ---------------------------------------------------------------------------
// Goal state setup
// ---------------------------------------------------------------------------

void set_goal_state() {
    blank_target = blank_tile_target_pos_linear;
    for (int t = 1; t <= tiles_count; ++t) {
        int pos = (t <= blank_target) ? (t - 1) : t;
        goal_row[t] = pos / grid_size;
        goal_col[t] = pos % grid_size;
    }
    goal_row[BLANK_TILE_ID] = blank_target / grid_size;
    goal_col[BLANK_TILE_ID] = blank_target % grid_size;
}

// ---------------------------------------------------------------------------
// Input
// ---------------------------------------------------------------------------

void read_from_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "Error opening file: %s\n", path); exit(1); }

    int n, target_index;
    fscanf(f, "%d %d", &n, &target_index);

    grid_size   = n;
    tiles_count = n * n - 1;
    blank_tile_target_index = (target_index < 0) ? tiles_count : target_index;
    blank_tile_target_pos_linear = blank_tile_target_index;

    for (int i = 0; i < n * n; ++i) {
        fscanf(f, "%d", &board[i]);
        if (board[i] == BLANK_TILE_ID) blank_curr = i;
    }
    fclose(f);
    init_offsets();
    set_goal_state();
}

void read_from_console() {
    int target_index;
    scanf("%d %d", &grid_size, &target_index);

    tiles_count = grid_size * grid_size - 1;
    blank_tile_target_index = (target_index < 0) ? tiles_count : target_index;
    blank_tile_target_pos_linear = blank_tile_target_index;

    for (int i = 0; i < grid_size * grid_size; ++i) {
        scanf("%d", &board[i]);
        if (board[i] == BLANK_TILE_ID) blank_curr = i;
    }
    init_offsets();
    set_goal_state();
}

// ---------------------------------------------------------------------------
// Solvability check
// ---------------------------------------------------------------------------

static size_t merge_count(int *arr, int *tmp, int n) {
    if (n < 2) return 0;
    int mid = n / 2;
    size_t inv = merge_count(arr, tmp, mid) + merge_count(arr + mid, tmp + mid, n - mid);
    int i = 0, j = mid, k = 0;
    while (i < mid && j < n) {
        if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
        else                  { tmp[k++] = arr[j++]; inv += (size_t)(mid - i); }
    }
    while (i < mid) tmp[k++] = arr[i++];
    while (j < n)   tmp[k++] = arr[j++];
    memcpy(arr, tmp, n * sizeof(int));
    return inv;
}

bool check_solvable() {
    int flat[MAX_TILES], tmp[MAX_TILES];
    int p = 0;
    for (int i = 0; i < grid_size * grid_size; ++i)
        if (board[i] != BLANK_TILE_ID) flat[p++] = board[i];
    size_t inv = merge_count(flat, tmp, tiles_count);
    if (grid_size % 2 == 1) return inv % 2 == 0;
    int blank_row_from_bottom  = (grid_size - 1) - (blank_curr   / grid_size);
    int target_row_from_bottom = (grid_size - 1) - (blank_target / grid_size);
    return (inv + (size_t)blank_row_from_bottom + (size_t)target_row_from_bottom) % 2 == 0;
}

// ---------------------------------------------------------------------------
// Heuristic: Manhattan distance
// ---------------------------------------------------------------------------

int calc_manhattan() {
    int cost = 0;
    for (int i = 0; i < grid_size * grid_size; ++i) {
        int t = board[i];
        if (t == BLANK_TILE_ID) continue;
        cost += abs(i / grid_size - goal_row[t]) + abs(i % grid_size - goal_col[t]);
    }
    return cost;
}

// ---------------------------------------------------------------------------
// Linear Conflict — full recompute (used once per IDA* iteration at root)
// ---------------------------------------------------------------------------

int calc_linear_conflict() {
    int conflict = 0;
    for (int row = 0; row < grid_size; ++row) {
        for (int c1 = 0; c1 < grid_size - 1; ++c1) {
            int t1 = board[row * grid_size + c1];
            if (t1 == BLANK_TILE_ID || goal_row[t1] != row) continue;
            for (int c2 = c1 + 1; c2 < grid_size; ++c2) {
                int t2 = board[row * grid_size + c2];
                if (t2 == BLANK_TILE_ID || goal_row[t2] != row) continue;
                if (goal_col[t1] > goal_col[t2]) conflict += 2;
            }
        }
    }
    for (int col = 0; col < grid_size; ++col) {
        for (int r1 = 0; r1 < grid_size - 1; ++r1) {
            int t1 = board[r1 * grid_size + col];
            if (t1 == BLANK_TILE_ID || goal_col[t1] != col) continue;
            for (int r2 = r1 + 1; r2 < grid_size; ++r2) {
                int t2 = board[r2 * grid_size + col];
                if (t2 == BLANK_TILE_ID || goal_col[t2] != col) continue;
                if (goal_row[t1] > goal_row[t2]) conflict += 2;
            }
        }
    }
    return conflict;
}

// ---------------------------------------------------------------------------
// Incremental MD update (O(1), called BEFORE the swap)
// ---------------------------------------------------------------------------
inline int update_md(int blank_pos, int np, int md) {
    int t = board[np];
    int g = grid_size;
    int old_dist = abs(np / g - goal_row[t]) + abs(np % g - goal_col[t]);
    int new_dist = abs(blank_pos / g - goal_row[t]) + abs(blank_pos % g - goal_col[t]);
    return md - old_dist + new_dist;
}

// ---------------------------------------------------------------------------
// Move validation
// ---------------------------------------------------------------------------

inline int neighbour_pos(int blank_pos, Move move) {
    int np = blank_pos + offset[move];
    if (np < 0 || np >= grid_size * grid_size) return -1;
    if (move == LEFT  && blank_pos % grid_size == 0)             return -1;
    if (move == RIGHT && blank_pos % grid_size == grid_size - 1) return -1;
    return np;
}

// ---------------------------------------------------------------------------
// IDA* search
// ---------------------------------------------------------------------------

static Move reverse_move(Move m) {
    switch (m) {
        case UP:    return DOWN;
        case DOWN:  return UP;
        case LEFT:  return RIGHT;
        case RIGHT: return LEFT;
        default:    return NEUTRAL;
    }
}

// h = md + lc (full heuristic), md passed separately for incremental update
int dls(int blank_pos, int limit, Move prev_move, int h, int md, int depth) {
    int cost = h + depth;
    if (cost > limit) return cost;

    if (md == 0) {  // solved: all tiles at goal positions
        shifts_history.push(prev_move);
        printf("%d\n", depth);
        return 0;
    }

    Move opposite = reverse_move(prev_move);
    int min_cost = MAX_HEURISTIC;

    for (int i = 0; i < NEIGHBOURS_COUNT; ++i) {
        Move move = static_cast<Move>(i);
        if (move == opposite) continue;

        int np = neighbour_pos(blank_pos, move);
        if (np < 0) continue;

        int new_md = update_md(blank_pos, np, md);  // O(1), before swap

        board[blank_pos] = board[np];
        board[np] = BLANK_TILE_ID;

        int new_h = new_md + calc_linear_conflict();  // LC recomputed after swap
        int result = dls(np, limit, move, new_h, new_md, depth + 1);

        board[np] = board[blank_pos];
        board[blank_pos] = BLANK_TILE_ID;

        if (result == 0) {
            shifts_history.push(prev_move);
            return 0;
        }
        if (result < min_cost) min_cost = result;
    }

    return min_cost;
}

void ida_star(int md, int lc) {
    int threshold = md;  // start with MD only, like colleague's bound = manhattan
    int result;
    do {
        result = dls(blank_curr, threshold, NEUTRAL, md + lc, md, 0);
        if (result == 0) return;
        threshold = result;
        md = calc_manhattan();
        lc = calc_linear_conflict();
        threshold = max(threshold, md + lc);
    } while (result != 0);
}

// ---------------------------------------------------------------------------
// Output
// ---------------------------------------------------------------------------

static int count_digits(int n) {
    return (n == 0) ? 1 : (int)floor(log10((double)n) + 1);
}

void display_solution_moves() {
    while (!shifts_history.empty()) {
        switch (shifts_history.top()) {
            case UP:    printf("up\n");    break;
            case RIGHT: printf("right\n"); break;
            case DOWN:  printf("down\n");  break;
            case LEFT:  printf("left\n");  break;
            default: break;
        }
        shifts_history.pop();
    }
}

string format_duration(long long ms) {
    long long s   = ms / 1000; ms %= 1000;
    long long min = s  / 60;   s  %= 60;
    long long h   = min / 60;  min %= 60;
    char buf[64];
    snprintf(buf, sizeof(buf), "Elapsed: %02lld:%02lld:%02lld.%03lld", h, min, s, ms);
    return buf;
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    if (argc > 1) read_from_file(argv[1]);
    else          read_from_console();

    int md = calc_manhattan();
    int lc = calc_linear_conflict();

    if (md + lc == 0) {
        printf("0\n\nSuccess!\n");
        return 0;
    }

    if (!check_solvable()) {
        printf("Puzzle cannot be solved!\n");
        return 0;
    }

    auto t0 = high_resolution_clock::now();
    ida_star(md, lc);
    auto t1 = high_resolution_clock::now();

    display_solution_moves();

    auto ms = duration_cast<milliseconds>(t1 - t0).count();
    printf("\n%s\n\nSuccess!\n", format_duration(ms).c_str());

    return 0;
}
