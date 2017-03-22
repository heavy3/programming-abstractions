//
// buffer.cpp
//
// This file implements the EditorBuffer class using an array representation.
// It has been augmented to support operations (movement and deletion) on 
// word-boundaries along with copy, cut, paste, search, and replace.
// 
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 10 (array-based)
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program extends code that comes from Figures 13-2, 13-4, 13-5.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 8/12/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include "buffer.h"
#include "error.h"
using namespace std;

static const char SPACE = ' ';
static const char NEWLINE = '\n';

// Implementation notes: Constructor and destructor
// ------------------------------------------------
// The constructor initializes the private fields.  The destructor
// frees the heap-allocated memory, which is the dynamic array.

EditorBuffer::EditorBuffer() {
    capacity = INITIAL_CAPACITY;
    array = new char[capacity];
    length = 0;
    cursor = 0;
    clipboardStr = "";
}

EditorBuffer::~EditorBuffer() {
    delete[] array;
}

// Implementation notes: moveCursor methods
// ----------------------------------------
// The six moveCursor methods simply adjust the value of cursor.
//
// Here is a visual way to picture the cursor as it relates to the array index.
//
// 1. The cursor is to the left of the character in the array with that index.
//
// 2. The character indexed by the cursor position is just to the right
//    of the cursor.
//
//      T h i s   i s   a   t e s t .
//      - - - - - - - - - - - - - - -
//      0 1 2 3 4 5 6 7 8 9 1 1 1 1 1 1
//                          0 1 2 3 4 5
//      ^                           ^
//      |                           |
//      |                           +- cursor = 14
//      |                              array[14] = '.'
//      |                              *This is a test|.
//      |
//      |                              cursor = length = 15(right-most position)
//      |                              array[length] = undef
//      |                              *This is a test.|
//      |
//      +---- cursor = 0 = left-most position
//            array[0] = 'T'
//            *|This is a test.

void EditorBuffer::moveCursorForward() {
    if (cursor < length) cursor++;
}

void EditorBuffer::moveCursorForwardWord() {
    
    // Normalize pre-move position.
    //
    // Move forward to the nearest beginning-of-word character by advancing
    // through any whitespace immediately to the right of the cursor.
    
    while (cursor < length &&
           (array[cursor] == SPACE || array[cursor] == NEWLINE)) {
        moveCursorForward();
    }
    
    // Advance through the word to the immediate right of the cursor.
    //
    // Words are space-delimited or newline-delimited so stop advancing
    // if we encounter either of those.
    
    while (cursor < length &&
           array[cursor] != SPACE &&
           array[cursor] != NEWLINE) {
        moveCursorForward();
    }
}

void EditorBuffer::moveCursorBackward() {
    if (cursor > 0) cursor--;
}

void EditorBuffer::moveCursorBackwardWord() {
    
    // Normalize pre-backup position.
    //
    // Backup to the nearest end-of-word character by scooting back
    // through any whitespace immediately to the left of the cursor.
    //
    // NB: Since array[cursor] is the character to the right of the cursor
    //           array[cursor - 1] is the character to the left of the cursor.
    
    while (cursor > 0 &&
           (array[cursor - 1] == SPACE || array[cursor - 1] == NEWLINE)) {
        moveCursorBackward();
    }
    
    // Backup through the word to the immediate left of the cursor.
    //
    // Words are space-delimited or newline-delimited so stop backing up
    // if we encounter either of those.
    
    while (cursor > 0 &&
           array[cursor - 1] != SPACE &&
           array[cursor - 1] != NEWLINE) {
        moveCursorBackward();
    }
}

void EditorBuffer::moveCursorToStart() {
    cursor = 0;
}

void EditorBuffer::moveCursorToEnd() {
    cursor = length;
}

// Implementation notes: character insertion and deletion
// ------------------------------------------------------
// Each of the functions that inserts or deletes characters must shift
// all subsequent characters in the array, either to make room for new
// insertions or to close up space left by deletions.

void EditorBuffer::insertCharacter(char ch) {
    if (length == capacity) expandCapacity();
    for (int i = length; i > cursor; i--) {
        array[i] = array[i - 1];
    }
    array[cursor] = ch;
    length++;
    cursor++;
}

void EditorBuffer::insertString(const std::string str) {
    for (int i = 0; i < str.length(); i++) {
        insertCharacter(str[i]);
    }
}

void EditorBuffer::copy(int numChars) {
    bool cut = false;
    clipboardStr = getText(cursor, numChars, cut);
}

void EditorBuffer::copyWords(int numWords) {
    bool cut = false;
    clipboardStr = getTextWords(cursor, numWords, cut);
}

void EditorBuffer::cut(int numChars) {
    bool cut = true;
    clipboardStr = getText(cursor, numChars, cut);
}

void EditorBuffer::cutWords(int numWords) {
    bool cut = true;
    clipboardStr = getTextWords(cursor, numWords, cut);
}

void EditorBuffer::paste() {
    insertString(clipboardStr);
}

void EditorBuffer::deleteCharacter() {
    if (cursor < length) {
        for (int i = cursor + 1; i < length; i++) {
            array[i - 1] = array[i];
        }
        length--;
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
    
    while (cursor < length &&
           (array[cursor] == SPACE ||
            array[cursor] == NEWLINE)) {
               deleteCharacter();
           }
    
    // Delete the word characters immediately to the right of the cursor
    // until we hit an end-of-word delimiter such has a space or a newline.
    
    while (cursor < length &&
           array[cursor] != SPACE &&
           array[cursor] != NEWLINE) {
        deleteCharacter();
    }
}

// Simple getter methods: getCursor, getText

string EditorBuffer::getText() const {
    return string(array, length);
}

int EditorBuffer::getCursor() const {
    return cursor;
}

// Implementation notes: search
// ----------------------------
// This is an O(n) memory usage algorithm that fetches /all/ the text
// after the cursor and leverages the str.find method to locate the
// sub-string of interest passed in by the client.
//
// In the event of a match, cursor position is advanced to the end of
// the sub-string within the editor buffer.
//

bool EditorBuffer::search(std::string str) {
    int nChars = length - cursor;
    std::string afterCursor = getText(cursor, nChars);
    int index = afterCursor.find(str);
    if (index == string::npos) {
        return false;
    } else {
        // Advance cursor
        cursor += index + str.length();
        if (cursor > length) {
            error("EditorBuffer::search Cursor overflow.");
        }
        return true;
    }
}

// Implementation notes: replace
// -----------------------------
// This method parses the input string for a '/' delimiter so the
// old and new substrings can be isolated.  A search is performed for
// the old string and, if found, is deleted by backing up the old.length
// number of characters and deleting those, followed by an insert of the
// new text.

bool EditorBuffer::replace(std::string thisThat) {
    int delimIndex = thisThat.find("/");
    
    // Validate sane delimiter position.
    //  1. exists in string
    //  2. not at end of string
    //  3. not at beginning of string
    
    if (delimIndex == string::npos) return false;
    if (delimIndex >= thisThat.length()-1) return false;
    if (delimIndex == 0) return false;
    
    string thisStr = thisThat.substr(0, delimIndex);
    string thatStr = thisThat.substr(delimIndex+1);
    
    if (search(thisStr)) {
        for (int i = 0; i < thisStr.size(); i++) {
            moveCursorBackward();
            deleteCharacter();
        }
        insertString(thatStr);
        return true;
    } else {
        return false;
    }
}

// Implementation notes: getText, getTextWords
// -------------------------------------------
// These private methods allow more precise access to the underlying
// array buffer as opposed to the public getText method which simply returns
// the entire buffer as a string.
//
// These are crafted for internal use by the public copy[Word]/cut[Word] methods
// which need to specify a subset of contiguous text to copy or cut.
//
// If the requested text would extend beyond the length of the current buffer,
// the longest possible text is returned.

std::string EditorBuffer::getText(int index, int numChars, bool cut) {
    std::string result = "";
    int maxN = length - index;
    int safeN = min(numChars, maxN);
    
    if (index >= 0 && index < length) {
        result = string(array + index, safeN);
        if (cut) {
            for (int i = 0; i < safeN; i++) {
                deleteCharacter();
            }
        }
    }
    return result;
}

std::string EditorBuffer::getTextWords(int index, int numWords, bool cut) {
    std::string result = "";
    int i = 0;  // Number of characters, relative to the start index, to copy.
    int relLength = length - index;
    
    if (index >= 0 && index < length) {
        while (numWords > 0) {
            for (; i < relLength; i++) {
                if (array[i + index] == SPACE || array[i + index] == NEWLINE) {
                    continue;   // Scoot past any prefixed whitespace before
                                // counting next word.
                }
                break;
            }
            for (; i < relLength; i++) {
                // Check for next end-of-word delimiter.
                if (array[i + index] == SPACE || array[i + index] == NEWLINE) {
                    --numWords;
                    break;
                }
            }
            if (i == relLength) {
                // We've parsed the whole string so force while to exit
                // even if we didn't reach the target word count.
                numWords = 0;
            }
        }
        int numChars = i;
        result = getText(index, numChars, cut);
    }
    return result;
}

// Implementation notes: expandCapacity
// ------------------------------------
// This private method doubles the size of the array whenever the old one
// runs out of space.  To do so, expandCapacity allocates a new array,
// copies the old character to the new array, and then frees the old array.

void EditorBuffer::expandCapacity() {
    char *oldArray = array;
    capacity *= 2;
    array = new char[capacity];
    for (int i = 0; i < length; i++) {
        array[i] = oldArray[i];
    }
    delete[] oldArray;
}