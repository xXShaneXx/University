#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "QuickSorts.hpp"
#include "logger.hpp"



class QuickSortsTest : public ::testing::Test {
protected:
    void SetUp() override {
        Logger::enabled = false;
        // Initialize test data
        unsorted = {5, 2, 9, 1, 5, 6};
        sorted = {1, 2, 5, 5, 6, 9};
    }

    std::vector<int> unsorted;
    std::vector<int> sorted;
};

TEST_F(QuickSortsTest, QuickSort_SortsCorrectly) {
    quicksorts::QuickSort(unsorted.begin(), unsorted.end());
    EXPECT_EQ(unsorted, sorted);
}

TEST_F(QuickSortsTest, DualPivotQuickSort_SortsCorrectly) {
    quicksorts::DualPivotQuickSort(unsorted.begin(), unsorted.end());
    EXPECT_EQ(unsorted, sorted);
}

TEST_F(QuickSortsTest, QuickSort_SingleElement) {
    std::vector<int> single = {42};
    quicksorts::QuickSort(single.begin(), single.end());
    EXPECT_EQ(single, std::vector<int>({42}));
}

TEST_F(QuickSortsTest, DualPivotQuickSort_SingleElement) {
    std::vector<int> single = {42};
    quicksorts::DualPivotQuickSort(single.begin(), single.end());
    EXPECT_EQ(single, std::vector<int>({42}));
}

TEST_F(QuickSortsTest, QuickSort_AlreadySorted) {
    std::vector<int> alreadySorted = {1, 2, 3, 4, 5};
    quicksorts::QuickSort(alreadySorted.begin(), alreadySorted.end());
    EXPECT_EQ(alreadySorted, std::vector<int>({1, 2, 3, 4, 5}));
}

TEST_F(QuickSortsTest, DualPivotQuickSort_AlreadySorted) {
    std::vector<int> alreadySorted = {1, 2, 3, 4, 5};
    quicksorts::DualPivotQuickSort(alreadySorted.begin(), alreadySorted.end());
    EXPECT_EQ(alreadySorted, std::vector<int>({1, 2, 3, 4, 5}));
}

TEST_F(QuickSortsTest, QuickSort_ReversedOrder) {
    std::vector<int> reversed = {9, 8, 7, 6, 5};
    quicksorts::QuickSort(reversed.begin(), reversed.end());
    EXPECT_EQ(reversed, std::vector<int>({5, 6, 7, 8, 9}));
}

TEST_F(QuickSortsTest, DualPivotQuickSort_ReversedOrder) {
    std::vector<int> reversed = {9, 8, 7, 6, 5};
    quicksorts::DualPivotQuickSort(reversed.begin(), reversed.end());
    EXPECT_EQ(reversed, std::vector<int>({5, 6, 7, 8, 9}));
}

TEST_F(QuickSortsTest, QuickSort_LargeDataset) {
    std::vector<int> large(1000);
    std::generate(large.begin(), large.end(), std::rand);
    std::vector<int> expected = large;
    std::sort(expected.begin(), expected.end());

    quicksorts::QuickSort(large.begin(), large.end());
    EXPECT_EQ(large, expected);
}

TEST_F(QuickSortsTest, DualPivotQuickSort_LargeDataset) {
    std::vector<int> large(1000);
    std::generate(large.begin(), large.end(), std::rand);
    std::vector<int> expected = large;
    std::sort(expected.begin(), expected.end());

    quicksorts::DualPivotQuickSort(large.begin(), large.end());
    EXPECT_EQ(large, expected);
}

TEST_F(QuickSortsTest, QuickSort_Duplicates) {
    std::vector<int> duplicates = {4, 2, 4, 2, 4, 2};
    std::vector<int> expected = {2, 2, 2, 4, 4, 4};
    quicksorts::QuickSort(duplicates.begin(), duplicates.end());
    EXPECT_EQ(duplicates, expected);
}

TEST_F(QuickSortsTest, DualPivotQuickSort_Duplicates) {
    std::vector<int> duplicates = {4, 2, 4, 2, 4, 2};
    std::vector<int> expected = {2, 2, 2, 4, 4, 4};
    quicksorts::DualPivotQuickSort(duplicates.begin(), duplicates.end());
    EXPECT_EQ(duplicates, expected);
}