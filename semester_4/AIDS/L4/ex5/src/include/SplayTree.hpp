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

class SplayTree {
private:
    Node* rightRotate(Node* x);

    Node* leftRotate(Node* x);

    Node* splay(Node* root, int key);

    Node* insert(Node* node, int key);

    Node* deleteKey(Node* root, int key);

    int height(Node* node) const;

public:
    Node* root = nullptr;
    
    void insert(int key);

    void remove(int key);

    int height() const;

};

void printTree(const Node* root, const std::string& prefix = "", bool isLeft = false);

#endif 
