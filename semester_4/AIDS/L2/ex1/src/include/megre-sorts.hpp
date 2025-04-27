#pragma once
#include "comperer.hpp"
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>

template <typename RandomIt>
void merge(RandomIt left, RandomIt mid, RandomIt right) {
    // Create temporary vectors to hold the left and right subarrays
    std::vector<typename RandomIt::value_type> leftSubarray(left, mid);
    std::vector<typename RandomIt::value_type> rightSubarray(mid, right);

    // Indices for iterating through the temporary arrays
    auto leftIndex = leftSubarray.begin();
    auto rightIndex = rightSubarray.begin();

    // Iterator for the merged array
    auto mergedIndex = left;

    // Merge the two subarrays
    while (leftIndex != leftSubarray.end() && rightIndex != rightSubarray.end()) {
        if (Comperer<typename RandomIt::value_type>::compare(*leftIndex, *rightIndex) != std::strong_ordering::greater) {
            Comperer<typename RandomIt::value_type>::move(&*mergedIndex, &*leftIndex);
            ++leftIndex;
        } else {
            Comperer<typename RandomIt::value_type>::move(&*mergedIndex, &*rightIndex);
            ++rightIndex;
        }
        ++mergedIndex;
    }

    // Copy any remaining elements from the left subarray
    while (leftIndex != leftSubarray.end()) {
        Comperer<typename RandomIt::value_type>::move(&*mergedIndex, &*leftIndex);
        ++leftIndex;
        ++mergedIndex;
    }

    // Copy any remaining elements from the right subarray
    while (rightIndex != rightSubarray.end()) {
        Comperer<typename RandomIt::value_type>::move(&*mergedIndex, &*rightIndex);
        ++rightIndex;
        ++mergedIndex;
    }
}

template <typename RandomIt>
void merge_sort(RandomIt begin, RandomIt end) {
    // Base case: If the range has one or no elements, it is already sorted
    if (std::distance(begin, end) <= 1) {
        return;
    }

    // Find the middle point of the range
    RandomIt mid = begin + std::distance(begin, end) / 2;

    // Recursively sort the left and right halves
    merge_sort(begin, mid);
    merge_sort(mid, end);

    // Merge the sorted halves
    merge(begin, mid, end);
}

template <typename RandomIt>
void smart_merge_sort(RandomIt begin, RandomIt end) {
    if(begin == end)
        return;
        
    using Run = std::pair<RandomIt, RandomIt>;
    std::list<Run> runs;

    // 1. Znajdowanie rosnących podciągów (runs)
    RandomIt runStart = begin;
    for (RandomIt it = std::next(begin); it != end; ++it) {
        if (Comperer<typename RandomIt::value_type>::compare(*(it - 1), *it) == std::strong_ordering::greater) {
            runs.emplace_back(runStart, it);
            runStart = it;
        }
    }
    runs.emplace_back(runStart, end);

    // 2. Główna pętla: scalanie sąsiednich najkrótszych runów
    while (runs.size() > 1) {
        auto bestPair = runs.begin();
        auto minCost = std::distance(bestPair->first, bestPair->second) + 
                       std::distance(std::next(bestPair)->first, std::next(bestPair)->second);

        auto it = runs.begin();
        for (auto next = std::next(it); next != runs.end(); ++it, ++next) {
            auto cost = std::distance(it->first, it->second) + std::distance(next->first, next->second);
            if (cost < minCost) {
                minCost = cost;
                bestPair = it;
            }
        }

        auto next = std::next(bestPair);
        merge(bestPair->first, next->first, next->second);
        bestPair->second = next->second;
        runs.erase(next);
    }
}



