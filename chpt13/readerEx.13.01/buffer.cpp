//
// buffer.cpp
//
// This file implements the EditorBuffer class using a linked list to represent
// the buffer.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code comes from Figure 13-9.
// --------------------------------------------------------------------------
//

#include <iostream>
#include "buffer.h"
using namespace std;

// Implementation notes: Constructor
// ---------------------------------
// This function initializes an empty editor buffer represented as a linked
// list.  In this representation, the empty buffer contains a "dummy" cell whose
// ch field is never used.  The constructor must allocate this dummy cell and
// set the internal pointers correctly.

EditorBuffer::EditorBuffer() {
    start = cursor = new Cell;
    start->link = NULL;
}

// Implementation notes: Constructor
// ---------------------------------
// The destructor must delete every cell in the buffer.  Note that the loop
// structure is not exactly the standard for-loop pattern for processing every
// cell within a linked list.  The complication that forces this change is that
// the body of the loop can't free the current cell and later have the for
// loop use the link field of that cell to move to the next one.  to avoid this
// problem, this implementation copies the link pointer before calling delete.

EditorBuffer::~EditorBuffer() {
    Cell *cp = start;
    while (cp != NULL) {
        Cell *next = cp->link;
        delete cp;
        cp = next;
    }
}

// Implementation notes: moveCursor methods
// ----------------------------------------
// The four methods that move the cursor have different time complexities
// because the structure of a linked list is asymmetrical with respect to
// moving backward and forward.  The moveForward and moveCursorToStart
// methods operate in constant time.  by constrast, the moveCursorBackward
// and moveCursorToEnd methods require a loop that runs in linear time.

void EditorBuffer::moveCursorForward() {
    if (cursor->link != NULL) {
        cursor = cursor->link;
    }
}

void EditorBuffer::moveCursorBackward() {
    Cell *cp = start;
    if (cursor != start) {
        while (cp->link != cursor) {
            cp = cp->link;
        }
        cursor = cp;
    }
}

void EditorBuffer::moveCursorToStart() {
    cursor = start;
}

void EditorBuffer::moveCursorToEnd() {
    while (cursor->link != NULL) {
        cursor = cursor->link;
    }
}

// Implementation notes: insertCharacter
// -------------------------------------
// The steps required to insert a new character are:
//
//  1. Allocate a new cell and put the new character in it.
//  2. Copy the pointer to the remainder of the list into the link.
//  3. Update the link in the current cell to point to the new cell.
//  4. Move the cursor forward over the inserted character.

void EditorBuffer::insertCharacter(char ch) {
    Cell *cp = new Cell;
    cp->ch = ch;
    cp->link = cursor->link;
    cursor->link = cp;
    cursor = cp;
}

// Implementation notes: deleteCharacter
// -------------------------------------
// The steps necesarry to delete the character after the cursor are:
//
// 1. Remove the current cell by pointing to its successor.
// 2. Free the cell to reclaim the memory.

void EditorBuffer::deleteCharacter() {
    if (cursor->link != NULL) {
        Cell *oldCell = cursor->link;
        cursor->link = cursor->link->link;
        delete oldCell;
    }
}

// Implementation notes: getText and getCursor
// -------------------------------------------
// The getText method uses the standard linked-list pattern to loop through
// the cells in the linked list.  The getCursor method must count the
// characters in the list unit it reaches the cursor.

string EditorBuffer::getText() const {
    string str = "";
    for (Cell *cp = start->link; cp != NULL; cp = cp->link) {
        str += cp->ch;
    }
    return str;
}

int EditorBuffer::getCursor() const {
    int nChars = 0;
    for (Cell *cp = start; cp != cursor; cp = cp->link) {
        nChars++;
    }
    return nChars;
}