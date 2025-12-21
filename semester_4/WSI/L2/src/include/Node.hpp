#ifndef NODE_HPP
#define NODE_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <functional>
#include "Heuristics.hpp"


static bool is_solvable(const std::vector<uint8_t>& board, size_t size) {
    size_t inv_count = 0;
    for (size_t i = 0; i < board.size(); ++i) {
        if (board[i] == 0) continue;
        for (size_t j = i + 1; j < board.size(); ++j) {
            if (board[j] == 0) continue;
            if (board[i] > board[j]) inv_count++;
        }
    }
    if (size % 2 == 1) {
        // Odd grid: solvable if inversions even
        return inv_count % 2 == 0;
    } else {
        // Even grid: blank row from bottom
        size_t zero_pos = 0;
        for (size_t i = 0; i < board.size(); ++i) {
            if (board[i] == 0) {
                zero_pos = i;
                break;
            }
        }
        size_t row_from_bottom = size - (zero_pos / size);
        return (row_from_bottom % 2 == 0) ? (inv_count % 2 == 1) : (inv_count % 2 == 0);
    }
}

struct Node {
    std::vector<uint8_t> board_;
    uint16_t g_; // g | f
    Node* from;
    std::shared_ptr<Heuristic> heuristic_;

    inline uint8_t f() const {
        return g_ + (*heuristic_)(board_); 
    }

    Node(std::vector<uint8_t> board, uint8_t g, std::shared_ptr<Heuristic> heuristic, Node* f) : 
        board_(board),
        g_(g),
        heuristic_(heuristic),
        from(f)
    {}

    inline std::vector<std::unique_ptr<Node>> get_neighbours() const {
        std::vector<std::unique_ptr<Node>> neighbours;
        size_t size = static_cast<size_t>(std::sqrt(board_.size()));
        size_t zero_pos = 0;
        for (size_t i = 0; i < board_.size(); ++i) {
            if (board_[i] == 0) {
                zero_pos = i;
                break;
            }
        }
        size_t row = zero_pos / size;
        size_t col = zero_pos % size;
    
        auto make_node = [&](size_t new_pos) {
            std::vector<uint8_t> new_board = board_;
            std::swap(new_board[zero_pos], new_board[new_pos]);
            return std::make_unique<Node>(new_board, g_ + 1, heuristic_, const_cast<Node*>(this));
        };
    
        if (row > 0) neighbours.push_back(make_node(zero_pos - size));      // Up
        if (row + 1 < size) neighbours.push_back(make_node(zero_pos + size)); // Down
        if (col > 0) neighbours.push_back(make_node(zero_pos - 1));         // Left
        if (col + 1 < size) neighbours.push_back(make_node(zero_pos + 1));    // Right
    
        return neighbours;
    }

    static std::vector<uint8_t> generate(size_t size) {
        std::vector<uint8_t> board(size * size);
        for (size_t i = 0; i < board.size(); ++i) board[i] = i;
        std::random_device rd;
        std::mt19937 g(rd());
        do {
            std::shuffle(board.begin(), board.end(), g);
        } while (!is_solvable(board, size));
        return board;
    }

    static std::vector<uint8_t> generate_puzzle_by_random_walk(size_t size, size_t k) {
        // Create solved board
        std::vector<uint8_t> board(size * size);
        for (size_t i = 0; i < board.size(); ++i) {
            board[i] = static_cast<uint8_t>(i + 1);
        }
        board.back() = 0; // Empty tile at the end
    
        // Find initial empty position (bottom-right corner)
        size_t empty_pos = board.size() - 1;
        
        // Random number generation
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dir_dist(0, 3); // 4 possible directions
        
        // Possible moves (up, down, left, right)
        const int dr[] = {-1, 1, 0, 0};
        const int dc[] = {0, 0, -1, 1};
        
        for (size_t step = 0; step < k; ++step) {
            std::vector<int> valid_moves;
            size_t row = empty_pos / size;
            size_t col = empty_pos % size;
            
            // Check which moves are valid
            for (int dir = 0; dir < 4; ++dir) {
                int new_row = static_cast<int>(row) + dr[dir];
                int new_col = static_cast<int>(col) + dc[dir];
                
                if (new_row >= 0 && new_row < static_cast<int>(size) && 
                    new_col >= 0 && new_col < static_cast<int>(size)) {
                    valid_moves.push_back(dir);
                }
            }
            
            // If no valid moves (shouldn't happen for k < size^2), break
            if (valid_moves.empty()) break;
            
            // Choose random valid move
            std::uniform_int_distribution<> move_dist(0, valid_moves.size() - 1);
            int move = valid_moves[move_dist(gen)];
            
            // Calculate new empty position
            int new_row = static_cast<int>(row) + dr[move];
            int new_col = static_cast<int>(col) + dc[move];
            size_t new_pos = static_cast<size_t>(new_row) * size + static_cast<size_t>(new_col);
            
            // Swap tiles
            std::swap(board[empty_pos], board[new_pos]);
            empty_pos = new_pos;
        }
        
        return board;
    }

    struct Compare {
        bool operator()(const Node* a, const Node* b) const {
            return a->f() > b->f();
        }
    };
};

bool is_solution(std::vector<uint8_t> &board) {
    for (size_t i = 0; i < board.size() - 1; ++i) {
        if (board[i] != i + 1) return false;
    }
    return board.back() == 0;
}

#endif