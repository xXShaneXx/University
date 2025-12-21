#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "insertion-sort.hpp"

TEST(InsertionSortTest, HandlesEmptyVector) {
    std::vector<int> vec;
    insertionSort(vec);
    EXPECT_TRUE(vec.empty());
}

TEST(QuickSortTest, HandlesSingleElement) {
    std::vector<int> vec = {1};
    insertionSort(vec);
    EXPECT_EQ(vec[0], 1);
}

TEST(InsertionSortTest, HandlesSortedVector) {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    insertionSort(vec);
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(InsertionSortTest, HandlesUnsortedVector) {
    std::vector<int> vec = {5, 4, 3, 2, 1};
    insertionSort(vec);
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(InsertionSortTest, HandlesDuplicateElements) {
    std::vector<int> vec = {3, 3, 2, 1, 2};
    insertionSort(vec);
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 2, 3, 3}));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}