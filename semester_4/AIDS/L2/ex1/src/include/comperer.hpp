#pragma once
#include <vector>

template <typename T>
class Comperer{
public:
    Comperer() : comparsion(0), moves(0) {}
    bool compare(T& x1, T& x2);
    void move(T* x1, T* x2);
    void check(const std::vector<T>& original_vec, const std::vector<T>& sorted_vec);
    int getMoves() { return moves; }
    int getComparsion() { return comparsion; }
private:
    int comparsion;
    int moves;
};