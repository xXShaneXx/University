#include "RBTree.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <map>
#include <matplot/matplot.h>

struct ExperimentResult {
    double avg_comparisons;
    double avg_pointer_ops;
    double avg_color_changes;
    double avg_rotations;
    double avg_height;
    int max_comparisons;
    int max_pointer_ops;
    int max_color_changes;
    int max_rotations;
    int max_height;
};

ExperimentResult runExperiment(int n, bool ascending_insert) {
    std::vector<int> keys(n);
    for (int i = 0; i < n; ++i) keys[i] = i + 1;
    
    std::random_device rd;
    std::mt19937 g(rd());
    
    // Prepare insertion sequence
    if (!ascending_insert) {
        std::shuffle(keys.begin(), keys.end(), g);
    }
    
    // Insertion phase
    RBTree tree;
    std::vector<OperationCounts> insert_counts;
    
    for (int key : keys) {
        OperationCounts counts;
        tree.insert(key, counts);
        insert_counts.push_back(counts);
    }
    
    // Prepare deletion sequence (always random)
    std::shuffle(keys.begin(), keys.end(), g);
    
    // Deletion phase
    std::vector<OperationCounts> delete_counts;
    
    for (int key : keys) {
        OperationCounts counts;
        tree.remove(key, counts);
        delete_counts.push_back(counts);
    }
    
    // Process results
    ExperimentResult result;
    
    // Insertion stats
    long total_insert_comps = 0;
    long total_insert_ops = 0;
    long total_insert_colors = 0;
    long total_insert_rotations = 0;
    int max_insert_comps = 0;
    int max_insert_ops = 0;
    int max_insert_colors = 0;
    int max_insert_rotations = 0;
    int max_insert_height = 0;
    
    for (const auto& count : insert_counts) {
        total_insert_comps += count.comparisons;
        total_insert_ops += count.pointer_ops;
        total_insert_colors += count.color_changes;
        total_insert_rotations += count.rotations;
        if (count.comparisons > max_insert_comps) max_insert_comps = count.comparisons;
        if (count.pointer_ops > max_insert_ops) max_insert_ops = count.pointer_ops;
        if (count.color_changes > max_insert_colors) max_insert_colors = count.color_changes;
        if (count.rotations > max_insert_rotations) max_insert_rotations = count.rotations;
        if (count.height > max_insert_height) max_insert_height = count.height;
    }
    
    // Deletion stats
    long total_delete_comps = 0;
    long total_delete_ops = 0;
    long total_delete_colors = 0;
    long total_delete_rotations = 0;
    int max_delete_comps = 0;
    int max_delete_ops = 0;
    int max_delete_colors = 0;
    int max_delete_rotations = 0;
    int max_delete_height = 0;
    
    for (const auto& count : delete_counts) {
        total_delete_comps += count.comparisons;
        total_delete_ops += count.pointer_ops;
        total_delete_colors += count.color_changes;
        total_delete_rotations += count.rotations;
        if (count.comparisons > max_delete_comps) max_delete_comps = count.comparisons;
        if (count.pointer_ops > max_delete_ops) max_delete_ops = count.pointer_ops;
        if (count.color_changes > max_delete_colors) max_delete_colors = count.color_changes;
        if (count.rotations > max_delete_rotations) max_delete_rotations = count.rotations;
        if (count.height > max_delete_height) max_delete_height = count.height;
    }
    
    // Combined stats
    result.avg_comparisons = (total_insert_comps + total_delete_comps) / (2.0 * n);
    result.avg_pointer_ops = (total_insert_ops + total_delete_ops) / (2.0 * n);
    result.avg_color_changes = (total_insert_colors + total_delete_colors) / (2.0 * n);
    result.avg_rotations = (total_insert_rotations + total_delete_rotations) / (2.0 * n);
    result.avg_height = (max_insert_height + max_delete_height) / 2.0;
    
    result.max_comparisons = std::max(max_insert_comps, max_delete_comps);
    result.max_pointer_ops = std::max(max_insert_ops, max_delete_ops);
    result.max_color_changes = std::max(max_insert_colors, max_delete_colors);
    result.max_rotations = std::max(max_insert_rotations, max_delete_rotations);
    result.max_height = std::max(max_insert_height, max_delete_height);
    
    return result;
}

void plotResults(const std::map<int, std::vector<ExperimentResult>>& ascending_results,
                 const std::map<int, std::vector<ExperimentResult>>& random_results,
                 const std::vector<int>& n_values) {
    using namespace matplot;
    
    // Prepare data
    std::vector<double> ns;
    std::vector<double> asc_avg_comp, rand_avg_comp;
    std::vector<double> asc_max_comp, rand_max_comp;
    std::vector<double> asc_avg_ops, rand_avg_ops;
    std::vector<double> asc_max_ops, rand_max_ops;
    std::vector<double> asc_avg_height, rand_avg_height;
    std::vector<double> asc_max_height, rand_max_height;
    
    for (int n : n_values) {
        ns.push_back(n);
        
        // Calculate averages for ascending scenario
        double asc_comp = 0, asc_ops = 0, asc_h = 0;
        double asc_mcomp = 0, asc_mops = 0, asc_mh = 0;
        
        for (const auto& res : ascending_results.at(n)) {
            asc_comp += res.avg_comparisons;
            asc_ops += res.avg_pointer_ops;
            asc_h += res.avg_height;
            asc_mcomp += res.max_comparisons;
            asc_mops += res.max_pointer_ops;
            asc_mh += res.max_height;
        }
        
        asc_avg_comp.push_back(asc_comp / ascending_results.at(n).size());
        asc_avg_ops.push_back(asc_ops / ascending_results.at(n).size());
        asc_avg_height.push_back(asc_h / ascending_results.at(n).size());
        asc_max_comp.push_back(asc_mcomp / ascending_results.at(n).size());
        asc_max_ops.push_back(asc_mops / ascending_results.at(n).size());
        asc_max_height.push_back(asc_mh / ascending_results.at(n).size());
        
        // Calculate averages for random scenario
        double rand_comp = 0, rand_ops = 0, rand_h = 0;
        double rand_mcomp = 0, rand_mops = 0, rand_mh = 0;
        
        for (const auto& res : random_results.at(n)) {
            rand_comp += res.avg_comparisons;
            rand_ops += res.avg_pointer_ops;
            rand_h += res.avg_height;
            rand_mcomp += res.max_comparisons;
            rand_mops += res.max_pointer_ops;
            rand_mh += res.max_height;
        }
        
        rand_avg_comp.push_back(rand_comp / random_results.at(n).size());
        rand_avg_ops.push_back(rand_ops / random_results.at(n).size());
        rand_avg_height.push_back(rand_h / random_results.at(n).size());
        rand_max_comp.push_back(rand_mcomp / random_results.at(n).size());
        rand_max_ops.push_back(rand_mops / random_results.at(n).size());
        rand_max_height.push_back(rand_mh / random_results.at(n).size());
    }
    
    // Plot comparison counts
    auto h = figure(true);
    h->size(1200, 800);
    
    subplot(2, 2, 0);
    hold(on);
    plot(ns, asc_avg_comp, "-o")->display_name("Ascending (avg)");
    plot(ns, rand_avg_comp, "-o")->display_name("Random (avg)");
    xlabel("n");
    ylabel("Comparisons");
    title("Average Comparisons per Operation");
    legend();
    hold(off);
    
    subplot(2, 2, 1);
    hold(on);
    plot(ns, asc_max_comp, "-o")->display_name("Ascending (max)");
    plot(ns, rand_max_comp, "-o")->display_name("Random (max)");
    xlabel("n");
    ylabel("Comparisons");
    title("Maximum Comparisons in Single Operation");
    legend();
    hold(off);
    
    subplot(2, 2, 2);
    hold(on);
    plot(ns, asc_avg_ops, "-o")->display_name("Ascending (avg)");
    plot(ns, rand_avg_ops, "-o")->display_name("Random (avg)");
    xlabel("n");
    ylabel("Pointer Operations");
    title("Average Pointer Operations per Operation");
    legend();
    hold(off);
    
    subplot(2, 2, 3);
    hold(on);
    plot(ns, asc_max_ops, "-o")->display_name("Ascending (max)");
    plot(ns, rand_max_ops, "-o")->display_name("Random (max)");
    xlabel("n");
    ylabel("Pointer Operations");
    title("Maximum Pointer Operations in Single Operation");
    legend();
    hold(off);
    
    save("rbtree_operations_comparison.png");
    
    // Plot height information
    auto h2 = figure(true);
    h2->size(800, 400);
    
    subplot(1, 2, 0);
    hold(on);
    plot(ns, asc_avg_height, "-o")->display_name("Ascending (avg)");
    plot(ns, rand_avg_height, "-o")->display_name("Random (avg)");
    xlabel("n");
    ylabel("Height");
    title("Average Tree Height");
    legend();
    hold(off);
    
    subplot(1, 2, 1);
    hold(on);
    plot(ns, asc_max_height, "-o")->display_name("Ascending (max)");
    plot(ns, rand_max_height, "-o")->display_name("Random (max)");
    xlabel("n");
    ylabel("Height");
    title("Maximum Tree Height");
    legend();
    hold(off);
    
    save("rbtree_height.png");
}

void runExperiments() {
    std::vector<int> n_values = {10000, 20000, 30000, 40000, 50000, 
                                60000, 70000, 80000, 90000, 100000};
    const int num_tests = 20;
    
    std::map<int, std::vector<ExperimentResult>> ascending_results;
    std::map<int, std::vector<ExperimentResult>> random_results;
    
    // Run experiments
    for (int n : n_values) {
        std::cout << "Running tests for n = " << n << std::endl;
        
        for (int i = 0; i < num_tests; ++i) {
            std::cout << "  Test " << i+1 << "/" << num_tests << std::endl;
            
            // Ascending insert scenario
            ascending_results[n].push_back(runExperiment(n, true));
            
            // Random insert scenario
            random_results[n].push_back(runExperiment(n, false));
        }
    }
    
    // Process and plot results
    plotResults(ascending_results, random_results, n_values);
}

int main() {
    runExperiments();
    return 0;
}