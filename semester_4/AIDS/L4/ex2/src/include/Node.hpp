#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <string>

struct Node {
    std::unique_ptr<Node> right;
    std::unique_ptr<Node> left;
    int value;

    Node(int val) : value(val), right(nullptr), left(nullptr) {}
};

// Instrumentation structure
struct OperationCounts {
    long comparisons = 0;
    long pointer_ops = 0;
    int height = 0;
};

// Function declarations with instrumentation
std::unique_ptr<Node> insert(std::unique_ptr<Node> root, int value, OperationCounts& counts);
std::unique_ptr<Node> deleteNode(std::unique_ptr<Node> root, int value, OperationCounts& counts);
Node* getSuccessor(const Node* curr, OperationCounts& counts);
int height(const Node* root, OperationCounts& counts);
void printTree(const Node* root, const std::string& prefix = "", bool isLeft = false);

#endif