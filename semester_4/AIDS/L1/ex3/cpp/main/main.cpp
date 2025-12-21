#include <cstdio>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "LinkedCircularList.h"

int main() 
{

    // Create two LinkedCircularLists with 10 non-negative numbers each
    LinkedCircularList list1;
    LinkedCircularList list2;

    for (int i = 0; i < 10; ++i)
    {
        insert(list1, std::rand() % 100);
        insert(list2, std::rand() % 100);
    }

    // Merge the two lists
    LinkedCircularList mergedList = merge(list1, list2);

    // Print the merged list
    std::cout << "Merged list: ";
    printList(mergedList);
    std::cout << std::endl;

    std::srand(std::time(nullptr));

    std::vector<int> T(10000);
    for (int& num : T)
    {
        num = std::rand() % 100001;
    }

    LinkedCircularList L;
    for (int num : T)
    {
        insert(L, num);
    }

    // Perform 1,000 random searches for numbers from the interval [0, 100000]
    int totalComparisons = 0;
    for (int i = 0; i < 1000; ++i)
    {
        int randomNum = std::rand() % 100001;
        totalComparisons += search(L, randomNum);
    }

    // Calculate the average cost of these searches
    double averageCost = static_cast<double>(totalComparisons) / 1000.0;
    std::cout << "Average cost of 1000 random searches: " << averageCost << " comparisons" << std::endl;

    // Perform 1,000 random searches for numbers that are in the list (chosen randomly from T)
    totalComparisons = 0;
    for (int i = 0; i < 1000; ++i)
    {
        int randomIndex = std::rand() % 10000;
        totalComparisons += search(L, T[randomIndex]);
    }

    // Calculate the average cost of these searches
    averageCost = static_cast<double>(totalComparisons) / 1000.0;
    std::cout << "Average cost of 1000 searches for numbers in the list: " << averageCost << " comparisons" << std::endl;

    return 0;
}