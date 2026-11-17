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
// Incremental heuristic update (O(N) per move — MD + LC together)
// ---------------------------------------------------------------------------
// When the blank at `blank_pos` swaps with the tile `t` at `tile_pos`:
//   - MD changes by (new_dist - old_dist) for tile t only
//   - LC changes along the row and column that tile t moves INTO
//     (the row/col of blank_pos) and OUT OF (the row/col of tile_pos).
//
// LC delta for a row r: count of tiles in goal-row r that are now in conflict
// with tile t after the move, minus count that were in conflict before.

inline int lc_row_delta(int tile, int old_col, int new_col, int row) {
    // How many tiles in `row` conflict with `tile` after minus before the move.
    // We scan the row in the current board state (after the swap has happened
    // conceptually, but we do the scan before/after carefully in the caller).
    // Actually this is called with the board already swapped, so we compute
    // conflicts of `tile` (now at new_col) vs every other tile in `row`.
    int delta = 0;
    int gc_tile = goal_col[tile];
    for (int c = 0; c < grid_size; ++c) {
        if (c == new_col) continue;
        int other = board[row * grid_size + c];
        if (other == BLANK_TILE_ID || goal_row[other] != row) continue;
        int gc_other = goal_col[other];
        // After move: conflict if (tile is left of other but goal says tile right of other) or vice versa
        bool conflict_after  = (new_col < c) ? (gc_tile > gc_other) : (gc_tile < gc_other);
        // Before move: tile was at old_col
        bool conflict_before = (old_col < c) ? (gc_tile > gc_other) : (gc_tile < gc_other);
        delta += (conflict_after ? 2 : 0) - (conflict_before ? 2 : 0);
    }
    return delta;
}

inline int lc_col_delta(int tile, int old_row, int new_row, int col) {
    int delta = 0;
    int gr_tile = goal_row[tile];
    for (int r = 0; r < grid_size; ++r) {
        if (r == new_row) continue;
        int other = board[r * grid_size + col];
        if (other == BLANK_TILE_ID || goal_col[other] != col) continue;
        int gr_other = goal_row[other];
        bool conflict_after  = (new_row < r) ? (gr_tile > gr_other) : (gr_tile < gr_other);
        bool conflict_before = (old_row < r) ? (gr_tile > gr_other) : (gr_tile < gr_other);
        delta += (conflict_after ? 2 : 0) - (conflict_before ? 2 : 0);
    }
    return delta;
}

// Returns updated h = md + lc after moving blank from blank_pos to np.
// The board is NOT swapped yet when this is called.
inline int update_heuristic(int blank_pos, int np, int h) {
    int t = board[np]; // tile sliding into blank_pos
    int g = grid_size;

    // Manhattan delta
    int old_dist = abs(np / g - goal_row[t]) + abs(np % g - goal_col[t]);
    int new_dist = abs(blank_pos / g - goal_row[t]) + abs(blank_pos % g - goal_col[t]);
    int md_delta = new_dist - old_dist;

    // LC delta — tile t moves from (np/g, np%g) to (blank_pos/g, blank_pos%g).
    int old_row = np / g,        new_row = blank_pos / g;
    int old_col = np % g,        new_col = blank_pos % g;

    int lc_delta = 0;

    // Row component: only relevant if tile t's goal row is one of the two rows.
    // We must evaluate BEFORE the swap (board still has original values).
    if (goal_row[t] == old_row) {
        // Tile is leaving old_row — count conflicts it had there (subtract)
        int gc_tile = goal_col[t];
        for (int c = 0; c < g; ++c) {
            if (c == old_col) continue;
            int other = board[old_row * g + c];
            if (other == BLANK_TILE_ID || goal_row[other] != old_row) continue;
            int gc_other = goal_col[other];
            bool had_conflict = (old_col < c) ? (gc_tile > gc_other) : (gc_tile < gc_other);
            if (had_conflict) lc_delta -= 2;
        }
    }
    if (goal_row[t] == new_row && new_row != old_row) {
        // Tile is entering new_row — count conflicts it will have there (add).
        // Scan board in new_row, excluding old position of tile t (which moves away)
        // and blank_pos (which will become the tile t's new spot — currently blank).
        int gc_tile = goal_col[t];
        for (int c = 0; c < g; ++c) {
            if (c == new_col) continue; // that's where tile t lands
            int pos = new_row * g + c;
            int other = (pos == np) ? BLANK_TILE_ID : board[pos]; // np will be vacated
            if (other == BLANK_TILE_ID || goal_row[other] != new_row) continue;
            int gc_other = goal_col[other];
            bool will_conflict = (new_col < c) ? (gc_tile > gc_other) : (gc_tile < gc_other);
            if (will_conflict) lc_delta += 2;
        }
    }

    // Column component
    if (goal_col[t] == old_col) {
        int gr_tile = goal_row[t];
        for (int r = 0; r < g; ++r) {
            if (r == old_row) continue;
            int other = board[r * g + old_col];
            if (other == BLANK_TILE_ID || goal_col[other] != old_col) continue;
            int gr_other = goal_row[other];
            bool had_conflict = (old_row < r) ? (gr_tile > gr_other) : (gr_tile < gr_other);
            if (had_conflict) lc_delta -= 2;
        }
    }
    if (goal_col[t] == new_col && new_col != old_col) {
        int gr_tile = goal_row[t];
        for (int r = 0; r < g; ++r) {
            if (r == new_row) continue;
            int pos = r * g + new_col;
            int other = (pos == np) ? BLANK_TILE_ID : board[pos];
            if (other == BLANK_TILE_ID || goal_col[other] != new_col) continue;
            int gr_other = goal_row[other];
            bool will_conflict = (new_row < r) ? (gr_tile > gr_other) : (gr_tile < gr_other);
            if (will_conflict) lc_delta += 2;
        }
    }

    return h + md_delta + lc_delta;
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

int dls(int blank_pos, int limit, Move prev_move, int h, int depth) {
    int cost = h + depth;
    if (cost > limit) return cost;

    if (h == 0) {
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

        int new_h = update_heuristic(blank_pos, np, h);

        board[blank_pos] = board[np];
        board[np] = BLANK_TILE_ID;

        int result = dls(np, limit, move, new_h, depth + 1);

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

void ida_star(int h_init) {
    int threshold = h_init;
    int result;
    do {
        result = dls(blank_curr, threshold, NEUTRAL, h_init, 0);
        if (result == 0) return;
        threshold = result;
        h_init = calc_manhattan() + calc_linear_conflict();
        threshold = max(threshold, h_init);
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

    int h = calc_manhattan() + calc_linear_conflict();

    if (h == 0) {
        printf("0\n\nSuccess!\n");
        return 0;
    }

    if (!check_solvable()) {
        printf("Puzzle cannot be solved!\n");
        return 0;
    }

    auto t0 = high_resolution_clock::now();
    ida_star(h);
    auto t1 = high_resolution_clock::now();

    display_solution_moves();

    auto ms = duration_cast<milliseconds>(t1 - t0).count();
    printf("\n%s\n\nSuccess!\n", format_duration(ms).c_str());

    return 0;
}
