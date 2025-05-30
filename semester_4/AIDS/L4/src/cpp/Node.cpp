#include "Node.hpp"
#include "memory"


std::shared_ptr<Node> insert(std::shared_ptr<Node> node, int value) {
    // If the tree is empty, return a new node
    if (node == nullptr) 
        return std::make_shared<Node>(value);
    
    // If the key is already present in the tree,
    // return the node
    if (node->value == value) 
        return node;
    
    // Otherwise, recur down the tree/ If the key
    // to be inserted is greater than the node's key,
    // insert it in the right subtree
    if (node->value < value) 
        node->right = insert(node->right, value);
    
    // If the key to be inserted is smaller than 
    // the node's key,insert it in the left subtree
    else 
        node->left = insert(node->left, value);
    
    // Return the (unchanged) node pointer
    return node;
}