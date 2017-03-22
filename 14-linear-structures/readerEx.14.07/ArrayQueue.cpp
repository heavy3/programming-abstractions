//
// ArrayQueue.cpp
//
// This file implements a Queue class based upon an array.
// It includes support for a reverse() method that does not require additional
// memory allocation.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// See chapter 5, exercise 13 for the non-templatized method.
// This code draws heavily upon code in Figures 14-6 and 14-7.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/6/16. (derivative work)
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

// Implementation notes: ArrayQueue constructor
// ---------------------------------------
// The constructor must create the array storage and initializes the fields.

#include <sstream>

template <typename ValueType>
ArrayQueue<ValueType>::ArrayQueue() {
    capacity = INITIAL_CAPACITY;
    array = new ValueType[capacity];
    head = 0;
    tail = 0;
}

// Implementation notes: ArrayQueue destructor
// --------------------------------------
// The destructor frees any heap memory allocated by the
// implementation.

template <typename ValueType>
ArrayQueue<ValueType>::~ArrayQueue() {
    delete[] array;
}

//
// Implementation notes: size
// --------------------------
// The size is calculated from head and tail using modular arithmetic.
//

template <typename ValueType>
int ArrayQueue<ValueType>::size() const {
    return (tail + capacity - head) % capacity;
}

//
// Implementation notes: isEmpty
// -----------------------------
// The queue is empty whenever the head and tail pointers are equal.  This
// interpretation means that the queue must always leave one unused space.

template <typename ValueType>
bool ArrayQueue<ValueType>::isEmpty() const {
    return head == tail;
}

//
// Implementation notes: clear
// ---------------------------
// The clear method need not take account of where in the ring buffer any
// existing data is stored and can simply reset the head and tail indices.
//

template <typename ValueType>
void ArrayQueue<ValueType>::clear() {
    head = tail = 0;
}

//
// Implementation notes: enqueue
// -----------------------------
// This method must first check to see whether there is enough room for
// the element and expand the array storage if necessary.  Because it is
// otherwise impossible to differentiate the case when a queue is empty
// from when it is completely full, this implementation expands the
// queue when the size is one less than the capacity.
//

template <typename ValueType>
void ArrayQueue<ValueType>::enqueue(ValueType value) {
    if (size() == capacity - 1) expandCapacity();
    array[tail] = value;
    tail = (tail + 1) % capacity;
}

// Implementation notes: dequeue, peek
// -----------------------------------
// These methods must check for an empty queue and report an
// error if there is not first element.

template <typename ValueType>
ValueType ArrayQueue<ValueType>::dequeue() {
    if (isEmpty()) error("dequeue: Attempting to dequeue an empty queue");
    ValueType value = array[head];
    head = (head + 1) % capacity;
    return value;
}

template <typename ValueType>
ValueType ArrayQueue<ValueType>::peek() const {
    if (isEmpty()) error("dequeue: Attempting to peek at an empty queue");
    return array[head];
}

// Implementation notes: reverse
// ------------------------------
// Reverses the order of elements in the queue using an in-place algorithm.

template <typename ValueType>
void ArrayQueue<ValueType>::reverse() {
    int count = size();
    int midway = count / 2;
    for (int i = 0; i < midway; i++) {
        int fromHead = (head + i) % capacity;
        int fromTail = (tail + capacity - 1 - i ) % capacity;
        swap(array[fromHead], array[fromTail]);
    }
}

// Implementation notes: toString
// ------------------------------
// Return a string representation of the queue contents.

template <typename ValueType>
std::string ArrayQueue<ValueType>::toString() const {
    std::ostringstream oss;
    
    int count = size();
    for (int i = 0; i < count; i++) {
        int index = (head + i) % capacity;
        oss << array[index] << " ";
    }
    return oss.str();
}

// Implementation notes: Deep copying support
// ------------------------------------------
// These methods implement deep copying for queues.

template <typename ValueType>
ArrayQueue<ValueType>::ArrayQueue(const ArrayQueue<ValueType> & src) {
    deepCopy(src);
}

template <typename ValueType>
ArrayQueue<ValueType> & ArrayQueue<ValueType>::operator=(const ArrayQueue<ValueType> & src) {
    if (this != &src) {
        delete[] array;
        deepCopy(src);
    }
    return *this;
}

template <typename ValueType>
void ArrayQueue<ValueType>::deepCopy(const ArrayQueue<ValueType> & src) {
    int count = src.size();
    capacity = count + src.capacity;
    array = new ValueType[capacity];
    for (int i = 0; i < count; i++) {
        array[i] = src.array[(src.head + i) % src.capacity];
    }
    head = 0;
    tail = count;
}

template <typename ValueType>
void ArrayQueue<ValueType>::expandCapacity() {
    int count = size();
    ValueType *oldArray = array;
    array = new ValueType[2 * capacity];
    for (int i = 0; i < count; i++) {
        array[i] = oldArray[(head + i) % capacity];
    }
    capacity *= 2;
    head = 0;
    tail = count;
    delete[] oldArray;
}

template <typename ValueType>
void ArrayQueue<ValueType>::swap(ValueType & x, ValueType & y) {
    ValueType tmp;
    
    tmp = x;
    x = y;
    y = tmp;
}
