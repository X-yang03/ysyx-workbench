#include <iostream>
#include <stdexcept>

template<typename T>
struct StackNode {
    T data;
    StackNode* next;
};

template<typename T>
struct Stack {
    StackNode<T>* head = nullptr;

    // Push an element onto the stack
    void push(const T& value) {
        StackNode<T>* newNode = new StackNode<T>{value, head};
        head = newNode;
    }

    // Pop an element from the stack
    void pop() {
        if (head) {
            StackNode<T>* temp = head;
            head = head->next;
            delete temp;
        } else {
            throw std::runtime_error("Stack is empty");
        }
    }

    // Get the top element of the stack
    T top() const {
        if (head) {
            return head->data;
        }
        throw std::runtime_error("Stack is empty");
    }

    // Check if the stack is empty
    bool isEmpty() const {
        return head == nullptr;
    }

    // Destructor to clean up memory
    ~Stack() {
        while (head) {
            pop();
        }
    }
};

// Example usage
int main() {
    Stack<int> s;
    s.push(10);
    s.push(20);
    std::cout << "Top: " << s.top() << std::endl;
    s.pop();
    std::cout << "Top after pop: " << s.top() << std::endl;
    return 0;
}

