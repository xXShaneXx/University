#pragma once
#include <algorithm>
#include "operationCounter.hpp"

template <typename RandomIt>
RandomIt partition(RandomIt left, RandomIt right) {
    auto pivot = *(right - 1);
    auto i = left - 1;
    for (auto j = left; j < right - 1; ++j) {
        if (OperationCounter::compare(*j, pivot)) {
            ++i;
            OperationCounter::swap(*i, *j);
        }
    }
    OperationCounter::swap(*(i + 1), *(right - 1));
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