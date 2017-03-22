//
// ArrayQueue.h
//
// This file exports an interface for an array-based Queue class
// that supports a reverse() method.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code draws heavily upon code in Figures 14-6 and 14-7.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/6/16. (derivative work)
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#ifndef _array_queue_h
#define _array_queue_h

#include "error.h"

//
//  Class: ArrayQueue<ValueType>
//  -----------------------
//  This class implements a queue of the specified value type.

template <typename ValueType>
class ArrayQueue {
    
public:
    
    //  Constructor: ArrayQueue
    //  Usage: ArrayQueue<ValueType> queue;
    //  ------------------------------
    //  Initializes a new empty queue.
    
    ArrayQueue();
    
    // Destructor: ~ArrayQueue
    // Usage: (usually implicit)
    // -------------------------
    // Frees any heap storage associated with this queue.
    
    ~ArrayQueue();
    
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
    
    // Method: reverse
    // Usage: queue.reverse();
    // -----------------------
    // Reverses the order of items in the queue.
    
    void reverse();
    
    // Method: toString
    // Usage: queue.toString();
    // -----------------------
    // Return a string representation of the queue contents.
    
    std::string toString() const;
    
    // Copy constructor: ArrayQueue
    // Usage: (usually implicit)
    // -------------------------
    // Initializes the current object to be a deep copy of the specified source.
    
    ArrayQueue(const ArrayQueue<ValueType> & src);
    
    // Operator: =
    // Usage: dst = src;
    // -----------------
    // Assigns src to dst so that the two queues are independent copies.
    
    ArrayQueue & operator=(const ArrayQueue<ValueType> & src);
    
    // Private section
    
    // Implementation notes
    // --------------------
    // The array-based queue stores the elements in successive index
    // positions in an array, just as a stack does.  What makes the
    // queue structure more complex is the need to avoid shifting
    // elements as the queue expands and contracts.  In the array
    // model, this goal is achieved by keeping track of both the
    // head and tail indices.  The tail index increases by one each
    // time an element is enqueued, and the head index increases by
    // one each time an element is dequeued.  Each index therefore
    // marches toward the end of the allocated array and will
    // eventually reach the end.  Rather than allocate new memory,
    // this implementation lets each index wrap around back to the
    // beginning as if the ends of the array of elements were joined
    // to form a circle.  This representation is called a ring buffer.
    //
    // The elements of the queue are stored in a dynamic array of the
    // specified element type.  If the space in the array is ever
    // exhausted, the implementation doubles the array capacity.
    // Note that the queue capacity is reached when there is still
    // one unsued element in the array.  If the queue is allowed to
    // fill completely, the head and tail indices will have the same
    // value, and the queue will appear empty.
    
private:
    
    static const int INITIAL_CAPACITY = 10;
    
    // Instance variables
    
    ValueType *array;           // A dynamic array of elements
    int capacity;               // The allocated size of the array
    int head;                   // The index of the head element
    int tail;                   // The index of the tail element
    
    // Private method prototypes
    
    void deepCopy(const ArrayQueue<ValueType> & src);
    void expandCapacity();
    void swap(ValueType & x, ValueType & y);
    
};

// Logically separate the templatized implementation of the stack
// from the interface through a *.cpp file.

#include "ArrayQueue.cpp"

#endif // _array_queue_h
