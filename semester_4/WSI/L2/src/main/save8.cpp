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
    // 8-puzzle patterns and filenames
    std::vector<std::vector<uint8_t>> patterns8 = {
        {1,2,3,4},
        {5,6,7,8}
    };
    std::vector<std::string> pdb_files8 = {"pdb_8_1.bin", "pdb_8_2.bin"};
    build_and_save_pdbs(3, patterns8, pdb_files8);

    std::cout << "Pattern databases built and saved." << std::endl;
    return 0;
}