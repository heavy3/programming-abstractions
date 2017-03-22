//
// main.cpp
//
// This program implements a function
//
//     removeDoubledLetters(str)
//
// that returns a new string in which any duplicated characters in str have
// been replaced by a single copy.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/4/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
using namespace std;

// Function protypes

void testRemoveDoubledLetters();
string removeDoubledLetters(string input);
bool isDoubled(string input);

// Main program

int main(int argc, const char * argv[]) {
    testRemoveDoubledLetters();
    return 0;
}

// 
// Function: testRemoveDoubledLetters
// Usage: testRemoveDoubledLetters();
// ----------------------------------
// Simple driver for removeDoubledLetters.
//

void testRemoveDoubledLetters() {
    string olde = "bookkeeper";
    string newe = removeDoubledLetters(olde);
    cout << "before: " << olde << " after: " << newe << endl;
    
    olde = "bookkeeperr";
    newe = removeDoubledLetters(olde);
    cout << "before: " << olde << " after: " << newe << endl;
    
    olde = "aa";
    newe = removeDoubledLetters(olde);
    cout << "before: " << olde << " after: " << newe << endl;
    
    olde = "a";
    newe = removeDoubledLetters(olde);
    cout << "before: " << olde << " after: " << newe << endl;
    
    olde = "aaay";
    newe = removeDoubledLetters(olde);
    cout << "before: " << olde << " after: " << newe << endl;
    
    olde = "bookkeeperrr";
    newe = removeDoubledLetters(olde);
    cout << "before: " << olde << " after: " << newe << endl;
}

//
// Function: removeDoubledLetters
// Usage: string after = removeDoubledLetters(before);
// ---------------------------------------------------
// Returns string with all double-letter sequences
// replaced with a single letter (e.g., letter --> leter).
//

string removeDoubledLetters(string input) {
    char currCh, nextCh = '\0';
    string output;
    
    if (input.length() < 2) return output = input;

    //
    // Iterate over length of string, with current and next
    // characters in tandem.
    //
    
    for (int i = 1; i < input.length(); i++) {
        currCh = input.at(i-1);
        nextCh = input.at(i);
        if (currCh != nextCh) {
            //
            // Only add the current character to the output
            // string if it is not a duplicate of her tandem neighbor.
            //
            output += currCh;
        }
    }
    
    //
    // The last character of the input string will land in nextCh coming
    // out of the iterator.  
    //
    // It will always be different than the last character in the output string
    // at this stage (since we've only added chars to the output under that
    // condition).
    //
    // Therefore it is always appropriate to append the last character.
    //
    
    output += nextCh;
    
    return output;
}