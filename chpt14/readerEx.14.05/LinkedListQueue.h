//
// LinkedListQueue.h
//
// This interface exports a template version of the LinkedListQueue class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code comes from Figures 14.6, 14.9, 14.10.
// --------------------------------------------------------------------------
//

#ifndef _linkedlist_queue_h
#define _linkedlist_queue_h

#include "error.h"

//
//  Class: LinkedListQueue<ValueType>
//  -----------------------
//  This class implements a queue of the specified value type.

template <typename ValueType>
class LinkedListQueue {
    
public:
    
    //  Constructor: LinkedListQueue
    //  Usage: LinkedListQueue<ValueType> queue;
    //  ------------------------------
    //  Initializes a new empty queue.
    
    LinkedListQueue();
    
    // Destructor: ~LinkedListQueue
    // Usage: (usually implicit)
    // -------------------------
    // Frees any heap storage associated with this queue.
    
    ~LinkedListQueue();
    
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
    
    // Copy constructor: LinkedListQueue
    // Usage: (usually implicit)
    // -------------------------
    // Initializes the current object to be a deep copy of the specified source.
    
    LinkedListQueue(const LinkedListQueue<ValueType> & src);
    
    // Operator: =
    // Usage: dst = src;
    // -----------------
    // Assigns src to dst so that the two queues are independent copies.
    
    LinkedListQueue & operator=(const LinkedListQueue<ValueType> & src);
    
    // Private section
    
    // Implementation notes: LinkedListQueue data structure
    // ------------------------------------------
    // The list-based queue uses a linked list to store the elements
    // of the queue.  To ensure that adding a new element to the tail
    // of the queue is fast, the data structure maintains a pointer to
    // the last cell in the queue as well as the first.  If the queue is
    // empty, both the head pointer and the tail pointer are set to NULL.
    //
    // The following diagram illustrates the structure of a queue
    // containing two elements, A and B.
    //
    //       +-------+        +-------+        +-------+
    //  head |   o---+------->|   A   |  +--==>|   B   |
    //       +-------+        +-------+  |  |  +-------+
    //  tail |   o---+---+    |   o---+--+  |  |  NULL |
    //       +-------+   |    +-------+     |  +-------+
    //                   |                  |
    //                   +------------------+
    //
    
private:
    
    // Type for linked list cell
    
    struct Cell {
        ValueType data;         // The data value
        Cell *link;             // Link to the next cell
    };
    
    // Instance variables
    
    Cell *head;                 // Pointer to the cell at the head
    Cell *tail;                 // Pointer to the cell at the tail
    int count;                  // Number of elements in the queue
    
    // Private method prototypes
    
    void deepCopy(const LinkedListQueue<ValueType> & src);
    
};

// Logically separate the templatized implementation of the queue
// from the interface through a *.cpp file.

#include "LinkedListQueue.cpp"

#endif // _linkedlist_queue_h
