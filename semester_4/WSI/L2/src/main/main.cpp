#include <queue>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Node.hpp"
#include "Heuristics.hpp"
#include "DisjointPatternDB.hpp"

// Hash function for board state
struct BoardHash {
    std::size_t operator()(const std::vector<uint8_t>& board) const {
        std::size_t hash = 0;
        for (auto v : board) {
            hash = hash * 31 + std::hash<uint8_t>()(v);
        }
        return hash;
    }
};

struct AStarResult {
    Node* solution;
    size_t visited;
    size_t generated;
};

AStarResult astar(const std::vector<uint8_t>& start_board, std::shared_ptr<Heuristic> heuristic) {
    auto cmp = Node::Compare();
    std::priority_queue<Node*, std::vector<Node*>, Node::Compare> open;
    std::unordered_set<std::vector<uint8_t>, BoardHash> closed;

    Node* start = new Node(start_board, 0, heuristic, nullptr);
    open.push(start);

    size_t visited = 0;
    size_t generated = 1;

    while (!open.empty()) {
        Node* current = open.top();
        open.pop();

        if (closed.count(current->board_)) {
            delete current;
            continue;
        }
        closed.insert(current->board_);
        visited++;

        if (is_solution(current->board_)) {
            return {current, visited, generated};
        }

        for (auto& neighbour : current->get_neighbours()) {
            if (!closed.count(neighbour->board_)) {
                open.push(neighbour.get());
                neighbour.release(); // Ownership to open queue
                generated++;
            }
        }
    }
    return {nullptr, visited, generated};
}

// Print solution steps from goal node
inline void print_solution_steps(Node* goal) {
    std::vector<Node*> path;
    for (Node* n = goal; n != nullptr; n = n->from) {
        path.push_back(n);
    }
    std::cout << "Number of steps: " << (path.size() - 1) << "\n";
    std::reverse(path.begin(), path.end());
    for (auto* n : path) {
        size_t size = static_cast<size_t>(std::sqrt(n->board_.size()));
        for (size_t i = 0; i < n->board_.size(); ++i) {
            std::cout << (int)n->board_[i] << " ";
            if ((i + 1) % size == 0) std::cout << "\n";
        }
        std::cout << "g=" << n->g_ << " f=" << (int)n->f() << "\n\n";
    }
}

int main() {
    std::vector<std::pair<std::vector<uint8_t>, std::string>> cases = {
        //{{1,2,3,4,5,6,7,0,8}, "Trivial (one move)"},
        //{{1,2,3,4,5,6,0,7,8}, "Easy"},
        //{{1,2,0,4,5,3,7,8,6}, "Medium"},
        //{{0,8,7,6,5,4,3,2,1}, "Hard (reversed)"},
        //{{8,6,7,2,5,4,3,0,1}, "Very Hard"},
        // 15-puzzle cases (4x4)
        {{ 1, 2, 3, 4,
           5, 6, 7, 8,
           9,10,11,12,
           13,14,15, 0 }, "15-puzzle Solved"},
        {{ 1, 2, 3, 4,
           5, 6, 7, 8,
           9,10,11,12,
           0,13,14,15 }, "15-puzzle Easy"},
        {{ 2, 3, 4, 8,
           1, 6, 7,12,
           5,10,11,15,
           9,13,14, 0 }, "15-puzzle Hard"},
        {{ 5, 1, 2, 4,
           9, 6, 3, 8,
           13,10,7,12,
           0,14,11,15 }, "15-puzzle Very Hard"},
        // Randomly generated cases
        {Node::generate(4), "Random 15-puzzle"}
    };

    auto heuristic = std::shared_ptr<Heuristic>(std::move(load_pdbs({"pdb_15_21.bin", "pdb_15_22.bin", "pdb_15_23.bin"})));

    for (const auto& [start, desc] : cases) {
        std::cout << "==== " << desc << " ====\n";
        auto result = astar(start, heuristic);

        std::cout << "Visited: " << result.visited << "\n";
        std::cout << "Generated: " << result.generated << "\n";

        if (result.solution) {
            std::cout << "Solution steps:\n";
            print_solution_steps(result.solution);
        } else {
            std::cout << "No solution found.\n";
        }
        std::cout << "====================\n\n";
    }
    return 0;
}