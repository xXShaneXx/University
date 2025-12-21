#include "SplayTree.hpp"
#include <algorithm>


Node* SplayTree::rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

Node* SplayTree::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

Node* SplayTree::splay(Node* root, int key) {
    if (!root || root->key == key)
        return root;

    if (key < root->key) {
        if (!root->left) return root;

        // Zig-Zig (Left Left)
        if (key < root->left->key) {
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
        }
        // Zig-Zag (Left Right)
        else if (key > root->left->key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right)
                root->left = leftRotate(root->left);
        }

        return (root->left) ? rightRotate(root) : root;
    } else {
        if (!root->right) return root;

        // Zag-Zig (Right Left)
        if (key < root->right->key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left)
                root->right = rightRotate(root->right);
        }
        // Zag-Zag (Right Right)
        else if (key > root->right->key) {
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }

        return (root->right) ? leftRotate(root) : root;
    }
}

Node* SplayTree::insert(Node* node, int key) {
    if (!node) return new Node(key);
    node = splay(node, key);
    if (node->key == key) return node;

    Node* newNode = new Node(key);

    if (key < node->key) {
        newNode->right = node;
        newNode->left = node->left;
        node->left = nullptr;
    } else {
        newNode->left = node;
        newNode->right = node->right;
        node->right = nullptr;
    }

    return newNode;
}

Node* SplayTree::deleteKey(Node* root, int key) {
    if (!root) return nullptr;

    root = splay(root, key);
    if (root->key != key) return root;

    Node* newRoot;
    if (!root->left) {
        newRoot = root->right;
    } else {
        newRoot = splay(root->left, key); // splay on the parent of the node to be removed
        newRoot->right = root->right;
    }

    delete root;
    return newRoot;
}

int SplayTree::height(Node* node) const {
    if (!node) return -1;
    return 1 + std::max(height(node->left), height(node->right));
}

void SplayTree::insert(int key) {
    root = insert(root, key); // splay done in insert
}

void SplayTree::remove(int key) {
    root = deleteKey(root, key); // splay done on parent
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

    // Recursively print the left and right subtrees
    printTree(root->left, prefix + (isLeft ? "│   " : "    "), true);
    printTree(root->right, prefix + (isLeft ? "│   " : "    "), false);
}

