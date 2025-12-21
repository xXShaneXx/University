#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <matplot/matplot.h>
#include "QuickSorts.hpp"
#include "operationCounter.hpp"
#include "logger.hpp"
#include "quick-sort.hpp"
#include "dual-pivot-quicksort.hpp"

void run_quicksort_experiments() {
    const size_t m = 20; // Number of repetitions
    const size_t step = 100;
    const size_t max_n = 1000;

    std::vector<size_t> n_values;
    for (size_t n = 100; n <= max_n; n += step) {
        n_values.push_back(n);
    }

    // Data structures for results
    std::vector<double> avg_time_quick(n_values.size(), 0.0);
    std::vector<double> avg_time_quick_select(n_values.size(), 0.0);
    std::vector<double> avg_time_dual(n_values.size(), 0.0);
    std::vector<double> avg_time_dual_select(n_values.size(), 0.0);
    
    std::vector<double> avg_comps_quick(n_values.size(), 0.0);
    std::vector<double> avg_comps_quick_select(n_values.size(), 0.0);
    std::vector<double> avg_comps_dual(n_values.size(), 0.0);
    std::vector<double> avg_comps_dual_select(n_values.size(), 0.0);

    // Worst-case data for standard quicksort (already sorted)
    std::vector<int> worst_case_data(max_n);
    std::iota(worst_case_data.begin(), worst_case_data.end(), 1);
    std::vector<double> worst_case_times(n_values.size(), 0.0);
    std::vector<double> worst_case_comps(n_values.size(), 0.0);

    for (size_t i = 0; i < n_values.size(); ++i) {
        size_t n = n_values[i];
        std::vector<int> arr(n);
        std::iota(arr.begin(), arr.end(), 1);
        std::random_device rd;
        std::mt19937 g(rd());

        // Temporary storage for measurements
        double total_time_quick = 0.0, total_time_quick_select = 0.0;
        double total_time_dual = 0.0, total_time_dual_select = 0.0;
        size_t total_comps_quick = 0, total_comps_quick_select = 0;
        size_t total_comps_dual = 0, total_comps_dual_select = 0;

        for (size_t rep = 0; rep < m; ++rep) {
            std::shuffle(arr.begin(), arr.end(), g);
            std::vector<int> arr_copy = arr;

            // Standard QuickSort
            OperationCounter::reset();
            auto start = std::chrono::high_resolution_clock::now();
            quickSort(arr_copy.begin(), arr_copy.end());
            auto end = std::chrono::high_resolution_clock::now();
            total_time_quick += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            total_comps_quick += OperationCounter::comparisons;

            // QuickSort with Select
            arr_copy = arr;
            OperationCounter::reset();
            start = std::chrono::high_resolution_clock::now();
            quicksorts::QuickSort(arr_copy.begin(), arr_copy.end());
            end = std::chrono::high_resolution_clock::now();
            total_time_quick_select += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            total_comps_quick_select += OperationCounter::comparisons;

            // Standard Dual-Pivot QuickSort 
            arr_copy = arr;
            OperationCounter::reset();
            start = std::chrono::high_resolution_clock::now();
            dual_pivot::DualPivotQuickSort(arr_copy.begin(), arr_copy.end());
            end = std::chrono::high_resolution_clock::now();
            total_time_dual += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            total_comps_dual += OperationCounter::comparisons;

            // Dual-Pivot with Select
            arr_copy = arr;
            OperationCounter::reset();
            start = std::chrono::high_resolution_clock::now();
            quicksorts::DualPivotQuickSort(arr_copy.begin(), arr_copy.end());
            end = std::chrono::high_resolution_clock::now();
            total_time_dual_select += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            total_comps_dual_select += OperationCounter::comparisons;
        }

        // Store averages
        avg_time_quick[i] = total_time_quick / m;
        avg_time_quick_select[i] = total_time_quick_select / m;
        avg_time_dual[i] = total_time_dual / m;
        avg_time_dual_select[i] = total_time_dual_select / m;
        
        avg_comps_quick[i] = static_cast<double>(total_comps_quick) / m;
        avg_comps_quick_select[i] = static_cast<double>(total_comps_quick_select) / m;
        avg_comps_dual[i] = static_cast<double>(total_comps_dual) / m;
        avg_comps_dual_select[i] = static_cast<double>(total_comps_dual_select) / m;

        // Measure worst-case for standard quicksort (already sorted data)
        std::vector<int> worst_case_copy(worst_case_data.begin(), worst_case_data.begin() + n);
        OperationCounter::reset();
        auto start = std::chrono::high_resolution_clock::now();
        std::sort(worst_case_copy.begin(), worst_case_copy.end());
        auto end = std::chrono::high_resolution_clock::now();
        worst_case_times[i] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        worst_case_comps[i] = OperationCounter::comparisons;
    }

    // Plotting runtime
    auto fig1 = matplot::figure(true);
    matplot::hold(matplot::on);
    
    auto q_line = matplot::plot(n_values, avg_time_quick, "b-");
    auto qs_line = matplot::plot(n_values, avg_time_quick_select, "b--");
    auto d_line = matplot::plot(n_values, avg_time_dual, "r-");
    auto ds_line = matplot::plot(n_values, avg_time_dual_select, "r--");

    q_line->line_width(2).display_name("QuickSort");
    qs_line->line_width(2).display_name("QuickSort+Select");
    d_line->line_width(2).display_name("Dual-Pivot");
    ds_line->line_width(2).display_name("Dual-Pivot+Select");

    matplot::xlabel("Array size (n)");
    matplot::ylabel("Average time (μs)");
    matplot::title("Sorting Algorithm Runtime");
    matplot::legend();
    matplot::grid(matplot::on);
    matplot::save("quicksort_times.png");

    // Plotting comparisons
    auto fig2 = matplot::figure(true);
    matplot::hold(matplot::on);
    
    q_line = matplot::plot(n_values, avg_comps_quick, "b-");
    qs_line = matplot::plot(n_values, avg_comps_quick_select, "b--");
    d_line = matplot::plot(n_values, avg_comps_dual, "r-");
    ds_line = matplot::plot(n_values, avg_comps_dual_select, "r--");


    q_line->line_width(2).display_name("QuickSort");
    qs_line->line_width(2).display_name("QuickSort+Select");
    d_line->line_width(2).display_name("Dual-Pivot");
    ds_line->line_width(2).display_name("Dual-Pivot+Select");

    matplot::xlabel("Array size (n)");
    matplot::ylabel("Average comparisons");
    matplot::title("Sorting Algorithm Comparisons");
    matplot::legend();
    matplot::grid(matplot::on);
    matplot::save("quicksort_comparisons.png");

    // Estimate constant factors
    auto estimate_constants = [&](const std::vector<double>& measurements, const std::string& name) {
        // Fit to n log n: find c where measurements ≈ c * n log n
        size_t last = n_values.size() - 1;
        double n = n_values[last];
        double nlogn = n * log2(n);
        double c = measurements[last] / nlogn;
        std::cout << name << " constant: " << c << "\n";
    };

    std::cout << "Average case constants:\n";
    estimate_constants(avg_comps_quick, "QuickSort comparisons");
    estimate_constants(avg_comps_quick_select, "QuickSort+Select comparisons");
    estimate_constants(avg_comps_dual, "Dual-Pivot comparisons");
    estimate_constants(avg_comps_dual_select, "Dual-Pivot+Select comparisons");
}

int main() {
    Logger::enabled = false;
    run_quicksort_experiments();
    return 0;
}