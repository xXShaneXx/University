#include "LinkedCircularList.h"
#include <iostream>
#include <cstdlib>
#include <ctime>


void insert(LinkedCircularList& list, int x)
{
    auto newNode = std::make_shared<Node>(x);
    if (!list.baseNode)
    {
        list.baseNode = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
    }
    else
    {
        auto lastNode = list.baseNode->prev.lock();
        newNode->next = list.baseNode;
        newNode->prev = lastNode;
        list.baseNode->prev = newNode;
        if (lastNode)
        {
            lastNode->next = newNode;
        }
    }
    list.size++;
}

LinkedCircularList merge(LinkedCircularList& l1, LinkedCircularList& l2)
{
    LinkedCircularList mergedList;

    if (!l1.baseNode) return l2;
    if (!l2.baseNode) return l1;

    auto l1Last = l1.baseNode->prev.lock();
    auto l2Last = l2.baseNode->prev.lock();

    l1Last->next = l2.baseNode;
    l2.baseNode->prev = l1Last;

    l2Last->next = l1.baseNode;
    l1.baseNode->prev = l2Last;

    mergedList.baseNode = l1.baseNode;
    mergedList.size = l1.size + l2.size;

    return mergedList;
}

int search(const LinkedCircularList& list, int x)
{
    if (!list.baseNode) return -1;

    std::srand(std::time(0));
    bool searchRight = std::rand() % 2;

    Node* current = list.baseNode.get();
    int comparisons = 0;

    if (searchRight)
    {
        do
        {
            comparisons++;
            if (current->x == x)
            {
                return comparisons;
            }
            current = current->next.get();
        } while (current != list.baseNode.get());
    }
    else
    {
        do
        {
            comparisons++;
            if (current->x == x)
            {
                return comparisons;
            }
            current = current->prev.lock().get();
        } while (current != list.baseNode.get());
    }

    return comparisons;
}

void printList(const LinkedCircularList& list)
{
    if (!list.baseNode)
    {
        std::cout << "List is empty." << std::endl;
        return;
    }

    std::shared_ptr<Node> current = list.baseNode;
    do
    {
        std::cout << current->x << " ";
        current = current->next;
    } while (current != list.baseNode);
    std::cout << std::endl;
}