#include <iostream>
#include <vector>
#include <algorithm>
#include "insertion-sort.hpp"
#include "SortUtility.hpp"
#include "comperer.hpp"

int main() {
    int n;

    // Read the size of the array from standard input
    if (!(std::cin >> n)) {
        std::cerr << "Error: Failed to read array size from input.\n";
        return 1;
    }

    // Read the array elements
    std::vector<int> arr = SortUtility::readInput(n);

    if (n < 40) {
        SortUtility::displayArray(arr, "Original array:");
    }

    Comperer<int>::resetCounters();
    insertionSort(arr);

    if (n < 40) {
        SortUtility::displayArray(arr, "Sorted array:");
    }

    SortUtility::displayStatistics();

    return 0;
}