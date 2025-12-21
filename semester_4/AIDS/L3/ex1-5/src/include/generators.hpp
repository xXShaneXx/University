#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <vector>
#include <random>
#include <algorithm>

inline std::vector<int> generateRandomInput(int n) {
    std::vector<int> data(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 2 * n - 1);
    for (int& x : data) {
        x = dist(gen);
    }
    return data;
}

// Generuje niemalejący ciąg liczb z zakresu [0, 2n - 1]
inline std::vector<int> generateNonDecreasingInput(int n) {
    std::vector<int> data = generateRandomInput(n);
    std::sort(data.begin(), data.end());
    return data;
}

#endif // GENERATOR_HPP
