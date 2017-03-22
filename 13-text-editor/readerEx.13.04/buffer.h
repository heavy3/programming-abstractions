//
// buffer.h
//
// This file defines the interface for the EditorBuffer class.  It optimizes
// the two-stack implementation in the text to use a single array, thereby
// cutting the allocated space by half.
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

#ifndef buffer_h
#define buffer_h

#include "tangostacks.h"

// Class: EditorBuffer
// -------------------
// This class represents an editor buffer, which maintains an ordered
// sequence of characters along with an insertion point called the cursor.

class EditorBuffer {
    
public:
    
    // Constructor: EditorBuffer
    // Usage: EditorBuffer buffer;
    // ---------------------------
    // Creates an empty editor buffer.
    
    EditorBuffer();
    
    // Destructor: ~EditorBuffer
    // Usage: (usually implicit)
    // -------------------------
    
    ~EditorBuffer();
    
    // Methods: moveCursorForward, moveCursorBackward
    // Usage: buffer.moveCursorForward();
    //        buffer.moveCursorBackward();
    // -----------------------------------
    // These functions move the cursor forward or backward one character,
    // respectively.  If the command would shift the cursor beyond either
    // end of the buffer, this method has no effect.
    
    void moveCursorForward();
    void moveCursorBackward();
    
    // Methods: moveCursorToStart, moveCursorToEnd
    // Usage: buffer.moveCursorToStart();
    //        buffer.moveCursorToEnd();
    // --------------------------------
    // These functions move the cursor to the start or the end of this buffer.
    
    void moveCursorToStart();
    void moveCursorToEnd();
    
    // Method: insertCharacter
    // Usage: buffer.insertCharacter(ch);
    // ----------------------------------
    // Inserts the character ch into this buffer at the cursor position,
    // leaving the cursor after the inserted character.
    
    void insertCharacter(const char ch);
    
    // Method: deleteCharacter
    // Usage: buffer.deleteCharacter();
    // --------------------------------
    // Deletes the character immediately after the cursor, if any.
    
    void deleteCharacter();
    
    // Method: insertString
    // Usage: buffer.insertString(str);
    // --------------------------------
    // Inserts the string into the buffer at the cursor position,
    // leaving the cursor after the inserted string.
    
    void insertString(const std::string str);
    
    // Method: getText
    // Usage: string str = buffer.getText();
    // -------------------------------------
    // Returns the contents of the buffer as a string.
    
    std::string getText() const;
    
    // Method: getCursor
    // Usage: int cursor = buffer.getCursor();
    // ---------------------------------------
    // Returns the index of the cursor.
    
    int getCursor() const;
    
private:
    
    // Implementation notes: Buffer data structure
    // -------------------------------------------
    // In the stack-based buffer model, the characters are stored in one of
    // two stacks depending upon if they exist before or after the
    // current cursor position.
    //
    // In each case, the characters proximate to the cursor are closer to the
    // top of the stack.  The advantage of this representation is that insertion
    // and deletion at the current cursor position occurs in constant time.
    //
    // Consideration has been given to minimizing memory usage as well.
    // A naive implementation might allocate N units of space for each stack,
    // handling the worst case when all the characters are to one side of the
    // cursor.  This O(2n) strategy leaves one stack completely full and the
    // other empty.  However, by allowing the two stacks to occupy opposite
    // ends of the same array and grow toward each other, memory usage is
    // minimized to O(n) in this scenario.
    

    // Private constants
    
    // Establish two constants for designating which of these stacks should
    // be mutated for the given operation.
    
    static const TangoStacks::WhichStack BEFORE_STACK = TangoStacks::LEFT;
    static const TangoStacks::WhichStack AFTER_STACK = TangoStacks::RIGHT;
    
    // Instance variables
    
    TangoStacks stacks;     // Define the before & after stacks that underly
                            // the EditorBuffer.
    
    // Make it illegal to copy editor buffers.
    
    EditorBuffer(const EditorBuffer & value) {}
    const EditorBuffer & operator=(const EditorBuffer & rhs) { return *this; }
    
    // Private method prototype
    
    void expandCapacity();
    
};

#endif // buffer_h
