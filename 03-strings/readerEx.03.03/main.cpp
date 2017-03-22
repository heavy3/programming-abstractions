//
// main.cpp
//
// This program implements a free function:
//
//     string substr(str, pos, n);
//
// which returns the substring of str beginning at pos and containing at 
// most n characters.
//
// The following edge cases are also handled:
//
//  • If n is missing or greater than the length of the string, the substring
//    should extend through the end of the original string.
//
//  • If pos is greater than the length of the string, substr should call 
//    error with an appropriate message.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/02/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
#include "error.h"
using namespace std;

// Function prototypes

string substr(string str, int pos, int n);
string substr(string str, int pos);
void testSubstr();

int main(int argc, char * arvg[]) {
    testSubstr();
    return 0;
}

//
// Function: testSubstr
// Usage: testSubstr()
// -------------------
// Simple test driver that illustrates nominal and edge-cases.
//
// Output:
// string: [We will we will rock you!]
// substr: [we will rock]
// string: [We will we will rock you!]
// substr: [we will rock you!]
// string: [We will we will rock you!]
// substr: [we will rock you!]
// Error: substr: startIndex out of bounds
//

void testSubstr() {
    string testStr = "We will we will rock you!";
    // ----+----1----+----2----+
    string newStr;
    
    // Nominal test
    
    newStr = substr(testStr, 8, 12);
    cout << "string: [" << testStr << "]" << endl;
    cout << "substr: [" << newStr  << "]" << endl;
    
    // Test that we adequately shorten number of chars if too big otherwise.
    
    newStr = substr(testStr, 8, 25);
    cout << "string: [" << testStr << "]" << endl;
    cout << "substr: [" << newStr  << "]" << endl;
    
    // Test if number of chars to copy is left out of function call.
    
    newStr = substr(testStr, 8);
    cout << "string: [" << testStr << "]" << endl;
    cout << "substr: [" << newStr  << "]" << endl;
    
    // Should fail with error since startIndex > string length.
    
    newStr = substr(testStr, 30, 5);
    cout << "string: [" << testStr << "]" << endl;
    cout << "substr: [" << newStr  << "]" << endl;
}

//
// Function: substr
// Usage: string str = substr("We will we will rock you", 8, 12);
// Output: str = "we will rock"
// --------------------------------------------------------------
// Returns a new string containing a substring of the original
// starting from a given index position and specified number of characters to
// include.
//
// Care is taken to avoid shorten the number of characters if the
// requested number extends beyond the length of the original string.
//
// A runtime error is generated according to the specifications of the problem
// if the starting index is beyond the end of the original string.
//

string substr(string str, int startIndex, int nChars) {
    string newStr;
    
    if (startIndex > (str.length() - 1)) {
        error("substr: startIndex out of bounds");
    }
    
    // Normalize the number of characters to copy into the substring.
    //
    // Guard against out-of-bounds error if number of characters to copy
    // indexes beyond the end of the original string.

    int _nChars = (startIndex + nChars > str.length()) ?
            str.length() - startIndex : nChars;
    
    for (int i = 0; i < _nChars; i++) {
        newStr += str[i + startIndex];
    }
    return newStr;
}

//
// Function: substr
// Usage: string str = substr("We will we will rock you", 8);
// Output: "we will rock you"
// ----------------------------------------------------------
// Overloaded version of substr that does not require number of characters 
// to be specified.  In this case, the sub-string will simply extend to the
// end of the original string from the starting index.
//

string substr(string str, int startIndex) {
    string newStr;
    
    if (startIndex > (str.length() - 1)) {
        error("substr: startIndex out of bounds");
    }
    
    int nChars = str.length() - startIndex;
    
    for (int i = 0; i < nChars; i++) {
        newStr += str[i + startIndex];
    }
    return newStr;
}
