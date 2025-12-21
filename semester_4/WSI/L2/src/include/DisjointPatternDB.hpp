#ifndef DISJOINTPATTERNDB_HPP
#define DISJOINTPATTERNDB_HPP

#include <unordered_map>
#include <vector>
#include <queue>
#include <cstdint>
#include <algorithm>
#include <string>
#include <memory>

#include "Heuristics.hpp"

// Key type: positions of pattern tiles + blank position
struct PatternKey {
    std::vector<uint8_t> tile_positions; // tile positions in order of pattern_tiles
    uint8_t blank_pos;

    bool operator==(const PatternKey& other) const {
        return tile_positions == other.tile_positions && blank_pos == other.blank_pos;
    }
};

// Hash for PatternKey
struct PatternKeyHash {
    std::size_t operator()(const PatternKey& k) const {
        std::size_t hash = 0;
        for (auto v : k.tile_positions) {
            hash ^= std::hash<uint8_t>()(v) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        hash ^= std::hash<uint8_t>()(k.blank_pos) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }
};

class PatternDatabase {
    std::vector<uint8_t> pattern_tiles; // e.g. {1,2,3,4,5,6,7}
    size_t size; // puzzle width/height

    std::unordered_map<PatternKey, int, PatternKeyHash> database;

    PatternKey make_key(const std::vector<uint8_t>& board) const;

public:
    PatternDatabase() = default; 
    PatternDatabase(size_t puzzle_size, const std::vector<uint8_t>& tile_pattern);
    int lookup(const std::vector<uint8_t>& board) const;

    void build_database();
    bool save_database(const std::string& filename) const;
    bool load_database(const std::string& filename);
};

class DisjointPatternDatabaseHeuristic : public Heuristic {
    std::vector<PatternDatabase> databases;
public:
DisjointPatternDatabaseHeuristic(const std::vector<PatternDatabase>& dbs) : databases(dbs) {}
    DisjointPatternDatabaseHeuristic(size_t puzzle_size, const std::vector<std::vector<uint8_t>>& patterns) {
        for (const auto& pattern : patterns) {
            databases.emplace_back(puzzle_size, pattern);
            databases.back().build_database();
        }
    }
    
    int operator()(const std::vector<uint8_t>& board) const override {
        int sum = 0;
        for (const auto& db : databases) {
            sum += db.lookup(board);
        }
        return sum;
    }
    Heuristic* clone() const override {
        return new DisjointPatternDatabaseHeuristic(*this);
    }

    std::string name() const override { return "PatterDatabase"; }
};

std::unique_ptr<DisjointPatternDatabaseHeuristic> load_pdbs(const std::vector<std::string>& filenames);

#endif