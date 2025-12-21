#include <iostream>
#include <vector>
#include <algorithm>
#include "quick-sort.hpp"
#include "SortUtility.hpp"
#include "comperer.hpp"

int main(int argc, char* argv[]) {

    int n;
    if (!(std::cin >> n)) {
        std::cerr << "Error: Failed to read array size from input.\n";
        return 1;
    }
    std::vector<int> arr = SortUtility::readInput(n);
    std::vector<int> sorted_arr = arr;

    if (n < 40) {
        SortUtility::displayArray(arr, "Original array:");
    }

    Comperer<int>::resetCounters();
    quickSort(sorted_arr.begin(), sorted_arr.end());

    if (n < 40) {
        SortUtility::displayArray(sorted_arr, "Sorted array:");
    }

    Comperer<int>::check(arr, sorted_arr);
    SortUtility::displayStatistics();

    return 0;
}