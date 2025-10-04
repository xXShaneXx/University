#include <iostream>
#include "BHeap.hpp"

int main() {
    BinomialHeap heap;

    heap.insert(10);
    heap.insert(3);
    heap.insert(7);
    heap.insert(21);
    heap.insert(14);

    heap.print();

    Node* minNode = heap.getMinimum();
    if (minNode) {
        std::cout << "\nMinimum: " << minNode->key << "\n";
    }

    return 0;
}