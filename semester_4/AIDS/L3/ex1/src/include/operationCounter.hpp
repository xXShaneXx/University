#ifndef OPERATION_COUNTER_HPP
#define OPERATION_COUNTER_HPP

#include <utility>
#include <iostream>

class OperationCounter {
public:
    inline static int comparisons = 0;
    inline static int swaps = 0;

    template <typename T>
    static bool compare(const T& a, const T& b) {
        ++comparisons;
        return a < b;
    }

    template <typename T>
    static void swap(T& a, T& b) {
        ++swaps;
        std::swap(a, b);
    }

    static void reset() {
        comparisons = 0;
        swaps = 0;
    }

    static void print() {
        std::cout << "Porównania: " << comparisons << ", Przestawienia: " << swaps << '\n';
    }
};

#endif // OPERATION_COUNTER_HPP
