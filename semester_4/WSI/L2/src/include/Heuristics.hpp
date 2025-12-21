#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include <vector>
#include <cstdint>
#include <math.h>
#include <string>

class Heuristic
{
    public:
        virtual ~Heuristic() = default;
        virtual int operator()(const std::vector<uint8_t>& board) const = 0;
        virtual Heuristic* clone() const = 0;
        virtual std::string name() const = 0;
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
        Heuristic* clone() const override { return new ManhattanHeuristic(*this); }
        std::string name() const override { return "Manhattan"; }
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
    Heuristic* clone() const override { return new MisplacedHeuristic(*this); }
    std::string name() const override { return "Misplaced"; }
};

class ManhattanLinearHeuristic : public Heuristic {
    public:
        int operator()(const std::vector<uint8_t>& board) const override {
            int heuristic_value = 0;
            size_t size = static_cast<size_t>(std::sqrt(board.size()));
    
            // Manhattan distance
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
    
            // Linear conflict (rows)
            for (size_t row = 0; row < size; ++row) {
                for (size_t col1 = 0; col1 < size; ++col1) {
                    size_t idx1 = row * size + col1;
                    uint8_t tile1 = board[idx1];
                    if (tile1 == 0) continue;
                    size_t goal_row1 = (tile1 - 1) / size;
                    size_t goal_col1 = (tile1 - 1) % size;
                    if (goal_row1 != row) continue;
                    for (size_t col2 = col1 + 1; col2 < size; ++col2) {
                        size_t idx2 = row * size + col2;
                        uint8_t tile2 = board[idx2];
                        if (tile2 == 0) continue;
                        size_t goal_row2 = (tile2 - 1) / size;
                        size_t goal_col2 = (tile2 - 1) % size;
                        if (goal_row2 == row && goal_col1 > goal_col2) {
                            heuristic_value += 2;
                        }
                    }
                }
            }
    
            // Linear conflict (columns)
            for (size_t col = 0; col < size; ++col) {
                for (size_t row1 = 0; row1 < size; ++row1) {
                    size_t idx1 = row1 * size + col;
                    uint8_t tile1 = board[idx1];
                    if (tile1 == 0) continue;
                    size_t goal_row1 = (tile1 - 1) / size;
                    size_t goal_col1 = (tile1 - 1) % size;
                    if (goal_col1 != col) continue;
                    for (size_t row2 = row1 + 1; row2 < size; ++row2) {
                        size_t idx2 = row2 * size + col;
                        uint8_t tile2 = board[idx2];
                        if (tile2 == 0) continue;
                        size_t goal_row2 = (tile2 - 1) / size;
                        size_t goal_col2 = (tile2 - 1) % size;
                        if (goal_col2 == col && goal_row1 > goal_row2) {
                            heuristic_value += 2;
                        }
                    }
                }
            }
    
            return heuristic_value;
        }
        Heuristic* clone() const override { return new ManhattanLinearHeuristic(*this); }
        std::string name() const override { return "ManhattanLinear"; }
    };

#endif // HEURISTICS_HPP