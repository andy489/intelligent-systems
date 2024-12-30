#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <cmath>

using namespace std;
using namespace std::chrono;

const string PUZZLES_RELATIVE_PATH = "./../puzzles/";
const string CURRENT_PUZZLE = "puzzle160-4x4-unsolvable.txt";

int grid_size;
int blank_tile_target_index;
int blank_tile_target_pos_linear;
int tiles_count;

vector<vector<int>> current_state_matrix;
vector<vector<int>> goal_state_matrix;
vector<pair<int, int>> goal_state_linear;

pair<int, int> blank_tile_target_coords;
pair<int, int> blank_tile_curr_coords;

enum Move { // clockwise
    UP = 0, RIGHT, DOWN, LEFT, NEUTRAL
};

stack<Move> shifts_history;

const int MAX_HEURISTIC = 1e4;
const int NEIGHBOURS_COUNT = 4;
const int BLANK_TILE_ID = 0;

void set_goal_state() {
    goal_state_matrix.resize(grid_size, vector<int>(grid_size));
    int tile_id = 1;
    for (int row = 0; row < grid_size; ++row) {
        for (int col = 0; col < grid_size; ++col) {
            if (row != blank_tile_target_coords.first || col != blank_tile_target_coords.second) {
                goal_state_matrix[row][col] = tile_id++;
            } else {
                goal_state_matrix[row][col] = BLANK_TILE_ID;
            }
        }
    }

    goal_state_linear.resize(tiles_count + 1);

    blank_tile_target_pos_linear = blank_tile_target_coords.first * grid_size + blank_tile_target_coords.second;
    for (int i = 0; i < tiles_count + 1; ++i) {
        goal_state_linear[i] = {i / grid_size, i % grid_size};
    }


    for (int i = blank_tile_target_pos_linear; i > 0; --i) {
        swap(goal_state_linear[i], goal_state_linear[i - 1]);
    }
}

void read_current_state_from_console() {
    cin >> grid_size >> blank_tile_target_index;
    if (blank_tile_target_index < 0) {
        blank_tile_target_index = tiles_count;
    }

    blank_tile_target_coords = {blank_tile_target_index / grid_size, blank_tile_target_index % grid_size};

    tiles_count = grid_size * grid_size - 1;
    current_state_matrix.resize(grid_size, vector<int>(grid_size));

    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            cin >> current_state_matrix[i][j];

            if (current_state_matrix[i][j] == BLANK_TILE_ID) {
                blank_tile_curr_coords = {i, j};
            }
        }
    }

    set_goal_state();
}

void read_current_state_from_file() {
    ifstream in_file(PUZZLES_RELATIVE_PATH + CURRENT_PUZZLE);

    if (!in_file.is_open()) {
        cerr << "Error opening the file!";
        exit(0);
    }

    in_file >> grid_size;
    tiles_count = grid_size * grid_size - 1;
    current_state_matrix.resize(grid_size, vector<int>(grid_size));

    in_file >> blank_tile_target_index;
    if (blank_tile_target_index < 0) {
        blank_tile_target_index = tiles_count;
    }

    blank_tile_target_coords = {blank_tile_target_index / grid_size, blank_tile_target_index % grid_size};

    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            in_file >> current_state_matrix[i][j];

            if (current_state_matrix[i][j] == BLANK_TILE_ID) {
                blank_tile_curr_coords = {i, j};
            }
        }
    }

    in_file.close();
    set_goal_state();
}

static int count_digits(int n) {
    if (n == 0) {
        return 1;
    }

    return floor(log10(n) + 1);
}

void display_grid_2d(vector<vector<int>> &grid) {
    int largest_tile_id_digits_count = count_digits(tiles_count);

    for (auto &row : grid) {
        for (int tile : row) {

            int curr_tile_id_digits_count = count_digits(tile);
            int leading_spaces = largest_tile_id_digits_count - curr_tile_id_digits_count;

            for (int i = 0; i < leading_spaces; ++i) {
                cout << ' ';
            }

            cout << tile << ' ';
        }
        cout << endl;
    }
}

void display_grid_1d_goal_state(vector<pair<int, int>> &grid) {
    for (int i = 0; i < grid.size() - 1; ++i) {
        cout << "{" << grid[i].first << "," << grid[i].second << "}, ";
    }

    cout << "{" << grid[grid.size() - 1].first << "," << grid[grid.size() - 1].second << "}";
}

size_t merge(vector<int> &tiles, vector<int> &L, vector<int> &R) {
    int left_count = L.size();
    int right_count = R.size();
    int i = 0, j = 0, k = 0;
    size_t inversions_count = 0;

    while (i < left_count && j < right_count) {
        if (L[i] < R[j]) {
            tiles[k++] = L[i++];
        } else {
            tiles[k++] = R[j++];
            inversions_count += left_count - i;
        }
    }
    while (i < left_count) {
        tiles[k++] = L[i++];
    }

    while (j < right_count) {
        tiles[k++] = R[j++];
    }

    return inversions_count;
}

size_t merge_sort(vector<int> &tiles) {
    int n = tiles.size();

    size_t inversions_count = 0;

    if (n < 2) {
        return inversions_count;
    }

    int mid = n / 2;
    vector<int> L(mid), R(n - mid); // aux vectors

    for (int i = 0; i < mid; ++i) {
        L[i] = tiles[i]; // left part
    }

    for (int i = mid; i < n; ++i) {
        R[i - mid] = tiles[i]; // right part
    }

    inversions_count += merge_sort(L);
    inversions_count += merge_sort(R);
    inversions_count += merge(tiles, L, R);

    return inversions_count;
}

int calc_total_manhattan_distance_cost() {
    int cost = 0;

    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {

            int curr_tile_id = current_state_matrix[i][j];

            if (curr_tile_id == BLANK_TILE_ID) {
                continue;
            }

            cost += abs(i - goal_state_linear[curr_tile_id].first) +
                    abs(j - goal_state_linear[curr_tile_id].second);
        }
    }

    return cost;
}

bool check_solvable(vector<vector<int>> &curr_grid) {
    int blank_tile_row = -1;

    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            if (curr_grid[i][j] == BLANK_TILE_ID) {
                blank_tile_row = i;
                break;
            }
        }
        if (blank_tile_row >= 0) {
            break;
        }
    }

    vector<int> state_clone(tiles_count);

    for (int i = 0, p = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            if (curr_grid[i][j] == BLANK_TILE_ID) {
                continue;
            }

            state_clone[p++] = curr_grid[i][j];
        }
    }

    // inversion = any pair of tiles i and j where i < j and i,j > 0, but i appears after j when
    // considering the board in row-major order (row 0, followed by row 1, and so forth)
    // type is size_t since inversions can quickly grow to n * n, where n = tiles_count + 1
    size_t inversions_count = merge_sort(state_clone);

    if (grid_size % 2 == 1) {
        return inversions_count % 2 == 0;
    } else {
        return (blank_tile_row + inversions_count) % 2 == 1;
    }
}

Move reverse_move(Move move) {
    switch (move) {
        case UP:
            return DOWN;
        case RIGHT:
            return LEFT;
        case DOWN:
            return UP;
        case LEFT:
            return RIGHT;
        default:
            return NEUTRAL;
    }
}

pair<int, int> next(pair<int, int> &blank_tile_coord, Move move) {
    pair<int, int> next_move = blank_tile_coord;

    switch (move) {
        case UP:
            ++next_move.first;
            break;
        case RIGHT:
            --next_move.second;
            break;
        case DOWN:
            --next_move.first;
            break;
        case LEFT:
            ++next_move.second;
            break;
        default:
            break;
    }

    return next_move;
}

inline int update_manhattan_cost(pair<int, int> current_blank_tile_coord, pair<int, int> prev_blank_tile_coord,
                                 int heuristic) {

    if (current_blank_tile_coord == prev_blank_tile_coord) {
        return heuristic;
    }

    int pos = current_state_matrix[prev_blank_tile_coord.first][prev_blank_tile_coord.second];
    pair<int, int> G = goal_state_linear[pos];
    // calculate Manhattan cost of shifted tile
    int upd1 = abs(current_blank_tile_coord.first - G.first) + abs(current_blank_tile_coord.second - G.second);
    // calculate Manhattan cost of blank tile
    int upd2 = abs(prev_blank_tile_coord.first - G.first) + abs(prev_blank_tile_coord.second - G.second);

    int new_heuristic = heuristic - upd1 + upd2;

    return new_heuristic;
}

bool validate_coord(pair<int, int> coord) {
    return (coord.first >= 0 && coord.first < grid_size) && (coord.second >= 0 && coord.second < grid_size);
}

void shift(pair<int, int> &a, pair<int, int> &b) {
    int temp = current_state_matrix[a.first][a.second];
    current_state_matrix[a.first][a.second] = current_state_matrix[b.first][b.second];
    current_state_matrix[b.first][b.second] = temp;
}

int depth_limited_search(pair<int, int> blank_tile_pos, int limit, Move prev_move, int curr_heuristic, int curr_depth) {

    Move opposite_move = reverse_move(prev_move);
    pair<int, int> prev_blank_coord = next(blank_tile_pos, opposite_move);

    curr_heuristic = update_manhattan_cost(blank_tile_pos, prev_blank_coord, curr_heuristic);

    int cost = curr_heuristic + curr_depth;

    if (cost > limit) {
        return cost;
    } else if (curr_heuristic == 0) {
        // goal state found!
        shifts_history.push(prev_move);
        printf("%d\n", curr_depth);
        return 0;
    }

    int min_heuristic = MAX_HEURISTIC;
    for (int i = 0; i < NEIGHBOURS_COUNT; ++i) {

        if (opposite_move == i) {
            continue;
        }

        pair<int, int> neighbour = next(blank_tile_pos, static_cast<Move>(i));

        if (!validate_coord(neighbour)) {
            continue;
        }

        shift(neighbour, blank_tile_pos);

        int current_result = depth_limited_search(neighbour, limit, static_cast<Move>(i), curr_heuristic,
                                                  curr_depth + 1);
        shift(neighbour, blank_tile_pos);

        if (current_result == 0) {
            // goal state found!
            shifts_history.push(prev_move);
            return 0;
        } else if (current_result < min_heuristic) {
            min_heuristic = current_result;
        }
    }

    return min_heuristic;
}

void display_solution_moves() {

    while (!shifts_history.empty()) {
        switch (shifts_history.top()) {
            case UP:
                printf("up\n");
                break;
            case RIGHT:
                printf("right\n");
                break;
            case DOWN:
                printf("down\n");
                break;
            case LEFT:
                printf("left\n");
                break;
            default:
                break;
        }

        shifts_history.pop();
    }
}

void ida_star() { // depth limited search with heuristic = manhattan distance
    int heuristic = calc_total_manhattan_distance_cost();

    if (heuristic == 0) {
        cout << 0 << endl;
        return;
    }

    int limit = heuristic;
    int depth = 0;
    int result = 0;

    do {
        result = depth_limited_search(blank_tile_curr_coords, limit, NEUTRAL, heuristic, depth);
        limit = result;
    } while (result);
}

int main() {
    // read_current_state_from_console();
    read_current_state_from_file();

    int heuristic = calc_total_manhattan_distance_cost();

    if (heuristic == 0) {
        cout << 0 << endl;
    } else {

        if (!check_solvable(current_state_matrix) || !check_solvable(goal_state_matrix)) {
            printf("Puzzle cannot be solved!");
            return 0;
        }

        ida_star(); // iterative deepening A* search
        display_solution_moves();
    }

    cout << endl << "Success!" << endl;

    return 0;
}