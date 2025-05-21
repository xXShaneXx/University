#pragma once
#include <algorithm>
#include "comperer.hpp"

template <typename RandomIt>
RandomIt partition(RandomIt left, RandomIt right) {
    auto pivot = *(right - 1);
    auto i = left - 1;
    for (auto j = left; j < right - 1; ++j) {
        if (Comperer<typename RandomIt::value_type>::compare(*j, pivot) == std::strong_ordering::less) {
            ++i;
            Comperer<typename RandomIt::value_type>::move(&*i, &*j);
        }
    }
    Comperer<typename RandomIt::value_type>::move(&*(i + 1), &*(right - 1));
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