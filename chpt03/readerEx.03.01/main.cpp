//
// main.cpp
//
// This program implements the function:
//
//    endsWith(str, suffix) 
//
// which returns true if the passed in string ends with the 
// provided suffix. Like startsWith(), the endsWith() function 
// allows the second argument to be either a string or a character.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/01/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

// Function prototypes

bool endsWith(string str, string suffix);
bool endsWith(string str, char suffixCh);
void testEndsWith();

int main(int argc, const char * argv[]) {
    testEndsWith();
    return 0;
}

//
// Function: endsWith
// Usage: if (endsWith(str, "sweet sorrow.")) cout << "word, yo" << endl;
// --------------------------------------------------------------------------
// Returns true is a string ends with a provided suffix string.
// Overloaded to return true if a string ends with a given suffix character.
//
// Algorithm calculates the point in the full string where substring comparison
// should begin and returns results of a substring comparison from that point.
//
// For example:
//
// Given this input string:   "Parting is such sweet sorrow."    length = 29
//                             ----+----1----+-^--2----+----    compare @ 16
// and this suffix string:                    "sweet sorrow."    length = 13
//
// 0-based start index is:     str.length() - suffix.length() = 29 - 13 = 16
//

bool endsWith(string str, string suffix) {
    if (suffix.length() > str.length()) return false;
    
    size_t startAt = str.length() - suffix.length();
    return (str.substr(startAt) == suffix);
}

bool endsWith(string str, char suffixCh) {
    if (str.length() < 1) {
        // Generally returns false since str is trivially empty,
        // unless the suffixCh is also trivially null :-)
        return (suffixCh == char(NULL));
    }
    size_t last = str.length() - 1;  // Correct for 0-based index.
    return (str[last] == suffixCh);
}

//
// Function testEndsWith
// Usage: testEndsWith();
// ----------------------
// Drives the endsWith() primitive with a simple mix of base strings and
// suffix strings, including edge cases such as empty strings and null
// suffix chars.
//

void testEndsWith() {
    string str[] = {"Parting is such sweet sorrow.", "Sorrow.", ""};
    string suffixes[] = { "sorrow.", "sorrow", "borrow.", "" };
    
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 4; i++) {
            (endsWith(str[j], suffixes[i])) ? cout << "PASS: " :
                                              cout << "FAIL: ";
            cout << "[" << str[j] << "]" << " ends with " << "[" << suffixes[i]
                 << "]" << "?" << endl;
    }
    
    char suffixChars[] = { '.', 'w', char(NULL)};
    
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 3; i++) {
            (endsWith(str[j], suffixChars[i])) ? cout << "PASS: " :
                                                 cout << "FAIL: ";
            cout << "[" << str[j] << "]" << " ends with " << "["
                 << suffixChars[i] << "]" << "?" << endl;
        }
}
