//
// ArrayStack.cpp
//
// This file implements an array-based stack class.
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

// Implementation section
// ----------------------
// C++ requires that the implementation for a template class be available
// to that compiler whenever that type is used.  The effect of this
// restriction is that header files must include the implementation.
// Clients should not need to look at any of the code beyond this point.
//

// Implementation notes: ArrayStack constructor
// ---------------------------------------
// The constructor must allocate the array storage for the stack elements
// and initialize the fields of the object.

template <typename ValueType>
ArrayStack<ValueType>::ArrayStack() {
    capacity = INITIAL_CAPACITY;
    array = new ValueType[capacity];
    count = 0;
}

// Implementation notes: ~ArrayStack
// ----------------------------
// The destructor frees any heap memory allocated by the class, which
// is just the dynamic array of elements.

template <typename ValueType>
ArrayStack<ValueType>::~ArrayStack() {
    delete[] array;
}

template <typename ValueType>
int ArrayStack<ValueType>::size() const {
    return count;
}

template <typename ValueType>
bool ArrayStack<ValueType>::isEmpty() const {
    return count == 0;
}

template <typename ValueType>
void ArrayStack<ValueType>::clear() {
    count = 0;
}

// Implementation notes: push
// --------------------------
// This function must first check to see whether there is enough room for
// the value and expand the array storage if necessary.

template <typename ValueType>
void ArrayStack<ValueType>::push(ValueType value) {
    if (count == capacity) expandCapacity();
    array[count++] = value;
}

// Implementation notes: pop, peek
// -------------------------------
// These functions must check for an empty stack and report an error if
// there is no top element.

template <typename ValueType>
ValueType ArrayStack<ValueType>::pop() {
    if (isEmpty()) error("pop: Attempting to pop an empty stack");
    return array[--count];
}

template <typename ValueType>
ValueType ArrayStack<ValueType>::peek() const {
    if (isEmpty()) error("peek: Attempting to peek at an empty stack");
    return array[count - 1];
}

// Implementation notes: copy constructor and assignment operator
// --------------------------------------------------------------
// These methods follow the standard template, leaving the work to deepCopy.

template <typename ValueType>
ArrayStack<ValueType>::ArrayStack(const ArrayStack<ValueType> & src) {
    deepCopy(src);
}

template <typename ValueType>
ArrayStack<ValueType> & ArrayStack<ValueType>::operator=(const ArrayStack<ValueType> & src) {
    if (this != &src) {
        delete[] array;
        deepCopy(src);
    }
    return *this;
}

// Implementation notes: deepCopy
// ------------------------------
// Copies the data from the src parameter into the current object.
// All dynamic memory is reallocated to create a "deep copy" in
// which the current object and the source object are independent.
// The capacity is set so the stack has some room to expand.

template <typename ValueType>
void ArrayStack<ValueType>::deepCopy(const ArrayStack<ValueType> & src) {
    capacity = src.count + INITIAL_CAPACITY;
    this->array = new ValueType[capacity];
    for (int i = 0; i < src.count; i++) {
        array[i] = src.array[i];
    }
    count = src.count;
}

// Implementation notes: expandCapacity
// ------------------------------------
// This private method doubles the capacity of the elements array whenever
// it runs out of space.  To do so, it must copy the pointer to the old
// array, allocate a new array with twice the capacity, copy the values
// from the old array to the new one, and finally free the old storage.

template <typename ValueType>
void ArrayStack<ValueType>::expandCapacity() {
    ValueType *oldArray = array;
    capacity *= 2;
    array = new ValueType[capacity];
    for (int i = 0; i < count; i++) {
        array[i] = oldArray[i];
    }
    delete[] oldArray;
}
