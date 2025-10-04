#ifndef BHEAP_HPP
#define BHEAP_HPP

#include <iostream>
#include <climits>

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
    Node* head;

    void binomialLink(Node* y, Node* z);
    Node* merge(Node* h1, Node* h2);

public:
    BinomialHeap();

    Node* getMinimum();
    void insert(int key);
    Node* unionHeaps(Node* h1, Node* h2);
    void mergeWith(BinomialHeap& other);
    Node* extractMin();
    void decreaseKey(Node* x, int newKey);
    void deleteNode(Node* x);
    void print();
    void printTree(Node* node);
};

#endif