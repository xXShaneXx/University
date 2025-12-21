#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <functional>
#include "logger.hpp"
#include "operationCounter.hpp"  

namespace selects {

    // RANDOMIZED PARTITION
    template<typename Iterator>
    Iterator RandomPartition(Iterator p, Iterator q) {
        Logger::logArray(p, q, "Przed RandomPartition");

        Iterator r = p + (std::rand() % (q - p));
        OperationCounter::swap(*r, *(q - 1));
        Iterator pivot = q - 1;
        Iterator i = p;

        for (Iterator j = p; j < pivot; ++j) {
            if (OperationCounter::compare(*j, *pivot)) {
                OperationCounter::swap(*i, *j);
                ++i;
            }
        }

        OperationCounter::swap(*i, *pivot);
        Logger::logArray(p, q, "Po RandomPartition");
        return i;
    }

    // RANDOMIZED SELECT
    template<typename Iterator>
    typename std::iterator_traits<Iterator>::value_type RandomSelect(Iterator p, Iterator q, int i) {
        Logger::logArray(p, q, "RandomSelect - aktualny zakres");

        if (q - p == 1)
            return *p;

        Iterator r = RandomPartition(p, q);
        int k = r - p + 1;

        if (i == k)
            return *r;
        else if (i < k)
            return RandomSelect(p, r, i);
        else
            return RandomSelect(r + 1, q, i - k);
    }

    template<typename Iterator>
    std::pair<Iterator, Iterator> Partition(Iterator p, Iterator q, 
        std::function<bool(const typename std::iterator_traits<Iterator>::value_type&)> pred_equal, 
        std::function<bool(const typename std::iterator_traits<Iterator>::value_type&)> pred_less) {
        Iterator less = p;
        Iterator equal = p;
    
        for (Iterator it = p; it != q; ++it) {
            if (pred_less(*it)) {
                OperationCounter::swap(*it, *equal);
                OperationCounter::swap(*equal, *less);
                ++less;
                ++equal;
            } else if (pred_equal(*it)) {
                OperationCounter::swap(*it, *equal);
                ++equal;
            }
        }
        return {less, equal}; // [p, less) < pivot, [less, equal) == pivot, [equal, q) > pivot
    }

    
    template<typename Iterator>
    typename std::iterator_traits<Iterator>::value_type Select(Iterator p, Iterator q, int i, int part_size=5) {
        Logger::logArray(p, q, "Select - aktualny zakres");
        
        using ValueType = typename std::iterator_traits<Iterator>::value_type;
    
        int n = q - p;
        if (n <= part_size) {
            std::sort(p, q, [](const ValueType& a, const ValueType& b) {
                return OperationCounter::compare(a, b);
            });
            Logger::logArray(p, q, "Select - posortowany mały zakres");
            return *(p + i - 1);  // 1-based indexing
        }
    
        std::vector<ValueType> medians;
        for (Iterator it = p; it < q; it += part_size) {
            Iterator group_end = (it + part_size < q) ? it + part_size : q;
            std::sort(it, group_end, [](const ValueType& a, const ValueType& b) {
                return OperationCounter::compare(a, b);
            });
            medians.push_back(*(it + (group_end - it - 1) / 2));
        }

        Logger::logArray(p, q, "Select - posortowany duży zakres");
        Logger::logArray(medians.begin(), medians.end(), "Select - mediany grup");
    
        ValueType median_of_medians = Select(medians.begin(), medians.end(), (medians.size() + 1) / 2);
    
        auto [low, high] = Partition(p, q,
            [&](const ValueType& val) { return val == median_of_medians; },
            [&](const ValueType& val) { return OperationCounter::compare(val, median_of_medians); }
        );

        Logger::logArray(p, low, "Select - mniejsze od pivota");
        Logger::logArray(low, high, "Select - równe pivotowi");
        Logger::logArray(high, q, "Select - większe od pivota");
    
        int k = low - p + 1;
        int m = high - p;  // high is exclusive
    
        if (i >= k && i <= m)
            return median_of_medians;
        else if (i < k)
            return Select(p, low, i);
        else
            return Select(high, q, i - m);
    }
    
    

} // namespace selects

#endif // FUNCTIONS_HPP
