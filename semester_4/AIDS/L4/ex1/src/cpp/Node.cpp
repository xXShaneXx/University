#include "Node.hpp"
#include <memory>
#include <iostream>


std::unique_ptr<Node> insert(std::unique_ptr<Node> node, int value) {
    // If the tree is empty, return a new node
    if (node == nullptr) 
        return std::make_unique<Node>(value);
    
    // If the value is already present in the tree,
    // return the node
    if (node->value == value) 
        return std::move(node);
    
    // Otherwise, recur down the tree/ If the value
    if (node->value < value) 
        node->right = insert(std::move(node->right), value);
    
    // If the value to be inserted is smaller than 
    // the node's value,insert it in the left subtree
    else 
        node->left = insert(std::move(node->left), value);
    
    // Return the (unchanged) node pointer
    return std::move(node);
}

Node* getSuccessor(const Node* curr) {
    if (!curr->right) return nullptr;
    
    Node* temp = curr->right.get();
    while (temp && temp->left)
        temp = temp->left.get();
    return temp;
}

std::unique_ptr<Node> deleteNode(std::unique_ptr<Node> root, int value) {
    // Base case
    if (root == nullptr)
        return nullptr;

    // If value to be searched is in a subtree
    if (root->value > value)
        root->left = deleteNode(std::move(root->left), value);
    else if (root->value < value)
        root->right = deleteNode(std::move(root->right), value);

    // If root matches with the given value
    else {
        // Cases when root has 0 children or only right child
        if (root->left == nullptr)
            return std::move(root->right);

        // When root has only left child
        if (root->right == nullptr)
            return std::move(root->left);

        // When both children are present
        Node* succ = getSuccessor(root.get());
        root->value = succ->value;
        root->right = deleteNode(std::move(root->right), succ->value);
    }
    
    return std::move(root);
}

int height(const Node* root) {
    if (root == nullptr)
        return 0;
    
    return 1 + std::max(height(root->left.get()), height(root->right.get()));
}

void printTree(const Node* root, const std::string& prefix, bool isLeft) {
    if (root == nullptr) {
        return;
    }

    std::cout << prefix;
    std::cout << (isLeft ? "├──" : "└──");
    std::cout << root->value << std::endl;

    // Recursively print the left and right subtrees
    printTree(root->left.get(), prefix + (isLeft ? "│   " : "    "), true);
    printTree(root->right.get(), prefix + (isLeft ? "│   " : "    "), false);
}