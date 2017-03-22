//
// MyString.cpp
//
// This file implements the MyString class.  Care has been taken to make
// this const correct.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 12, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/14/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include "MyString.h"
#include "error.h"

// Implementation notes: constructor
// ---------------------------------
// The constructors initialize the instance variables.
// If the constructor is called with a C++ string, memory
// is allocated in the heap to accommodate the string.
    
MyString::MyString() {
    strbuf = new Vector<char>;
}

MyString::MyString(std::string str) {
    strbuf = new Vector<char>;
    int len = str.length();
    for (int i = 0; i < len; i++) {
        strbuf->add(str[i]);
    }
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
        delete strbuf;
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
    for (int i = 0; i < strbuf->size(); i++) {
        result += std::string(1, strbuf->get(i));
    }
    return result;
}
    
int MyString::length() const {
    return strbuf->size();
}
    
// Implementation notes: substr
// ----------------------------
// Returns a portion of a MyString begining at the start index and continuing
// for n characters.  If n is not specified, then the remainder of the string
// (relative to start) is returned.  If start is out of range, and error message
// is displayed and processing halts.

MyString MyString::substr(int start, int n) const {
    std::string result;
    
    if (start >= strbuf->size()) {
         error("MyString::substr start index is out of range.");
    }

    // If n not specified (i.e., n == npos by default), then
    // recalculate n to be the remaining characters in the string.
    
    n = (n == std::string::npos) ? strbuf->size() - start : n;
    for (int i = 0; i < n; i++) {
        result += std::string(1, strbuf->get(start + i));
    }
    
    return result;
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
// Leverages the corresponding operator in the underlying Vector
// representation of the class.

MyString & MyString::operator+=(const MyString & myStr) {
    *(this->strbuf) += *(myStr.strbuf);
    return *this;
}

// Implementation notes: [] (bracket selection operator)
// -----------------------------------------------------
// Overloads the bracket operator to allow selection of individual characters
// in the MyString.

char & MyString::operator[](int n) {
    return (*strbuf)[n];
}

// Implementation notes: deepCopy
// ------------------------------
// Copies the data from the src parameter into the current object.
// All dynamic memory is reallocated such that the current object
// and the source object refer to independent allocations.

void MyString::deepCopy(const MyString & src) {
    strbuf = new Vector<char>;
    
    // Rely upon deep copy of vector ADT.
    
    *strbuf = *src.strbuf;
}

// Operators: ==, <=, >=, !=, <, >
// Usage: if (myStr1 == myStr2) { . . . }
// --------------------------------------
// Relational operators that return a boolean value of true or false
// depending upon the evaluation of the given expression.

bool operator==(const MyString & myStr1, const MyString & myStr2) {
    bool result = true;
    if (myStr1.length() == myStr2.length()) {
        for (int i = 0; i < myStr1.length(); i++) {
            if (myStr1.strbuf->get(i) != myStr2.strbuf->get(i)) {
                result = false;
            }
        }
    } else {
        result = false;
    }
    return result;
}

bool operator<(const MyString & myStr1, const MyString & myStr2) {
    int minLen = min(myStr1.length(), myStr2.length());
    
    for (int i = 0; i < minLen; i++) {
        if (myStr1.strbuf->get(i) > myStr2.strbuf->get(i)) {
            return false;
        }
        if (myStr1.strbuf->get(i) < myStr2.strbuf->get(i)) {
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

bool operator>(const MyString & myStr1, const MyString & myStr2) {
    int minLen = min(myStr1.length(), myStr2.length());
    
    for (int i = 0; i < minLen; i++) {
        if (myStr1.strbuf->get(i) < myStr2.strbuf->get(i)) {
            return false;
        }
        if (myStr1.strbuf->get(i) > myStr2.strbuf->get(i)) {
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

bool operator<=(const MyString & myStr1, const MyString & myStr2) {
    return (myStr1 < myStr2 || myStr1 == myStr2);
}

bool operator>=(const MyString & myStr1, const MyString & myStr2) {
    return (myStr1 > myStr2 || myStr1 == myStr2);
}

bool operator!=(const MyString & myStr1, const MyString & myStr2) {
    return !(myStr1 == myStr2);
}

// Implementation notes: << (insertion stream operator)
// ----------------------------------------------------
// Inserts a C++ string-version of MyString into the output stream.

std::ostream & operator<<(std::ostream & os, MyString & myStr) {
    os << myStr.toString();
    return os;
}
