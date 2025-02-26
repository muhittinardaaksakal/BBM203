#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    front = -1;
    rear = -1;
}

//Helper function to decide is queue contains province
bool Queue::contains(int province) const {
    if (isEmpty()) {
        return false;
    }

    int i = front;
    while (i != rear) {
        if (data[i] == province) {
            return true;
        }
        i = (i + 1) % MAX_QUEUE_SIZE;
    }

    // Explicitly check the rear element
    if (data[rear] == province) {
        return true;
    }

    return false;
}


// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    // TODO: Your code here
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        std::cerr << "Error: Queue overflow. Cannot enqueue " << province << std::endl;

        return;
    }


    if (isEmpty()) {
        front = rear = 0;
    } else {
        rear = (rear + 1) % MAX_QUEUE_SIZE; // Implement circular structure
    }
    // Add the province
    data[rear] = province;
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    // TODO: Your code here
    if (isEmpty()) {
        std::cerr << "Error: Queue underflow. Cannot dequeue from an empty queue." << std::endl;

        return -1;
    }

    int result = data[front];

    if (front == rear) {
        front = rear = -1;
    } else {
        front = (front + 1) % MAX_QUEUE_SIZE;
    }

    return result;
}

// Returns the front province without removing it
int Queue::peek() const {
    // TODO: Your code here
    if (isEmpty()) {
        std::cerr << "Error: Queue underflow. Cannot peek into an empty queue." << std::endl;
        return -1;
    }

    return data[front];
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    // TODO: Your code here
    if (front == -1) {

        return true;
    }
    return false;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    // TODO: Your code here
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        std::cerr << "Error: Queue overflow. Cannot enqueue priority element " << province << std::endl;
        return;
    }
    if (isEmpty()) {
        front = rear = 0;
        data[front] = province;
        return;
    }
    front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    data[front] = province;
}


