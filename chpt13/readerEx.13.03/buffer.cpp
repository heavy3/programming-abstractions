//
// buffer.cpp
//
// This file implements the EditorBuffer class using the abstraction
// of a pair of character stacks.  Everything before the current
// cursor position is considered to be in the 'before stack'.  Everything
// after the cursor is in the 'after stack'.
//
// For example, if the text consisted simply of the word "HELLO" and the
// cursor was between the L's, the stacks would look like this:
//
//      H E L L O
//           ^
//            ---
//        L  /   \
//        E |     |  L
//        H |     v  O
//      -------   ------------
//      before    after cursor
//      stack     stack
//
// To read the contents of the buffer, you need to read up through the 
// before stack and then down the after stack.
//
// If you examine buffer.h more closely, you'll discover that both
// stacks reside at opposite ends of a single managed array I'm
// calling a 'tango stack' which is more memory efficient than managing
// two separate stacks.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program extends code that comes from Figures 13-2, 13-6, 13-7.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/27/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include "buffer.h"
using namespace std;

// Implementation notes: Constructor and destructor
// ------------------------------------------------
// In this implementation, all dynamic allocation is managed by the
// CharStack class, which means there is no work for EditorBuffer to do.

EditorBuffer::EditorBuffer() { }
EditorBuffer::~EditorBuffer() { }

// Implementation notes: moveCursor methods
// ----------------------------------------
// The four moveCursor methods use push and pop to transfer values between
// the two stacks.

void EditorBuffer::moveCursorForward() {
    if (!stacks.isEmpty(AFTER_STACK)) {
        stacks.push(stacks.pop(AFTER_STACK), BEFORE_STACK);
    }
}

void EditorBuffer::moveCursorBackward() {
    if (!stacks.isEmpty(BEFORE_STACK)) {
        stacks.push(stacks.pop(BEFORE_STACK), AFTER_STACK);
    }
}

void EditorBuffer::moveCursorToStart() {
    while (!stacks.isEmpty(BEFORE_STACK)) {
        stacks.push(stacks.pop(BEFORE_STACK), AFTER_STACK);
    }
}

void EditorBuffer::moveCursorToEnd() {
    while (!stacks.isEmpty(AFTER_STACK)) {
        stacks.push(stacks.pop(AFTER_STACK), BEFORE_STACK);
    }
}

// Implementation notes: character insertion and deletion
// ------------------------------------------------------
// Each of the functions that inserts or deletes characters can do so
// with a single push or pop operation.

void EditorBuffer::insertCharacter(char ch) {
    stacks.push(ch, BEFORE_STACK);
}

void EditorBuffer::deleteCharacter() {
    if (!stacks.isEmpty(AFTER_STACK)) {
        stacks.pop(AFTER_STACK);
    }
}

// Implementation notes: getText and getCursor
// -------------------------------------------
// The only difficult part of implementing these operators is making
// sure that the state of the buffer is restored after copying the characters
// from the two stacks.

string EditorBuffer::getText() const {
    TangoStacks copyStacks = stacks;

    string str = "";
    while (!copyStacks.isEmpty(BEFORE_STACK)) {
        str = copyStacks.pop(BEFORE_STACK) + str;
    }
    while (!copyStacks.isEmpty(AFTER_STACK)) {
        str += copyStacks.pop(AFTER_STACK);
    }
    return str;
}

int EditorBuffer::getCursor() const {
    return stacks.size(BEFORE_STACK);
}