#include <iostream>
#include "NPuzzle.hpp"
#include "DisjointPatternDB.hpp"
#include <vector>
#include <string>

int main() {
    // 15-puzzle pattern DB filenames
    std::vector<std::string> pdb_files15 = {"pdb_15_1.bin", "pdb_15_2.bin", "pdb_15_3.bin", "pdb_15_4.bin"};

    // Load the pattern databases
    auto heuristic = load_pdbs(pdb_files15);

    // Test cases for 15-puzzle
    std::vector<std::pair<std::string, std::vector<uint8_t>>> test_cases = {
        {"Solved", {
            1,  2,  3,  4,
            5,  6,  7,  8,
            9, 10, 11, 12,
            13,14, 15,  0
        }},
        {"One move", {
            1,  2,  3,  4,
            5,  6,  7,  8,
            9, 10, 11, 12,
            13,14,  0, 15
        }},
        {"Scrambled", {
            5,  1,  2,  4,
            9,  6,  3,  8,
            13,10, 7, 12,
            0, 14, 11, 15
        }},
        {"Hard", {
            12, 1, 10, 2,
            7, 11, 4, 14,
            5, 0, 9, 15,
            8, 13, 6, 3
        }}
    };

    for (const auto& [desc, board] : test_cases) {
        std::cout << "\nCase: " << desc << std::endl;
        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                uint8_t val = board[i * 4 + j];
                if (val == 0)
                    std::cout << "   ";
                else
                    std::cout << static_cast<int>(val) << " ";
            }
            std::cout << std::endl;
        }
        int h = (*heuristic)(board);
        std::cout << "Disjoint Pattern DB heuristic value: " << h << std::endl;
    }

    return 0;
}