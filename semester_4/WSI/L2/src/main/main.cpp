#include <iostream>
#include "NPuzzle.hpp"
#include <queue>
#include <unordered_set>
#include <vector>
#include <utility>

std::tuple<bool, std::vector<NPuzzle>, size_t> solve_bfs(const NPuzzle& initial) {
    std::queue<std::vector<NPuzzle>> q; // Przechowuje ścieżki
    std::unordered_set<NPuzzle> visited;
    
    if (initial.is_solution()) {
        return {true, {initial}, visited.size()};
    }
    
    q.push({initial});
    visited.insert(initial);
    
    while (!q.empty()) {
        auto current_path = q.front();
        auto current = current_path.back();
        q.pop();
        
        for (NPuzzle& neighbor : current.generate_moves()) {
            if (neighbor.is_solution()) {
                current_path.push_back(neighbor);
                return {true, current_path, visited.size()};
            }
            
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                auto new_path = current_path;
                new_path.push_back(neighbor);
                q.push(new_path);
            }
        }
    }
    
    return {false, {}, visited.size()};
}

void print_board(const NPuzzle& puzzle) {
    size_t size = puzzle.get_size();
    const auto& board = puzzle.get_board();
    
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            size_t pos = i * size + j;
            if (board[pos] == 0) {
                std::cout << "  ";
            } else {
                std::cout << static_cast<int>(board[pos]) << " ";
            }
        }
        std::cout << std::endl;
    }
}

void print_solution(const std::vector<NPuzzle>& solution) {
    if (solution.empty()) return;
    
    std::cout << "Initial state:" << std::endl;
    print_board(solution[0]);
    
    std::cout << "\nSolution steps (" << solution.size() - 1 << " moves):" << std::endl;
    for (size_t i = 1; i < solution.size(); ++i) {
        std::cout << "Move " << i << ":" << std::endl;
        print_board(solution[i]);
        std::cout << std::endl;
    }
}

int main() {
    std::cout << "Solving 3x3 puzzle:" << std::endl;
    NPuzzle puzzle(3, {1, 2, 3, 4, 5, 6, 0, 7, 8});
    
    auto [solvable, solution, visited_count] = solve_bfs(puzzle);
    std::cout << "Solvable: " << std::boolalpha << solvable << std::endl;
    
    if (solvable) {
        print_solution(solution);
    }

    std::cout << "Visited states: " << visited_count << std::endl;

    std::cout << "\nSolving 2x2 puzzle:" << std::endl;
    NPuzzle puzzle2(2, {1, 0, 2, 3});
    
    auto [solvable2, solution2, visited_count2] = solve_bfs(puzzle2);
    std::cout << "Solvable: " << std::boolalpha << solvable2 << std::endl;
    
    if (solvable2) {
        print_solution(solution2);
    }

    std::cout << "Visited states: " << visited_count2 << std::endl;

    return 0;
}