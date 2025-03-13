#pragma once 

#include <memory>

struct Node
{
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;
    int x;
    Node(int value = 0) : next(nullptr), prev(), x(value) {} 
};

struct LinkedCircularList
{
    int size;
    std::shared_ptr<Node> baseNode;

    LinkedCircularList(): size(0), baseNode(nullptr) {}
};

void insert(LinkedCircularList& list, int x);
LinkedCircularList merge(LinkedCircularList& l1, LinkedCircularList& l2);
int search(const LinkedCircularList& list, int x);
void printList(const LinkedCircularList& list);
