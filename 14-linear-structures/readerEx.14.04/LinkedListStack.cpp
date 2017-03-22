//
// LinkedListStack.cpp
//
// This file implements a linked list based stack class.
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

//
// Implementation section
// ----------------------
// C++ requires that the implementation for a template class be available
// to that compiler whenever that type is used.  Clients should not need
// to look at any of the code beyond this point.
//

// Implementation notes: LinkedListStack constructor
// ---------------------------------------
// The constructor must create an empty linked list and initialize the count.

template <typename ValueType>
LinkedListStack<ValueType>::LinkedListStack() {
    list = NULL;
    count = 0;
}

// Implementation notes: LinkedListStack destructor
// --------------------------------------
// The destructor frees any heap memory allocated by the
// implementation.  Because clear frees each element it processes,
// this implementation of the destructor simply calls that method.

template <typename ValueType>
LinkedListStack<ValueType>::~LinkedListStack() {
    clear();
}

//
// Implementation notes: size, isEmpty
// -----------------------------------
// These methods use the count variable and therefore run in constant time.
//

template <typename ValueType>
int LinkedListStack<ValueType>::size() const {
    return count;
}

template <typename ValueType>
bool LinkedListStack<ValueType>::isEmpty() const {
    return count == 0;
}

//
// Implementation notes: clear
// ---------------------------
// This method chains a new element onto the front of the list where it
// becomes the top of the stack.
//

template <typename ValueType>
void LinkedListStack<ValueType>::clear() {
    while (count > 0) {
        pop();
    }
}

//
// Implementation notes: push
// --------------------------
// This method chains a new element onto the front of the list where it
// becomes the top of the stack.
//

template <typename ValueType>
void LinkedListStack<ValueType>::push(ValueType value) {
    Cell *cp = new Cell;
    cp->data = value;
    cp->link = list;
    list = cp;
    count++;
}

// Implementation notes: pop, peek
// -------------------------------
// These methods must check for an empty stack and report an error if
// there is no top element.  The pop method must free the cell to ensure
// that the implementation does not leak memory as it executes.

template <typename ValueType>
ValueType LinkedListStack<ValueType>::pop() {
    if (isEmpty()) error("pop: Attempting to pop an empty stack");
    Cell *cp = list;
    ValueType result = cp->data;
    list = list->link;
    count--;
    delete cp;
    return result;
}

template <typename ValueType>
ValueType LinkedListStack<ValueType>::peek() const {
    if (isEmpty()) error("peek: Attempting to peek at an empty stack");
    return list->data;
}

// Implementation notes: copy constructor and assignment operator
// --------------------------------------------------------------
// These methods follow the standard template, leaving the work to deepCopy.

template <typename ValueType>
LinkedListStack<ValueType>::LinkedListStack(const LinkedListStack<ValueType> & src) {
    deepCopy(src);
}

template <typename ValueType>
LinkedListStack<ValueType> & LinkedListStack<ValueType>::operator=(const LinkedListStack<ValueType> & src) {
    if (this != &src) {
        clear();
        deepCopy(src);
    }
    return *this;
}

// Implementation notes: deepCopy
// ------------------------------
// The deepCopy method creates a copy of the cells in the linked list.
// The variable tail keeps track of the last cell in the chain.

template <typename ValueType>
void LinkedListStack<ValueType>::deepCopy(const LinkedListStack<ValueType> & src) {
    count = src.count;
    Cell *tail = NULL;
    for (Cell *cp = src.list; cp != NULL; cp = cp->link) {
        Cell *ncp = new Cell;
        ncp->data = cp->data;
        if (tail == NULL) {
            list = ncp;
        } else {
            tail->link = ncp; // wire-in
        }
        tail = ncp; // wire-out
    }
    if (tail != NULL) tail->link = NULL;
}
