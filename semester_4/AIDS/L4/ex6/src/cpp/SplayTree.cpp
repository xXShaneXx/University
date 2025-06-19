#include "SplayTree.hpp"
#include <algorithm>

Node* SplayTree::rightRotate(Node* x, OperationCounts& counts) {
    counts.rotations++;
    counts.pointer_ops += 4; // 4 pointer operations in rotation
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

Node* SplayTree::leftRotate(Node* x, OperationCounts& counts) {
    counts.rotations++;
    counts.pointer_ops += 4; // 4 pointer operations in rotation
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

Node* SplayTree::splay(Node* root, int key, OperationCounts& counts) {
    counts.comparisons++;
    if (!root || root->key == key)
        return root;

    counts.comparisons++;
    if (key < root->key) {
        counts.pointer_ops++;
        if (!root->left) return root;

        counts.comparisons++;
        if (key < root->left->key) {
            // Zig-Zig (Left Left)
            counts.pointer_ops++;
            root->left->left = splay(root->left->left, key, counts);
            root = rightRotate(root, counts);
        }
        else if (key > root->left->key) {
            // Zig-Zag (Left Right)
            counts.comparisons++;
            counts.pointer_ops++;
            root->left->right = splay(root->left->right, key, counts);
            if (root->left->right)
                root->left = leftRotate(root->left, counts);
        }

        counts.pointer_ops++;
        return (root->left) ? rightRotate(root, counts) : root;
    } else {
        counts.pointer_ops++;
        if (!root->right) return root;

        counts.comparisons++;
        if (key < root->right->key) {
            // Zag-Zig (Right Left)
            counts.pointer_ops++;
            root->right->left = splay(root->right->left, key, counts);
            if (root->right->left)
                root->right = rightRotate(root->right, counts);
        }
        else if (key > root->right->key) {
            // Zag-Zag (Right Right)
            counts.comparisons++;
            counts.pointer_ops++;
            root->right->right = splay(root->right->right, key, counts);
            root = leftRotate(root, counts);
        }

        counts.pointer_ops++;
        return (root->right) ? leftRotate(root, counts) : root;
    }
}

Node* SplayTree::insert(Node* node, int key, OperationCounts& counts) {
    counts.comparisons++;
    if (!node) {
        counts.pointer_ops++; // for new node creation
        return new Node(key);
    }
    
    node = splay(node, key, counts);
    counts.comparisons++;
    if (node->key == key) return node;

    counts.pointer_ops++; // for new node creation
    Node* newNode = new Node(key);

    counts.comparisons++;
    if (key < node->key) {
        counts.pointer_ops += 4; // 4 pointer assignments
        newNode->right = node;
        newNode->left = node->left;
        node->left = nullptr;
    } else {
        counts.pointer_ops += 4; // 4 pointer assignments
        newNode->left = node;
        newNode->right = node->right;
        node->right = nullptr;
    }

    counts.height = height(newNode);
    return newNode;
}

Node* SplayTree::deleteKey(Node* root, int key, OperationCounts& counts) {
    counts.comparisons++;
    if (!root) return nullptr;

    root = splay(root, key, counts);
    counts.comparisons++;
    if (root->key != key) return root;

    Node* newRoot;
    counts.pointer_ops++; // for left check
    if (!root->left) {
        counts.pointer_ops++; // for right assignment
        newRoot = root->right;
    } else {
        counts.pointer_ops++; // for splay and right assignment
        newRoot = splay(root->left, key, counts);
        newRoot->right = root->right;
    }

    counts.pointer_ops++; // for delete
    delete root;
    counts.height = height(newRoot);
    return newRoot;
}

int SplayTree::height(Node* node) const {
    if (!node) return -1;
    return 1 + std::max(height(node->left), height(node->right));
}

void SplayTree::insert(int key, OperationCounts& counts) {
    root = insert(root, key, counts);
    lastOpCounts = counts;
}

void SplayTree::remove(int key, OperationCounts& counts) {
    root = deleteKey(root, key, counts);
    lastOpCounts = counts;
}

int SplayTree::height() const {
    return height(root);
}

void printTree(const Node* root, const std::string& prefix, bool isLeft) {
    if (root == nullptr) {
        return;
    }

    std::cout << prefix;
    std::cout << (isLeft ? "├──" : "└──");
    std::cout << root->key << std::endl;

    printTree(root->left, prefix + (isLeft ? "│   " : "    "), true);
    printTree(root->right, prefix + (isLeft ? "│   " : "    "), false);
}