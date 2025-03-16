#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <algorithm>

void insertionSort(std::vector<int>& arr) {
    for(int i=1; i<arr.size(); i++)
    {
        int j = i-1;
        int key = arr[i];
        while(j >= 0 && arr[j] > key)
        {
            arr[j+1] = arr[j];
            j--;  
        }
        arr[j+1] = key;
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