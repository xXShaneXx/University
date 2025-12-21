#ifndef BINARYSEARCH_HPP
#define BINARYSEARCH_HPP
#include "operationCounter.hpp"
#include <algorithm>

template<typename Iterator>
bool BinarySearch(Iterator left, Iterator right, const typename std::iterator_traits<Iterator>::value_type& x) {
    if (left >= right) 
        return false;

    Iterator mid = left + (right - left) / 2; 
    OperationCounter::comparisons++;
    if (*mid == x) 
        return true;

    OperationCounter::comparisons++;
    if (*mid < x) 
        return BinarySearch(mid + 1, right, x);
    else 
        return BinarySearch(left, mid, x);
}

#endif // BINARYSEARCH_HPP