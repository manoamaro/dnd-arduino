//
// Created by Manoel Amaro on 22.02.24.
//

#include "queue.h"

template<typename T, size_t Size>
FixedQueue<T, Size>::FixedQueue() : head(0), tail(0), count(0) {}

template<typename T, size_t Size>
bool FixedQueue<T, Size>::enqueue(const T& item) {
    if (count == Size) {
        return false; // Queue is full
    }
    data[tail] = item;
    tail = (tail + 1) % Size;
    ++count;
    return true;
}

template<typename T, size_t Size>
bool FixedQueue<T, Size>::dequeue(T& item) {
    if (count == 0) {
        return false; // Queue is empty
    }
    item = data[head];
    head = (head + 1) % Size;
    --count;
    return true;
}

template<typename T, size_t Size>
bool FixedQueue<T, Size>::front(T& item) {
    if (count == 0) {
        return false; // Queue is empty
    }
    item = data[head];
    return true;
}

template<typename T, size_t Size>
bool FixedQueue<T, Size>::isEmpty() const {
    return count == 0;
}

template<typename T, size_t Size>
bool FixedQueue<T, Size>::isFull() const {
    return count == Size;
}

template<typename T, size_t Size>
size_t FixedQueue<T, Size>::size() const {
    return count;
}
