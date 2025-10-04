long long comparison_counter = 0;

#include "BHeap.hpp"
#include <iostream>
#include <climits>
using namespace std;

Node::Node(int value)
    : key(value), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}

BinomialHeap::BinomialHeap() : head(nullptr) {}

void BinomialHeap::binomialLink(Node* y, Node* z) {
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
}

Node* BinomialHeap::merge(Node* h1, Node* h2) {
    if (!h1) return h2;
    if (!h2) return h1;

    Node* head = nullptr;
    Node** pos = &head;

    while (h1 && h2) {
        if (++comparison_counter && h1->degree <= h2->degree) {
            *pos = h1;
            h1 = h1->sibling;
        } else {
            *pos = h2;
            h2 = h2->sibling;
        }
        pos = &((*pos)->sibling);
    }

    *pos = (h1) ? h1 : h2;
    return head;
}

Node* BinomialHeap::getMinimum() {
    Node* y = nullptr;
    Node* x = head;
    int min = INT_MAX;

    while (x) {
        if (x->key < min) {
            min = x->key;
            y = x;
        }
        x = x->sibling;
    }
    return y;
}

void BinomialHeap::insert(int key) {
    BinomialHeap tempHeap;
    tempHeap.head = new Node(key);
    head = unionHeaps(head, tempHeap.head);
}

Node* BinomialHeap::unionHeaps(Node* h1, Node* h2) {
    Node* newHead = merge(h1, h2);
    if (!newHead) return nullptr;

    Node* prev_x = nullptr;
    Node* x = newHead;
    Node* next_x = x->sibling;

    while (next_x) {
        if ((x->degree != next_x->degree) ||
            (next_x->sibling && next_x->sibling->degree == x->degree)) {
            prev_x = x;
            x = next_x;
        } else if (++comparison_counter && x->key <= next_x->key) {
            x->sibling = next_x->sibling;
            binomialLink(next_x, x);
        } else {
            if (!prev_x) newHead = next_x;
            else prev_x->sibling = next_x;

            binomialLink(x, next_x);
            x = next_x;
        }
        next_x = x->sibling;
    }
    return newHead;
}

void BinomialHeap::mergeWith(BinomialHeap& other) {
    head = unionHeaps(head, other.head);
    other.head = nullptr;
}

Node* BinomialHeap::extractMin() {
    if (!head) return nullptr;

    Node* prevMin = nullptr;
    Node* minNode = head;
    Node* prev = nullptr;
    Node* current = head;
    int minKey = head->key;

    while (current) {
        if (current->key < minKey) {
            minKey = current->key;
            minNode = current;
            prevMin = prev;
        }
        prev = current;
        current = current->sibling;
    }

    if (prevMin) prevMin->sibling = minNode->sibling;
    else head = minNode->sibling;

    Node* child = minNode->child;
    Node* revChild = nullptr;
    while (child) {
        Node* next = child->sibling;
        child->sibling = revChild;
        child->parent = nullptr;
        revChild = child;
        child = next;
    }

    head = unionHeaps(head, revChild);
    return minNode;
}

void BinomialHeap::decreaseKey(Node* x, int newKey) {
    if (newKey > x->key) {
        cerr << "Błąd: nowy klucz jest większy.\n";
        return;
    }

    x->key = newKey;
    Node* y = x;
    Node* z = y->parent;

    while (z && y->key < z->key) {
        swap(y->key, z->key);
        y = z;
        z = y->parent;
    }
}

void BinomialHeap::deleteNode(Node* x) {
    decreaseKey(x, INT_MIN);
    Node* removed = extractMin();
    delete removed;
}

bool BinomialHeap::isEmpty() const {
    return head == nullptr;
}

void BinomialHeap::print() {
    cout << "Kopiec (drzewa binomialne):\n";
    Node* current = head;
    while (current) {
        cout << "Stopień " << current->degree << ": ";
        printTree(current);
        cout << "\n";
        current = current->sibling;
    }
}

void BinomialHeap::printTree(Node* node) {
    if (!node) return;
    cout << node->key << " ";
    printTree(node->child);
    printTree(node->sibling);
}