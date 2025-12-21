#pragma once 

struct Node
{
    Node* node;
    int x;
    Node(int value = 0) : node(nullptr), x(value) {} 
};

struct LinkedCircularList
{
    int size;
    Node* baseNode;

    LinkedCircularList(): size(0), baseNode(nullptr) {}
};

void insert(LinkedCircularList& list, int x);
LinkedCircularList merge(LinkedCircularList& l1, LinkedCircularList& l2);
int search(const LinkedCircularList& list, int x);
void printList(const LinkedCircularList& list);
