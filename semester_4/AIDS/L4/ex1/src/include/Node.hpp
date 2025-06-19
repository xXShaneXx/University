#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <string>

struct Node
{
    std::unique_ptr<Node> right;
    std::unique_ptr<Node> left;
    int value;

    Node(int val) : value(val), right(nullptr), left(nullptr) {}
};

// Function declarations
std::unique_ptr<Node> insert(std::unique_ptr<Node> root, int value);
std::unique_ptr<Node> deleteNode(std::unique_ptr<Node> root, int value);
Node* getSuccessor(const Node* curr);
int height(const Node* root);
void printTree(const Node* root, const std::string& prefix = "", bool isLeft = false);

#endif