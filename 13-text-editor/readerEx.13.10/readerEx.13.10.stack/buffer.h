//
// buffer.h
//
// This file defines the interface for the EditorBuffer class.  It optimizes
// the two-stack implementation in the text to use a single array, thereby
// cutting the allocated space by half.
//
// It has been augmented to support operations (movement and deletion) on 
// word-boundaries along with copy, cut, paste, search, and replace.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 10 (stack-based)
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program extends code that comes from Figures 13-2, 13-6, 13-7.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 8/12/16.
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
    // These methods move the cursor forward or backward one character,
    // respectively.  If the command would shift the cursor beyond either
    // end of the buffer, this method has no effect.
    
    void moveCursorForward();
    void moveCursorBackward();
    
    // Methods: moveCursorForwardWord, moveCursorBackwardWord
    // Usage: buffer.moveCursorForwardWord();
    //        buffer.moveCursorBackwardWord();
    // ---------------------------------------
    // These methods move the cursor forward or backward one word,
    // respectively.  If the command would shift the cursor beyond either
    // end of the buffer, this method has no effect.
    
    void moveCursorForwardWord();
    void moveCursorBackwardWord();
    
    // Methods: moveCursorToStart, moveCursorToEnd
    // Usage: buffer.moveCursorToStart();
    //        buffer.moveCursorToEnd();
    // --------------------------------
    // These methods move the cursor to the start or the end of this buffer.
    
    void moveCursorToStart();
    void moveCursorToEnd();
    
    // Method: insertCharacter
    // Usage: buffer.insertCharacter(ch);
    // ----------------------------------
    // Inserts the character ch into this buffer at the cursor position,
    // leaving the cursor after the inserted character.
    
    void insertCharacter(const char ch);
    
    // Method: insertString
    // Usage: buffer.insertString(str);
    // --------------------------------
    // Inserts the string into the buffer at the cursor position,
    // leaving the cursor after the inserted string.
    
    void insertString(const std::string str);
    
    // Method: copy, copyWords, cut, cutWords
    // Usage: copy(numChars); copyWords(numWords);
    //        cut(numChars);  cutWords(numWords);
    // ------------------------------------------
    // Copy Methods
    // Copies the next n-characters or n-words to the clipboard string, preserving
    // the edit buffer.
    //
    // Cut Methods
    // Copies the next n-characters or n-words to the clipboard string, removing
    // the clipped text from the edit buffer.
    
    void copy(int numChars);
    void copyWords(int numWords);
    void cut(int numChars);
    void cutWords(int numWords);
    
    //
    // Method: paste
    // Usage: paste();
    // ---------------
    // Pastes the contents of the clipboard string to the edit buffer at the
    // cursor.
    
    void paste();
    
    // Method: deleteCharacter
    // Usage: deleteCharacter();
    // -------------------------
    // Deletes the character immediately after the cursor, if any.
    
    void deleteCharacter();
    
    // Method: deleteWord
    // Usage: buffer.deleteWord();
    // --------------------------------
    // Deletes the word immediately after the cursor, if any.
    //
    // A word consists of a consecutive sequence of alphanumeric characters
    // (i.e., letters or digits) and includes any adjacent nonalphanumeric
    // characters between the cursor and the word (except for spaces).
    
    void deleteWord();
    
    // Method: getText
    // Usage: string str = buffer.getText();
    // -------------------------------------
    // Returns the contents of the buffer as a string.
    //
    // See also getText(stack, numChars, bCut);         (private method)
    //          getTextWords(stack, numWords, bCut);    (private method)
    //
    
    std::string getText();
    
    // Method: getCursor
    // Usage: int cursor = buffer.getCursor();
    // ---------------------------------------
    // Returns the index of the cursor.
    
    int getCursor() const;
    
    // Method: search
    // Usage: bool found = buffer.search("am i here?");
    // ------------------------------------------------
    // Returns true if the provided text if found somewhere after the cursor.
    // If, so, the cursor is repositioned after the last character in the
    // search string.  Otherwise false is returned and the cursor remains
    // unchanged.
    
    bool search(std::string str);
    
    // Method: replace
    // Usage: bool replaced = buffer.replace("this/that");
    // ---------------------------------------------------
    // Returns true if the input parameter is well formed and yields a
    // successfull search (relative to the cursor position) and replace of
    // the specified text.
    
    bool replace(std::string thisThat);
    
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
    // cursor.  This 2N memory strategy leaves one stack completely full and the
    // other empty.  However, by allowing the two stacks to occupy opposite
    // ends of the same array and grow toward each other, memory usage is
    // minimized to N in this scenario.
    

    // Private constants
    
    // Establish two constants for designating which of these stacks should
    // be mutated for the given operation.
    
    static const TangoStacks::WhichStack BEFORE_STACK = TangoStacks::LEFT;
    static const TangoStacks::WhichStack AFTER_STACK = TangoStacks::RIGHT;
    
    // Instance variables
    
    TangoStacks stacks;     // Define the before & after stacks that underly
                            // the EditorBuffer.  TangoStacks occupy opposite
                            // ends of the same array with heads the advance
                            // toward or retreat from the middle of the array.
    
    std::string clipboardStr; // Clipboard string for cut and paste operations.
    
    // Make it illegal to copy editor buffers.
    
    EditorBuffer(const EditorBuffer & value) {}
    const EditorBuffer & operator=(const EditorBuffer & rhs) { return *this; }
    
    // Private method prototypes
    
    // Method: expandCapacity
    // Usage: if (length == capacity) expandCapacity();
    // ------------------------------------------------
    // Expands the underlying array that backs the editor buffer when it has
    // been filled.  A new array, twice the size of the original, is created
    // and initialized with the previous array contents.
    
    void expandCapacity();
    
    // Methods: getText, getTextWords
    // Usage: string str = getText(cursor, 5, false);
    //        string words = getTextWords(cursor, 3, true);
    // ----------------------------------------------------
    // Returns the next N characters or words, relative to the specified array
    // index.
    //
    // Text may be optionally cut from the edit buffer.
    //
    // These private methods simplify the implementation of the copy and cut
    // methods.
    
    std::string getText(TangoStacks::WhichStack stack, int numChars,
                        bool cut = false);
    std::string getTextWords(TangoStacks::WhichStack stack, int numWords,
                             bool cut = false);
    
};

#endif // buffer_h
