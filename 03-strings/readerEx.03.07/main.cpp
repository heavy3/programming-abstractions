//
// main.cpp
//
// This program implements the function
//
//    string removeCharacters(string str, string remove);
//
// which returns a new string consisting of the characters in str 
// after removing all instances of the characters in the 'remove'
// string.
//  
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/4/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

// Function prototypes

void testRemoveCharacters();
string removeCharacters(string orig, string delChars);
bool iskeeper(char ch, string delChars);

// Main program

int main(int argc, char * argv[]) {
    testRemoveCharacters();
    return 0;
}

//
// Function: testRemoveCharacters
// Usage: testRemoveCharacters();
// ------------------------------
// Simple test driver for removeCharacters();
// Drives routine with some hardcoded test strings.
//

void testRemoveCharacters() {
    string orig = "counterrevolutionaries";
    string delChars = "aeiou";
    string newStr = removeCharacters(orig, delChars);
    cout << orig << " - " << delChars << " = " << newStr << endl;
    
    orig = "blah";
    delChars = "BLAH";
    newStr = removeCharacters(orig, delChars);
    cout << orig << " - " << delChars << " = " << newStr << endl;
    
    orig = "";
    delChars = "deleteme";
    newStr = removeCharacters(orig, delChars);
    cout << orig << " - " << delChars << " = " << newStr << endl;
    
    orig = "delete nothing";
    delChars = "";
    newStr = removeCharacters(orig, delChars);
    cout << orig << " - " << delChars << " = " << newStr << endl;
}

//
// Function: removeCharacters
// Usage: new abbreviate = removeCharacters("shorten me", "oe")
// Output: abbreviate = "shrtn m"
// ------------------------------------------------------------
// Removes certain letters from an input string according to
// a second "delete" string containing the characters to filter.
// Result is returned as a new string.
//
// TODO: Find something better than O(n^2).
//

string removeCharacters(string orig, string delChars) {
    string result;
    
    for (int i = 0; i < orig.length(); i++) {
        if (iskeeper(orig[i], delChars)) {
            result += orig[i];
        }
    }
    return result;
}

//
// Function: iskeeper
// Usage: if (iskeeper(inputCh, delStr)) { ... }
// ----------------------------------------------
// Returns true if a given input character should be preserved since
// it is not in the list of delete characters.
//

bool iskeeper(char ch, string delChars) {
    bool decision = true;
    
    for (int i = 0; i < delChars.length(); i++) {
        if (ch == delChars[i]) {
            decision = false;
            break;
        }
    }
    return decision;
}