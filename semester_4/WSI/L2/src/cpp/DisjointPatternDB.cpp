#include "DisjointPatternDB.hpp"
#include <set>
#include <cassert>
#include <fstream>
#include <memory>

PatternDatabase::PatternDatabase(size_t puzzle_size, const std::vector<uint8_t>& tile_pattern)
    : pattern_tiles(tile_pattern), size(puzzle_size) {
    build_database();
}

// Create a key from a board: get the positions of pattern tiles and blank
PatternKey PatternDatabase::make_key(const std::vector<uint8_t>& board) const {
    std::vector<uint8_t> tile_positions;
    uint8_t blank_pos = 0;
    std::unordered_map<uint8_t, uint8_t> tile_to_index;

    for (uint8_t i = 0; i < board.size(); ++i) {
        tile_to_index[board[i]] = i;
        if (board[i] == 0) {
            blank_pos = i;
        }
    }

    for (uint8_t tile : pattern_tiles) {
        tile_positions.push_back(tile_to_index[tile]);
    }

    return PatternKey{tile_positions, blank_pos};
}

// Build the DB using BFS from goal state
void PatternDatabase::build_database() {
    size_t N = size * size;
    // Goal board: tiles 1..N-1 in order, last is 0
    std::vector<uint8_t> goal_board(N);
    for (size_t i = 0; i < N - 1; ++i) goal_board[i] = i + 1;
    goal_board[N - 1] = 0;

    // Get goal pattern key
    PatternKey goal_key = make_key(goal_board);

    std::queue<std::pair<std::vector<uint8_t>, int>> q;
    std::unordered_map<PatternKey, int, PatternKeyHash> visited;

    q.push({goal_board, 0});
    visited[goal_key] = 0;

    std::set<uint8_t> tile_set(pattern_tiles.begin(), pattern_tiles.end());

    while (!q.empty()) {
        auto [cur_board, cost] = q.front();
        q.pop();

        PatternKey cur_key = make_key(cur_board);

        // Find blank position
        size_t blank_pos = 0;
        for (; blank_pos < cur_board.size(); ++blank_pos)
            if (cur_board[blank_pos] == 0) break;
        size_t row = blank_pos / size, col = blank_pos % size;
        std::vector<int> drow = {-1, 1, 0, 0};
        std::vector<int> dcol = {0, 0, -1, 1};

        for (int dir = 0; dir < 4; ++dir) {
            int nrow = static_cast<int>(row) + drow[dir];
            int ncol = static_cast<int>(col) + dcol[dir];
            if (nrow < 0 || nrow >= static_cast<int>(size) || ncol < 0 || ncol >= static_cast<int>(size)) continue;
            size_t neighbor = static_cast<size_t>(nrow) * size + static_cast<size_t>(ncol);

            std::vector<uint8_t> next_board = cur_board;
            std::swap(next_board[blank_pos], next_board[neighbor]);
            PatternKey next_key = make_key(next_board);

            // Increment cost ONLY if we moved a pattern tile
            int next_cost = cost;
            if (tile_set.count(cur_board[neighbor])) { // The tile moved into the blank is in the pattern
                next_cost = cost + 1;
            }
            // If the tile moved is not in the pattern, cost does not increase

            // Only update if new or found a cheaper cost
            if (visited.find(next_key) == visited.end() || visited[next_key] > next_cost) {
                visited[next_key] = next_cost;
                q.push({next_board, next_cost});
            }
        }
    }
    database = std::move(visited);
}

int PatternDatabase::lookup(const std::vector<uint8_t>& board) const {
    PatternKey key = make_key(board);
    auto it = database.find(key);
    return it != database.end() ? it->second : 0;
}


bool PatternDatabase::save_database(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out) return false;

    // Write pattern size and pattern tiles
    size_t pattern_size = pattern_tiles.size();
    out.write(reinterpret_cast<const char*>(&pattern_size), sizeof(pattern_size));
    out.write(reinterpret_cast<const char*>(pattern_tiles.data()), pattern_size * sizeof(uint8_t));
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));

    // Write number of entries
    size_t num_entries = database.size();
    out.write(reinterpret_cast<const char*>(&num_entries), sizeof(num_entries));

    for (const auto& [key, cost] : database) {
        // Write tile_positions
        size_t key_size = key.tile_positions.size();
        out.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
        out.write(reinterpret_cast<const char*>(key.tile_positions.data()), key_size * sizeof(uint8_t));
        // Write blank_pos
        out.write(reinterpret_cast<const char*>(&key.blank_pos), sizeof(key.blank_pos));
        // Write cost
        out.write(reinterpret_cast<const char*>(&cost), sizeof(cost));
    }
    return true;
}

bool PatternDatabase::load_database(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;

    // Read pattern size and pattern tiles
    size_t pattern_size = 0;
    in.read(reinterpret_cast<char*>(&pattern_size), sizeof(pattern_size));
    pattern_tiles.resize(pattern_size);
    in.read(reinterpret_cast<char*>(pattern_tiles.data()), pattern_size * sizeof(uint8_t));
    in.read(reinterpret_cast<char*>(&size), sizeof(size));

    // Read number of entries
    size_t num_entries = 0;
    in.read(reinterpret_cast<char*>(&num_entries), sizeof(num_entries));
    database.clear();

    for (size_t i = 0; i < num_entries; ++i) {
        PatternKey key;
        size_t key_size = 0;
        in.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));
        key.tile_positions.resize(key_size);
        in.read(reinterpret_cast<char*>(key.tile_positions.data()), key_size * sizeof(uint8_t));
        in.read(reinterpret_cast<char*>(&key.blank_pos), sizeof(key.blank_pos));
        int cost = 0;
        in.read(reinterpret_cast<char*>(&cost), sizeof(cost));
        database[key] = cost;
    }
    return true;
}

std::unique_ptr<DisjointPatternDatabaseHeuristic> load_pdbs(const std::vector<std::string>& filenames) {
    std::vector<PatternDatabase> dbs;
    for (const auto& fname : filenames) {
        PatternDatabase db;
        if (!db.load_database(fname)) {
            throw std::runtime_error("Failed to load pattern DB: " + fname);
        }
        dbs.push_back(std::move(db));
    }
    auto heuristic = std::make_unique<DisjointPatternDatabaseHeuristic>(dbs);
    return heuristic;
}
