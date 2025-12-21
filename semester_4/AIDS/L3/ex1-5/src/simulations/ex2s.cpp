#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <matplot/matplot.h>
#include "selects.hpp"
#include "operationCounter.hpp"
#include "logger.hpp"

void run_simulation() {
    const size_t m = 50; // Number of repetitions
    const size_t step = 1000;
    const size_t max_n = 50000;

    std::vector<size_t> n_values;
    for (size_t n = 100; n <= max_n; n += step) {
        n_values.push_back(n);
    }

    // Define k values as fractions of n: n/20, n/2, 3n/4
    auto get_k_values = [](size_t n) -> std::vector<size_t> {
        return {n / 20, n / 2, 3 * n / 4};
    };

    // Data structures to hold results for each k
    std::vector<std::vector<double>> avg_select_comparisons(3, std::vector<double>(n_values.size(), 0.0));
    std::vector<std::vector<double>> avg_select_swaps(3, std::vector<double>(n_values.size(), 0.0));
    std::vector<std::vector<double>> avg_random_select_comparisons(3, std::vector<double>(n_values.size(), 0.0));
    std::vector<std::vector<double>> avg_random_select_swaps(3, std::vector<double>(n_values.size(), 0.0));

    for (size_t i = 0; i < n_values.size(); ++i) {
        size_t n = n_values[i];
        std::vector<int> arr(n);
        std::iota(arr.begin(), arr.end(), 1); // Fill with 1..n
        std::random_device rd;
        std::mt19937 g(rd());

        auto k_values = get_k_values(n);

        for (size_t k_idx = 0; k_idx < k_values.size(); ++k_idx) {
            size_t k = k_values[k_idx];
            if (k == 0) k = 1; // Ensure k is at least 1

            size_t total_select_comparisons = 0;
            size_t total_select_swaps = 0;
            size_t total_random_select_comparisons = 0;
            size_t total_random_select_swaps = 0;

            for (size_t rep = 0; rep < m; ++rep) {
                std::shuffle(arr.begin(), arr.end(), g);

                // Test Select
                OperationCounter::reset();
                selects::Select(arr.begin(), arr.end(), k);
                total_select_comparisons += OperationCounter::comparisons;
                total_select_swaps += OperationCounter::swaps;

                // Test RandomSelect
                OperationCounter::reset();
                selects::RandomSelect(arr.begin(), arr.end(), k);
                total_random_select_comparisons += OperationCounter::comparisons;
                total_random_select_swaps += OperationCounter::swaps;
            }

            avg_select_comparisons[k_idx][i] = static_cast<double>(total_select_comparisons) / m;
            avg_select_swaps[k_idx][i] = static_cast<double>(total_select_swaps) / m;
            avg_random_select_comparisons[k_idx][i] = static_cast<double>(total_random_select_comparisons) / m;
            avg_random_select_swaps[k_idx][i] = static_cast<double>(total_random_select_swaps) / m;
        }
    }

    // Plotting comparisons
    auto fig1 = matplot::figure(true);
    matplot::hold(matplot::on);
    std::vector<std::string> k_labels = {"k = n/20", "k = n/2", "k = 3n/4"};
    std::vector<std::string> colors = {"b", "g", "r"};

    for (size_t k_idx = 0; k_idx < 3; ++k_idx) {
        auto line1 = matplot::plot(n_values, avg_select_comparisons[k_idx]);
        line1->line_width(2);
        line1->color(colors[k_idx]);
        line1->display_name("Select (" + k_labels[k_idx] + ")");
    
        auto line2 = matplot::plot(n_values, avg_random_select_comparisons[k_idx]);
        line2->line_width(2);
        line2->color(colors[k_idx]);
        line2->line_style("--");
        line2->display_name("RandomSelect (" + k_labels[k_idx] + ")");
    }
    
    matplot::xlabel("n (Array Size)");
    matplot::ylabel("Average Comparisons");
    matplot::title("Average Comparisons vs Array Size");
    matplot::legend();
    matplot::grid(matplot::on);
    matplot::save("ex2_comparisons.png");
    
    // Plotting swaps
    auto fig2 = matplot::figure(true);
    matplot::hold(matplot::on);
    
    for (size_t k_idx = 0; k_idx < 3; ++k_idx) {
        auto line1 = matplot::plot(n_values, avg_select_swaps[k_idx]);
        line1->line_width(2);
        line1->color(colors[k_idx]);
        line1->display_name("Select (" + k_labels[k_idx] + ")");
    
        auto line2 = matplot::plot(n_values, avg_random_select_swaps[k_idx]);
        line2->line_width(2);
        line2->color(colors[k_idx]);
        line2->line_style("--");
        line2->display_name("RandomSelect (" + k_labels[k_idx] + ")");
    }
    
    matplot::xlabel("n (Array Size)");
    matplot::ylabel("Average Swaps");
    matplot::title("Average Swaps vs Array Size");
    matplot::legend();
    matplot::grid(matplot::on);
    matplot::save("ex2_swaps.png");
}

int main() {
    Logger::enabled = false;
    run_simulation();
    return 0;
}