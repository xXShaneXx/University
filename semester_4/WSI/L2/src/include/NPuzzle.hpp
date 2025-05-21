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
        NPuzzle(const NPuzzle& other);
        void set_heuristic(std::unique_ptr<Heuristic> heuristic);
        void generate(size_t size);
        bool isSolvable() const;
        
        size_t get_size() const;
        size_t get_empty_pos() const;
        const std::vector<uint8_t>& get_board() const;
        
        bool is_solution() const;
        std::vector<NPuzzle> generate_moves() const;
        
        bool operator==(const NPuzzle& other) const;
        bool operator<(const NPuzzle& other) const {
            return this->total_cost() > other.total_cost();
        }
        void increase_moves();
        int total_cost() const;

    private:
        NPuzzle create_swapped(size_t new_pos) const;
        std::unique_ptr<Heuristic> heuristic_;
};

namespace std {
    template<>
    struct hash<NPuzzle> {
        size_t operator()(const NPuzzle& puzzle) const;
    };
}

#endif // NPUZZLE_HPP