//
// queue.h
//
// This file exports a templatized interface for a ring-buffer queue backed
// by a managed array.
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

#ifndef _queue_h
#define _queue_h

#include "error.h"

//
// Class: Queue<ValueType>
// -----------------------
// This class implements a queue of the specified value type.

template <typename ValueType>
class Queue {
    
public:
    
// Constructor: Queue
// Usage: Queue<ValueType> queue;
// ------------------------------
// Initializes a new empty queue.
    
    Queue();
    
// Destructor: ~Queue
// Usage: (usually implicit)
// -------------------------
// Frees any heap storage associated with this queue.
    
    ~Queue();
    
// Method: size
// Usage: int nElems = queue.size();
// --------------------------------
// Returns the number of values in this queue.
    
    int size() const;
    
// Method: isEmpty
// Usage: if (queue.isEmpty()) . . .
// ---------------------------------
// Returns true if this queue contains no elements.
    
    bool isEmpty() const;
    
// Method: clear
// Usage: queue.clear();
// --------------------
// Removes all elements from this queue.
    
    void clear();
    
// Method: enqueue
// Usage: queue.enqueue(value);
// -------------------------
// Adds value to the end of the queue.
    
    void enqueue(ValueType value);
    
// Method: dequeue
// Usage: ValueType value = queue.dequeue();
// -------------------------------------
// Removes and returns the first item in the queue.  This method
// signals an error if called on an empty queue.
    
    ValueType dequeue();
    
// Method: peek
// Usage: ValueType value = queue.peek();
// --------------------------------------
// Returns the first value in the queue without
// removing it.  Raises and error if called on an empty queue.
    
    ValueType peek() const;
    
// Copy constructor: Queue
// Usage: (usually implicit)
// -------------------------
// Initializes the current object to be a deep copy of the specified source.
    
    Queue(const Queue<ValueType> & src);
    
// Operator: =
// Usage: dst = src;
// -----------------
// Assigns src to dst so that the two queues are independent copies.
    
    Queue & operator=(const Queue<ValueType> & src);
    
// Method: getCapacity
// Usage: int thecapacity = getCapacity();
//        int theInitCap = getInitialCapacity();
// ---------------------------------------------
// Returns the current capacity (or initial capacity) of the underlying array
// primarily for test purposes so array expansion can be validated.
    
    int getCapacity() const;
    int getInitialCapacity() const;
    
// Private section
    
// Implementation notes
// --------------------
// The queue uses an array to hold elements in successive order. As the array fills, 
// a count of elements is maintained such that the tail (which serves as the next 
// insertion point) may be computed from some modulo math with the head index.  This 
// has the effect of given the array a 'ring'.  When the array fills, the capacity 
// of the array is doubled.

head index is incremented to identify

private:
    
    static const int INITIAL_CAPACITY = 10;
    
// Instance variables
    
    ValueType *array;           // A dynamic array of elements
    int capacity;               // The allocated size of the array
    int head;                   // The index of the head element
    int count;                  // The number of items in the queue.
    
// Private method prototypes
    
    void deepCopy(const Queue<ValueType> & src);
    void expandCapacity();
    int tail() const;
};

// Separate the templatized implementation of the queue from the 
// interface through a *.cpp file.

#include "queue.cpp"

#endif // _queue_h
