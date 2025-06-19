#include <iostream>
#include "NPuzzle.hpp"
#include "DisjointPatternDB.hpp"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <vector>
#include <utility>

int main() {
    // 8-puzzle pattern DB filenames
    std::vector<std::string> pdb_files8 = {"pdb_8_1.bin", "pdb_8_2.bin"};

    // Load the pattern databases
    auto heuristic = load_pdbs(pdb_files8);

    // Example 8-puzzle board (can be any configuration)
    std::vector<uint8_t> board = {
        1, 2, 3,
        4, 5, 6,
        7, 0, 8
    };

    // Print the board
    std::cout << "Testing board:" << std::endl;
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            uint8_t val = board[i * 3 + j];
            if (val == 0)
                std::cout << "  ";
            else
                std::cout << static_cast<int>(val) << " ";
        }
        std::cout << std::endl;
    }

    // Compute and print the heuristic value
    int h = (*heuristic)(board);
    std::cout << "Disjoint Pattern DB heuristic value: " << h << std::endl;

    return 0;
}