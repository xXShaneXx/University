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

class RBTree {
public:
    RBTree()=default;

    void insert(int value);
    void remove(int value); //yyy

    int height() const;
    void printTree() const;

private:
    std::unique_ptr<Node> root;

    int heightHelper(const Node* node) const;

    // Rotations
    void leftRotate(Node* x);
    void rightRotate(Node* y);

    // Insert helpers
    void insertFixup(Node* z);

    // Delete helpers
    void transplant(Node* u, std::unique_ptr<Node> v);
    std::unique_ptr<Node>& getParentUniquePtr(Node* node);
    void deleteFixup(Node* x, Node* xParent); //yyyy
    Node* findNode(Node* current, int value) const;
    Node* treeMinimum(Node* node) const;

    void printTree(const Node* node, const std::string& prefix = "", bool isLeft = false) const;
};

#endif // RBTREE_HPP
