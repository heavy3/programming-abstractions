//
// tangostacks.h
//
// This file describes the class for stacks that occupy opposing ends of
// a character array and advance toward (or retreat from) the middle as items
// are pushed on or popped off the stacks.
//
//  character array
//  +-------------------------------------+
//  |left -->[head]        [head]<-- right|
//  +-------------------------------------+
//
// This affords us the ability to host the 'before' and 'after' stacks of an
// editor buffer in half the space.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 10
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 8/12/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#ifndef tangostacks_h
#define tangostacks_h

#include <string>

//
//  Class: TangoStacks
//  ------------------
//  A tango stack encapsulates dual stacks occupying opposite ends of the same
//  array space with heads facing each other.  They are suited for applications
//  where growth of one stack implies shrinkage of the other, though this
//  complementary behavior is not strictly required.
//
//  Growth is toward the middle of the array, in the direction of the opposing
//  stack.  This strategy yields memory efficiency in certain applications
//  where stack operations have a complementary nature (the head of one stack
//  may be popped then pushed to the other).
//
//  Safeguards are in place to prevent the two stacks from colliding.  In the
//  event of a full array, a new array with doubled capacity is created and the
//  contents from the previous array are copied into the larger array.

class TangoStacks {
public:
    
//  Enumeration: WhichStack
//  -----------------------
//  Specifies which stack should be operated upon for a given method.
    
    enum WhichStack {LEFT, RIGHT};
    
//  Constructor: TangoStack
//  Usage: TangoStacks stacks;
//  --------------------------
//  Initializes dual-stacks so they can contain characters.
    
    TangoStacks();
    
// Destructor: ~TangoStacks
// Usage: (usually implicit)
// -------------------------
// Frees any heap storage associated with the stacks.
    
    ~TangoStacks();
    
// Method: size
// Usage: int totalElems = stacks.size();
//        int nElems = stacks.size(LEFT);
// ------------------------------------
// Returns the number of characters in the stack indicated.
// If no stack is specified, the total size for both stacks is returned.
    
    int size() const;
    int size(const WhichStack stack) const;
    
// Method: isEmpty
// Usage: if (stacks.isEmpty()) { . . . }
//        if (stacks.isEmpty(LEFT)) { . . . }
// ------------------------------------------
// Returns true if this stack contains no characters.
   
    bool isEmpty() const;
    bool isEmpty(const WhichStack stack) const;
    
// Method: clear
// Usage: stacks.clear();
//        stacks.clear(LEFT);
// --------------------------
// Removes all characters from this stack.
    
    void clear();
    void clear(WhichStack stack);
    
// Method: push
// Usage: stacks.push(ch, LEFT);
// -----------------------------
// Pushes the character ch onto this stack.
    
    void push(const char ch, const WhichStack stack);
    
// Method: pop
// Usage: char ch = stacks.pop();
// ------------------------------
// Removes the top character from this stack and returns it.
    
    char pop(const WhichStack stack);
    
// Method: peek
// Usage: char ch = stacks.peek(LEFT);
// -----------------------------------
// Returns the value of top character from this stack without
// removing it.  Raises and error if called on an empty stack.
    
    char peek(const WhichStack stack) const;
    
// Method: getText
// Usage: string mystr = stacks.getText(RIGHT, rcount);
//        string mystr = stacks.getText(LEFT, lcount, true);
// ---------------------------------------------------------
// Returns the next n-characters, relative to the head of the specified stack,
// as a string.  Character-order within the string is assumed to be pop-order,
// though the underlying array is not actually popped (unless the cut
// parameter is set true.  Character order may be reversed as desired.
// reversed as desired.

    std::string getText(const WhichStack stack, int numChars,
                    bool reverse = false, bool cut = false);
    
// Method: getText
// Usage: string myWords = stacks.getTextWords(RIGHT, rcount);
// -----------------------------------------------------------
// Effectively walks back through the specified stack until the desired
// number of words have been encountered and returns
// those as a string.  Support is given for reversing the string
// as well as cutting the string from the buffer by popping the stack
// accordingly.
    
    std::string getTextWords(const WhichStack stack, int numWords,
                        bool reverse = false, bool cut = false);
    
// Copy constructor: TangoStacks
// Usage: (usually implicit)
// -------------------------
// Initializes the current object to be a deep copy of the specified source.
    
    TangoStacks(const TangoStacks & src);
    
// Operator: =
// Usage: dst = src;
// -----------------
// Assigns src to dst so that the two stacks are independent copies.
    
    TangoStacks & operator=(const TangoStacks & src);
    
// Private section

private:
    
// Private constants
    
    static const int INITIAL_CAPACITY = 10;
    
// Instance variables
    
    char *array;        // Dynamic array of characters
    int capacity;       // Allocated size of that array
    int count;          // Current count of all chars on the dual stacks.
    int lcount;         // Count of chars pushed on the left stack.
    int rcount;         // Count of chars pushed on the right stack.
    
// Private method prototypes
    
    void deepCopy(const TangoStacks & src);
    void expandCapacity();
    int getArrayIndex(const WhichStack stack, int elemDepth) const;
    
};

#endif // tangostacks_h
