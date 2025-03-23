#pragma once
#include <vector>
#include <compare>

template <typename T>
class Comperer {
public:
    static void resetCounters() {
        comparsion = 0;
        moves = 0;
    }

    static std::strong_ordering compare(const T& x1, const T& x2) {
        comparsion++;
        if (x1 < x2) {
            return std::strong_ordering::less;
        } else if (x1 > x2) {
            return std::strong_ordering::greater;
        } else {
            return std::strong_ordering::equal;
        }
    }

    static void move(T* x1, T* x2) {
        moves++;
        std::swap(*x1, *x2);
    }

    static void check(const std::vector<T>& original_vec, const std::vector<T>& sorted_vec);

    static int getMoves() { return moves; }
    static int getComparsion() { return comparsion; }

private:
    static int comparsion;
    static int moves;
};

// Initialize static member variables
template <typename T>
int Comperer<T>::comparsion = 0;

template <typename T>
int Comperer<T>::moves = 0;