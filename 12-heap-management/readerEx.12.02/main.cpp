//
// main.cpp
//
// This program implements:
//
//      char *copyCString(char *str);
//
// which allocates enough memory for the C-style string str and then copies
// the characters—along with the terminating null character—into the newly
// allocated memory.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 12, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/11/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "error.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 12.02\n";
const std::string DETAIL = "Safe strcpy that avoids buffer overflow.";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

char * copyCString(char *str);

// Main program

int main() {
    cout << BANNER << endl << endl;;
    
    char myStr[] = "hello, world";
    cout << "Original string: " << myStr << endl;

    char * newStr = copyCString(myStr);
    cout << "Copy of string: " << newStr << endl;
    
    delete[] newStr;
    
    return 0;
}

// Function: copyCString
// Usage: char * newStr = copyCString(oldStr);
// -------------------------------------------
// Returns a pointer to a heap copy of the C-style string passed in.
// Replicates the functionality of strcpy but adds the safety of determining
// the proper size of the target string before allocating that amount
// of space on the heap for the copy.
//
// The client must free the memory allocated once the string is no longer
// needed.

char * copyCString(char *str) {
    int size = 0;
    
    while (str[size++] != char(NULL));
    
    char * newStr = NULL;
    if (size > 0) {
        newStr = new char[size];
        for (int i = 0; i <= size; i++) {
            newStr[i] = str[i];
        }
    }
    return newStr;
}