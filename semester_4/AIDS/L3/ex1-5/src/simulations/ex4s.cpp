#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <matplot/matplot.h>
#include "binarySearch.hpp"
#include "operationCounter.hpp"

void run_binary_search_experiment() {
    const size_t m = 100; // Number of repetitions for random case
    const size_t step = 1000;
    const size_t max_n = 100000;

    std::vector<size_t> n_values;
    for (size_t n = 1000; n <= max_n; n += step) {
        n_values.push_back(n);
    }

    // Scenario descriptions
    enum Scenario {
        FIRST_ELEMENT,
        MIDDLE_ELEMENT,
        LAST_ELEMENT,
        NOT_FOUND,
        RANDOM_ELEMENT
    };

    // Data structures for results
    std::vector<std::vector<double>> avg_comparisons(5, std::vector<double>(n_values.size(), 0.0));
    std::vector<std::vector<double>> avg_times(5, std::vector<double>(n_values.size(), 0.0));
    std::vector<std::string> scenario_names = {
        "First element",
        "Middle element",
        "Last element",
        "Not found",
        "Random element"
    };

    for (size_t i = 0; i < n_values.size(); ++i) {
        size_t n = n_values[i];
        std::vector<int> arr(n);
        std::iota(arr.begin(), arr.end(), 1); // Sorted array 1..n
        std::random_device rd;
        std::mt19937 gen(rd());

        // Test different scenarios
        for (int scenario = 0; scenario < 5; ++scenario) {
            size_t total_comparisons = 0;
            double total_time = 0.0;
            size_t reps = (scenario == RANDOM_ELEMENT) ? m : 1;

            for (size_t rep = 0; rep < reps; ++rep) {
                int x;
                switch (scenario) {
                    case FIRST_ELEMENT: x = arr.front(); break;
                    case MIDDLE_ELEMENT: x = arr[n/2]; break;
                    case LAST_ELEMENT: x = arr.back(); break;
                    case NOT_FOUND: x = 0; break; // Not in array
                    case RANDOM_ELEMENT: {
                        std::uniform_int_distribution<> dist(0, n-1);
                        x = arr[dist(gen)];
                        break;
                    }
                }

                OperationCounter::reset();
                auto start = std::chrono::high_resolution_clock::now();
                BinarySearch(arr.begin(), arr.end(), x);
                auto end = std::chrono::high_resolution_clock::now();

                total_comparisons += OperationCounter::comparisons;
                total_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            }

            avg_comparisons[scenario][i] = static_cast<double>(total_comparisons) / reps;
            avg_times[scenario][i] = total_time / reps;
        }
    }

    // Theoretical complexity: log2(n) + O(1)
    std::vector<double> theoretical_comparisons;
    for (size_t n : n_values) {
        theoretical_comparisons.push_back(log2(n));
    }

    // Plotting comparisons
    auto fig1 = matplot::figure(true);
    matplot::hold(matplot::on);
    
    // Plot theoretical curve
    auto theory_line = matplot::plot(n_values, theoretical_comparisons, "k--");
    theory_line->line_width(2);
    theory_line->display_name("Theoretical (log₂n)");

    // Plot empirical results
    std::vector<std::string> colors = {"b", "g", "r", "c", "m"};
    for (int scenario = 0; scenario < 5; ++scenario) {
        auto line = matplot::plot(n_values, avg_comparisons[scenario]);
        line->color(colors[scenario]);
        line->line_width(2);
        line->display_name(scenario_names[scenario]);
    }

    matplot::xlabel("Array size (n)");
    matplot::ylabel("Average comparisons");
    matplot::title("Binary Search Comparisons");
    matplot::legend();
    matplot::grid(matplot::on);
    matplot::save("binary_search_comparisons.png");

    // Plotting times
    auto fig2 = matplot::figure(true);
    matplot::hold(matplot::on);
    
    // Plot empirical results
    for (int scenario = 0; scenario < 5; ++scenario) {
        auto line = matplot::plot(n_values, avg_times[scenario]);
        line->color(colors[scenario]);
        line->line_width(2);
        line->display_name(scenario_names[scenario]);
    }

    matplot::xlabel("Array size (n)");
    matplot::ylabel("Average time (ns)");
    matplot::title("Binary Search Runtime");
    matplot::legend();
    matplot::grid(matplot::on);
    matplot::save("binary_search_times.png");

    // Estimate O(1) factors
    std::cout << "Estimated O(1) factors:\n";
    for (int scenario = 0; scenario < 5; ++scenario) {
        double avg_constant = 0.0;
        size_t last_idx = n_values.size() - 1;
        
        // Compare last measurement with theoretical expectation
        double measured = avg_comparisons[scenario][last_idx];
        double theoretical = log2(n_values[last_idx]);
        double constant = measured - theoretical;
        
        std::cout << scenario_names[scenario] << ": " << constant << " comparisons\n";
    }
}

int main() {
    run_binary_search_experiment();
    return 0;
}