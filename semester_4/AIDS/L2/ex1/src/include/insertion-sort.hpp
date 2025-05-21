#pragma once
#include <vector>
#include "comperer.hpp"

template <typename T>
void insertionSort(std::vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        T key = arr[i];
        size_t j = i;
        while (j > 0 && Comperer<T>::compare(arr[j - 1], key) == std::strong_ordering::greater) {
            Comperer<T>::move(&arr[j], &arr[j - 1]);
            --j;
        }
        Comperer<T>::move(&arr[j], &key);
    }
}