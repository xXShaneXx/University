#include <iostream>
#include <vector>
#include <algorithm>
#include "megre-sorts.hpp"
#include "SortUtility.hpp"
#include "comperer.hpp"

int main(int argc, char* argv[]) {
    int n;
    if (!(std::cin >> n)) {
        std::cerr << "Error: Failed to read array size from input.\n";
        return 1;
    }

    // Read the input array
    std::vector<int> arr = SortUtility::readInput(n);
    std::vector<int> sorted_arr = arr;

    // Display the original array if n < 40
    if (n < 40) {
        SortUtility::displayArray(arr, "Original array:");
    }

    // Reset comparison and move counters
    Comperer<int>::resetCounters();

    // Sort the array using smart_merge_sort
    smart_merge_sort(sorted_arr.begin(), sorted_arr.end());

    // Display the sorted array if n < 40
    if (n < 40) {
        SortUtility::displayArray(sorted_arr, "Sorted array:");
    }

    // Verify the sorting and display statistics
    Comperer<int>::check(arr, sorted_arr);
    SortUtility::displayStatistics();

    return 0;
}