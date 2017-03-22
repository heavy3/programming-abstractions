//
// queue.cpp
//
// This file implements an array-based ring-buffer queue that overcomes
// the typical idiom of leaving one slot empty to simplify distinguishing between
// and empty queue and a full queue (wherein the head and tail pointers are
// the same).
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 6
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file extends code from Figures 14.7.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/6/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

// Implementation notes: Queue constructor
// ---------------------------------------
// The constructor must create the array storage and initializes the fields.

template <typename ValueType>
Queue<ValueType>::Queue() {
    capacity = INITIAL_CAPACITY;
    array = new ValueType[capacity];
    head = 0;
    count = 0;
}

// Implementation notes: Queue destructor
// --------------------------------------
// The destructor frees any heap memory allocated by the
// implementation.

template <typename ValueType>
Queue<ValueType>::~Queue() {
    delete[] array;
}

//
// Implementation notes: size
// --------------------------
// The size is calculated from a count of the number of items in the queue.
//

template <typename ValueType>
int Queue<ValueType>::size() const {
    return count;
}

//
// Implementation notes: isEmpty
// -----------------------------
// The queue is empty whenever the count of items is zero.

template <typename ValueType>
bool Queue<ValueType>::isEmpty() const {
    return count == 0;
}

//
// Implementation notes: clear
// ---------------------------
// The clear method need not take account of where in the ring buffer any
// existing data is stored and can simply reset the head and count variables.
//

template <typename ValueType>
void Queue<ValueType>::clear() {
    head = count = 0;
}

//
// Implementation notes: enqueue
// -----------------------------
// This method must first check to see whether there is enough room for
// the element and expand the array storage if necessary.

template <typename ValueType>
void Queue<ValueType>::enqueue(ValueType value) {
    if (size() == capacity) expandCapacity();
    array[tail()] = value;
    ++count;
}

// Implementation notes: dequeue, peek
// -----------------------------------
// These methods must check for an empty queue and report an error if there is
// not a first element.

template <typename ValueType>
ValueType Queue<ValueType>::dequeue() {
    if (isEmpty()) error("dequeue: Attempting to dequeue an empty queue");
    ValueType value = array[head];
    head = (head + 1) % capacity;
    --count;
    return value;
}

template <typename ValueType>
ValueType Queue<ValueType>::peek() const {
    if (isEmpty()) error("dequeue: Attempting to peek at an empty queue");
    return array[head];
}

// Implementation notes: tail
// --------------------------
// Use modular arithmetic to compute the tail index where new values
// arrive in the queue.
//
// Computing tail from head and count allows full use of the underlying
// array since it otherwise avoids confusion between empty and full queues
// should the tail index wrap around to the same value as the head index
// within the ring buffer.

template <typename ValueType>
int Queue<ValueType>::tail() const {
    return (head + count) % capacity;
}

// Implementation notes: getCapacity, getInitialCapacity
// -----------------------------------------------------
// Returns the current capacity (and INITIAL_CAPACITY) of the underlying array
// primarily for test purposes so array expansion can be validated.
//
// TODO: Should I be using 'friend' or 'protected' status since this
//       method is of only specialized interest to unit test drivers and not
//       for general consumption by most clients?

template <typename ValueType>
int Queue<ValueType>::getCapacity() const {
    return capacity;
}

template <typename ValueType>
int Queue<ValueType>::getInitialCapacity() const {
    return INITIAL_CAPACITY;
}

// Implementation notes: Deep copying support
// ------------------------------------------
// These methods implement deep copying for queues.

template <typename ValueType>
Queue<ValueType>::Queue(const Queue<ValueType> & src) {
    deepCopy(src);
}

template <typename ValueType>
Queue<ValueType> & Queue<ValueType>::operator=(const Queue<ValueType> & src) {
    if (this != &src) {
        delete[] array;
        deepCopy(src);
    }
    return *this;
}

template <typename ValueType>
void Queue<ValueType>::deepCopy(const Queue<ValueType> & src) {
    int count = src.size();
    capacity = count + src.capacity;
    array = new ValueType[capacity];
    for (int i = 0; i < count; i++) {
        array[i] = src.array[(src.head + i) % src.capacity];
    }
    head = 0;
}

template <typename ValueType>
void Queue<ValueType>::expandCapacity() {
    ValueType *oldArray = array;
    array = new ValueType[2 * capacity];
    for (int i = 0; i < count; i++) {
        array[i] = oldArray[(head + i) % capacity];
    }
    capacity *= 2;
    head = 0;
    delete[] oldArray;
}
