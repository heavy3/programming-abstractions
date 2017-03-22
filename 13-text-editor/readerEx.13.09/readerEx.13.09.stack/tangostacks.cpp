//
// tangostacks.cpp
//
// This file implements the TangoStacks class.
//
// This version is 'const correct', meaning the class uses the const keyword
// to indicate which parameters are subject to change and which methods are
// capable of changing the underlying object state.
//
// Augmented with more nuanced variants of getText to support copy, cut,
// paste, and search.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 8/12/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include "tangostacks.h"
#include "error.h"
using namespace std;

static const char SPACE = ' ';
static const char NEWLINE = '\n';

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

// Implementation notes: getText, getTextWords
// -------------------------------------------
// Leverage the underlying array implementation of the stacks
// to efficiently iterate over a contiguous section of the array
// until the desired number of characters or words have been encountered.
// We fallback on the stack abstraction to pop text off the stack
// if clients wants to cut the string.

std::string TangoStacks::getText(const WhichStack stack, int numChars,
                                 bool reverse, bool cut) {
    std::string result = "";
    int maxN = (stack == LEFT) ? lcount : rcount;
    int safeN = min(numChars, maxN);
    
    // Get the text without popping the stack.
    
    for (int i = 0; i < safeN; i++) {
        int index = getArrayIndex(stack, i);
        if (reverse) {
            result = array[index] + result;
        } else {
            result = result + array[index];
        }
    }
    
    // Cut the text from the buffer if need be.

    if (cut) {
        for (int i = 0; i < safeN; i++) {
            pop(stack);
        }
    }
    
    return result;
}

std::string TangoStacks::getTextWords(const WhichStack stack, int numWords,
                                 bool reverse, bool cut) {
    std::string result = "";
    int i = 0; // Number of characters, relative to the head to return.
    int maxDepth = (stack == LEFT) ? lcount : rcount;
    
    while (numWords > 0) {
        int index = 0;
        for (; i < maxDepth; i++) {
            index = getArrayIndex(stack, i);
            if (array[index] == SPACE || array[index] == NEWLINE) {
                continue;   // Scoot past any prefixed whitespace before
                            // counting next word.
            }
            break;
        }
        for (; i < maxDepth; i++) {
            // Check for next end-of-word delimiter.
            index = getArrayIndex(stack, i);
            if (array[index] == SPACE || array[index] == NEWLINE) {
                --numWords;
                break;
            }
        }
        if (i == maxDepth) {
            // We've reached the end of the stack, so force while to
            // exit even if we didn't reach the target word count.
            numWords = 0;
        }
        int numChars = i;
        result = getText(stack, numChars, reverse, cut);
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

// Implementation notes: getArrayIndex
// -----------------------------------
// This private method maps a stack element's depth to it's location in
// in the array hosting the stack.
//
// Elements at the head of the stack are considered to have a depth of 0.
//

int TangoStacks::getArrayIndex(WhichStack stack, int elemDepth) const {
    int index;
    
    if (stack == LEFT) {
        if (lcount == 0) error("getArrayIndex: No index for empty L-stack.");
        index = lcount - elemDepth - 1;
    } else {
        if (rcount == 0) error("getArrayIndex: No index for empty R-stack.");
        index = capacity - (rcount - elemDepth);
    }
    if (index < 0) error("getArrayIndex: underflow");
    if (index >= capacity) error("getArrayIndex: overflow");
    return index;
}
