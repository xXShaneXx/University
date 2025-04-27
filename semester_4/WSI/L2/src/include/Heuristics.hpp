#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include <vector>
#include <cstdint>
#include <math.h>

class Heuristic
{
    public:
        virtual ~Heuristic() = default;
        virtual int operator()(const std::vector<uint8_t>& board) const = 0;
};

class ManhattanHeuristic : public Heuristic {
    public:
        int operator()(const std::vector<uint8_t>& board) const override {
            int heuristic_value = 0;
            size_t size = static_cast<size_t>(std::sqrt(board.size()));
    
            for (size_t i = 0; i < board.size(); ++i) {
                if (board[i] != 0) {
                    size_t target_row = (board[i] - 1) / size;
                    size_t target_col = (board[i] - 1) % size;
                    size_t current_row = i / size;
                    size_t current_col = i % size;
                    heuristic_value += std::abs(static_cast<int>(target_row - current_row)) +
                                       std::abs(static_cast<int>(target_col - current_col));
                }
            }
            return heuristic_value;
        }
};

class MisplacedHeuristic : public Heuristic {
public:
    int operator()(const std::vector<uint8_t>& board) const override {
        int heuristic_value = 0;
        size_t size = static_cast<size_t>(std::sqrt(board.size()));
        for(size_t i = 0; i < board.size(); i++) {
            if(board[i] != 0) {
                size_t target_row = (board[i] - 1) / size;
                size_t target_col = (board[i] - 1) % size;
                size_t current_row = i / size;
                size_t current_col = i % size;
                if(target_row != current_row || target_col != current_col)
                    heuristic_value++;
            }
        }
        return heuristic_value;
    }
};

#endif // HEURISTICS_HPP