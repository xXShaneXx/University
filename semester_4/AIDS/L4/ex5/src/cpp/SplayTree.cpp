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

void RBTree::leftRotate(Node* x) {
    if (!x || !x->right) {
        return;
    }

    // Get the unique_ptr reference in the parent that owns x
    // This allows to move 'x' out and then move 'y' back into this slot later.
    std::unique_ptr<Node>& x_original_owner_slot = getParentUniquePtr(x);

    // Move x and y out of their unique_ptrs
    std::unique_ptr<Node> temp_x = std::move(x_original_owner_slot);
    std::unique_ptr<Node> temp_y = std::move(temp_x->right);

    // y's left subtree becomes x's right subtree
    temp_x->right = std::move(temp_y->left);
    if (temp_x->right) {
        temp_x->right->parent = temp_x.get();
    }

    // y replaces x in the tree
    temp_y->parent = temp_x->parent;

    // x becomes y's left child
    temp_y->left = std::move(temp_x);
    temp_y->left->parent = temp_y.get();

    // Attach rotated subtree back to tree
    x_original_owner_slot = std::move(temp_y);
}

void RBTree::rightRotate(Node* y) {
    if (!y || !y->left) {
        return;
    }

    // Get the unique_ptr reference in the parent that owns y
    std::unique_ptr<Node>& y_original_owner_slot = getParentUniquePtr(y);

    // Move y and x out of their unique_ptrs
    std::unique_ptr<Node> temp_y = std::move(y_original_owner_slot);
    std::unique_ptr<Node> temp_x = std::move(temp_y->left);

    // x's right subtree becomes y's left subtree
    temp_y->left = std::move(temp_x->right);
    if (temp_y->left) {
        temp_y->left->parent = temp_y.get();
    }

    // x replaces y in the tree
    temp_x->parent = temp_y->parent;

    // y becomes x's right child
    temp_x->right = std::move(temp_y);
    temp_x->right->parent = temp_x.get();

    // Attach rotated subtree back to tree
    y_original_owner_slot = std::move(temp_x);
}



void RBTree::insert(int value) {
    Node* x = root.get();
    Node* y = nullptr;

    auto z = std::make_unique<Node>(value);
    Node* zRaw = z.get();  // Store raw pointer before moving

    while (x != nullptr) {
        y = x;
        if (z->value < x->value)
            x = x->left.get();
        else
            x = x->right.get();
    }
    z->parent = y;

    if (y == nullptr)
        root = std::move(z);
    else if (z->value < y->value)
        y->left = std::move(z);
    else
        y->right = std::move(z);

    zRaw->left = nullptr;
    zRaw->right = nullptr;
    zRaw->color = Color::RED;

    insertFixup(zRaw);
}

void RBTree::insertFixup(Node* z) {
    while (z->parent && z->parent->color == Color::RED) {
        Node* p = z->parent; // Parent of z
        Node* gp = p->parent; // Grandparent of z

        // If parent is red, grandparent must exist and be black.
        // This check is mainly for robustness against corrupted trees or edge cases not handled.
        if (!gp) break;

        if (p == gp->left.get()) { // Parent is left child of grandparent
            Node* u = gp->right.get(); // Uncle

            if (u && u->color == Color::RED) { // Case 1: Uncle is RED
                p->color = Color::BLACK;
                u->color = Color::BLACK;
                gp->color = Color::RED;
                z = gp; // Move z up to grandparent, continue loop
            } else { // Uncle is BLACK (Case 2 or 3)
                if (z == p->right.get()) { // Case 2: Z is a right child (Zig-Zag)
                    z = p; // Move z up to parent (original 'p')
                    leftRotate(z); // Rotate around this new 'z' (original 'p')
                    // After this rotation, 'z' is still the same node, but its parent and children relationships have changed.
                    // 'z' is now the root of the subtree that was just rotated, and its new parent is the original 'gp'.
                    // So, 'p' needs to be updated to 'z->parent' for the next step, which is original 'gp'.
                    p = z->parent; // Update p to reflect current parent of z (which is original gp)
                }
                // Case 3 (Z is a left child, or became one after Case 2)
                p->color = Color::BLACK; // original parent (or updated p) becomes black
                gp->color = Color::RED; // original grandparent becomes red
                rightRotate(gp); // Rotate around original grandparent
            }
        } else { // Parent is right child of grandparent (Mirror Cases)
            Node* u = gp->left.get(); // Uncle (mirror)

            if (u && u->color == Color::RED) { // Case 1 (Mirror): Uncle is RED
                p->color = Color::BLACK;
                u->color = Color::BLACK;
                gp->color = Color::RED;
                z = gp; // Move z up to grandparent, continue loop
            } else { // Uncle is BLACK (Case 2 or 3 Mirror)
                if (z == p->left.get()) { // Case 2 (Mirror): Z is a left child (Zag-Zig)
                    z = p; // Move z up to parent
                    rightRotate(z); // Rotate around this new 'z'
                    // Similar to above, update 'p' to reflect current parent of 'z'
                    p = z->parent;
                }
                // Case 3 (Mirror) (Z is a right child, or became one after Case 2 Mirror)
                p->color = Color::BLACK;
                gp->color = Color::RED;
                leftRotate(gp);
            }
        }
    }
    if (root)
        root->color = Color::BLACK; // Ensure root is always BLACK
}

void RBTree::transplant(Node* u, std::unique_ptr<Node> v) {
    if (u->parent == nullptr) {
        root = std::move(v);
        if (root)
            root->parent = nullptr;
    } else if (u == u->parent->left.get()) {
        u->parent->left = std::move(v);
        if (u->parent->left)
            u->parent->left->parent = u->parent;
    } else {
        u->parent->right = std::move(v);
        if (u->parent->right)
            u->parent->right->parent = u->parent;
    }
}

void RBTree::remove(int value) {
    Node* z = findNode(root.get(), value);
    if (!z) return; // Node not found

    Node* y = z;
    Color y_original_color = y->color;
    Node* x = nullptr;
    Node* xParent = nullptr;

    // Case 1: z has no left child
    if (!z->left) {
        x = z->right.get();
        xParent = z->parent;
        transplant(z, std::move(z->right));
    }
    // Case 2: z has no right child
    else if (!z->right) {
        x = z->left.get();
        xParent = z->parent;
        transplant(z, std::move(z->left));
    }
    // Case 3: z has two children
    else {
        y = treeMinimum(z->right.get());
        y_original_color = y->color;
        x = y->right.get();
        
        if (y->parent == z) { // y is z's direct right child
            xParent = y;
            
            // Save z's left child before transplant
            auto temp_left = std::move(z->left);
            
            // Replace z with y
            transplant(z, std::move(z->right));
            
            // Now y is at z's position
            // Need to find y again since its ownership has changed
            Node* new_y = findNode(root.get(), y->value);
            
            // Attach z's left subtree to y
            new_y->left = std::move(temp_left);
            if (new_y->left) new_y->left->parent = new_y;
        } else {
            // y is not z's direct right child
            xParent = y->parent;
            
            // Simply swap values instead of moving nodes
            z->value = y->value;
            
            // Remove y from its position
            transplant(y, std::move(y->right));
            
            // We've already moved y's value to z, so no need to continue
            if (y_original_color == Color::BLACK) {
                deleteFixup(x, xParent);
            }
            return;
        }
    }

    if (y_original_color == Color::BLACK) {
        deleteFixup(x, xParent);
    }
}

void RBTree::deleteFixup(Node* x, Node* xParent) {
    // x can be nullptr if the removed node was a leaf that was black.
    // xParent is the parent of the *conceptual* node that was at x's position.
    // This is crucial because if x is nullptr, xParent is still needed to navigate up the tree.

    while (x != root.get() && (x == nullptr || x->color == Color::BLACK)) {
        if (x == (xParent ? xParent->left.get() : nullptr)) { // x is left child
            Node* w = xParent->right.get(); // w is x's sibling

            // Case 1: Sibling w is RED
            if (w && w->color == Color::RED) {
                w->color = Color::BLACK;
                xParent->color = Color::RED;
                leftRotate(xParent);
                w = xParent->right.get(); // Update w after rotation
            }

            // Case 2: Sibling w is BLACK, and both of w's children are BLACK
            if (w && (!w->left || w->left->color == Color::BLACK) &&
                (!w->right || w->right->color == Color::BLACK)) {
                w->color = Color::RED;
                x = xParent; // Move up the tree
                xParent = x->parent;
            } else {
                // Case 3: Sibling w is BLACK, w's left child is RED, and w's right child is BLACK
                if (w && (!w->right || w->right->color == Color::BLACK)) {
                    if (w->left) w->left->color = Color::BLACK;
                    w->color = Color::RED;
                    rightRotate(w); // Perform right rotation on w
                    w = xParent->right.get(); // Update w after rotation
                }
                // Case 4: Sibling w is BLACK, and w's right child is RED
                if (w) { // w must not be null here
                    w->color = xParent->color;
                    xParent->color = Color::BLACK;
                    if (w->right) w->right->color = Color::BLACK;
                    leftRotate(xParent); // Perform left rotation on xParent
                }
                x = root.get(); // Set x to root to terminate loop (tree is balanced)
                xParent = nullptr; // Clear xParent
            }
        } else { // Symmetric case: x is right child
            Node* w = xParent->left.get(); // w is x's sibling

            // Case 1 mirror: Sibling w is RED
            if (w && w->color == Color::RED) {
                w->color = Color::BLACK;
                xParent->color = Color::RED;
                rightRotate(xParent);
                w = xParent->left.get(); // Update w after rotation
            }

            // Case 2 mirror: Sibling w is BLACK, and both of w's children are BLACK
            if (w && (!w->right || w->right->color == Color::BLACK) &&
                (!w->left || w->left->color == Color::BLACK)) {
                w->color = Color::RED;
                x = xParent; // Move up the tree
                xParent = x->parent;
            } else {
                // Case 3 mirror: Sibling w is BLACK, w's right child is RED, and w's left child is BLACK
                if (w && (!w->left || w->left->color == Color::BLACK)) {
                    if (w->right) w->right->color = Color::BLACK;
                    w->color = Color::RED;
                    leftRotate(w); // Perform left rotation on w
                    w = xParent->left.get(); // Update w after rotation
                }
                // Case 4 mirror: Sibling w is BLACK, and w's left child is RED
                if (w) { // w must not be null here
                    w->color = xParent->color;
                    xParent->color = Color::BLACK;
                    if (w->left) w->left->color = Color::BLACK;
                    rightRotate(xParent); // Perform right rotation on xParent
                }
                x = root.get(); // Set x to root to terminate loop
                xParent = nullptr; // Clear xParent
            }
        }
    }
    if (x) { // If x is not nullptr (e.g., if x was red originally), set its color to BLACK
        x->color = Color::BLACK;
    }
}

Node* RBTree::findNode(Node* current, int value) const {
    if (current == nullptr || current->value == value) {
        return current;
    }
    
    if (value < current->value) {
        return findNode(current->left.get(), value);
    } else {
        return findNode(current->right.get(), value);
    }
}

Node* RBTree::treeMinimum(Node* node) const {
    Node* current = node;
    while (current && current->left) {
        current = current->left.get();
    }
    return current;
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

void RBTree::printTree() const {
    if (root == nullptr) {
        return;
    }
    printTree(root.get());
}

void RBTree::printTree(const Node* node, const std::string& prefix, bool isLeft) const {
    if (node == nullptr) return;  // Base case: return if node is nullptr
    
    std::cout << prefix;
    std::cout << (isLeft ? "├──" : "└──");
    
    // Print node value and color
    if (node->color == Color::RED) {
        std::cout << "\033[1;31m" << node->value << " (R)\033[0m" << std::endl;
    } else {
        std::cout << "\033[1;30m" << node->value << " (B)\033[0m" << std::endl;
    }

    // Recursively print left and right subtrees (only if they exist)
    if (node->left) {
        printTree(node->left.get(), prefix + (isLeft ? "│   " : "    "), true);
    }
    if (node->right) {
        printTree(node->right.get(), prefix + (isLeft ? "│   " : "    "), false);
    }
}
