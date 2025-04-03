#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "hybrid-sort.hpp" // Updated header file

TEST(HybridSortTest, HandlesEmptyVector) {
    std::vector<int> vec;
    hybridSort(vec.begin(), vec.end(), 4);
    EXPECT_TRUE(vec.empty());
}

TEST(HybridSortTest, HandlesSingleElement) {
    std::vector<int> vec = {1};
    hybridSort(vec.begin(), vec.end(), 4);
    EXPECT_EQ(vec[0], 1);
}

TEST(HybridSortTest, HandlesSortedVector) {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    hybridSort(vec.begin(), vec.end(), 4);
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(HybridSortTest, HandlesUnsortedVector) {
    std::vector<int> vec = {5, 4, 3, 2, 1};
    hybridSort(vec.begin(), vec.end(), 4);
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(HybridSortTest, HandlesDuplicateElements) {
    std::vector<int> vec = {3, 3, 2, 1, 2};
    hybridSort(vec.begin(), vec.end(), 4);
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 2, 3, 3}));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}