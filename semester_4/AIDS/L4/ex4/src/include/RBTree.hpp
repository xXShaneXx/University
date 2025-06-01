#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <memory>
#include <string>

enum class Color : bool { RED, BLACK };

struct Node {
    int value;
    Color color;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Node* parent;

    Node(int val)
        : value(val), color(Color::RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

struct OperationCounts {
    long comparisons = 0;
    long pointer_ops = 0;
    long color_changes = 0;
    long rotations = 0;
    int height = 0;
};

class RBTree {
public:
    RBTree()=default;

    void insert(int value, OperationCounts& counts);
    void remove(int value, OperationCounts& counts);

    int height() const;
    void printTree() const;

    OperationCounts getLastOperationCounts() const { return lastOpCounts; }

private:
    std::unique_ptr<Node> root;
    OperationCounts lastOpCounts;

    int heightHelper(const Node* node) const;

    // Rotations
    void leftRotate(Node* x, OperationCounts& counts);
    void rightRotate(Node* y, OperationCounts& counts);

    // Insert helpers
    void insertFixup(Node* z, OperationCounts& counts);

    // Delete helpers
    void transplant(Node* u, std::unique_ptr<Node> v, OperationCounts& counts);
    std::unique_ptr<Node>& getParentUniquePtr(Node* node);
    void deleteFixup(Node* x, Node* xParent, OperationCounts& counts);
    Node* findNode(Node* current, int value, OperationCounts& counts) const;
    Node* treeMinimum(Node* node, OperationCounts& counts) const;

    void printTree(const Node* node, const std::string& prefix = "", bool isLeft = false) const;
};

#endif // RBTREE_HPP