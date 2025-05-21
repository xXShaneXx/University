#pragma once
#include "quick-sort.hpp"
#include "insertion-sort.hpp"

template <typename RandomIt>
void hybridSort(RandomIt left, RandomIt right, size_t threshold = 10) {
    if (std::distance(left, right) <= static_cast<ptrdiff_t>(threshold)) {
        std::vector<typename RandomIt::value_type> temp(left, right);
        insertionSort(temp);
        std::copy(temp.begin(), temp.end(), left);
    } else {
        auto q = partition(left, right);
        hybridSort(left, q, threshold);
        hybridSort(q + 1, right, threshold);
    }
}