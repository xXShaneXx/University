#include <iostream>
#include <vector>
#include <algorithm>
#include "hybrid-sort.hpp"
#include "SortUtility.hpp"
#include "comperer.hpp"

int main(int argc, char* argv[]) {
    int n;
    if (!(std::cin >> n)) {
        std::cerr << "Error: Failed to read array size from input.\n";
        return 1;
    }
    std::vector<int> arr = SortUtility::readInput(n);

    if (n < 40) {
        SortUtility::displayArray(arr, "Original array:");
    }

    Comperer<int>::resetCounters();
    hybridSort(arr.begin(), arr.end(), 4);

    if (n < 40) {
        SortUtility::displayArray(arr, "Sorted array:");
    }

    SortUtility::displayStatistics();

    return 0;
}