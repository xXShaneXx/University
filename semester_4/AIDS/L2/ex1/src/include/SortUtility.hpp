#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include "comperer.hpp"

class SortUtility {
public:
    static std::vector<int> readInput(int n) {
        std::vector<int> arr(n);
        for (int i = 0; i < n; ++i) {
            std::cin >> arr[i];
        }
        return arr;
    }

    static void displayArray(const std::vector<int>& arr, const std::string& message = "") {
        if (!message.empty()) {
            std::cout << message << std::endl;
        }
        for (const auto& num : arr) {
            std::cout << std::setw(2) << num << " ";
        }
        std::cout << std::endl;
    }

    static void displayStatistics() {
        std::cout << "Total comparisons: " << Comperer<int>::getComparsion() << std::endl;
        std::cout << "Total moves: " << Comperer<int>::getMoves() << std::endl;
    }
};