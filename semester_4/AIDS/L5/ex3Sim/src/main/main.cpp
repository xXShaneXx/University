
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

vector<int> generate_random_sequence(int n) {
    vector<int> data(n);
    iota(data.begin(), data.end(), 1);
    shuffle(data.begin(), data.end(), mt19937(random_device{}()));
    return data;
}

int main() {
    vector<int> ns;
    vector<double> avg_comparisons_per_n;

    for (int n = 100; n <= 10000; n += 100) {
        long long total_comparisons = 0;

        for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp) {
            BinomialHeap H1, H2, H;
            auto data1 = generate_random_sequence(n);
            auto data2 = generate_random_sequence(n);

            for (int i = 0; i < n; ++i) {
                H1.insert(data1[i]);
                H2.insert(data2[i]);
            }

            // Heap Union
            H.head = H.unionHeaps(H1.head, H2.head);

            vector<int> extracted;
            for (int i = 0; i < 2 * n; ++i) {
                Node* minNode = H.extractMin();
                if (!minNode) {
                    cerr << "ERROR: Tried to extract from empty heap for n = " << n << "\n";
                    return 1;
                }
                int min_val = minNode->key;
                extracted.push_back(min_val);
                delete minNode;
            }

            // Check if extracted sequence is sorted
            if (!is_sorted(extracted.begin(), extracted.end())) {
                cerr << "ERROR: Extracted sequence not sorted for n = " << n << "\n";
                return 1;
            }

            total_comparisons += comparison_counter;
            comparison_counter = 0;
        }

        ns.push_back(n);
        avg_comparisons_per_n.push_back(static_cast<double>(total_comparisons) / (n * NUM_EXPERIMENTS));
        cout << "n = " << n << ", avg comparisons/n = " << avg_comparisons_per_n.back() << "\n";
    }

    // Plot average comparison cost
    auto fig = figure(true);
    plot(ns, avg_comparisons_per_n)->line_width(2);
    xlabel("n");
    ylabel("Average comparisons per n");
    title("Binomial Heap: Avg comparison cost vs. n");
    save("comparison_vs_n.png");
    show();

    return 0;
}
