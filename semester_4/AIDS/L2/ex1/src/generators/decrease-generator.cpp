#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " array size\n";
        return 1;
    }

    int n = std::stoi(argv[1]);

    // Output the size of the array
    std::cout << n << "\n";

    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2 * n - 1);

    // Generate random numbers
    std::vector<int> numbers(n);
    for (int i = 0; i < n; ++i) {
        numbers[i] = dis(gen);
    }

    // Sort the numbers in descending order
    std::sort(numbers.begin(), numbers.end(), std::greater<int>());

    // Output the sorted numbers
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}