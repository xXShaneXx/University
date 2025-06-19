#include "SplayTree.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

void demonstrateScenario1(int n) {
    std::cout << "\n=== SCENARIUSZ 1 ===" << std::endl;
    std::cout << "Wstawianie rosnącego ciągu 1.." << n << ", następnie usuwanie losowej permutacji\n" << std::endl;
    
    SplayTree tree;
    
    // Wstawianie rosnącego ciągu
    for (int i = 1; i <= n; ++i) {
        std::cout << "insert " << i << std::endl;
        tree.insert(i);
        printTree(tree.root);
        std::cout << "\nWysokość drzewa: " << tree.height() << "\n" << std::endl;
    }
    
    // Generowanie losowej permutacji do usuwania
    std::vector<int> permutation(n);
    for (int i = 0; i < n; ++i) permutation[i] = i + 1;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(permutation.begin(), permutation.end(), std::default_random_engine(seed));
    
    // Usuwanie losowej permutacji
    for (int val : permutation) {
        std::cout << "delete " << val << std::endl;
        tree.remove(val);
        printTree(tree.root);
        if (tree.root) {
            std::cout << "\nWysokość drzewa: " << tree.height() << "\n" << std::endl;
        } else {
            std::cout << "\nDrzewo jest puste\n" << std::endl;
        }
    }
}

void demonstrateScenario2(int n) {
    std::cout << "\n=== SCENARIUSZ 2 ===" << std::endl;
    std::cout << "Wstawianie losowej permutacji 1.." << n << ", następnie usuwanie losowej permutacji\n" << std::endl;
    
    SplayTree tree;
    
    // Generowanie losowej permutacji do wstawiania
    std::vector<int> insertPermutation(n);
    for (int i = 0; i < n; ++i) insertPermutation[i] = i + 1;
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(insertPermutation.begin(), insertPermutation.end(), std::default_random_engine(seed1));
    
    // Wstawianie losowej permutacji
    for (int val : insertPermutation) {
        std::cout << "insert " << val << std::endl;
        tree.insert(val);
        printTree(tree.root);
        std::cout << "\nWysokość drzewa: " << tree.height() << "\n" << std::endl;
    }
    
    // Generowanie losowej permutacji do usuwania
    std::vector<int> deletePermutation(n);
    for (int i = 0; i < n; ++i) deletePermutation[i] = i + 1;
    unsigned seed2 = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(deletePermutation.begin(), deletePermutation.end(), std::default_random_engine(seed2));
    
    // Usuwanie losowej permutacji
    for (int val : deletePermutation) {
        std::cout << "delete " << val << std::endl;
        tree.remove(val);
        printTree(tree.root);
        if (tree.root) {
            std::cout << "\nWysokość drzewa: " << tree.height() << "\n" << std::endl;
        } else {
            std::cout << "\nDrzewo jest puste\n" << std::endl;
        }
    }
}

int main() {
    const int n = 30;
    
    // Demonstracja scenariusza 1
    demonstrateScenario1(n);
    
    // Demonstracja scenariusza 2
    demonstrateScenario2(n);
    
    return 0;
}