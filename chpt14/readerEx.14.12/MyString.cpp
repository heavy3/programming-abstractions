//
// MyString.cpp
//
// This file implements the MyString class using a linked-list.
//
// TODO: Fix const correctness of comparison operators (<, >, <=, >=) methods.
// TODO: Fix code duplication.  Add primitive for moving to a specific index.
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

#include "MyString.h"
#include "error.h"

// Implementation notes: constructor
// ---------------------------------
// The constructors initialize the instance variables.
// If the constructor is called with a C++ string, memory
// is allocated in the heap to accommodate the string.
    
MyString::MyString() {
    
    // Point the head of the buffer to a dummy cell that
    // simplifies list-mutation code at the modest expense of
    // a few bytes of memory.
    
    strbuf = new ChCellT;
    strbuf->next = NULL;
    count = 0;
}

MyString::MyString(std::string str) {
    strbuf = new ChCellT;
    strbuf->next = NULL;
    count = str.length();
    
    ChCellT *pCh = strbuf;
    for (int i = 0; i < count; i++) {
        ChCellT *pNewCh = new ChCellT;
        pNewCh->ch = str[i];
        pNewCh->next = pCh->next;   // wire out
        pCh->next = pNewCh;         // wire in
        pCh = pCh->next;
    }
}

MyString::MyString(char ch) {
    strbuf = new ChCellT;
    strbuf->next = new ChCellT;
    strbuf->next->ch = ch;
    strbuf->next->next = NULL;
    count = 1;
}

// Implementation notes: copy constructor and assignment operator
// --------------------------------------------------------------
// These methods make it possible to pass a MyString by value or
// assign one MyString to another.  The underlying work is done by
// a deepCopy method.

MyString::MyString(const MyString & srcStr) {
    deepCopy(srcStr);
}

MyString & MyString::operator=(const MyString & srcStr) {
    if (this != &srcStr) {
        
        // Free any dynamically allocated heap memory
        // already associated with the target MyString object
        // before we deep copy into it.
        
        // Delete starting from list head head to tail, being careful
        // not to orphan any cells associated with the tail
        // as we proceed.
        
        ChCellT *pDelMe = strbuf;
        while (pDelMe != NULL) {
            ChCellT *pSaveNext = pDelMe->next;
            delete pDelMe;
            pDelMe = pSaveNext;
        }
        deepCopy(srcStr);
    }
    return *this;
}

MyString::~MyString() {
    delete strbuf;
    strbuf = NULL;
}
    
// Implementation notes: toString
// ------------------------------
// Returns a C++ string equivalent of a MyString by appending each character
// of the strbuf to a null C++ string.
    
std::string MyString::toString() const {
    std::string result;
    ChCellT *pCh = strbuf;
    for (int i = 0; i < count; i++) {
        if (pCh->next == NULL) break;
        result += pCh->next->ch;
        pCh = pCh->next;
    }
    return result;
}
    
int MyString::length() const {
    return count;
}
 
// Implementation notes: substr
// ----------------------------
// Returns a portion of a MyString begining at the start index and continuing
// for n characters.  If n is not specified, then the remainder of the string
// (relative to start) is returned.  If start is out of range, and error message
// is displayed and processing halts.

MyString MyString::substr(int start, int n) const {
    // TODO: Cheating a little here by relying upon a std::string.
    std::string result;
    
    if (start >= this->length()) {
         error("MyString::substr start index is out of range.");
    }

    // If n not specified (i.e., n == npos by default), then
    // recalculate n to be the remaining characters in the string.
    
    n = (n == std::string::npos) ? this->length() - start : n;
    ChCellT *pSrcCh = this->strbuf->next;
    for (int i = 0; i < start; i++) {
        pSrcCh = pSrcCh->next;
    }
    
    for (int i = 0; i < n; i++) {
        result += pSrcCh->ch;
        pSrcCh = pSrcCh->next;
    }

    return result;  // copy constructor converts this from string to MyString
}

// Implementation notes: + (concantenation operator)
// -------------------------------------------------
// Concatenate two MyStrings together to make a third.
// Leverages the copy constructor and the overloaded += operator.
    
MyString MyString::operator+(const MyString & myStr) const {
    MyString result(*this);
    
    result += myStr;
    return result;
}

// Implementation notes: += (append operator)
// ------------------------------------------
// Appends a MyString to the current MyString object pointed to by this.

MyString & MyString::operator+=(const MyString & myStr) {
    if (myStr.length() == 0) {
        return *this;
    }
    
    ChCellT *pDstCh = this->strbuf;
    for (int i = 0; i < this->length(); i++) {
        pDstCh = pDstCh->next;
    }
    
    ChCellT *pSrcCh = myStr.strbuf;
    if (pSrcCh == NULL) return *this;       // defensive
    
    for (int i = 0; i < myStr.count; i++) {
        if (pSrcCh->next == NULL) break;
        
        ChCellT *pNewCh = new ChCellT;
        pNewCh->ch = pSrcCh->next->ch;      // add src ch to new cell
        pNewCh->next = NULL;                // wire out
        
        pDstCh->next = pNewCh;              // wire in
        
        pSrcCh = pSrcCh->next;              // advance src and dst pointers
        pDstCh = pDstCh->next;
        (this->count)++;
    }
    return *this;
}

// Implementation notes: [] (bracket selection operator)
// -----------------------------------------------------
// Overloads the bracket operator to allow selection of individual characters
// in the MyString.

char & MyString::operator[](int n) {
    if (n >= this->count || n < 0) error ("Index out of range");
    if (this->count <= 0) error("Empty string");
    ChCellT *pCell = this->strbuf->next;    // account for dummy cell at head
    
    for (int i = 0; i < n; i++) {
        pCell = pCell->next;
    }
    return pCell->ch;
}

// Implementation notes: deepCopy
// ------------------------------
// Copies the data from the src parameter into the current object.
// All dynamic memory is reallocated such that the current object
// and the source object refer to independent allocations.

void MyString::deepCopy(const MyString & src) {
    this->strbuf = new ChCellT;
    this->strbuf->next = NULL;
    this->count = src.count;
    
    ChCellT *pDstCh = this->strbuf;
    ChCellT *pSrcCh = src.strbuf;
    
    for (int i = 0; i < src.count; i++) {
        if (pSrcCh->next == NULL) break;
        ChCellT *pNewCh = new ChCellT;
        pNewCh->ch = pSrcCh->next->ch;      // add src ch to new cell
        
        pNewCh->next = pDstCh->next;        // wire out
        pDstCh->next = pNewCh;              // wire in
        
        pSrcCh = pSrcCh->next;              // advance src and dst pointers
        pDstCh = pDstCh->next;
    }
}
 
// Operators: ==, <=, >=, !=, <, >
// Usage: if (myStr1 == myStr2) { . . . }
// --------------------------------------
// Relational operators that return a boolean value of true or false
// depending upon the evaluation of the given expression.

bool operator==(const MyString & myStr1, const MyString & myStr2) {
    bool result = true;
    if (myStr1.length() != myStr2.length()) return false;
    
    MyString::ChCellT *pDstCh = myStr1.strbuf;
    MyString::ChCellT *pSrcCh = myStr2.strbuf;
    
    for (int i = 0; i < myStr2.length(); i++) {
        if (pSrcCh->next == NULL) break;
        
        if (pSrcCh->next->ch != pDstCh->next->ch) {
            result = false;
            break;
        }
        
        pSrcCh = pSrcCh->next;              // advance src and dst pointers
        pDstCh = pDstCh->next;
    }
    return result;
}

bool operator!=(const MyString & myStr1, const MyString & myStr2) {
    return !(myStr1 == myStr2);
}

// TODO: Make the next 4 member functions const correct.

bool operator<(MyString & myStr1, MyString & myStr2) {
    int minLen = std::min(myStr1.length(), myStr2.length());
    
    for (int i = 0; i < minLen; i++) {
        if (myStr1[i] > myStr2[i]) {
            return false;
        }
        if (myStr1[i] < myStr2[i]) {
            return true;
        }
    }
    if (myStr1.length() >= myStr2.length()) {
        
        // If two strings share a common prefix, the longer string
        // is lexigraphically greater than the shorter.
        //
        // In the case of identical strings, return false as well
        // since we're looking for strictly < relationships.
        
        return false;
    }
    return true;
}

bool operator>(MyString & myStr1, MyString & myStr2) {
    int minLen = std::min(myStr1.length(), myStr2.length());
    
    for (int i = 0; i < minLen; i++) {
        if (myStr1[i] < myStr2[i]) {
            return false;
        }
        if (myStr1[i] > myStr2[i]) {
            return true;
        }
    }
    
    if (myStr1.length() <= myStr2.length()) {
        
        // If two strings share a common prefix, the longer string
        // is lexigraphically greater than the shorter.
        //
        // In the case of identical strings, return false as well
        // since we're looking for strictly > relationships.
        
        return false;
    }
    return true;
}

bool operator<=(MyString & myStr1, MyString & myStr2) {
    return (myStr1 < myStr2 || myStr1 == myStr2);
}

bool operator>=(MyString & myStr1, MyString & myStr2) {
    return (myStr1 > myStr2 || myStr1 == myStr2);
}

// Implementation notes: << (insertion stream operator)
// ----------------------------------------------------
// Inserts a C++ string-version of MyString into the output stream.

std::ostream & operator<<(std::ostream & os, MyString & myStr) {
    os << myStr.toString();
    return os;
}
