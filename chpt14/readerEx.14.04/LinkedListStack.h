//
// LinkedListStack.h
//
// This file exports an interface for a template version of a linked list 
// based stack class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code comes from Figures 14.1, 14.4, 14.5.
// --------------------------------------------------------------------------
//

#ifndef _linkedlist_stack_h
#define _linkedlist_stack_h

#include "error.h"

//
//  Class: LinkedListStack<ValueType>
//  -----------------------
//  This class implements a stack of the specified value type.

template <typename ValueType>
class LinkedListStack {
    
public:
    
//  Constructor: LinkedListStack
//  Usage: LinkedListStack<ValueType> stack;
//  ------------------------------
//  Initializes a new empty stack.
    
    LinkedListStack();
    
// Destructor: ~LinkedListStack
// Usage: (usually implicit)
// -------------------------
// Frees any heap storage associated with this stack.
    
    ~LinkedListStack();
    
// Method: size
// Usage: int nElems = stack.size();
// --------------------------------
// Returns the number of values in this stack.
    
    int size() const;
    
// Method: isEmpty
// Usage: if (stack.isEmpty()) . . .
// ---------------------------------
// Returns true if this stack contains no elements.
    
    bool isEmpty() const;
    
// Method: clear
// Usage: stack.clear();
// --------------------
// Removes all elements from this stack.
    
    void clear();
    
// Method: push
// Usage: stack.push(value);
// -------------------------
// Pushes the element value onto this stack.
    
    void push(ValueType value);
    
// Method: pop
// Usage: ValueType value = stack.pop();
// -------------------------------------
// Removes the top element from this stack and returns it.
    
    ValueType pop();
    
// Method: peek
// Usage: ValueType value = stack.peek();
// --------------------------------------
// Returns the value of top element from this stack without
// removing it.  Raises and error if called on an empty stack.
    
    ValueType peek() const;
    
// Copy constructor: LinkedListStack
// Usage: (usually implicit)
// -------------------------
// Initializes the current object to be a deep copy of the specified source.
    
    LinkedListStack(const LinkedListStack<ValueType> & src);
    
// Operator: =
// Usage: dst = src;
// -----------------
// Assigns src to dst so that the two stacks are independent copies.
    
    LinkedListStack & operator=(const LinkedListStack<ValueType> & src);
    
// Private section
    
// Implementation notes
// --------------------
// This version of the stack.h interface uses a linked list to store
// the elements of the stack.  The top item is always at the front of
// the linked list and is therefore always accessible without searching.
// The private data also keeps track of the number of elements so that
// the size method runs in constant time.

private:
    
// Type for linked list cell
    
    struct Cell {
        ValueType data; // The data value
        Cell *link;     // Link to the next cell
    };
    
// Instance variables
    
    Cell *list;         // Initial pointer in the list
    int count;          // Number of elements
    
// Private method prototypes
    
    void deepCopy(const LinkedListStack<ValueType> & src);
    
};

// Logically separate the templatized implementation of the stack
// from the interface through a *.cpp file.

#include "LinkedListStack.cpp"

#endif // _linkedlist_stack_h
