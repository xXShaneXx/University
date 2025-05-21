#pragma once
#include <algorithm>
#include "operationCounter.hpp"

namespace dual_pivot {

template <typename RandomIt>
std::pair<RandomIt, RandomIt> partition(RandomIt left, RandomIt right) {
    auto pivotLeft = *left;
    auto pivotRight = *(right - 1);

    // Ensure pivotLeft <= pivotRight
    if (OperationCounter::compare(pivotLeft, pivotRight)) {
        OperationCounter::swap(*left, *(right - 1));
        std::swap(pivotLeft, pivotRight);
    }

    auto i = left + 1; // Pointer for elements less than pivotLeft
    auto j = left + 1; // Pointer for elements between pivotLeft and pivotRight
    auto k = right - 2; // Pointer for elements greater than pivotRight

    while (j <= k) {
        if (OperationCounter::compare(*j, pivotLeft)) {
            OperationCounter::swap(*i, *j);
            ++i;
            ++j;
        } else if (!OperationCounter::compare(*j, pivotRight)) {
            OperationCounter::swap(*j, *k);
            --k;
        } else {
            ++j;
        }
    }

    // Place pivots in their correct positions
    OperationCounter::swap(*left, *(i - 1));
    OperationCounter::swap(*(right - 1), *(k + 1));

    return {i - 1, k + 1}; // Return the positions of the pivots
}

template <typename RandomIt>
void DualPivotQuickSort(RandomIt left, RandomIt right) {
    if (std::distance(left, right) > 1) {
        auto [pivot1, pivot2] = dual_pivot::partition(left, right);

        // Recursively sort the three partitions
        DualPivotQuickSort(left, pivot1);
        DualPivotQuickSort(pivot1 + 1, pivot2);
        DualPivotQuickSort(pivot2 + 1, right);
    }
}

} // namespace dual_pivot