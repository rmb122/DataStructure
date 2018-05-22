#include <iostream>
#include <stdexcept>

template <typename type>
class node {
public:
    node *nextNode;
    type value;

    node(node *nextNode, type value): nextNode(nextNode), value(value) {};
    node(type value): nextNode(nullptr), value(value) {};
};

template <typename type>
class listQueue {
private:
    node<type> *head;
    node<type> *rear;
    unsigned int size;

public:
    listQueue(): head(nullptr), rear(nullptr), size(0) {};
    ~listQueue() {
        while (head != nullptr) {
            node<type> *temp = head->nextNode;
            delete head;
            head = temp;
        }
    }

    unsigned int getSize() {
        return size;
    }

    bool isEmpty() {
        return head == nullptr;
    }

    void push_back(type value) {
        node<type> *ptr = new node<type>(value);
        size++;
        if (head == nullptr) {
            head = ptr;
            rear = ptr;
        } else {
            rear->nextNode = ptr;
            rear = ptr;
        }
    }

    type getHead() {
        if (isEmpty()) {
            throw std::runtime_error("The queue is empty.");
        }
        return head->value;
    }

    type pop() {
        if (isEmpty()) {
            throw std::runtime_error("The queue is empty.");
        }

        type val = head->value;
        if (rear == head) {
            delete rear;
            rear = nullptr;
            head = nullptr;
        } else {
            node<type> *temp = head;
            head = head->nextNode;
            delete temp;
        }

        size--;
        return val;
    }

    void reverse() {
        if (isEmpty()) {
            return;
        }

        node<type> *reversing = head;
        node<type> *lastNode = nullptr;
        node<type> *temp;

        while (reversing != nullptr) {
            temp = reversing->nextNode;
            reversing->nextNode = lastNode;
            lastNode = reversing;
            reversing = temp;
        }

        temp = head;
        head = rear;
        rear = temp;
        return;
    }
};

void printPascalTriangle(unsigned int hight) {
    listQueue<unsigned long long> queue;
    queue.push_back(1); //��һ������
    queue.push_back(0);

    for (unsigned int i = 0; i < hight; ++i) {
        unsigned long long lastNum = queue.pop();  //ÿ����ʼ�̶�Ϊ1
        queue.push_back(1);

        while (lastNum != 0) {
            std::cout << lastNum << "\t";
            queue.push_back(lastNum + queue.getHead());
            lastNum = queue.pop();
        }

        queue.push_back(0); //ÿ�н�β�̶�Ϊ0
        std::cout << std::endl;
    }
}