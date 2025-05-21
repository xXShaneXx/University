#pragma once
#include <algorithm>
#include "comperer.hpp"

template <typename RandomIt>
std::pair<RandomIt, RandomIt> partition(RandomIt left, RandomIt right) {
    auto pivotLeft = *left;
    auto pivotRight = *(right - 1);

    // Ensure pivotLeft <= pivotRight
    if (Comperer<typename RandomIt::value_type>::compare(pivotLeft, pivotRight) == std::strong_ordering::greater) {
        Comperer<typename RandomIt::value_type>::move(&*left, &*(right - 1));
        Comperer<typename RandomIt::value_type>::move(&pivotLeft, &pivotRight);
    }

    auto i = left + 1; // Pointer for elements less than pivotLeft
    auto j = left + 1; // Pointer for elements between pivotLeft and pivotRight
    auto k = right - 2; // Pointer for elements greater than pivotRight

    while (j <= k) {
        if (Comperer<typename RandomIt::value_type>::compare(*j, pivotLeft) == std::strong_ordering::less) {
            Comperer<typename RandomIt::value_type>::move(&*i, &*j);
            ++i;
            ++j;
        } else if (Comperer<typename RandomIt::value_type>::compare(*j, pivotRight) == std::strong_ordering::greater) {
            Comperer<typename RandomIt::value_type>::move(&*j, &*k);
            --k;
        } else {
            ++j;
        }
    }

    // Place pivots in their correct positions
    Comperer<typename RandomIt::value_type>::move(&*(left), &*(i - 1));
    Comperer<typename RandomIt::value_type>::move(&*(right - 1), &*(k + 1));

    return {i - 1, k + 1}; // Return the positions of the pivots
}

template <typename RandomIt>
void DualPivotQuickSort(RandomIt left, RandomIt right) {
    if (std::distance(left, right) > 1) {
        auto [pivot1, pivot2] = partition(left, right);

        // Recursively sort the three partitions
        DualPivotQuickSort(left, pivot1);
        DualPivotQuickSort(pivot1 + 1, pivot2);
        DualPivotQuickSort(pivot2 + 1, right);
    }
}