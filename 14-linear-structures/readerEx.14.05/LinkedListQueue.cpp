//
// LinkedListQueue.cpp
//
// This file implements a template version of the LinkedListQueue class.
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

// Implementation section
// ----------------------
// C++ requires that the implementation for a template class be available
// to that compiler whenever that type is used.  The effect of this
// restriction is that header files must include the implementation.
// Clients should not need to look at any of the code beyond this point.
//
// Eric Roberts, Programming Abstractions in C++ (©) 2012, Chpt 14
//
// This code comes from Figures 14.6, 14.9, 14.10.

// Implementation notes: LinkedListQueue constructor
// -------------------------------------------------
// The constructor creates an empty linked list and sets the count to 0.
//

template <typename ValueType>
LinkedListQueue<ValueType>::LinkedListQueue() {
    head = tail = NULL;
    count = 0;
}

// Implementation notes: LinkedListQueue destructor
// ------------------------------------------------
// The destructor frees any heap memory allocated by the queue.
//

template <typename ValueType>
LinkedListQueue<ValueType>::~LinkedListQueue() {
    clear();
}

//
// Implementation notes: size, isEmpty, clear
// ------------------------------------------
// These methods use the count variable and therefore run in constant time.
//

template <typename ValueType>
int LinkedListQueue<ValueType>::size() const {
    return count;
}

template <typename ValueType>
bool LinkedListQueue<ValueType>::isEmpty() const {
    return count == 0;
}

template <typename ValueType>
void LinkedListQueue<ValueType>::clear() {
    while (count > 0) {
        dequeue();
    }
}

//
// Implementation notes: enqueue
// -----------------------------
// This method allocates a new list cell and chains it in at the tail of
// the queue.  If the queue is currently empty, the new cell must also
// become the head pointer in the queue.
//

template <typename ValueType>
void LinkedListQueue<ValueType>::enqueue(ValueType value) {
    Cell *cp = new Cell;
    cp->data = value;
    cp->link = NULL;
    if (head == NULL) {
        head = cp;
    } else {
        tail->link = cp;
    }
    tail = cp;
    count++;
}

//
// Implementation notes: dequeue, peek
// -----------------------------------
// These methods must check for an empty queue and report an error if
// there is no first element.  The dequeue method must also check for
// the case in which the queue becomes empty and set both the head
// and tail pointers to NULL.
//

template <typename ValueType>
ValueType LinkedListQueue<ValueType>::dequeue() {
    if (isEmpty()) error("dequeue: Attempting to dequeue an empty queue");
    Cell *cp = head;
    ValueType result = cp->data;
    head = cp->link;
    if (head == NULL) tail = NULL;
    delete cp;
    count--;
    return result;
}

template <typename ValueType>
ValueType LinkedListQueue<ValueType>::peek() const {
    if (isEmpty()) error("dequeue: Attempting to peek at an empty queue");
    return head->data;
}

//
// Implementation notes: copy constructor and assignment operator
// --------------------------------------------------------------
// These methods follow the standard template, leaving the work to deepCopy.
//

template <typename ValueType>
LinkedListQueue<ValueType>::LinkedListQueue(const LinkedListQueue<ValueType> & src) {
    deepCopy(src);
}

template <typename ValueType>
LinkedListQueue<ValueType> & LinkedListQueue<ValueType>::operator=(const LinkedListQueue<ValueType> & src) {
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
// This implementation simply walks down the linked list in the
// source object and enques each value in the destination.
//

template <typename ValueType>
void LinkedListQueue<ValueType>::deepCopy(const LinkedListQueue<ValueType> & src) {
    head = NULL;
    tail = NULL;
    count = 0;
    for (Cell *cp = src.head; cp != NULL; cp = cp->link) {
        enqueue(cp->data);
    }
}
