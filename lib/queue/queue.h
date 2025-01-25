//
// Created by Manoel Amaro on 22.02.24.
//

#ifndef DND_QUEUE_H
#define DND_QUEUE_H

#include "stddef.h"

template<typename T, size_t Size>
class FixedQueue {
private:
    T data[Size];
    size_t head; // Points to the first element
    size_t tail; // Points to the next insertion point
    size_t count; // Number of elements in the queue
public:
    FixedQueue();
    // Add an item to the queue
    bool enqueue(const T &item);
    // Remove an item from the queue
    bool dequeue(T &item);
    // Get the first item in the queue
    bool front(T &item);
    // Check if the queue is empty
    bool isEmpty() const;
    // Check if the queue is full
    bool isFull() const;
    // Get the number of items in the queue
    size_t size() const ;
};

#endif //DND_QUEUE_H
