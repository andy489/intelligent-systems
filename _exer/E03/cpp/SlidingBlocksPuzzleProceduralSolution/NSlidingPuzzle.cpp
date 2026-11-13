#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#include <stack>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Maximum supported grid dimension (10x10 = 100 tiles).
static const int MAX_DIM  = 10;
static const int MAX_TILES = MAX_DIM * MAX_DIM;

int grid_size;
int tiles_count;
int blank_tile_target_index;
int blank_tile_target_pos_linear;

// Flat board: index = row * grid_size + col.
int board[MAX_TILES];

// goal_row[tile_id] and goal_col[tile_id] — separate arrays avoid pair indirection.
int goal_row[MAX_TILES];
int goal_col[MAX_TILES];

int blank_curr;   // linear index of blank tile in current state
int blank_target; // linear index of blank tile in goal state

const int BLANK_TILE_ID = 0;

// Direction encoding (clockwise, same semantics as before):
//   LEFT  — blank moves left  (tile to its left  slides right into blank)
//   RIGHT — blank moves right (tile to its right slides left  into blank)
//   DOWN  — blank moves down  (tile below        slides up   into blank)
//   UP    — blank moves up    (tile above        slides down into blank)
enum Move { LEFT = 0, RIGHT, DOWN, UP, NEUTRAL };

// Runtime-initialised offsets (grid_size not constexpr).
int offset[5];

void init_offsets() {
    offset[LEFT]    = -1;
    offset[RIGHT]   = +1;
    offset[DOWN]    = +grid_size;
    offset[UP]      = -grid_size;
    offset[NEUTRAL] = 0;
}

const int NEIGHBOURS_COUNT = 4;
const int MAX_HEURISTIC = 1 << 15; // 32768, well above any real cost

stack<Move, vector<Move>> shifts_history;

// ---------------------------------------------------------------------------
// Goal state setup
// ---------------------------------------------------------------------------

void set_goal_state() {
    blank_target = blank_tile_target_pos_linear;

    // Build goal_row / goal_col lookup tables.
    // goal_state tiles are numbered 1..tiles_count left-to-right, top-to-bottom,
    // with the blank at blank_target.
    // Tile at linear position p (in goal order, skipping blank):
    //   tile_id = p < blank_target ? p + 1 : p
    // So goal position of tile t is:
    //   linear_pos = t <= blank_target ? t - 1 : t
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

void read_from_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Error opening file: %s\n", path);
        exit(1);
    }

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

// ---------------------------------------------------------------------------
// Solvability check
// ---------------------------------------------------------------------------

static size_t merge_count(int *arr, int *tmp, int n) {
    if (n < 2) return 0;
    int mid = n / 2;
    size_t inv = merge_count(arr, tmp, mid) + merge_count(arr + mid, tmp + mid, n - mid);

    int i = 0, j = mid, k = 0;
    while (i < mid && j < n) {
        if (arr[i] <= arr[j]) { tmp[k++] = arr[i++]; }
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
    int blank_row = blank_curr / grid_size;

    for (int i = 0; i < grid_size * grid_size; ++i) {
        if (board[i] != BLANK_TILE_ID) flat[p++] = board[i];
    }

    size_t inv = merge_count(flat, tmp, tiles_count);

    if (grid_size % 2 == 1) return inv % 2 == 0;
    return ((size_t) blank_row + inv) % 2 == 1;
}

// ---------------------------------------------------------------------------
// Heuristic: Manhattan distance + Linear Conflict
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

// Linear conflict: two tiles in the same row/column, both already in their
// goal row/column, but in reversed order — they need at least 2 extra moves.
int calc_linear_conflict() {
    int conflict = 0;

    // Row conflicts
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

    // Column conflicts
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
// Incremental Manhattan update (O(1) per move)
// ---------------------------------------------------------------------------

// When the blank at `blank_pos` swaps with the tile at `from_pos` (the tile
// slides into the blank), the heuristic changes only for that one tile.
inline int update_heuristic(int blank_pos, int from_pos, int h) {
    int t = board[from_pos]; // tile about to move into blank_pos
    if (t == BLANK_TILE_ID) return h; // shouldn't happen

    int gr = goal_row[t], gc = goal_col[t];

    int old_dist = abs(from_pos / grid_size - gr) + abs(from_pos % grid_size - gc);
    int new_dist = abs(blank_pos / grid_size - gr) + abs(blank_pos % grid_size - gc);

    return h - old_dist + new_dist;
}

// ---------------------------------------------------------------------------
// Move validation
// ---------------------------------------------------------------------------

// Returns the linear index the blank would move to, or -1 if invalid.
inline int neighbour_pos(int blank_pos, Move move) {
    int np = blank_pos + offset[move];
    if (np < 0 || np >= grid_size * grid_size) return -1;
    // Prevent wrapping across rows for LEFT/RIGHT
    if (move == LEFT  && blank_pos % grid_size == 0)              return -1;
    if (move == RIGHT && blank_pos % grid_size == grid_size - 1)  return -1;
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

// Returns 0 on success, or the minimum exceeded cost (next threshold).
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

        // Swap
        board[blank_pos] = board[np];
        board[np] = BLANK_TILE_ID;

        int result = dls(np, limit, move, new_h, depth + 1);

        // Undo swap
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

// IDA* with Manhattan + Linear Conflict.
// Linear Conflict is computed only at the root of each iteration to set the
// initial threshold — it is admissible and still tightens every threshold.
// Inside dls, only Manhattan distance is tracked incrementally (O(1) per move).
// This is correct because the incremental h is still admissible (pure MD ≤ true cost).
void ida_star(int md_init, int lc_init) {
    int threshold = md_init + lc_init;
    int result;

    do {
        // Pass pure Manhattan as the running heuristic into dls so the
        // incremental update stays consistent.  The threshold already
        // incorporates LC from this iteration's root.
        result = dls(blank_curr, threshold, NEUTRAL, md_init, 0);
        if (result == 0) return;
        threshold = result;
        // Refresh both components for the next iteration's threshold.
        md_init = calc_manhattan();
        lc_init = calc_linear_conflict();
        threshold = max(threshold, md_init + lc_init);
    } while (result != 0);
}

// ---------------------------------------------------------------------------
// Output
// ---------------------------------------------------------------------------

static int count_digits(int n) {
    return (n == 0) ? 1 : (int) floor(log10((double) n) + 1);
}

void display_grid() {
    int w = count_digits(tiles_count);
    for (int i = 0; i < grid_size * grid_size; ++i) {
        int leading = w - count_digits(board[i]);
        for (int k = 0; k < leading; ++k) putchar(' ');
        printf("%d ", board[i]);
        if ((i + 1) % grid_size == 0) putchar('\n');
    }
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
    if (argc > 1) {
        read_from_file(argv[1]);
    } else {
        read_from_console();
    }

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
