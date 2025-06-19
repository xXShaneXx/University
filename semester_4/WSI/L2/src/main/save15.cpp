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

void build_and_save_pdbs(size_t puzzle_size, const std::vector<std::vector<uint8_t>>& patterns, const std::vector<std::string>& filenames) {
    for (size_t i = 0; i < patterns.size(); ++i) {
        PatternDatabase db(puzzle_size, patterns[i]);
        db.save_database(filenames[i]);
    }
}

int main() {
    // 15-puzzle patterns and filenames
    std::vector<std::vector<uint8_t>> patterns15 = {
        {1,2,3,4,5},
        {6,7,8,9,10},
        {11, 12, 13, 14, 15}
    };
    std::vector<std::string> pdb_files15 = {"pdb_15_21.bin", "pdb_15_22.bin", "pdb_15_23.bin"};
    build_and_save_pdbs(4, patterns15, pdb_files15);

    std::cout << "Pattern databases built and saved." << std::endl;
    return 0;
}