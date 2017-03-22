//
// PriorityQueue.h
//
// This interface exports a template version of the PriorityQueue class.
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

#ifndef _priority_queue_h
#define _priority_queue_h

#include "LinkedListQueue.h"
#include "set.h"
#include "map.h"
#include "error.h"

//
//  Class: PriorityQueue<ValueType>
//  -----------------------
//  This class implements a queue of the specified value type.

template <typename ValueType>
class PriorityQueue {
    
public:
    
    //  Constructor: PriorityQueue
    //  Usage: PriorityQueue<ValueType> queue;
    //  ------------------------------
    //  Initializes a new empty queue.
    
    PriorityQueue();
    
    // Destructor: ~PriorityQueue
    // Usage: (usually implicit)
    // -------------------------
    // Frees any heap storage associated with this queue.
    
    ~PriorityQueue();
    
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
    
    void enqueue(ValueType value, double p);
    
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
    
    // Copy constructor: PriorityQueue
    // Usage: (usually implicit)
    // -------------------------
    // Initializes the current object to be a deep copy of the specified source.
    
    PriorityQueue(const PriorityQueue<ValueType> & src);
    
    // Operator: =
    // Usage: dst = src;
    // -----------------
    // Assigns src to dst so that the two queues are independent copies.
    
    PriorityQueue & operator=(const PriorityQueue<ValueType> & src);
    
    // Private section
    
    // Implementation notes: PriorityQueue data structure
    // --------------------------------------------------
    // The following diagram illustrates the conceptual structure of
    // the priority queue implemented as a set of priority
    // elements aggregated with a map of queues using priority as the key.
    //
    // Initially, I hoped for just a set of priority queues,
    // however the comparator function for maintaining set ordering
    // was beyond my ability given it required factoring out the queue
    // portion of the element.  I'm sure there's an elegant
    // way to do this.  But I settled upon adding a dedicated
    // map like so:
    //
    //   +---------------------------------------------------+
    //   | Priority Queue                                    |
    //   | +-----------------------------------------------+ |
    //   | |  Set             +--------+                   | |
    //   | |                  |priority|                   | |
    //   | |                  |  n + 1 |                   | |
    //   | |                  +--------+                   | |
    //   | |    +--------+                   +--------+    | |
    //   | |    |priority|                   |priority|    | |
    //   | |    |    n   |                   |  n + 2 |    | |
    //   | |    +--------+                   +--------+    | |
    //   | +-----------------------------------------------+ |
    //   | +-----------------------------------+             |
    //   | |  Map   key           value        |             |
    //   | |    +--------+  +---------------+  |             |
    //   | |    |priority|--|Q (linked list)|  |             |
    //   | |    |  n + 1 |  +---------------+  |             |
    //   | |    +--------+                     |             |
    //   | |    +--------+  +---------------+  |             |
    //   | |    |priority|--|Q (linked list)|  |             |
    //   | |    |    n   |  +---------------+  |             |
    //   | |    +--------+                     |             |
    //   | |        ...           ...          |             |
    //   | +-----------------------------------+             |
    //   +---------------------------------------------------+
    //
    // Priority value establishes ordering.  Lower priority
    // values have higher precedence.  The Set.first() method allows us
    // to discern the overall 'head' of the queue, so to speak.
    // The resulting priority indexes into the map where the
    // associated queue is found as a prelude to dequeue, or
    // peek operations.  For enqueue, the client passes in the
    // priority value so we don't need to invoke first().  We
    // simply index into the map directly if a corresponding
    // queue already exists at that priority.
    
private:

    // Private method prototypes
    
    void deepCopy(const PriorityQueue<ValueType> & src);
    
    // Instance variables
    
    Set<double> priorities;
    Map<double, LinkedListQueue<ValueType> > queues;
    int count;
};

// Logically separate the templatized implementation of the queue
// from the interface through a *.cpp file.

#include "PriorityQueue.cpp"

#endif // _priority_queue_h
