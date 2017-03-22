//
// buffer.cpp
//
// This file implements the EditorBuffer class using a doubly linked list to
// represent the buffer.
//
// The big changes over the singly linked list implementation include:
//
// * abandoning the use of NULL to detect reaching the end of the list.
//   Since the last cell loops back to the dummy cell at the start of the list,
//   we now just test if we've reached that starting cell.
//
// * optimizing backward movement by using prev link in each cell to reverse
//   direction in constant time.
//
// * optimizing jumps to the end of the buffer by simply transiting through
//   start->prev.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file extends code from Figure 13-9.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/5/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include "buffer.h"
using namespace std;

static const char SPACE = ' ';
static const char NEWLINE = '\n';

// Implementation notes: Constructor
// ---------------------------------
// This function initializes an empty editor buffer represented as a linked
// list.  In this representation, the empty buffer contains a "dummy" cell whose
// ch field is never used.  The constructor must allocate this dummy cell and
// set the internal pointers correctly.
//
// Initially, the dummy cell is linked to itself in the forward and backward
// directions.

EditorBuffer::EditorBuffer() {
    start = cursor = new Cell;
    start->next = start;
    start->prev = start;
    clipboardStr = "";
}

// Implementation notes: Constructor
// ---------------------------------
// The destructor must delete every cell in the buffer.  Note that the loop
// structure is not exactly the standard for-loop pattern for processing every
// cell within a linked list.  The complication that forces this change is that
// the body of the loop can't free the current cell and later have the for
// loop use the next field of that cell to move to the next one.  To avoid this
// problem, this implementation copies the next pointer before calling delete.

EditorBuffer::~EditorBuffer() {
    Cell *cp = start;
    if (cp != NULL) {
        while (cp->next != start) {
            Cell *next = cp->next;
            delete cp;
            cp = next;
        }
        delete start;
    }
}

// Implementation notes: moveCursor methods
// ----------------------------------------
// The six methods that move the cursor have uniform time complexities
// because the structure of a double linked list is symmetrical with respect to
// moving backward and forward.  The moveCursorForward, moveCursorBackward,
// moveCursorToStart, moveCursorToEnd all operate in constant time complexity,
// whereas the moveCursorForwardWord and moveCursorBackwardWord are O(n)
// where n represents the number of characters in the word.

void EditorBuffer::moveCursorForward() {
    if (cursor->next != start) {
        cursor = cursor->next;
    }
}

void EditorBuffer::moveCursorForwardWord() {
    
    // Normalize pre-move position.
    //
    // Move forward to the nearest beginning-of-word character by advancing
    // through any whitespace immediately to the right of the cursor.
    
    while (cursor->next != start &&
           ((cursor->next->ch == SPACE) ||
            (cursor->next->ch == NEWLINE))) {
               moveCursorForward();
           }
    
    // Advance through the word to the immediate right of the cursor.
    //
    // Words are space-delimited or newline-delimited so stop advancing
    // if we encounter either of those.
    
    while (cursor->next != start &&
           cursor->next->ch != SPACE &&
           cursor->next->ch != NEWLINE) {
        moveCursorForward();
    }
}

void EditorBuffer::moveCursorBackward() {
    if (cursor != start) {
        cursor = cursor->prev;
    }
}

void EditorBuffer::moveCursorBackwardWord() {
    
    // Normalize pre-backup position.
    //
    // Backup to the nearest end-of-word character by scooting back
    // through any whitespace immediately to the left of the cursor.
    
    while (cursor != start &&
           (cursor->ch == SPACE || cursor->ch == NEWLINE)) {
               moveCursorBackward();
    }
    
    // Backup through the word to the immediate left of the cursor.
    //
    // Words are space-delimited or newline-delimited so stop backing up
    // if we encounter either of those.
    
    while (cursor != start && cursor->ch != SPACE && cursor->ch != NEWLINE) {
        moveCursorBackward();
    }
}

void EditorBuffer::moveCursorToStart() {
    cursor = start;
}

void EditorBuffer::moveCursorToEnd() {
    cursor = start->prev;
}

// Implementation notes: insertCharacter
// -------------------------------------
// The steps required to insert a new character are:
//
//  1. Allocate a new cell and put the new character in it.
//  2. Copy the pointer to the remainder of the list into the next link.
//  3. Update the new cell's previous link to be the cursor itself
//     since, by convention, this points to the character ahead of the one
//     we're inserting.
//  4. Update the next link in the current cell to point to the new cell.
//  5. If the cursor /was/ pointing to the last character in the list,
//     update the prev link of the upstream cell to the cursor to point
//     to the newly added character (which was inserted at the end of the list).
//  6. Move the cursor forward over the inserted character.

void EditorBuffer::insertCharacter(char ch) {
    Cell *cp = new Cell;
    cp->ch = ch;
    cp->next = cursor->next;  // wire out
    cp->prev = cursor;        // wire out
    cursor->next = cp;        // wire in
    if (cursor->prev->prev == cursor) {
        cursor->prev->prev = cp;  // wire in
    }
    cursor = cp;              // advance cursor after inserted character
}

// Implementation notes: insertString
// ----------------------------------
// Extend the notion of insertion to apply to a string of characters.

void EditorBuffer::insertString(const std::string str) {
    for (int i = 0; i < str.length(); i++) {
        insertCharacter(str[i]);
    }
}

// Method: copy, copyWords, paste
// Usage: copy(nChars);
//        copyWords(nWords);
//        paste();
// -------------------------
// Copies the next n-characters following the cursor to a private
// buffer in anticipation of a subsequent paste command.

void EditorBuffer::copy(int nChars) {
    clipboardStr = getText(cursor).substr(0, nChars);
}

void EditorBuffer::copyWords(int nWords) {
    string charString = getText(cursor);
    int i = 0;
    
    while (nWords > 0) {
        for (; i < charString.length(); i++) {
            if (charString[i] == SPACE || charString[i] == NEWLINE) {
                continue;   // Scoot past any prefixed whitespace before
                            // counting next word.
            }
            break;
        }
        for (; i < charString.length(); i++) {
            // Check for next end-of-word delimiter.
            if (charString[i] == SPACE || charString[i] == NEWLINE) {
                --nWords;
                break;
            }
        }
        if (i == charString.length()) {
            // We've parsed the whole string so force while to exit
            // even if we didn't reach the target word count.
            nWords = 0;
        }
    }
    clipboardStr = charString.substr(0, i);
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

// Implementation notes: deleteCharacter
// -------------------------------------
// The steps necesarry to delete the character after the passed in cell pointer
// are:
//
// 1. Remove the current cell by pointing to its successor.  (By convention,
//    we expect to delete the cell just beyond the cell pointer passed in.)
//
// 2. Fix up any prev link to the deleted character by updating to the
//    deleted character's predecessor.
//
// 3. Free the cell to reclaim the memory.

void EditorBuffer::deleteCharacter() {
    deleteCharacter(cursor);
}

void EditorBuffer::deleteCharacter(Cell *cp) {
    if (cp->next != start) {        // eol test
        Cell *oldCell = cp->next;
        cp->next = cp->next->next;  // Update the next pointer of the upstream
                                    // char to point to char beyond the
                                    // one we're deleting.
        
        cp->next->prev = cp;        // Update prev link of char downstream
                                    // to the one we deleted.  It's pointing
                                    // to the char we deleted unless we take
                                    // action!  Point it the char upstream
                                    // of the one we're deleting.
        delete oldCell;
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
    
    while (cursor->next != start &&
          (cursor->next->ch == SPACE || cursor->next->ch == NEWLINE)) {
        deleteCharacter();
    }
    
    // Delete the word characters until we hit an end-of-word delimiter
    // such as a space or a newline.
    
    while (cursor->next != start &&
           cursor->next->ch != SPACE &&
           cursor->next->ch != NEWLINE) {
        deleteCharacter();
    }
}

// Implementation notes: getCursor
// -------------------------------
// The getCursor method counts the characters in the list until it reaches
// the cursor.

int EditorBuffer::getCursor() const {
    int nChars = 0;
    for (Cell *cp = start; cp != cursor; cp = cp->next) {
        nChars++;
    }
    return nChars;
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
    std::string afterCursor = getText(cursor);
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

// Implementation notes: getText
// -----------------------------
// The getText method uses the standard linked-list pattern to loop through
// the cells in the linked list.

string EditorBuffer::getText() const {
    string str = "";
    for (Cell *cp = start->next; cp != start; cp = cp->next) {
        str += cp->ch;
    }
    return str;
}

string EditorBuffer::getText(const Cell *startCell) const {
    string str = "";
    for (Cell *cp = startCell->next; cp != start; cp = cp->next) {
        str += cp->ch;
    }
    return str;
}

// Implementation notes: getText, getTextWords (private variants)
// --------------------------------------------------------------
// The private variants of getText are used internally in support of copy[Word]/
// cut[Word] methods that need more granular text selection.
// They effectively allow the left and right bounds of the desired text to be
// specified through a startCell and a numChars or numWords count.
//
// If the requested text would extend beyond the length of the current buffer,
// the longest possible text is returned.

std::string EditorBuffer::getText(Cell *startCell, int numChars, bool cut) {
    std::string result = "";
    
    // Assume startCell points to cell just prior to the one we want.
    
    Cell *cp = startCell;
    
    while (cp->next != start && numChars > 0) {
        result += cp->next->ch;
        --numChars;
        if (cut) {
            deleteCharacter(cp);
        } else {
            cp = cp->next;
        }
    }
    return result;
}

std::string EditorBuffer::getTextWords(Cell *startCell, int numWords,
                                       bool cut) {
    std::string result = "";
    Cell *cp = startCell;
    char nextCh;

    while (cp->next != start && numWords > 0) {
        nextCh = getText(cp, 1, cut)[0];
        result += nextCh;
        if (nextCh == SPACE || nextCh == NEWLINE) {
            continue;   // Scoot past any prefixed whitespace before
                        // counting next word.
        }
        break;
    }
    while (cp->next != start && numWords > 0) {
        nextCh = getText(cp, 1, cut)[0];
        result += nextCh;
        // Check for next end-of-word delimiter.
        if (nextCh == SPACE || nextCh == NEWLINE) {
            --numWords;
            break;
        }
    }
    return result;
}