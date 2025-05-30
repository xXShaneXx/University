#ifndef NODE_HPP
#define NODE_HPP

#include <memory> // For std::shared_ptr

struct Node
{
    std::shared_ptr<Node> parent;
    std::shared_ptr<Node> right;
    std::shared_ptr<Node> left;
    int value;

    Node(int val) : value(val), parent(nullptr), right(nullptr), left(nullptr) {}
};

// Function declarations
std::shared_ptr<Node> insert(std::shared_ptr<Node> root, int value);
std::shared_ptr<Node> deleteNode(std::shared_ptr<Node> root, int value);
int height(const std::shared_ptr<Node>& root);

#endif