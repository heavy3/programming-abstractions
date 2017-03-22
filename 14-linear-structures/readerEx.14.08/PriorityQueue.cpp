//
// PriorityQueue.cpp
//
// This file implements a template version of the PriorityQueue class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file extends code from Figures 14.6, 14.9, 14.10.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/23/17.
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//

// Implementation notes: PriorityQueue constructor
// -------------------------------------------------
// The constructor creates an empty linked list and sets the count to 0.
//

template <typename ValueType>
PriorityQueue<ValueType>::PriorityQueue() {
    count = 0;
}

// Implementation notes: PriorityQueue destructor
// ------------------------------------------------
// The destructor frees any heap memory allocated by the queue.
//

template <typename ValueType>
PriorityQueue<ValueType>::~PriorityQueue() {
    count = 0;
    priorities.clear();
    queues.clear();
}

//
// Implementation notes: size, isEmpty, clear
// ------------------------------------------
// These methods use the count variable and therefore run in constant time.
//

template <typename ValueType>
int PriorityQueue<ValueType>::size() const {
    return count;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::isEmpty() const {
    return count == 0;
}

template <typename ValueType>
void PriorityQueue<ValueType>::clear() {
    // clear individual linked list queues
    queues.clear();

    // clear queue nodes from set
    priorities.clear();
}

//
// Implementation notes: enqueue
// -----------------------------
// This method checks the set for the presence of a matching priority
// element.  If not found, it is added to the set.  (The set is essential
// for managing the subqueues in priority order with Set.first() granting
// access to the overall 'head' of the priority queue.)
//
// The priority is then used as a key into a map so the right queue
// can be referenced for enqueuing the value passed in from the client.
//

template <typename ValueType>
void PriorityQueue<ValueType>::enqueue(ValueType value, double priority) {
    if (!priorities.contains(priority)) {
        priorities.add(priority);
    }
    queues[priority].enqueue(value);
    ++count;
}

//
// Implementation notes: dequeue, peek
// -----------------------------------
// These methods must check for an empty queue and report an error if
// there is no first element.
//

template <typename ValueType>
ValueType PriorityQueue<ValueType>::dequeue() {
    ValueType result;
    
    if (priorities.isEmpty()) error("dequeue: Attempting to dequeue an empty queue");
    double priority = priorities.first();
    if (queues[priority].isEmpty()) {
        error("dequeue: set is not empty but queue is :-/");
    }
    
    result = queues[priority].dequeue();
    --count;
    
    if (queues[priority].isEmpty()) {
        priorities.remove(priority);
    }
    return result;
}

template <typename ValueType>
ValueType PriorityQueue<ValueType>::peek() const {
    if (priorities.isEmpty())
        error("peek: Attempting to peek at an empty queue set");
    double priority = priorities.first();
    if (queues[priority].isEmpty())
        error("peek: Attempting to peek at an empty queue");
    return queues[priority].peek();
}

//
// Implementation notes: copy constructor and assignment operator
// --------------------------------------------------------------
// These methods follow the standard template, leaving the work to deepCopy.
//

template <typename ValueType>
PriorityQueue<ValueType>::PriorityQueue(const PriorityQueue<ValueType> & src) {
    deepCopy(src);
}

template <typename ValueType>
PriorityQueue<ValueType> & PriorityQueue<ValueType>::operator=(const PriorityQueue<ValueType> & src) {
    if (this != &src) {
        clear();
        deepCopy(src);
    }
    return *this;
}

//
// Implementation notes: deepCopy
// ------------------------------
// Copies the data from the src parameter into the current object.
//

template <typename ValueType>
void PriorityQueue<ValueType>::deepCopy(const PriorityQueue<ValueType> & src) {
    count = src.count;
    priorities = src.priorities;
    queues = src.queues;
}
