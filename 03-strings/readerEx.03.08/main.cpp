//
// main.cpp
//
// This program implements the removeCharacters() function using an 
// in-place algorithm.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 8
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

void testRemoveCharactersInPlace();
void removeCharactersInPlace(string & str, string delChars);
bool iskeeper(char ch, string delChars);

// Main program

int main(int argc, char * argv[]) {
    testRemoveCharactersInPlace();
    return 0;
}

//
// Function: testRemoveCharactersInPlace
// Usage: testRemoveCharactersInPlace();
// ------------------------------
// Simple test driver for removeCharactersInPlace();
// Drives routine with some hardcoded test strings.
//

void testRemoveCharactersInPlace() {
    string reference = "counterrevolutionaries";
    string delChars = "aeiou";
    string mutateMe = reference;
    
    removeCharactersInPlace(mutateMe, delChars);
    cout << reference << " - " << delChars << " = " << mutateMe << endl;
    
    reference = mutateMe = "blah";
    delChars = "BLAH";
    removeCharactersInPlace(mutateMe, delChars);
    cout << reference << " - " << delChars << " = " << mutateMe << endl;
    
    reference = mutateMe = "";
    delChars = "deleteme";
    removeCharactersInPlace(mutateMe, delChars);
    cout << reference << " - " << delChars << " = " << mutateMe << endl;
    
    reference = mutateMe = "delete nothing";
    delChars = "";
    removeCharactersInPlace(mutateMe, delChars);
    cout << reference << " - " << delChars << " = " << mutateMe << endl;
}

//
// Function: removeCharactersInPlace
// Usage: str = "shorten me"; removeCharactersInPlace(str, "oe")
// Output: str = "shrtn m"
// ------------------------------------------------------------
// Removes certain letters from an input string according to
// a second "delete" string containing the characters to filter.
// Result mutates original string passed in.
//
// Simple character-by-character O(n^2) iteration with no attempt
// at performance.  Going for improved readability since I'm a C++
// noob.
//

void removeCharactersInPlace(string & str, string delChars) {
    string result;
    
    for (int i = 0; i < str.length(); i++) {
        if (iskeeper(str[i], delChars)) {
            result += str[i];
        }
    }
    str = result;
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