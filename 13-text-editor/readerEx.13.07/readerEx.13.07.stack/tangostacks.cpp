//
// tangostacks.cpp
//
// This file implements the TangoStacks class.
//
// This version is 'const correct', meaning the class uses the const keyword
// to indicate which parameters are subject to change and which methods are
// capable of changing the underlying object state.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 8/02/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include "tangostacks.h"
#include "error.h"
using namespace std;

// Implementation notes: constructor and destructor
// ------------------------------------------------
// The constructor must allocate the array storage for the stack elements
// and initialize the fields of the object.  The destructor frees any heap
// memory allocated by the class, which is just the array of elements.

TangoStacks::TangoStacks() {
    capacity = INITIAL_CAPACITY;
    array = new char[capacity];
    count = lcount = rcount = 0;
}

// Implementation notes: ~TangoStacks
// --------------------------------
// The destructor frees any heap memory allocated by the class, which
// is just the dynamic array of elements.

TangoStacks::~TangoStacks() {
    delete[] array;
}

// Implementation notes: size, isEmpty, clear
// ------------------------------------------
// These methods are each a single line and need no detailed documentation.

int TangoStacks::size() const {
    return count;
}

int TangoStacks::size(const WhichStack stack) const {
    return (stack == LEFT) ? lcount : rcount;
}

bool TangoStacks::isEmpty() const {
    return count == 0;
}

bool TangoStacks::isEmpty(const WhichStack stack) const {
    return (stack == LEFT) ? (lcount == 0) : (rcount == 0);
}

void TangoStacks::clear() {
    count = lcount = rcount = 0;
}

void TangoStacks::clear(const WhichStack stack) {
    if (stack == LEFT) {
        count -= lcount;
        lcount = 0;
    } else {
        count -= rcount;
        rcount = 0;
    }
}

// Implementation notes: push
// --------------------------
// This function must first check to see whether there is enough room for
// the character and expand the array storage if necessary.

void TangoStacks::push(const char ch, const WhichStack stack) {
    if (count == capacity) expandCapacity();
    if (stack == LEFT) {
        array[lcount++] = ch;
    } else {
        array[capacity - ++rcount] = ch;
    }
    count++;
}

// Implementation notes: pop, peek
// -------------------------------
// These functions must check for an empty stack and report an error if
// there is no top element.

char TangoStacks::pop(const WhichStack stack) {
    char result;
    if (isEmpty(stack)) error("pop: Attempting to pop an empty stack");
    if (stack == LEFT) {
        result = array[--lcount];
    } else {
        result = array[capacity - rcount--];
    }
    --count;
    return result;
}

char TangoStacks::peek(const WhichStack stack) const {
    char ch;
    if (isEmpty(stack)) error("peek: Attempting to peek at an empty stack");
    if (stack == LEFT) {
        ch = array[lcount - 1];
    } else {
        ch = array[capacity - rcount];
    }
    return ch;
}

// Implementation notes: getText
// -----------------------------
// Leverage the underlying array implementation of the stacks
// to efficiently iterate over a contiguous section of the array.
//
// This requires no additional memory whereas actually replicating the
// stacks (to avoid mutating the originals) and then popping the results
// requires twice the memory.

std::string TangoStacks::getText(const WhichStack stack, const int nChar,
                                bool reverse) const {
    std::string result = "";
    int maxN = (stack == LEFT) ? lcount : rcount;
    int normalN = min(nChar, maxN);
    
    for (int i = 0; i < normalN; i++) {
        int index;
        if (stack == LEFT) {
            if (reverse) {
                index = (lcount - 1) - (normalN - 1 - i);
            } else {
                index = (lcount - 1) - i;
            }
        } else { // stack == RIGHT
            if (reverse) {
                index = (capacity - rcount) + (normalN - 1 - i);
            } else {
                index = capacity - (rcount - i);
            }
        }
        result += array[index];
    }
    return result;
}

// Implementation notes: copy constructor and assignment operator
// --------------------------------------------------------------
// These methods make it possible to pass a TangoStacks by value or
// assign one TangoStacks to another.  The actual work is done by the
// private deepCopy method, which represents a useful pattern
// for designing other classes that need to implement deep copying.

TangoStacks::TangoStacks(const TangoStacks & src) {
    deepCopy(src);
}

TangoStacks & TangoStacks::operator=(const TangoStacks & src) {
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

void TangoStacks::deepCopy(const TangoStacks & src) {
    array = new char[src.capacity];
    for (int i = 0; i < src.lcount; i++) {
        array[i] = src.array[i];
    }
    for (int i = 0; i < src.rcount; i++) {
        array[src.capacity - i - 1] = src.array[src.capacity - i - 1];
    }
    count = src.count;
    lcount = src.lcount;
    rcount = src.rcount;
    capacity = src.capacity;
}

// Implementation notes: expandCapacity
// ------------------------------------
// This private method doubles the capacity of the elements array whenever
// it runs out of space.  To do so, it must copy the pointer to the old
// array, allocate a new array with twice the capacity, copy the characters
// from the old array to the new one, and finally free the old storage.

void TangoStacks::expandCapacity() {
    char *oldArray = array;
    int oldCapacity = capacity;
    
    capacity *= 2;
    array = new char[capacity];
    for (int i = 0; i < lcount; i++) {
        array[i] = oldArray[i];
    }
    for (int i = 0; i < rcount; i++) {
        array[capacity - i - 1] = oldArray[oldCapacity - i - 1];
    }
    delete[] oldArray;
}
