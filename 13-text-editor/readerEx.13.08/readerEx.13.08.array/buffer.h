//
// buffer.h
//
// This file defines the interface for the EditorBuffer class.
// It has been augmented to support operations (movement and deletion) on 
// word-boundaries along with copy, cut, and paste.
// 
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 8 (array-based)
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program extends code that comes from Figure 13-2.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 8/4/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#ifndef buffer_h
#define buffer_h

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
    // These functions move the cursor to the start or the end of this buffer.
    
    void moveCursorToStart();
    void moveCursorToEnd();
    
    // Method: insertCharacter
    // Usage: buffer.insertCharacter(ch);
    // ----------------------------------
    // Inserts the character ch into this buffer at the cursor position,
    // leaving the cursor after the inserted character.
    
    void insertCharacter(char ch);
    
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
    // Copies the next numChars or numWords to the clipboard string, preserving
    // the edit buffer.
    //
    // Cut Methods
    // Copies the next numChars or numWords to the clipboard string, removing
    // the clipped text from the edit buffer.
    
    void copy(int numChars);
    void copyWords(int numWords);
    void cut(int numChars);
    void cutWords(int numWords);
    
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
    // In the array-based implementation of the buffer, the characters in the
    // buffer are stored in a dynamic array.  In addition to the array, the
    // structure keeps track of the capacity of the buffer, the length of the
    // buffer, and the cursor position.  The cursor position is the index of
    // the character that follows where the cursor would appear on the screen.
    
    // Constants
    
    static const int INITIAL_CAPACITY = 10;
    
    // Instance variables
    
    char *array;        // Dynamic array of characters
    int capacity;       // Allocated size of that array
    int length;         // Number of character in buffer
    int cursor;         // Index of character after cursor
    std::string clipboardStr;  // Clipboard string.
    
    // Make it illegal to copy editor buffers
    
    EditorBuffer(const EditorBuffer & value) {}
    const EditorBuffer & operator=(const EditorBuffer & rhs) { return *this; }
    
    // Private method prototypes
    
    // Method: expandCapacity
    // Usage: if (length == capacity) expandCapacity();
    // ------------------------------------------------
    // Exands the underlying array that backs the editor buffer when it has
    // been filled.  A new array, twice the size of the original, is created
    // and initialized with the buffer contents.
    
    void expandCapacity();
    
    // Methods: getText, getTextWords
    // Usage: string str = getText(cursor, 5, false);
    //        string words = getTextWords(cursor, 3, true);
    // ----------------------------------------------------
    // Returns the next N characters or words, relative to the specified array
    // index.
    //
    // Text may be optionally cut from the edit buffer.
    
    std::string getText(int index, int numChars, bool cut);
    std::string getTextWords(int index, int numWords, bool cut);
    
};

#endif // buffer_h
