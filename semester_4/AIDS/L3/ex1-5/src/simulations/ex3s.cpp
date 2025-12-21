#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <matplot/matplot.h>
#include "selects.hpp"
#include "operationCounter.hpp"
#include "logger.hpp"

void run_group_size_experiment() {
    const size_t m = 50; // Number of repetitions
    const size_t step = 1000;
    const size_t max_n = 50000;
    const std::vector<int> group_sizes = {3, 5, 7, 9};

    std::vector<size_t> n_values;
    for (size_t n = 100; n <= max_n; n += step) {
        n_values.push_back(n);
    }

    // Data structures for results
    std::vector<std::vector<double>> avg_comparisons(group_sizes.size(), 
        std::vector<double>(n_values.size(), 0.0));
    std::vector<std::vector<double>> avg_swaps(group_sizes.size(), 
        std::vector<double>(n_values.size(), 0.0));
    std::vector<std::vector<double>> avg_times(group_sizes.size(), 
        std::vector<double>(n_values.size(), 0.0));

    for (size_t i = 0; i < n_values.size(); ++i) {
        size_t n = n_values[i];
        std::vector<int> arr(n);
        std::iota(arr.begin(), arr.end(), 1);
        std::random_device rd;
        std::mt19937 g(rd());

        for (size_t gs_idx = 0; gs_idx < group_sizes.size(); ++gs_idx) {
            int group_size = group_sizes[gs_idx];
            size_t k = n / 2; // Median selection

            size_t total_comparisons = 0;
            size_t total_swaps = 0;
            double total_time = 0.0;

            for (size_t rep = 0; rep < m; ++rep) {
                std::shuffle(arr.begin(), arr.end(), g);
                OperationCounter::reset();

                auto start = std::chrono::high_resolution_clock::now();
                selects::Select(arr.begin(), arr.end(), k, group_size);
                auto end = std::chrono::high_resolution_clock::now();

                total_comparisons += OperationCounter::comparisons;
                total_swaps += OperationCounter::swaps;
                total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            }

            avg_comparisons[gs_idx][i] = static_cast<double>(total_comparisons) / m;
            avg_swaps[gs_idx][i] = static_cast<double>(total_swaps) / m;
            avg_times[gs_idx][i] = total_time / m;
        }
    }

    // Plotting comparisons
    auto fig1 = matplot::figure(true);
    matplot::hold(matplot::on);
    std::vector<std::string> colors = {"b", "g", "r", "c"};
    std::vector<std::string> labels;
    for (int gs : group_sizes) {
        labels.push_back("k=" + std::to_string(gs));
    }

    for (size_t gs_idx = 0; gs_idx < group_sizes.size(); ++gs_idx) {
        auto line = matplot::plot(n_values, avg_comparisons[gs_idx]);
        line->color(colors[gs_idx]);
        line->line_width(2);
        line->display_name(labels[gs_idx]);
    }
    matplot::xlabel("n (Array Size)");
    matplot::ylabel("Average Comparisons");
    matplot::title("Comparisons for Different Group Sizes");
    matplot::legend();
    matplot::grid(matplot::on);
    matplot::save("ex3_select_group_comparisons.png");

    // Plotting swaps
    auto fig2 = matplot::figure(true);
    matplot::hold(matplot::on);
    for (size_t gs_idx = 0; gs_idx < group_sizes.size(); ++gs_idx) {
        auto line = matplot::plot(n_values, avg_swaps[gs_idx]);
        line->line_width(2);
        line->color(colors[gs_idx]);
        line->display_name(labels[gs_idx]);
    }
    matplot::xlabel("n (Array Size)");
    matplot::ylabel("Average Swaps");
    matplot::title("Swaps for Different Group Sizes");
    matplot::legend();
    matplot::grid(matplot::on);
    matplot::save("ex3_select_group_swaps.png");

    // Plotting times
    auto fig3 = matplot::figure(true);
    matplot::hold(matplot::on);
    for (size_t gs_idx = 0; gs_idx < group_sizes.size(); ++gs_idx) {
        auto line = matplot::plot(n_values, avg_times[gs_idx]);
        line->line_width(2);
        line->color(colors[gs_idx]);
        line->display_name(labels[gs_idx]);
    }
    matplot::xlabel("n (Array Size)");
    matplot::ylabel("Average Time (μs)");
    matplot::title("Runtime for Different Group Sizes");
    matplot::legend();
    matplot::grid(matplot::on);
    matplot::save("ex3_select_group_times.png");
}

int main() {
    Logger::enabled = false;
    run_group_size_experiment();
    return 0;
}