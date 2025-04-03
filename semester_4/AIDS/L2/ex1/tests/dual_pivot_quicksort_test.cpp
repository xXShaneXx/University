#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "dual-pivot-quicksort.hpp"

TEST(DualPivotQuickSortTest, HandlesEmptyVector) {
    std::vector<int> vec;
    DualPivotQuickSort(vec.begin(), vec.end());
    EXPECT_TRUE(vec.empty());
}

TEST(DualPivotQuickSortTest, HandlesSingleElement) {
    std::vector<int> vec = {1};
    DualPivotQuickSort(vec.begin(), vec.end());
    EXPECT_EQ(vec[0], 1);
}

TEST(DualPivotQuickSortTest, HandlesSortedVector) {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    DualPivotQuickSort(vec.begin(), vec.end());
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(DualPivotQuickSortTest, HandlesUnsortedVector) {
    std::vector<int> vec = {5, 4, 3, 2, 1};
    DualPivotQuickSort(vec.begin(), vec.end());
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(DualPivotQuickSortTest, HandlesDuplicateElements) {
    std::vector<int> vec = {3, 3, 2, 1, 2};
    DualPivotQuickSort(vec.begin(), vec.end());
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 2, 3, 3}));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}