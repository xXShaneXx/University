#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <algorithm>

void insertionSort(std::vector<int>& arr) {
    for(auto it = arr.begin() + 1; it != arr.end(); ++it)
    {
        auto key = *it;
        auto j = it - 1;
        while(j >= arr.begin() && *j > key)
        {
            *(j + 1) = *j;
            --j;
        }
        *(j + 1) = key;
    }
}

int main(int argc, char* argv[]){
    if(argc != 2){
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
    insertionSort(sorted_arr);

    std::cout << "Original array: ";
    for(const auto& num : arr) {
        std::cout << num << " ";
    }
    std::cout << "\nSorted array: ";
    for(const auto& num : sorted_arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;



    return 0;
}