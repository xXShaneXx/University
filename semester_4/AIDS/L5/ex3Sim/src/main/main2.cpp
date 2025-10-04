#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include "BHeap.hpp"
#include <matplot/matplot.h>

using namespace std;
using namespace matplot;

const int NUM_EXPERIMENTS = 5;
const int HISTORICAL_N = 500;
const int MIN_N = 100;
const int MAX_N = 10000;
const int STEP = 100;

vector<int> generate_random_sequence(int n) {
    vector<int> data(n);
    iota(data.begin(), data.end(), 1);
    shuffle(data.begin(), data.end(), mt19937(random_device{}()));
    return data;
}

void plot_operation_history(int n, int attempt) {
    vector<long long> comparisons;
    vector<string> operation_types;
    vector<size_t> operation_indices;
    size_t op_counter = 0;

    // 1. MakeHeap operations (H1 and H2)
    comparison_counter = 0;
    BinomialHeap H1;
    operation_types.push_back("Make-Heap");
    comparisons.push_back(comparison_counter);
    operation_indices.push_back(++op_counter);

    comparison_counter = 0;
    BinomialHeap H2;
    operation_types.push_back("Make-Heap");
    comparisons.push_back(comparison_counter);
    operation_indices.push_back(++op_counter);

    // 2. Insert operations
    auto data1 = generate_random_sequence(n);
    auto data2 = generate_random_sequence(n);
    
    for (int i = 0; i < n; ++i) {
        comparison_counter = 0;
        H1.insert(data1[i]);
        operation_types.push_back("Heap-Insert");
        comparisons.push_back(comparison_counter);
        operation_indices.push_back(++op_counter);

        comparison_counter = 0;
        H2.insert(data2[i]);
        operation_types.push_back("Heap-Insert");
        comparisons.push_back(comparison_counter);
        operation_indices.push_back(++op_counter);
    }

    // 3. Heap-Union
    comparison_counter = 0;
    BinomialHeap H;
    H.head = H.unionHeaps(H1.head, H2.head);
    operation_types.push_back("Heap-Union");
    comparisons.push_back(comparison_counter);
    operation_indices.push_back(++op_counter);

    // 4. Extract-Min operations
    vector<int> extracted;
    for (int i = 0; i < 2 * n; ++i) {
        comparison_counter = 0;
        Node* minNode = H.extractMin();
        if (!minNode) {
            cerr << "ERROR: Empty heap during Extract-Min (n=" << n << ")\n";
            return;
        }
        extracted.push_back(minNode->key);
        operation_types.push_back("Extract-Min");
        comparisons.push_back(comparison_counter);
        operation_indices.push_back(++op_counter);
        delete minNode;
    }

    if (!is_sorted(extracted.begin(), extracted.end())) {
        cerr << "ERROR: Extract-Min sequence not sorted (n=" << n << ")\n";
        return;
    }

    // Create the plot
    auto fig = figure(true);
    fig->size(1000, 600);
    
    // Group by operation type for better visualization
    vector<string> unique_ops = {"Make-Heap", "Heap-Insert", "Heap-Union", "Extract-Min"};
    vector<string> colors = {"b", "g", "r", "m"};
    
    for (size_t i = 0; i < unique_ops.size(); ++i) {
        vector<size_t> indices;
        vector<long long> values;
        
        for (size_t j = 0; j < operation_types.size(); ++j) {
            if (operation_types[j] == unique_ops[i]) {
                indices.push_back(operation_indices[j]);
                values.push_back(comparisons[j]);
            }
        }
        
        vector<double> x(indices.begin(), indices.end());
        vector<double> y(values.begin(), values.end());
        plot(x, y, "-")->color(colors[i]).display_name(unique_ops[i]);
        hold(on);
    }

    // Calculate and plot average lines
    vector<pair<string, double>> averages;
    for (const auto& op : unique_ops) {
        double sum = 0;
        int count = 0;
        for (size_t i = 0; i < operation_types.size(); ++i) {
            if (operation_types[i] == op) {
                sum += comparisons[i];
                count++;
            }
        }
        if (count > 0) {
            averages.emplace_back(op, sum/count);
        }
    }

    for (const auto& avg : averages) {
        vector<double> x = {0, (double)operation_indices.back()};
        vector<double> y = {avg.second, avg.second};
        plot(x, y, "--")->color("k").display_name(avg.first + " avg: " + to_string(avg.second));
    }

    title("Binomial Heap Operations - Attempt " + to_string(attempt) + " (n=" + to_string(n) + ")");
    xlabel("Operation Index");
    ylabel("Number of Comparisons");
    matplot::legend();
    grid(on);
    
    string filename = "binomial_heap_attempt_" + to_string(attempt) + ".png";
    save(filename);
    cout << "Saved plot to: " << filename << endl;
}

void plot_average_comparisons() {
    vector<int> ns;
    vector<double> makeheap_avg;
    vector<double> insert_avg;
    vector<double> union_avg;
    vector<double> extractmin_avg;

    for (int n = MIN_N; n <= MAX_N; n += STEP) {
        vector<long long> makeheap_comps;
        vector<long long> insert_comps;
        vector<long long> union_comps;
        vector<long long> extractmin_comps;

        for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp) {
            // MakeHeap
            comparison_counter = 0;
            BinomialHeap H1, H2;
            makeheap_comps.push_back(comparison_counter);
            makeheap_comps.push_back(comparison_counter);

            // Insert
            auto data1 = generate_random_sequence(n);
            auto data2 = generate_random_sequence(n);
            for (int i = 0; i < n; ++i) {
                comparison_counter = 0;
                H1.insert(data1[i]);
                insert_comps.push_back(comparison_counter);

                comparison_counter = 0;
                H2.insert(data2[i]);
                insert_comps.push_back(comparison_counter);
            }

            // Union
            comparison_counter = 0;
            BinomialHeap H;
            H.head = H.unionHeaps(H1.head, H2.head);
            union_comps.push_back(comparison_counter);

            // ExtractMin
            for (int i = 0; i < 2 * n; ++i) {
                comparison_counter = 0;
                Node* minNode = H.extractMin();
                if (!minNode) break;
                extractmin_comps.push_back(comparison_counter);
                delete minNode;
            }
        }

        ns.push_back(n);
        makeheap_avg.push_back(accumulate(makeheap_comps.begin(), makeheap_comps.end(), 0.0) / makeheap_comps.size());
        insert_avg.push_back(accumulate(insert_comps.begin(), insert_comps.end(), 0.0) / insert_comps.size());
        union_avg.push_back(accumulate(union_comps.begin(), union_comps.end(), 0.0) / union_comps.size());
        extractmin_avg.push_back(accumulate(extractmin_comps.begin(), extractmin_comps.end(), 0.0) / extractmin_comps.size());

        cout << "n=" << n << " | MakeHeap: " << makeheap_avg.back() 
             << " | Insert: " << insert_avg.back() 
             << " | Union: " << union_avg.back()
             << " | ExtractMin: " << extractmin_avg.back() << endl;
    }

    auto fig = figure(true);
    fig->size(1200, 800);
    
    auto makeheap_plot = plot(ns, makeheap_avg, "-o");
    makeheap_plot->display_name("Make-Heap");
    makeheap_plot->line_width(2);
    hold(on);
    auto insert_plot = plot(ns, insert_avg, "-s");
    insert_plot->display_name("Heap-Insert");
    insert_plot->line_width(2);
    auto union_plot = plot(ns, union_avg, "-d");
    union_plot->display_name("Heap-Union");
    union_plot->line_width(2);
    auto extractmin_plot = plot(ns, extractmin_avg, "-^");
    extractmin_plot->display_name("Extract-Min");
    extractmin_plot->line_width(2);
    
    title("Binomial Heap - Average Comparisons vs Input Size");
    xlabel("Input Size (n)");
    ylabel("Average Comparisons per Operation");
    matplot::legend();
    grid(on);
    
    save("binomial_heap_averages.png");
    cout << "Saved averages plot to: binomial_heap_averages.png" << endl;
    show();
}

int main() {
    // Historical operation plots for n=500
    for (int attempt = 1; attempt <= NUM_EXPERIMENTS; ++attempt) {
        plot_operation_history(HISTORICAL_N, attempt);
    }

    // Average comparisons plot
    plot_average_comparisons();

    return 0;
}