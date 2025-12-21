#ifndef NPUZZLE_HPP
#define NPUZZLE_HPP

#include <vector>
#include <cstdint>
#include <memory>

#include "Heuristics.hpp"

class NPuzzle {
    private:
        size_t size_;  // Board size (size_ × size_)
        std::vector<uint8_t> board_;
        size_t empty_pos_;
        int g = 0;


    public:
        NPuzzle(size_t size, const std::vector<uint8_t>& tiles);
        NPuzzle(size_t size);
        NPuzzle();
        NPuzzle(const NPuzzle& other);
        void set_heuristic(std::shared_ptr<Heuristic> heuristic);
        void generate(size_t size);
        bool isSolvable() const;
        
        size_t get_size() const;
        size_t get_empty_pos() const;
        const std::vector<uint8_t>& get_board() const;
        
        bool is_solution() const;
        std::vector<std::shared_ptr<NPuzzle>> generate_moves() const;
        
        bool operator==(const NPuzzle& other) const;
        bool operator!=(const NPuzzle& other) const;
        NPuzzle& operator=(const NPuzzle& other);
        bool operator<(const NPuzzle& other) const {
            return this->total_cost() > other.total_cost();
        }
        void increase_moves();
        int total_cost() const;

    private:
        NPuzzle create_swapped(size_t new_pos) const;
        std::shared_ptr<Heuristic> heuristic_;
};

namespace std {
    template<>
    struct hash<NPuzzle> {
        size_t operator()(const NPuzzle& puzzle) const {
            size_t seed = 0;
            for (auto tile : puzzle.get_board()) {
                seed ^= std::hash<uint8_t>{}(tile) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}

struct PuzzleCompare {
    bool operator()(const std::shared_ptr<NPuzzle>& a, const std::shared_ptr<NPuzzle>& b) const {
        return a->total_cost() > b->total_cost(); // min-heap
    }
};

struct NPuzzlePtrHash {
    size_t operator()(const std::shared_ptr<NPuzzle>& p) const {
        size_t seed = 0;
        for (auto tile : p->get_board()) {
            seed ^= std::hash<uint8_t>{}(tile) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct NPuzzlePtrEqual {
    bool operator()(const std::shared_ptr<NPuzzle>& a, const std::shared_ptr<NPuzzle>& b) const {
        return a->get_board() == b->get_board();
    }
};
#endif // NPUZZLE_HPP