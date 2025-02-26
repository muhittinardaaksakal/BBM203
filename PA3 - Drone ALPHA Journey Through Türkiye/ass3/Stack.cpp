#include "Stack.h"
#include <iostream>
#include <stack>

// Constructor to initialize an empty stack
Stack::Stack() {
    // TODO: Your code here
    top = -1;
}

// Adds a province to the top of the stack
void Stack::push(int province) {
    // TODO: Your code here
    if (top >= MAX_SIZE - 1) {
        std::cerr << "Error: Stack overflow. Cannot push element: " << province << std::endl;
        return;
    }
    top++;
    data[top] = province;
}


// Removes and returns the top province from the stack
int Stack::pop() {
    // TODO: Your code here
    if (isEmpty()) {
        std::cerr << "Error: Stack underflow. Cannot pop from an empty stack." << std::endl;
        return -1;
    }
    top--;
    return data[top + 1];
}


// Returns the top province without removing it
int Stack::peek() const {
    // TODO: Your code here
    if (isEmpty()) {

        std::cerr << "Stack underflow: Cannot peek from an empty stack." << std::endl;
        return -1;
    }

    return data[top];
}


bool Stack::isEmpty() const {
    // TODO: Your code here
    if (top == -1) {
        return true;
    }
    return false;
}

// Function to get the current size of the stack
int Stack::getSize() const {
    // TODO: Your code here
    return top + 1;
}
