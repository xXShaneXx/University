#ifndef QUICKSORTS_HPP
#define QUICKSORTS_HPP

#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <functional>
#include "operationCounter.hpp"
#include "logger.hpp"
#include "selects.hpp" 

namespace quicksorts {

    // Partition using a pivot
    template<typename Iterator>
    Iterator Partition(Iterator p, Iterator q, const typename std::iterator_traits<Iterator>::value_type& pivot) {
        Logger::logArray(p, q, "Before Partition");

        Iterator i = p;
        Iterator j = q - 1;

        while (i <= j) {
            while (OperationCounter::compare(*i, pivot)) ++i; // i < pivot
            while (OperationCounter::compare(pivot, *j)) --j; // pivot < j

            if (i <= j) {
                OperationCounter::swap(*i, *j);
                ++i;
                --j;
            }
        }

        Logger::logArray(p, q, "After Partition");
        return i; // Return the partition point
    }

    // QuickSort Implementation with Select
    template<typename Iterator>
    void QuickSort(Iterator p, Iterator q) {
        if (p < q - 1) { // Base case: range has 1 or no elements
            int n = q - p;
            auto pivot = selects::Select(p, q, (n + 1) / 2); // Median-of-medians

            Iterator partitionPoint = Partition(p, q, pivot);

            QuickSort(p, partitionPoint);
            QuickSort(partitionPoint, q);
        }
    }

    // Dual-Pivot Partition using Select
template<typename Iterator>
std::pair<Iterator, Iterator> DualPivotPartition(Iterator p, Iterator q, 
                                                 const typename std::iterator_traits<Iterator>::value_type& pivot1,
                                                 const typename std::iterator_traits<Iterator>::value_type& pivot2) {
    Logger::logArray(p, q, "Before DualPivotPartition");

    Iterator i = p;
    Iterator j = p;
    Iterator k = q - 1;

    while (j <= k) {
        if (OperationCounter::compare(*j, pivot1)) { // j < pivot1
            OperationCounter::swap(*i, *j);
            ++i;
            ++j;
        } else if (OperationCounter::compare(pivot2, *j)) { // j > pivot2
            OperationCounter::swap(*j, *k);
            --k;
        } else {
            ++j;
        }
    }

    Logger::logArray(p, q, "After DualPivotPartition");
    return {i, k + 1}; // Return iterators to the boundaries of the partitions
}

// Dual-Pivot QuickSort Implementation with Select
template<typename Iterator>
void DualPivotQuickSort(Iterator p, Iterator q) {
    if (p < q - 1) { // Base case: range has 1 or no elements
        int n = q - p;
        
        // For small arrays, fall back to regular QuickSort
        if (n < 10) {
            quicksorts::QuickSort(p, q);
            return;
        }

        // Use Select to find two pivots
        auto pivot1 = selects::Select(p, q, n / 4);       // First pivot (lower quartile)
        auto pivot2 = selects::Select(p, q, 3 * n / 4);   // Second pivot (upper quartile)

        // Ensure pivot1 <= pivot2
        if (OperationCounter::compare(pivot2, pivot1)) {
            std::swap(pivot1, pivot2);
        }

        // If all elements are equal, no need to sort
        if (!OperationCounter::compare(pivot1, pivot2)) {
            return;
        }

        // Partition the range into three parts
        auto [low, high] = DualPivotPartition(p, q, pivot1, pivot2);

        // Recursively sort the three partitions
        DualPivotQuickSort(p, low);       // Elements less than pivot1
        DualPivotQuickSort(low, high);     // Elements between pivot1 and pivot2
        DualPivotQuickSort(high, q);       // Elements greater than pivot2
    }
}

} // namespace quicksorts

#endif