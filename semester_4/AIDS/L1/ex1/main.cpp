#include <cstdio>
#include <stdexcept>

struct Node
{
    Node* node;
    int x;
};

struct Stack
{
    Node * top{nullptr};
    void push(int x)
    {
        Node* newNode = new Node{.node=top, .x=x};
        top = newNode;
    }

    int pop(void)
    {
        if(!top)
            throw std::runtime_error("Stack is empty");
        Node* nextTop = top->node;
        int value = top->x;
        delete top;
        top = nextTop;
        return value;
    }

    ~Stack()
    {
        while(top)
        {
            Node* temp = top;
            top = top->node;
            delete top;
        }
    }
};

struct Queue
{
    Node* head{nullptr};
    Node* tail{nullptr};

    ~Queue()
    {
        while (head)
        {
            Node* temp = head;
            head = head->node;
            delete temp;
        }
    }

    void enQueue(int x)
    {
        Node* newNode = new Node{nullptr, x};
        if (!tail)
        {
            head = tail = newNode;
        }
        else
        {
            tail->node = newNode;
            tail = newNode;
        }
    }

    int deQueue(void)
    {
        if (!head)
        {
            throw std::runtime_error("Queue is empty");
        }
        Node* nextHead = head->node;
        int value = head->x;
        delete head;
        head = nextHead;
        if (!head)
        {
            tail = nullptr;
        }
        return value;
    }
};

int main() 
{
    Stack stack;
    for(int i=0; i<50; i++)
    {
        printf("Stack push %d\n", i);
        stack.push(i);
    }

    for(int i=0; i<50; i++)
    {
        printf("Stack pop %d \n", stack.pop());
    }
    printf("\n");

    Queue queue;
    for(int i=0; i<50; i++)
    {
        printf("Queue  enqueue%d\n", i);
        queue.enQueue(i);
    }

    for(int i=0; i<50; i++)
    {
        printf("Queue dequeue %d \n", queue.deQueue());
    }

    return 0;
}
