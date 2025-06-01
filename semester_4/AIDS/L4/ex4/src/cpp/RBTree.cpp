#include "RBTree.hpp"
#include <memory>
#include <iostream>

std::unique_ptr<Node>& RBTree::getParentUniquePtr(Node* node) {
    if (!node->parent) {
        return root;
    }
    if (node->parent->left.get() == node) {
        return node->parent->left;
    } else {
        return node->parent->right;
    }
}

void RBTree::leftRotate(Node* x, OperationCounts& counts) {
    counts.rotations++;
    if (!x || !x->right) {
        return;
    }

    counts.pointer_ops++; // for getParentUniquePtr
    std::unique_ptr<Node>& x_original_owner_slot = getParentUniquePtr(x);

    counts.pointer_ops += 2; // for move operations
    std::unique_ptr<Node> temp_x = std::move(x_original_owner_slot);
    std::unique_ptr<Node> temp_y = std::move(temp_x->right);

    counts.pointer_ops++; // for right assignment
    temp_x->right = std::move(temp_y->left);
    if (temp_x->right) {
        counts.pointer_ops++; // for parent assignment
        temp_x->right->parent = temp_x.get();
    }

    counts.pointer_ops++; // for parent assignment
    temp_y->parent = temp_x->parent;

    counts.pointer_ops++; // for left assignment
    temp_y->left = std::move(temp_x);
    counts.pointer_ops++; // for parent assignment
    temp_y->left->parent = temp_y.get();

    counts.pointer_ops++; // for final move
    x_original_owner_slot = std::move(temp_y);
}

void RBTree::rightRotate(Node* y, OperationCounts& counts) {
    counts.rotations++;
    if (!y || !y->left) {
        return;
    }

    counts.pointer_ops++; // for getParentUniquePtr
    std::unique_ptr<Node>& y_original_owner_slot = getParentUniquePtr(y);

    counts.pointer_ops += 2; // for move operations
    std::unique_ptr<Node> temp_y = std::move(y_original_owner_slot);
    std::unique_ptr<Node> temp_x = std::move(temp_y->left);

    counts.pointer_ops++; // for left assignment
    temp_y->left = std::move(temp_x->right);
    if (temp_y->left) {
        counts.pointer_ops++; // for parent assignment
        temp_y->left->parent = temp_y.get();
    }

    counts.pointer_ops++; // for parent assignment
    temp_x->parent = temp_y->parent;

    counts.pointer_ops++; // for right assignment
    temp_x->right = std::move(temp_y);
    counts.pointer_ops++; // for parent assignment
    temp_x->right->parent = temp_x.get();

    counts.pointer_ops++; // for final move
    y_original_owner_slot = std::move(temp_x);
}

void RBTree::insert(int value, OperationCounts& counts) {
    Node* x = root.get();
    Node* y = nullptr;

    auto z = std::make_unique<Node>(value);
    Node* zRaw = z.get();

    while (x != nullptr) {
        counts.comparisons++;
        y = x;
        if (z->value < x->value) {
            counts.pointer_ops++;
            x = x->left.get();
        } else {
            counts.pointer_ops++;
            x = x->right.get();
        }
    }

    counts.pointer_ops++; // for parent assignment
    z->parent = y;

    if (y == nullptr) {
        counts.pointer_ops++; // for root assignment
        root = std::move(z);
    } else if (z->value < y->value) {
        counts.comparisons++;
        counts.pointer_ops++; // for left assignment
        y->left = std::move(z);
    } else {
        counts.comparisons++;
        counts.pointer_ops++; // for right assignment
        y->right = std::move(z);
    }

    zRaw->left = nullptr;
    zRaw->right = nullptr;
    zRaw->color = Color::RED;

    insertFixup(zRaw, counts);
    lastOpCounts = counts;
}

void RBTree::insertFixup(Node* z, OperationCounts& counts) {
    while (z->parent && z->parent->color == Color::RED) {
        counts.comparisons++;
        Node* p = z->parent;
        Node* gp = p->parent;

        if (!gp) break;

        if (p == gp->left.get()) {
            counts.comparisons++;
            Node* u = gp->right.get();

            if (u && u->color == Color::RED) {
                counts.color_changes += 3;
                p->color = Color::BLACK;
                u->color = Color::BLACK;
                gp->color = Color::RED;
                z = gp;
            } else {
                if (z == p->right.get()) {
                    counts.comparisons++;
                    z = p;
                    leftRotate(z, counts);
                    p = z->parent;
                }
                counts.color_changes += 2;
                p->color = Color::BLACK;
                gp->color = Color::RED;
                rightRotate(gp, counts);
            }
        } else {
            counts.comparisons++;
            Node* u = gp->left.get();

            if (u && u->color == Color::RED) {
                counts.color_changes += 3;
                p->color = Color::BLACK;
                u->color = Color::BLACK;
                gp->color = Color::RED;
                z = gp;
            } else {
                if (z == p->left.get()) {
                    counts.comparisons++;
                    z = p;
                    rightRotate(z, counts);
                    p = z->parent;
                }
                counts.color_changes += 2;
                p->color = Color::BLACK;
                gp->color = Color::RED;
                leftRotate(gp, counts);
            }
        }
    }
    if (root) {
        counts.color_changes++;
        root->color = Color::BLACK;
    }
    counts.height = height();
}

Node* RBTree::findNode(Node* current, int value, OperationCounts& counts) const {
    counts.comparisons++;
    if (current == nullptr || current->value == value) {
        return current;
    }
    
    counts.comparisons++;
    if (value < current->value) {
        counts.pointer_ops++;
        return findNode(current->left.get(), value, counts);
    } else {
        counts.pointer_ops++;
        return findNode(current->right.get(), value, counts);
    }
}

int RBTree::height() const {
    return heightHelper(root.get());
}

int RBTree::heightHelper(const Node* node) const {
    if (node == nullptr)
        return 0;
    
    return 1 + std::max(heightHelper(node->left.get()), 
                        heightHelper(node->right.get()));
}