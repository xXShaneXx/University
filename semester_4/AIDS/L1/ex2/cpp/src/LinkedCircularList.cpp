#include "LinkedCircularList.h"
#include <iostream>

void insert(LinkedCircularList& list, int x)
{
    list.size++;
    Node* newNode = new Node(x);
    if(!list.baseNode)
    {
        list.baseNode = newNode;
        newNode->node = list.baseNode; 
    }
    else
    {
        newNode->node = list.baseNode->node;
        list.baseNode->node = newNode;

        //move base to added note not nessecerly
        //list.baseNode = newNode;
    }
}

LinkedCircularList merge(LinkedCircularList& l1, LinkedCircularList& l2)
{
    LinkedCircularList mergedList;
    mergedList.size = l1.size + l2.size;

    if (!l1.baseNode)  // If l1 is empty, just set mergedList to l2
    {
        mergedList = l2;
        return mergedList;
    }

    if (!l2.baseNode)  // If l2 is empty, just set mergedList to l1
    {
        mergedList = l1;
        return mergedList;
    }

    // Copy l1 to mergedList
    Node* current = l1.baseNode;
    do
    {
        insert(mergedList, current->x);
        current = current->node;
    } while (current != l1.baseNode);

    // Copy l2 to mergedList
    current = l2.baseNode;
    do
    {
        insert(mergedList, current->x);
        current = current->node;
    } while (current != l2.baseNode);

    return mergedList;
}

int search(const LinkedCircularList& list, int x)
{
    if (!list.baseNode) return -1;

    Node* current = list.baseNode;
    int comparisons = 0;
    do
    {
        comparisons++;
        if (current->x == x)
        {
            return comparisons;
        }
        current = current->node;
    } while (current != list.baseNode);

    return comparisons;
}

void printList(const LinkedCircularList& list)
{
    if (!list.baseNode)
    {
        std::cout << "List is empty." << std::endl;
        return;
    }

    Node* current = list.baseNode;
    do
    {
        std::cout << current->x << " ";
        current = current->node;
    } while (current != list.baseNode);
    std::cout << std::endl;
}