//
// main.cpp
//
// This program implements:
//
//    string capitalize(str);
//
// It returns a string with the initial character and all other letters 
// converted to lowercase.  (Non-letter characters are unaffected.)
//
// For example:
//
//    capitalize("BOOLEAN") --> "Boolean"
//    capitalize("boolean") --> "Boolean"
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/2/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cctype>
#include <string>
#include "error.h"
using namespace std;

// Function prototypes

void testCapitalize();
string capitalize(string str);

int main(int argc, char * argv[]) {
    error("hello");
    testCapitalize();
    return 0;
}

//
// Function: testCapitalize
// Usage: testCapitalize()
// Output:
// ---------------------------------------
// string: [BOOLEAN]
// capitalize: [Boolean]
// string: [boolean]
// capitalize: [Boolean]
// ---------------------------------------
// Simple test driver that illustrates nominal and edge-cases
//

void testCapitalize() {
    string testStr1 = "BOOLEAN";
    string testStr2 = "boolean";
    string newStr;
    
    // Nominal test.
    
    newStr = capitalize(testStr1);
    cout << "string: [" << testStr1 << "]"    << endl;
    cout << "capitalize: [" << newStr  << "]" << endl;
    
    // Test that we adequately shorten number of chars if too big otherwise.
    
    newStr = capitalize(testStr2);
    cout << "string: [" << testStr2 << "]"    << endl;
    cout << "capitalize: [" << newStr  << "]" << endl;
    
}

//
// Function: captialize
// Usage: string myCapStr("lowercase LAMENESS");
// Output: "Lowercase lameness"
// ---------------------------------------------
// Returns a string in which the initial character is capitalized (if it is a 
// letter) and all other letters are converted to lowercase. Characters other 
// than letters are not affected.
//

string capitalize(string str) {
    string newStr;
    
    for (int i = 0; i < str.length(); i++) {
        newStr += tolower(str[i]);
    }
    newStr[0] = toupper(newStr[0]); // C++ allows this mutation, unlike Java.
    return newStr;
}