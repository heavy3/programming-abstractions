//
// ArrayStack.h
//
// This file exports an interface for a template version of an array-based 
// stack class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code comes from Figures 14.1 - 14.3.
// --------------------------------------------------------------------------
//

#ifndef _array_stack_h
#define _array_stack_h

#include "error.h"

//
//  Class: ArrayStack<ValueType>
//  -----------------------
//  This class implements a stack of the specified value type.

template <typename ValueType>
class ArrayStack {
    
public:
    
//  Constructor: ArrayStack
//  Usage: ArrayStack<ValueType> stack;
//  ------------------------------
//  Initializes a new empty stack.
    
    ArrayStack();
    
// Destructor: ~ArrayStack
// Usage: (usually implicit)
// -------------------------
// Frees any heap storage associated with this stack.
    
    ~ArrayStack();
    
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
    
// Copy constructor: ArrayStack
// Usage: (usually implicit)
// -------------------------
// Initializes the current object to be a deep copy of the specified source.
    
    ArrayStack(const ArrayStack<ValueType> & src);
    
// Operator: =
// Usage: dst = src;
// -----------------
// Assigns src to dst so that the two stacks are independent copies.
    
    ArrayStack & operator=(const ArrayStack<ValueType> & src);
    
// Private section
    
// Implementation notes
// --------------------
// This version of the stack.h interface uses a dynamic array to store
// the elements of the stack.  The array begins with INITIAL_CAPACITY
// elements and doubles the size whenever it runs out of space.  This
// discipline guarantees that the push method has O(1) amortized cost.

private:
    
// Private constants
    
    static const int INITIAL_CAPACITY = 10;
    
// Instance variables
    
    ValueType *array;   // Dynamic array of the elements
    int capacity;       // The allocated size of the array
    int count;          // The number of stack elements
    
// Private method prototypes
    
    void deepCopy(const ArrayStack<ValueType> & src);
    void expandCapacity();
    
};

// Logically separate the templatized implementation of the stack
// from the interface through a *.cpp file.

#include "ArrayStack.cpp"

#endif // _array_stack_h
