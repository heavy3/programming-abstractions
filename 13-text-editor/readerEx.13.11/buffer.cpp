//
// buffer.cpp
//
// This file implements the EditorBuffer class using a linked list to 
// represent the buffer.  It uses NULL to designate a cursor at the beginning
// of the buffer (as opposed to relying upon a 'dummy-cell' strategy at
// the head of the list).
//
// It has been augmented to support operations (movement and deletion) on 
// word-boundaries along with copy, cut, paste, search, and replace.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 11
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file extends code from Figure 13-9.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 8/24/16.
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

EditorBuffer::EditorBuffer() {
    start = cursor = NULL;
    clipboardStr = "";
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
// The six methods that move the cursor have different time complexities
// because the structure of a linked list is asymmetrical with respect to
// moving backward and forward.  The moveForward and moveCursorToStart
// methods operate in constant time.  By constrast, the moveCursorBackward
// and moveCursorToEnd methods require a loop that runs in linear time.

void EditorBuffer::moveCursorForward() {
    if (start != NULL) {
        if (cursor == NULL) {
            cursor = start;
        } else {
            if (cursor->link != NULL) {
                cursor = cursor->link;
            }
        }
    }
}

void EditorBuffer::moveCursorForwardWord() {
    
    if (start != NULL) {    // Non-empty list.
        
        // Normalize pre-move position.

        if (cursor == NULL) {
            cursor = start;
        }
        
        // Move forward to the nearest beginning-of-word character by advancing
        // through any whitespace immediately to the right of the cursor.
        
        while (cursor->link != NULL &&
               ((cursor->ch == SPACE) ||
                (cursor->ch == NEWLINE))) {
                   moveCursorForward();
        }
    
        // Advance through the word to the immediate right of the cursor.
        //
        // Words are space-delimited or newline-delimited so stop advancing
        // if we encounter either of those.
    
        while (cursor->link != NULL &&
               cursor->ch != SPACE &&
               cursor->ch != NEWLINE) {
            moveCursorForward();
        }
    }
}

void EditorBuffer::moveCursorBackward() {
    if (cursor != NULL && start != NULL) {
        if (cursor == start) {
            cursor = NULL;
        } else {
            Cell *cp = start;
            while (cp->link != cursor) {
                cp = cp->link;
            }
            cursor = cp;
        }
    }
}

void EditorBuffer::moveCursorBackwardWord() {
    
    if (start != NULL) {    // Non-empty list.
        
        // Normalize pre-backup position.
        
        // Backup to the nearest end-of-word character by scooting back
        // through any whitespace immediately to the left of the cursor.
        
        while (cursor != NULL &&
              (cursor->ch == SPACE || cursor->ch == NEWLINE)) {
                   moveCursorBackward();
        }
    
        // Backup through the word to the immediate left of the cursor.
        //
        // Words are space-delimited or newline-delimited so stop backing up
        // if we encounter either of those.
        
        while (cursor != NULL &&
               cursor->ch != SPACE && cursor->ch != NEWLINE) {
            moveCursorBackward();
        }
    }
}

void EditorBuffer::moveCursorToStart() {
    cursor = NULL;
}

void EditorBuffer::moveCursorToEnd() {
    if (start != NULL) {    // Non-empty list.
        if (cursor == NULL) {
            cursor = start;
        }
        while (cursor->link != NULL) {
            cursor = cursor->link;
        }
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
    
    if (start == NULL || cursor == NULL) {
        
        // Empty list or cursor at head of list.
        
        cp->link = start;       // Wire-out
        start = cp;             // Wire-in
        
    } else {
        
        // Wire-out to letter following the cursor.
        
        cp->link = cursor->link;
        
        // Wire-in.  Letter before the cursor should point to the
        // new character we're inserting.
        
        cursor->link = cp;
    }
    
    // Advance cursor past the char we just inserted.
    // (By convenction, the cursor points to the character it follows.)
    
    cursor = cp;    // Make cursor logically follow after the
                    // new char we just inserted.

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
// The steps necesary to delete the character after the cursor are:
//
// 1. Remove the current cell by pointing to its successor.
// 2. Free the cell to reclaim the memory.

void EditorBuffer::deleteCharacter() {
    if (start != NULL) {        // Non-empty list.  So we have stuff to delete.
        
        Cell *oldCell;
        if (cursor == NULL) {   // Cursor at head of list.
            oldCell = start;
            if (start->link != NULL) {
                start->link = start->link->link;
            } else {
                start = NULL;
            }
            delete oldCell;
        } else if (cursor->link != NULL) {
            oldCell = cursor->link;
            cursor->link = cursor->link->link;
            delete oldCell;
        }
        
    }
}

void EditorBuffer::deleteCharacter(Cell *cp) {
    if (cp == cursor) {
        deleteCharacter();
    } else {
        if (cp->link != NULL) {
            Cell *oldCell = cp->link;
            cp->link = cp->link->link;
            delete oldCell;
        }
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
    
    if (start != NULL) {    // Non-empty list, so stuff to delete.
        Cell *cp = (cursor == NULL) ? start : cursor;
        while (cp->link != NULL &&
              (cp->link->ch == SPACE || cp->link->ch == NEWLINE)) {
            deleteCharacter();
        }
        
        // Delete the word characters until we hit an end-of-word delimiter
        // such as a space or a newline.
        
        while (cp->link != NULL &&
               cp->link->ch != SPACE &&
               cp->link->ch != NEWLINE) {
            deleteCharacter();
        }
    }
}

// Implementation notes: getCursor
// -------------------------------
// The getCursor method counts the characters in the list until it reaches
// the cursor.

int EditorBuffer::getCursor() const {
    int nChars = 0;
    if (cursor == NULL) return nChars;
    
    nChars++;
    for (Cell *cp = start; cp != cursor; cp = cp->link) {
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

// Implementation notes: 
// -----------------------------
// The getText method uses the standard linked-list pattern to loop through
// the cells in the linked list.

string EditorBuffer::getText() const {
    string str = "";
    for (Cell *cp = start; cp != NULL; cp = cp->link) {
        str += cp->ch;
    }
    return str;
}

// Implementation notes: getText relative to a given cell.
// -------------------------------------------------------
// This variant of getText returns text from a given cell to the end of the
// linked list of characters.

string EditorBuffer::getText(Cell *startingFromCell) const {
    string str = "";
    
    // Normalize for beginning of list.
    
    if (start != NULL) {            // Non-empty list.
        if (startingFromCell == NULL) {    // Cursor at head of list passed in.
            str += start->ch;
            startingFromCell = start;
        }
        for (Cell *cp = startingFromCell->link; cp != NULL; cp = cp->link) {
            str += cp->ch;
        }
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
    
    if (start != NULL) {
        if (cp == NULL) {
            cp = start;
            result += cp->ch;
            numChars--;
        } // Normalize for beginning of list.
        
        while (cp->link != NULL && numChars > 0) {
            result += cp->link->ch;
            --numChars;
            if (cut) {
                deleteCharacter(cp);
            } else {
                cp = cp->link;
            }
        }
    }
    return result;
}

std::string EditorBuffer::getTextWords(Cell *startCell, int numWords,
                                       bool cut) {
    std::string result = "";
    Cell *cp = startCell;
    char nextCh;

    if (cp == NULL) { cp = start; } // Normalize for beginning of list.
    
    while (cp != NULL && numWords > 0) {
        nextCh = getText(cp, 1, cut)[0];
        result += nextCh;
        if (nextCh == SPACE || nextCh == NEWLINE) {
            continue;   // Scoot past any prefixed whitespace before
                        // counting next word.
        }
        break;
    }
    while (cp != NULL && numWords > 0) {
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