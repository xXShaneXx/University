#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <vector>
#include <random>

inline std::vector<int> generateInput(int n) {
    std::vector<int> data(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 2 * n - 1);
    for (int& x : data) {
        x = dist(gen);
    }
    return data;
}

#endif // GENERATOR_HPP
