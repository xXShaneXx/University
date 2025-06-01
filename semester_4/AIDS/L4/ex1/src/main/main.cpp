#include "Node.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

void demonstrateScenario1(int n) {
    std::cout << "\n=== SCENARIUSZ 1 ===" << std::endl;
    std::cout << "Wstawianie rosnącego ciągu 1.." << n << ", następnie usuwanie losowej permutacji\n" << std::endl;
    
    std::unique_ptr<Node> root = nullptr;
    
    // Wstawianie rosnącego ciągu
    for (int i = 1; i <= n; ++i) {
        std::cout << "insert " << i << std::endl;
        root = insert(std::move(root), i);
        printTree(root.get());
        std::cout << "\nWysokość drzewa: " << height(root.get()) << "\n" << std::endl;
    }
    
    // Generowanie losowej permutacji do usuwania
    std::vector<int> permutation(n);
    for (int i = 0; i < n; ++i) permutation[i] = i + 1;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(permutation.begin(), permutation.end(), std::default_random_engine(seed));
    
    // Usuwanie losowej permutacji
    for (int val : permutation) {
        std::cout << "delete " << val << std::endl;
        root = deleteNode(std::move(root), val);
        printTree(root.get());
        if (root) {
            std::cout << "\nWysokość drzewa: " << height(root.get()) << "\n" << std::endl;
        } else {
            std::cout << "\nDrzewo jest puste\n" << std::endl;
        }
    }
}

void demonstrateScenario2(int n) {
    std::cout << "\n=== SCENARIUSZ 2 ===" << std::endl;
    std::cout << "Wstawianie losowej permutacji 1.." << n << ", następnie usuwanie losowej permutacji\n" << std::endl;
    
    std::unique_ptr<Node> root = nullptr;
    
    // Generowanie losowej permutacji do wstawiania
    std::vector<int> insertPermutation(n);
    for (int i = 0; i < n; ++i) insertPermutation[i] = i + 1;
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(insertPermutation.begin(), insertPermutation.end(), std::default_random_engine(seed1));
    
    // Wstawianie losowej permutacji
    for (int val : insertPermutation) {
        std::cout << "insert " << val << std::endl;
        root = insert(std::move(root), val);
        printTree(root.get());
        std::cout << "\nWysokość drzewa: " << height(root.get()) << "\n" << std::endl;
    }
    
    // Generowanie losowej permutacji do usuwania
    std::vector<int> deletePermutation(n);
    for (int i = 0; i < n; ++i) deletePermutation[i] = i + 1;
    unsigned seed2 = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(deletePermutation.begin(), deletePermutation.end(), std::default_random_engine(seed2));
    
    // Usuwanie losowej permutacji
    for (int val : deletePermutation) {
        std::cout << "delete " << val << std::endl;
        root = deleteNode(std::move(root), val);
        printTree(root.get());
        if (root) {
            std::cout << "\nWysokość drzewa: " << height(root.get()) << "\n" << std::endl;
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