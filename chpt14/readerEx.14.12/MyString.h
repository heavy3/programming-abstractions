//
// MyString.h
//
// This file exports an interface for a linked-list based MyString class.
//
// TODO: Fix const correctness of comparison operators (<, >, <=, >=) methods.
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/4/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#ifndef MyString_h
#define MyString_h

#include <string>
#include <ostream>

//  Class: MyString
//  ---------------
//  This class provides a simplified replacement for the standard C++ string.

class MyString {
public:

    // Constructor: MyString, MyString(str)
    // Usage: MyString myStr;
    //        MyString myStr(cppStr);
    // ------------------------------
    
    MyString();
    MyString(std::string str);
    MyString(char ch);
    
    // Destructor: ~MyString
    // Usage: (usually implicit);
    // --------------------------
    // Frees any heap storage asociated with the string.
    
    ~MyString();
    
    // Method: toString
    // Usage: std::string cppStr = myStr.toString();
    // ---------------------------------------------
    // Converts a MyString to a C++ string.
    
    std::string toString() const;
    
    // Method: length
    // Usage: int len = myStr.length();
    // --------------------------------
    // Returns the length of a string.
    
    int length() const;
    
    // Method: substr
    // Usage: MyString mySubStr = myStr.substr(start, n);
    // --------------------------------------------------
    // Returns a substring of the current string object.
    // The string begins at index position start and continues
    // for n characters or through the end of the string.
    
    MyString substr(int start = 0, int n = std::string::npos) const;
    
    // Operator: +, +=
    // Usage: MyString myStr = myStr1 + myStr2;
    //        MyString myStr += myStr1;
    // ----------------------------------------
    // Concantenate two MyStrings to form a third MyString.
    // Appends a MyString to an existing MyString.
    //
    // In both cases, the operand to the right of the operator is
    // passed as a parameter.
    //
    // The operand to the left of the operator
    // is implicitly referenced with a 'this' pointer and need not
    // be passed as a formal parameter.
    //
    // See: p.275
    
    MyString operator+(const MyString & myStr) const;
    MyString & operator+=(const MyString & myStr);
    
    // Operator: []
    // Usage: char ch = myStr[3];
    //        myStr[3] = 'a';
    // --------------------------
    // Bracket selection operator that selects a given character position
    // within the string for read or write access.  Calls an error handler
    // that displays a message (and halts the program) if the input index is
    // out of bounds.
    
    char & operator[](int n);
    
    friend bool operator==(const MyString & myStr1, const MyString & myStr2);
    friend bool operator!=(const MyString & myStr1, const MyString & myStr2);
    // TODO: Make these const correct.
    friend bool operator<=(MyString & myStr1, MyString & myStr2);
    friend bool operator>=(MyString & myStr1, MyString & myStr2);
    friend bool operator<(MyString & myStr1, MyString & myStr2);
    friend bool operator>(MyString & myStr1, MyString & myStr2);
    friend std::ostream & operator<<(std::ostream & os, MyString & myStr);
    
    // Copy Constructor: MyString
    // Usage: MyString(myStr);
    // -----------------------
    // Creates a new object by copying the contents of the passed-in object.
    //
    // Uses deep copying to ensure the copied object's heap memory references
    // are associated with separately allocated memory independent of the
    // source object.
    
    MyString(const MyString & srcStr);
    
    // Assignment Operator: =
    // Usage: MyString myStr2 = myStr1;
    // --------------------------------
    // Assignment operator that supports assigning the contents of one
    // instance of MyString to another.  Supports deep copying of heap
    // based data.

    MyString & operator=(const MyString & myStr);
    
private:
    // Private constants
    
    // Private variables
    
    struct ChCellT {
        char ch;
        ChCellT *next;
    };
    
    ChCellT * strbuf; // Pointer to heap where string contents reside.
    int count;
    
    // Private method prototypes
    
    void deepCopy(const MyString & src);
    void insertCh(char ch);
};

// Operators: ==, <=, >=, !=, <, >
// Usage: if (myStr1 == myStr2) { . . . }
// --------------------------------------
// Relational operators that return a boolean value of true or false
// depending upon the evaluation of the given expression.

bool operator==(const MyString & myStr1, const MyString & myStr2);
bool operator!=(const MyString & myStr1, const MyString & myStr2);

// TODO: Make these const correct.
bool operator>=(MyString & myStr1, MyString & myStr2);
bool operator<=(MyString & myStr1, MyString & myStr2);
bool operator<(MyString & myStr1, MyString & myStr2);
bool operator>(MyString & myStr1, MyString & myStr2);

// Operator: <<
// Usage: std::cout << MyString << std::endl;
// ------------------------------------------
// Overloads the stream insertion operator to write a MyString
// to an output stream such as the console.
//
// See: p.272

std::ostream & operator<<(std::ostream & os, MyString & myStr);

#endif // MyString_h
