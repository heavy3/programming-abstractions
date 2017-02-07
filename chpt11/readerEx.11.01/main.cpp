//
// main.cpp
//
// This program implements a recursive algorithm for generating a sequence
// of binary numbers of a given bit-length.
//
// For example,
//
//    generateBinaryCode(3);
//
// will generate the following console output:
//
//    000
//    001
//    010
//    011
//    100
//    101
//    110
//    111
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 11, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 6/14/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "error.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 11.01\n";
const std::string DETAIL = "Generate Binary Numbers Recursively";
const std::string BANNER = HEADER + DETAIL;
const int MAX_NBITS = 3;

// Prototypes

void generateBinaryCode(int nBits);
void generateBinaryCodeRec(int nBits, string code);

// Main program

int main() {
    cout << BANNER << endl << endl;;
    
    for (int nBits = 1; nBits <= MAX_NBITS; nBits++) {
        cout << nBits << "-length binary codes" << endl;
        cout << "-----------------------------" << endl;
        generateBinaryCode(nBits);
        cout << endl;
    }
    return 0;
}

// Function: generateBinaryCode
// Usage: generateBinaryCode(3);
// -----------------------------
// Displays all unique n-length binary codes to the console in ascending
// order, one code per line.
//
// Wrapper function for generateBinaryCodeRec;

void generateBinaryCode(int nBits) {
    generateBinaryCodeRec(nBits, "");
}

// Function: generateBinaryCodeRec
// Usage: generateBinaryCodeRec(n, "");
// ------------------------------------
// Displays all unique n-length binary codes to the console in ascending
// order, one code per line.
//
// Uses recursion to build-up a code from right to left until
// nBits diminishes to 0 and we have the desired code length.
//
// The recursive insight is that binary numbers with n-bits
// are comprised of two sets:
//
// • binary numbers with n - 1 bits preceded by a 0.
// • binary numbers with n - 1 bits preceded by a 1.

void generateBinaryCodeRec(int nBits, string code) {
    
    // base case: number of bits are 0
    if (nBits <= 0) {
        if (code.size() > 0) cout << code << endl;
        return;
    }

    // n-length code ==  n-1 length code + "0"
    //                   n-1 length code + "1"
    
    generateBinaryCodeRec(nBits - 1, code + string("0"));
    generateBinaryCodeRec(nBits - 1, code + string("1"));
}