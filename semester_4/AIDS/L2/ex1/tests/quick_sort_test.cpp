#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

TEST(QuickSortTest, HandlesEmptyVector) {
    std::vector<int> vec;
    quickSort(vec.begin(), vec.end());
    EXPECT_TRUE(vec.empty());
}

TEST(QuickSortTest, HandlesSingleElement) {
    std::vector<int> vec = {1};
    quickSort(vec.begin(), vec.end());
    EXPECT_EQ(vec[0], 1);
}

TEST(QuickSortTest, HandlesSortedVector) {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    quickSort(vec.begin(), vec.end());
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(QuickSortTest, HandlesUnsortedVector) {
    std::vector<int> vec = {5, 4, 3, 2, 1};
    quickSort(vec.begin(), vec.end());
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(QuickSortTest, HandlesDuplicateElements) {
    std::vector<int> vec = {3, 3, 2, 1, 2};
    quickSort(vec.begin(), vec.end());
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 2, 3, 3}));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}