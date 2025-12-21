#ifndef SPLAYTREE_HPP
#define SPLAYTREE_HPP

#include <iostream>
#include <algorithm>

struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

struct OperationCounts {
    long comparisons = 0;
    long pointer_ops = 0;
    long rotations = 0;
    int height = 0;
};

class SplayTree {
private:
    Node* rightRotate(Node* x, OperationCounts& counts);
    Node* leftRotate(Node* x, OperationCounts& counts);
    Node* splay(Node* root, int key, OperationCounts& counts);
    Node* insert(Node* node, int key, OperationCounts& counts);
    Node* deleteKey(Node* root, int key, OperationCounts& counts);
    int height(Node* node) const;

public:
    Node* root = nullptr;
    OperationCounts lastOpCounts;
    
    void insert(int key, OperationCounts& counts);
    void remove(int key, OperationCounts& counts);
    int height() const;
    OperationCounts getLastOperationCounts() const { return lastOpCounts; }
};

void printTree(const Node* root, const std::string& prefix = "", bool isLeft = false);

#endif