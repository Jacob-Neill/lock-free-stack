#include <atomic>
#include <optional>

template<class T>
struct Node {
    T data;
    Node* next;
};

template<class T>
class Stack {//Treiber stack
    std::atomic<Node*> head;
    std::atomic<Node*> toDelete;
public:
    Stack():head{nullptr}, toDelete{nullptr} {}
    ~Stack();
    void push(T data);
    std::optional<T> pop();
    bool empty() {return head == nullptr;}
};

template<class T>
Stack<T>::~Stack() {
    while (!empty()) {
        pop();//dump everything into toDelete
    }

    Node* current = toDelete;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

template<class T>
void Stack<T>::push(T data) {
    Node* newNode = new Node{data, head};
    while (!head.compare_exchange_weak(newNode->next, newNode));
}

template<class T>
std::optional<T> Stack<T>::pop() {
    if(empty()) return std::nullopt;

    Node* oldHead = head;

    while(!head.compare_exchange_weak(oldHead, oldHead->next));

    oldHead->next = toDelete;
    while(!toDelete.compare_exchange_weak(oldHead->next, oldHead));

    return oldHead->data;
}
