#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <algorithm>

template <typename RandomIt>
RandomIt partition(RandomIt left, RandomIt right) {
    auto pivot = *(right - 1);
    auto i = left - 1;
    for (auto j = left; j < right - 1; ++j) {
        if (*j < pivot) {
            ++i;
            std::iter_swap(i, j);
        }
    }
    std::iter_swap(i + 1, right - 1);
    return i + 1;
}

template <typename RandomIt>
void quickSort(RandomIt left, RandomIt right) {
    if (left < right) {
        auto q = partition(left, right);
        quickSort(left, q);
        quickSort(q + 1, right);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " array size\n";
        return 1;
    }

    int n = std::stoi(argv[1]);
    std::vector<int> arr(n);
    std::generate(arr.begin(), arr.end(), []() {
        int x;
        std::cin >> x;
        return x;
    });
    std::vector<int> sorted_arr = arr;
    quickSort(sorted_arr.begin(), sorted_arr.end());

    std::cout << "Original array: ";
    for (const auto& num : arr) {
        std::cout << num << " ";
    }
    std::cout << "\nSorted array: ";
    for (const auto& num : sorted_arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}