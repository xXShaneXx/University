#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <memory>
#include <chrono>
#include <random>
#include "Node.hpp"
#include "Heuristics.hpp"
#include "DisjointPatternDB.hpp"
#include <matplot/matplot.h>
#include <numeric> 

using namespace matplot;

struct VectorHash {
    std::size_t operator()(const std::vector<uint8_t>& v) const noexcept {
        std::size_t hash = 0;
        for (auto x : v) hash = hash * 31 + std::hash<uint8_t>()(x);
        return hash;
    }
};

struct SimulationResult {
    int solution_depth;
    int visited_nodes;
    int created_nodes;
    double time_sec;
    std::string heuristic_name;
};

struct PuzzleStats {
    std::vector<uint8_t> board;
    std::vector<SimulationResult> results;
};

// Funkcja rozwiązująca układankę z daną heurystyką
SimulationResult solve_with_heuristic(const std::vector<uint8_t>& board, 
                                     const std::shared_ptr<Heuristic>& heuristic,
                                     double time_limit_sec = 50.0) {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::priority_queue<Node*, std::vector<Node*>, Node::Compare> open_set;
    std::unordered_map<std::vector<uint8_t>, int, VectorHash> g_values;
    int created_nodes = 0;
    int visited_nodes = 0;

    auto start_node = new Node(board, 0, heuristic, nullptr);
    open_set.push(start_node);
    g_values[board] = 0;
    created_nodes++;

    while (!open_set.empty()) {
        // Check time limit
        auto now = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() / 1000.0;
        if (elapsed > time_limit_sec) {
            // Free memory
            while (!open_set.empty()) {
                delete open_set.top();
                open_set.pop();
            }
            return {-1, visited_nodes, created_nodes, elapsed, heuristic->clone()->name()};
        }

        Node* current = open_set.top();
        open_set.pop();
        visited_nodes++;

        if (is_solution(current->board_)) {
            // Oblicz głębokość rozwiązania
            int depth = current->g_;
            auto end = std::chrono::high_resolution_clock::now();
            double time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;

            // Zwolnienie pamięci
            delete current;
            while (!open_set.empty()) {
                delete open_set.top();
                open_set.pop();
            }

            return {depth, visited_nodes, created_nodes, time, heuristic->clone()->name()};
        }

        auto neighbours = current->get_neighbours();
        for (auto& neighbour : neighbours) {
            if (g_values.find(neighbour->board_) == g_values.end() || 
                neighbour->g_ < g_values[neighbour->board_]) {
                g_values[neighbour->board_] = neighbour->g_;
                open_set.push(neighbour.release());
                created_nodes++;
            }
        }
        delete current;
    }

    return {-1, visited_nodes, created_nodes, 0.0, heuristic->clone()->name()};
}

// Generowanie losowych układanek
std::vector<PuzzleStats> generate_random_puzzles(size_t puzzle_size, size_t count) {
    std::vector<PuzzleStats> puzzles;
    for (size_t i = 0; i < count; ++i) {
        PuzzleStats stats;
        stats.board = Node::generate(puzzle_size);
        puzzles.push_back(stats);
    }
    return puzzles;
}

// Wykres 1: Wydajność heurystyk względem głębokości rozwiązania
void plot_heuristic_vs_depth(const std::vector<PuzzleStats>& puzzles) {
    figure();
    hold(on);
    
    std::map<std::string, std::vector<double>> depth_data;
    std::map<std::string, std::vector<double>> visited_data;
    
    for (const auto& puzzle : puzzles) {
        for (const auto& result : puzzle.results) {
            if (result.solution_depth > 0) {
                depth_data[result.heuristic_name].push_back(result.solution_depth);
                visited_data[result.heuristic_name].push_back(result.visited_nodes);
            }
        }
    }
    
    for (const auto& [name, depths] : depth_data) {
        const auto& visited = visited_data[name];
        scatter(depths, visited)->display_name(name);
    }
    
    xlabel("Solution depth");
    ylabel("Visited nodes");
    title("Heuristic performance vs solution depth");
    legend();
    save("heuristic_vs_depth.png");
}

// Wykres 2: Stosunek stanów odwiedzonych do utworzonych
void plot_visited_vs_created(const std::vector<PuzzleStats>& puzzles) {
    figure();
    hold(on);
    
    std::map<std::string, std::vector<double>> ratio_data;
    
    for (const auto& puzzle : puzzles) {
        for (const auto& result : puzzle.results) {
            if (result.solution_depth > 0 && result.created_nodes > 0) {
                double ratio = static_cast<double>(result.visited_nodes) / result.created_nodes;
                ratio_data[result.heuristic_name].push_back(ratio);
            }
        }
    }
    
    std::vector<std::string> names;
    std::vector<double> averages;
    
    for (const auto& [name, ratios] : ratio_data) {
        names.push_back(name);
        averages.push_back(mean(ratios));
    }
    
    std::vector<size_t> x(names.size());
    std::iota(x.begin(), x.end(), 0);
    auto b = bar(x, averages);
    xticklabels(names);
    ylabel("Visited/Created nodes ratio");
    title("Efficiency of heuristics (higher is better)");
    save("visited_vs_created.png");
}

// Wykres 3: Rozkład trudności układanek
void plot_difficulty_distribution(const std::vector<PuzzleStats>& puzzles) {
    std::vector<int> depths;
    for (const auto& puzzle : puzzles) {
        if (!puzzle.results.empty() && puzzle.results[0].solution_depth > 0) {
            depths.push_back(puzzle.results[0].solution_depth);
        }
    }
    
    figure();
    hist(depths);
    xlabel("Solution depth");
    ylabel("Number of puzzles");
    title("Puzzle difficulty distribution");
    save("difficulty_distribution.png");
}

// Wykres 4: Porównanie czasu wykonania heurystyk
void plot_time_comparison(const std::vector<PuzzleStats>& puzzles) {
    figure();
    hold(on);
    
    std::map<std::string, std::vector<double>> times;
    
    for (const auto& puzzle : puzzles) {
        for (const auto& result : puzzle.results) {
            if (result.solution_depth > 0) {
                times[result.heuristic_name].push_back(result.time_sec);
            }
        }
    }
    
    std::vector<std::string> names;
    std::vector<double> avg_times;
    
    for (const auto& [name, time_vec] : times) {
        names.push_back(name);
        avg_times.push_back(mean(time_vec));
    }
    
    std::vector<size_t> x(names.size());
    std::iota(x.begin(), x.end(), 0);
    auto b = bar(x, avg_times);
    xticklabels(names);
    ylabel("Average time (seconds)");
    title("Heuristic execution time comparison");
    save("time_comparison.png");
}

// Funkcja główna symulacji
void run_simulation() {
    const size_t puzzle_size = 4; // 3x3 puzzle
    const size_t num_puzzles = 100; // Liczba układanek do testowania
    
    // Inicjalizacja heurystyk
    //auto manhattan = std::make_shared<ManhattanHeuristic>();
    //auto misplaced = std::make_shared<MisplacedHeuristic>();
    auto manhattan_linear = std::make_shared<ManhattanLinearHeuristic>();
    
    // Dla PDB potrzebujemy załadować bazy danych
    //std::vector<std::vector<uint8_t>> patterns = {
       //{1, 2, 3}, // Pierwszy wiersz
        //{4, 5, 6}, // Drugi wiersz
        //{7, 8}     // Dwie ostatnie komórki (bez 0)
    //};
    auto pdb_heuristic = load_pdbs({"pdb_15_1.bin", "pdb_15_2.bin", "pdb_15_3.bin", "pdb_15_4.bin"});
    
    std::vector<std::shared_ptr<Heuristic>> heuristics = {
        //manhattan,
        //misplaced,
        manhattan_linear,
        std::shared_ptr<Heuristic>(pdb_heuristic.release())
    };
    
    // Generowanie układanek
    auto puzzles = generate_random_puzzles(puzzle_size, num_puzzles);
    
    // Rozwiązywanie każdej układanki z każdą heurystyką
    for (auto& puzzle : puzzles) {
        for (const auto& heuristic : heuristics) {
            auto result = solve_with_heuristic(puzzle.board, heuristic);
            puzzle.results.push_back(result);
        }
    }
    
    // Generowanie wykresów
    plot_heuristic_vs_depth(puzzles);
    plot_visited_vs_created(puzzles);
    plot_difficulty_distribution(puzzles);
    plot_time_comparison(puzzles);
    
    // Dodatkowe wykresy dla PDB
    // (Tutaj można dodać specyficzne analizy dla Disjoint Pattern Databases)
    
    std::cout << "Simulation completed. Charts saved to disk." << std::endl;
}

int main() {
    try {
        run_simulation();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}