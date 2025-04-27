#pragma once
#include <vector>
#include <compare>
#include <stdexcept>

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

    static void check(const std::vector<T>& original_vec, const std::vector<T>& sorted_vec)
    {
        // Check if sorted_vec is a sorted version of original_vec
        std::vector<T> temp = original_vec;
        std::sort(temp.begin(), temp.end());

        if (temp != sorted_vec) {
            throw std::logic_error("The sorted vector is not a correctly sorted version of the original vector.");
        }
    }

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