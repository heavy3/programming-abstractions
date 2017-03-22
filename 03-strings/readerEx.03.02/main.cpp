//
// main.cpp
//
// This program implements the function trim(str) which returns a new
// string with a whitespace characters removed from the beginning and
// end of the input string.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/02/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

//
// huh, a mystery ...
//
// This idiom runs as expected, iterating backwards through a string ...
//
//    string str = "blah";
//    for (int i = int(str.length() - 1); i >= 0; i--) {
//         ...
//    }
//
// But this does /not/ work as expected :-/
//
//    string str = "blah";
//    for (size_t i = str.length() - 1; i == 0; i--) {
//        ...
//    }
//
// instead it never enters the body of the loop.  Why?
//

#include <iostream>
#include <cctype>
#include <string>
using namespace std;

// Function prototypes

string trimFront(string str);
string trimBack(string str);
string trim(string str);

//
// Typical output looks like this:
// ------------------------------------------------------------------------
// Before: [    trim my bangs, sis]
//  After: [trim my bangs, sis]
// Unchgd: [    trim my bangs, sis]
//
// Before: [trim my neck, bro   ]
//  After: [trim my neck, bro]
// Unchgd: [trim my neck, bro   ]
//
// Before: [  give me the full trim  ]
//  After: [give me the full trim]
// Unchgd: [  give me the full trim  ]
// ------------------------------------------------------------------------
//

int main(int argc, char * argv[]) {
    string testStringFront = "    trim my bangs, sis";
    string testStringBack = "trim my neck, bro   ";
    string testStringBoth1 = "  give me the full trim  ";
    string testStringBoth2 = "nothing to trim";
    string testStringBoth3 = ""; // bald
    
    cout << "Before: [" << testStringFront << "]  " << endl;
    cout << " After: [" << trimFront(testStringFront) << "]  " << endl;
    cout << "Unchgd: [" << testStringFront << "]  " << endl << endl;
    
    cout << "Before: [" << testStringBack << "]  " << endl;
    cout << " After: [" << trimBack(testStringBack) << "]  " << endl;
    cout << "Unchgd: [" << testStringBack << "]  " << endl << endl;
    
    cout << "Before: [" << testStringBoth1 << "]  " << endl;
    cout << " After: [" << trim(testStringBoth1) << "]  " << endl;
    cout << "Unchgd: [" << testStringBoth1 << "]  " << endl << endl;
    
    cout << "Before: [" << testStringBoth2 << "]  " << endl;
    cout << " After: [" << trim(testStringBoth2) << "]  " << endl;
    cout << "Unchgd: [" << testStringBoth2 << "]  " << endl << endl;

    cout << "Before: [" << testStringBoth3 << "]  " << endl;
    cout << " After: [" << trim(testStringBoth3) << "]  " << endl;
    cout << "Unchgd: [" << testStringBoth3 << "]  " << endl << endl;
    
    return 0;
}

//
// Function: trim
// Usage: string str = trim("  full trim, please   ");
// ---------------------------------------------------
// Returns a new string with whitespace removed from beginning and end
// of passed-in string.
//

string trim(string str) {
    return (trimFront(trimBack(str)));
}

//
// Function: trimFront
// Usage: string myTrimmedString = trimFront("   trim me, bro");
// -------------------------------------------------------------
// Returns a new string which equals the passed-in string less any
// whitespace at the beginning of the string.
//
// Iterates character by character for the entire string.
// Once any prefixed whitespace is bypassed, the remainder of characters
// are copied from input string to output string via catenation operator.
//

string trimFront(string untrimmed) {
    string trimmed = "";
    bool stopTrimming = false;
    
    for (size_t i = 0; i < untrimmed.length(); i++) {
        if (stopTrimming) {
            trimmed += untrimmed[i];
        } else {
            if (!isspace(untrimmed[i])) {
                stopTrimming = true;
                trimmed += untrimmed[i];
            }
        }
    }
    return trimmed;
}

//
// Function: trimBack
// Usage: string myTrimmedString = trimBack("trim my neck, bro  ");
// ----------------------------------------------------------------
// Returns a new string which equals the passed-in string less any
// whitespace at the end of the string.
//
// Iterates character by character from back to front for  the entire string.
// Once any suffix whitespace is bypassed, the remainder of characters
// are copied from input string to output string via catenation operator.
//

string trimBack(string untrimmed) {
    string trimmed = "";
    bool stopTrimming = false;
    
    for (int i = int(untrimmed.length() - 1); i >= 0; i--) {
        if (stopTrimming) {
            trimmed = untrimmed[i] + trimmed;
        } else {
            if (!isspace(untrimmed[i])) {
                stopTrimming = true;
                trimmed = untrimmed[i] + trimmed;
            }
        }
    }
    return trimmed;
}