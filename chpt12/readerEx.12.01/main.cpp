//
// main.cpp
//
// This program implements a function, createIndexArray(n), which allocates
// a dynamic array of n integers in which each integer is initialized to 
// its own index.
//
// For example, 
//
//    createIndexArray(8) returns a pointer to the following array on the heap:
//
//         -   -   -   -   -   -   -   -
//    -->[ 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 ]
//         -   -   -   -   -   -   -   -
//         0   1   2   3   4   5   6   7
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 12, Exercise 1
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

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 12.01\n";
const std::string DETAIL = "createIndexArray(n) on heap";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

int * createIndexArray(int n);
void printIndexArray(const int * array, const int nElems);

// Main program

int main() {
    cout << BANNER << endl << endl;;
    
    int size = 8;
    int * array = createIndexArray(size);
    printIndexArray(array, size);
    delete[] array;
    return 0;
}

// Function: createIndexArray
// Usage: int *pArray = createIndexArray(nElems);
// ----------------------------------------------
// Returns a pointer to an integer array of size n residing on the
// heap.

int * createIndexArray(int n) {
    int *array = new int[n];
    
    for (int i = 0; i < n; i++) {
        array[i] = i;
    }
    return array;
}

// Function: printIndexArray
// Usage: printIndexArray(pArray, n);
// ---------------------------------------
// Prints the contents of an n-element integer array to the console.

void printIndexArray(const int * array, const int nElems) {
    for (int i = 0; i < nElems; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}