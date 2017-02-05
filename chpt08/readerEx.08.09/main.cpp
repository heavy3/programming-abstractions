//
// main.cpp
//
// This program implements the function,
//
//      void listPermutations(string str);
//
// which lists a permutations of string without resorting to using
// sets or applying string methods (other than length or selection).
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/10/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "simpio.h"
using namespace std;

// Prototypes

void listPermutations(string str);
void permuteStr(string & str, int index);
void swapCh(string & str, int i1, int i2);

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.09\n";
const std::string DETAIL = "Optimized String Permutations";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    string str = getLine("Enter a string: ");
    
    cout << "The permutations of \"" << str << "\" are:" << endl;
    listPermutations(str);
    
    return 0;
}

//
// Function: listPermutations
// Usage: listPermutations("my string");
// -------------------------------------
// Function that efficiently enumerates the permutations of an input string.
// This is basically a wrapper script for permuteStr.
//

void listPermutations(string str) {
    permuteStr(str, 0);
}

//
// Function: permuteStr
// Usage: premuteStr("my string", 0);
// ----------------------------------
// Enumerates the permutations of the input string relative to an index.
// If the index is 0, the entire string is permuted.
// Output is sent to the console.
//
// Algorithm:  (Courtesy Eric Roberts)
//
// a) At each level, pass the entire string along with an index that
//    indicates where the permutation process starts. Characters in the
//    string before this index stay where they are; characters at or after
//    that position must go through all their permutations.
//
// b) The simple case occurs when the index reaches the end of the string.
//
// c) The recursive case operates by swapping the character at the index
//    position with every other character in the string and then generating
//    every permutation starting with the next higher index and then swapping
//    the characters back to ensure that the original order is restored.
//
// Limitations:
//
// Does not filter out duplicates if an input string contains multiples
// of a given character.
//

void permuteStr(string & str, int index) {
    if (index == (str.length() - 1)) {
        cout << str << endl;
    } else {
        for (int i = index; i < str.length(); i++) {
            swapCh(str, index, i);
            permuteStr(str, index + 1);
            swapCh(str, i, index);
        }
    }
}

//
// Function: swapCh
// Usage: swapCh(str, index1, index2);
// -----------------------------------
// Swaps two characters within a string.  The characters are designated
// by positional index.
//

void swapCh(string & str, int i1, int i2) {
    if (i1 != i2) {
        char tmp = str[i1];
        str[i1] = str[i2];
        str[i2] = tmp;
    }
}
