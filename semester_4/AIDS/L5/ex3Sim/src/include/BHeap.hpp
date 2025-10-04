#ifndef BHEAP_HPP
#define BHEAP_HPP

#include <iostream>
#include <climits>

extern long long comparison_counter;

struct Node {
    int key;
    int degree;
    Node* parent;
    Node* child;
    Node* sibling;

    Node(int value);
};

class BinomialHeap {
private:

    void binomialLink(Node* y, Node* z);
    Node* merge(Node* h1, Node* h2);

public:
    Node* head;
    BinomialHeap();

    Node* getMinimum();
    void insert(int key);
    Node* unionHeaps(Node* h1, Node* h2);
    void mergeWith(BinomialHeap& other);
    Node* extractMin();
    void decreaseKey(Node* x, int newKey);
    void deleteNode(Node* x);
    bool isEmpty() const;
    void print();
    void printTree(Node* node);
};

#endif