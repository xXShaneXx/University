#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "megre-sorts.hpp"

// Test for merge_sort
TEST(MergeSortTest, HandlesEmptyVector) {
    std::vector<int> vec;
    merge_sort(vec.begin(), vec.end());
    EXPECT_TRUE(vec.empty());
}

TEST(MergeSortTest, HandlesSingleElement) {
    std::vector<int> vec = {1};
    merge_sort(vec.begin(), vec.end());
    EXPECT_EQ(vec[0], 1);
}

TEST(MergeSortTest, HandlesSortedVector) {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    merge_sort(vec.begin(), vec.end());
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(MergeSortTest, HandlesUnsortedVector) {
    std::vector<int> vec = {5, 4, 3, 2, 1};
    merge_sort(vec.begin(), vec.end());
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(MergeSortTest, HandlesDuplicateElements) {
    std::vector<int> vec = {3, 3, 2, 1, 2};
    merge_sort(vec.begin(), vec.end());
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 2, 3, 3}));
}

// Test for smart_merge_sort
TEST(SmartMergeSortTest, HandlesEmptyVector) {
    std::vector<int> vec;
    smart_merge_sort(vec.begin(), vec.end());
    EXPECT_TRUE(vec.empty());
}

TEST(SmartMergeSortTest, HandlesSingleElement) {
    std::vector<int> vec = {1};
    smart_merge_sort(vec.begin(), vec.end());
    EXPECT_EQ(vec[0], 1);
}

TEST(SmartMergeSortTest, HandlesSortedVector) {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    smart_merge_sort(vec.begin(), vec.end());
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(SmartMergeSortTest, HandlesUnsortedVector) {
    std::vector<int> vec = {5, 4, 3, 2, 1};
    smart_merge_sort(vec.begin(), vec.end());
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(SmartMergeSortTest, HandlesDuplicateElements) {
    std::vector<int> vec = {3, 3, 2, 1, 2};
    smart_merge_sort(vec.begin(), vec.end());
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 2, 3, 3}));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}