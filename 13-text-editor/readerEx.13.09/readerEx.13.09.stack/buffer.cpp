//
// buffer.cpp
//
// This file implements the EditorBuffer class using the abstraction
// of a pair of character stacks.  Everything before the current
// cursor position is considered to be in the 'before stack'.  Everything
// after the cursor is in the 'after stack'.
//
// It has been augmented to support operations (movement and deletion) on 
// word-boundaries along with copy, cut, paste, and search.
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
// Chapter 13, Exercise 9 (stack-based)
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program extends code that comes from Figures 13-2, 13-6, 13-7.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 8/12/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include "buffer.h"
using namespace std;

static const char SPACE = ' ';
static const char NEWLINE = '\n';

// Implementation notes: Constructor and destructor
// ------------------------------------------------
// In this implementation, all dynamic allocation is managed by the
// CharStack class, which means there is no work for EditorBuffer to do.

EditorBuffer::EditorBuffer() { clipboardStr = ""; }

EditorBuffer::~EditorBuffer() { }

// Implementation notes: moveCursor methods
// ----------------------------------------
// The six moveCursor methods use push and pop to transfer values between
// the two stacks.  Some of the methods move forward or backward relative
// to the nearest word.  Words are delimited by spaces or newline characters
// and, for simplicity, embody adjacent punctuation such as commas and periods.

void EditorBuffer::moveCursorForward() {
    if (!stacks.isEmpty(AFTER_STACK)) {
        stacks.push(stacks.pop(AFTER_STACK), BEFORE_STACK);
    }
}

void EditorBuffer::moveCursorForwardWord() {
    
    // Normalize pre-move position.
    //
    // Move forward to the nearest beginning-of-word character by advancing
    // through any whitespace immediately to the right of the cursor.
    
    while (!stacks.isEmpty(AFTER_STACK) &&
           ((stacks.peek(AFTER_STACK) == SPACE ||
             stacks.peek(AFTER_STACK) == NEWLINE))) {
        moveCursorForward();
    }
    
    // Advance through the word to the immediate right of the cursor.
    //
    // Words are space-delimited or newline-delimited so stop advancing
    // if we encounter either of those.
    
    while (!stacks.isEmpty(AFTER_STACK) &&
           stacks.peek(AFTER_STACK) != SPACE &&
           stacks.peek(AFTER_STACK) != NEWLINE) {
        moveCursorForward();
    }
}

void EditorBuffer::moveCursorBackward() {
    if (!stacks.isEmpty(BEFORE_STACK)) {
        stacks.push(stacks.pop(BEFORE_STACK), AFTER_STACK);
    }
}

void EditorBuffer::moveCursorBackwardWord() {
    
    // Normalize pre-backup position.
    //
    // Backup to the nearest end-of-word character by scooting back
    // through any whitespace immediately to the left of the cursor.
    
    while (!stacks.isEmpty(BEFORE_STACK) &&
          ((stacks.peek(BEFORE_STACK) == SPACE ||
            stacks.peek(BEFORE_STACK) == NEWLINE))) {
        moveCursorBackward();
    }
    
    // Backup through the word to the immediate left of the cursor.
    //
    // Words are space-delimited or newline-delimited so stop backing up
    // if we encounter either of those.
    
    while (!stacks.isEmpty(BEFORE_STACK) &&
            stacks.peek(BEFORE_STACK) != SPACE &&
            stacks.peek(BEFORE_STACK) != NEWLINE) {
        moveCursorBackward();
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

void EditorBuffer::insertCharacter(const char ch) {
    stacks.push(ch, BEFORE_STACK);
}

// Implementation notes: insertString
// ----------------------------------
// Extend the notion of insertion to apply to a string of characters.

void EditorBuffer::insertString(const std::string str) {
    for (int i = 0; i < str.length(); i++) {
        insertCharacter(str[i]);
    }
}

void EditorBuffer::deleteCharacter() {
    if (!stacks.isEmpty(AFTER_STACK)) {
        stacks.pop(AFTER_STACK);
    }
}

// Implementation notes: deleteWord
// --------------------------------
// Deletes all the characters from the cursor position through to the end
// of the word just to the right of the cursor.  If the cursor is within the
// whitespace preceding a word, that is deleted first, followed by the word
// itself.

void EditorBuffer::deleteWord() {
    
    // Delete any preceding whitespace immediately to the right of the cursor.
    
    while (!stacks.isEmpty(AFTER_STACK) &&
           (stacks.peek(AFTER_STACK) == SPACE ||
            stacks.peek(AFTER_STACK) == NEWLINE)) {
               deleteCharacter();
           }
    
    // Delete the word characters until we hit an end-of-word delimiter
    // such has a space or a newline.
    
    while (!stacks.isEmpty(AFTER_STACK) &&
           stacks.peek(AFTER_STACK) != SPACE &&
           stacks.peek(AFTER_STACK) != NEWLINE) {
        deleteCharacter();
    }
}

// Method: copy, paste
// Usage: copy(numChars);
// -----------------------
// Copies the next n-characters following the cursor to a private
// buffer in anticipation of a subsequent paste command.

void EditorBuffer::copy(int numChars) {
    clipboardStr = stacks.getText(AFTER_STACK, numChars);
}

void EditorBuffer::copyWords(int numWords) {
    bool reverse = false;
    bool cut = false;
    clipboardStr = stacks.getTextWords(AFTER_STACK, numWords, reverse, cut);
}

void EditorBuffer::cut(int numChars) {
    bool reverse = false;
    bool cut = true;
    clipboardStr = stacks.getText(AFTER_STACK, numChars, reverse, cut);
}

void EditorBuffer::cutWords(int numWords) {
    bool reverse = false;
    bool cut = true;
    clipboardStr = stacks.getTextWords(AFTER_STACK, numWords, reverse, cut);
}

void EditorBuffer::paste() {
    insertString(clipboardStr);
}

// Implementation notes: getText
// -----------------------------
// Rather than go through the overhead of copying the stacks and mutating
// the copies with pops to get at the underlying text, leverage the
// lower-level TangoStacks::getText() method that directly iterates over the
// array upon with the TangoStacks are built.  Additional memory footprint
// goes from O(N) to O(1) since the iteration happens across an in-place
// structure instead of a copy of stacks.

string EditorBuffer::getText() {
    string str = "";
    bool reverse = true;
    bool cut = false;
    str = stacks.getText(BEFORE_STACK, stacks.size(BEFORE_STACK), reverse, cut);
    reverse = false;
    str += stacks.getText(AFTER_STACK, stacks.size(AFTER_STACK), reverse, cut);
    return str;
}

std::string EditorBuffer::getText(TangoStacks::WhichStack stack, int numChars,
                                  bool cut) {
    std::string result = "";
    int maxN = stacks.size(stack);
    int safeN = min(numChars, maxN);
    
    bool reverse = (stack == BEFORE_STACK) ? true : false;
    result = stacks.getText(stack, safeN, reverse, cut);
    return result;
}

int EditorBuffer::getCursor() const {
    return stacks.size(BEFORE_STACK);
}

// Implementation notes: search
// ----------------------------
// This is an O(n) memory usage algorithm that fetches /all/ the text
// after the cursor and leverages the str.find method to locate the
// sub-string of interest passed in by the client.
//
// In the event of a match, cursor position is advanced to the end of
// the sub-string within the editor buffer.

bool EditorBuffer::search(std::string str) {
    std::string afterCursor = getText(AFTER_STACK, stacks.size(AFTER_STACK));
    int index = afterCursor.find(str);
    if (index == string::npos) {
        return false;
    } else {
        // Advance cursor
        int cursorHops = index + str.length();
        for (int i = 0; i < cursorHops; i++) {
            moveCursorForward();
        }
        return true;
    }
}