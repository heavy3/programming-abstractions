//
// main.cpp
//
// This program implements the function
//
//    string replaceAll(string str, char c1, char c2);
//
// which returns a copy of str with every occurrence of c1 replaced by c2.
//  
// For example:
//
//    replaceAll("nannies", 'n', 'd'); --> "daddies"
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 10
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/5/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

// Function prototypes

string replaceAll(string str, char thisCh, char withThatCh);
string replaceAll(string str, string thisStr, string withThatStr);
void testReplaceAll();

// Main function

int main(int argc,  char * argv[]) {
    testReplaceAll();
}

// Function definitions

//
// Function: testReplaceAll
// Usage: testReplaceAll();
// ------------------------
// Simple driver for replaceAll.  Exercise both variants.
//

void testReplaceAll() {
    string str = "nannies";
    
    string output = replaceAll(str, 'n', 'd');
    cout << "input: " << str << "\toutput: " << output << endl;

    str = "badboy";
    output = replaceAll(str, "bad", "fan");
    cout << "input: " << str << "\toutput: " << output << endl;

}

//
// Function: replaceAll
// Usage: string output = replaceAll("nannies", 'n', 'd');
// Usage: string output = replaceAll("badboy", "bad", "fan");
// ---------------------------------------------------------
// Returns a new string that reflects the input string with a character
// or substring substitution.
//
// For uniformity of implementation across overloaded variants (and to
// get experience with some of the higher-order string primitives, I'm
// gonna use the str.find() + str.replace pattern to locate and mutate
// a copy of the input string.
//

string replaceAll(string input, char thisThing, char substr2) {
    size_t matchIndex;
    size_t startingHere = 0;
    
    matchIndex = input.find(thisThing, startingHere);
    while (matchIndex != string::npos) {
        input.at(matchIndex) = substr2;
        
        startingHere = matchIndex;
        matchIndex = input.find(thisThing, startingHere);
    }
    return input;
}

// Overloaded variant of above which handles substring swaps
// versus single character replacement.

string replaceAll(string str, string substr1, string substr2) {
    size_t index = 0;
    
    while (true) {
        index = str.find(substr1, index);
        if (index == string::npos) break;
        str.replace(index, substr1.length(), substr2);
    }
    return str; // Mutated copy of original since it was passed in by value.
}