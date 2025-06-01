#include "Node.hpp"
#include <memory>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>

std::unique_ptr<Node> insert(std::unique_ptr<Node> node, int value, OperationCounts& counts) {
    counts.comparisons++;
    if (node == nullptr) {
        counts.pointer_ops++; // for creating new node
        return std::make_unique<Node>(value);
    }
    
    counts.comparisons++;
    if (node->value == value) {
        counts.pointer_ops++; // for move operation
        return std::move(node);
    }
    
    counts.comparisons++;
    if (node->value < value) {
        counts.pointer_ops += 2; // read and assignment
        node->right = insert(std::move(node->right), value, counts);
    } else {
        counts.pointer_ops += 2; // read and assignment
        node->left = insert(std::move(node->left), value, counts);
    }
    
    counts.pointer_ops++; // for move operation
    counts.height = height(node.get(), counts);
    return std::move(node);
}

Node* getSuccessor(const Node* curr, OperationCounts& counts) {
    counts.pointer_ops++; // for checking right
    if (!curr->right) return nullptr;
    
    counts.pointer_ops++; // for getting right
    Node* temp = curr->right.get();
    while (temp) {
        counts.pointer_ops++; // for checking left
        counts.pointer_ops++; // for getting left
        if (!temp->left) break;
        temp = temp->left.get();
        counts.pointer_ops++; // for assignment
    }
    return temp;
}

std::unique_ptr<Node> deleteNode(std::unique_ptr<Node> root, int value, OperationCounts& counts) {
    counts.comparisons++;
    if (root == nullptr)
        return nullptr;

    counts.comparisons++;
    if (root->value > value) {
        counts.pointer_ops += 2; // read and assignment
        root->left = deleteNode(std::move(root->left), value, counts);
    }
    else if (root->value < value) {
        counts.comparisons++;
        counts.pointer_ops += 2; // read and assignment
        root->right = deleteNode(std::move(root->right), value, counts);
    }
    else {
        // Cases when root has 0 children or only right child
        counts.pointer_ops++; // for checking left
        if (root->left == nullptr) {
            counts.pointer_ops++; // for move operation
            return std::move(root->right);
        }

        // When root has only left child
        counts.pointer_ops++; // for checking right
        if (root->right == nullptr) {
            counts.pointer_ops++; // for move operation
            return std::move(root->left);
        }

        // When both children are present
        Node* succ = getSuccessor(root.get(), counts);
        root->value = succ->value;
        counts.pointer_ops += 2; // for value assignment and move
        root->right = deleteNode(std::move(root->right), succ->value, counts);
    }
    
    counts.pointer_ops++; // for move operation
    counts.height = height(root.get(), counts);
    return std::move(root);
}

int height(const Node* root, OperationCounts& counts) {
    counts.pointer_ops++; // for nullptr check
    if (root == nullptr)
        return 0;
    
    counts.pointer_ops += 2; // for left and right accesses
    return 1 + std::max(height(root->left.get(), counts), 
                       height(root->right.get(), counts));
}