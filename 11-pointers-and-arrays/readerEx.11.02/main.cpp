//
// main.cpp
//
// This program implements a recursive function for generating
// a sequence of Gray binary codes.  Consecutive Gray codes differ
// in only 1 bit value.
//
// For example,
//
//    generateGrayCode(3);
//
// produces the following output:
//
//    000   // 0
//    001   // 1
//    011   // 2
//    010   // 3
//    110   // 4
//    111   // 5
//    101   // 6
//    100   // 7
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 11, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// The algorithm for this code comes from the problem statement.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 6/15/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <bitset>
#include "vector.h"
#include "error.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 11.02\n";
const std::string DETAIL = "Generate Gray Code Recursively";
const std::string BANNER = HEADER + DETAIL;
const int MAX_NBITS = 3;

// Prototypes

void generateGrayCode(const int nBits);
void generateGrayCodeRec(const int nBits, Vector<string> codes);

// Main program

int main() {
    cout << BANNER << endl << endl;;
    
    for (int nBits = 0; nBits <= MAX_NBITS; nBits++) {
        cout << nBits << "-length Gray code" << endl;
        cout << "------------------" << endl;
        generateGrayCode(nBits);
        cout << endl;
    }
    return 0;
}

// Function: generateGrayCode
// Usage: generateGrayCode(3);
// -----------------------------
// Displays all unique n-length Gray codes to the console in ascending
// order, one code per line.
//
// Wrapper function for generateGrayCodeRec;

void generateGrayCode(const int nBits) {
    Vector<string> codesSoFar;
    codesSoFar += "0";
    codesSoFar += "1";
    generateGrayCodeRec(nBits, codesSoFar);
}

// Function: generateGrayCodeRec
// Usage: generateGrayCodeRec(n, codesVector);
// -------------------------------------------
// Outputs to the console a list of binary Gray codes, one per line,
// in ascending order.  The number of bits in each code word is determined
// by the value of n passed in.
//
// Uses recursion to build-up a code from right to left until
// nBits diminishes to 0 and we have the desired code length.
//
// Courtesy: Eric Roberts
//
// The recursive insight that you need to create a Gray code of N bits
// is summarized in the following informal procedure:
//
// 1. Write down the Gray code for N – 1 bits.
//
// 2. Copy that same list in reverse order below the original one.
//
// 3. Add a 0 bit in front of the encodings in the original half of the list
//    and a 1 bit in front of those in the reversed copy.
//
// TODO: This is a bit memory intense.  I'm sure there is a clever way
//       to pass the codes by reference and incrementally build upon them.

void generateGrayCodeRec(const int nBits, Vector<string> codes) {
    
    // Base case: number of bits are 0 or 1
    
    if (nBits == 0) return;
    
    if (nBits == 1) {
        for (string code : codes) {
            cout << code << endl;
        }
        return;
    }

    // Recursive case:
    //
    // build N length vector of Gray codes from N-1 length vector of codes by:
    //
    // 1. prepending "0" to N-1 codes
    // 2. prepending "1" to N-1 codes in reverse order.
    
    Vector<string> newCodes;
    for (int i = 0; i < codes.size(); i++) {
        newCodes += string("0") + codes[i];
    }
    for (int i = codes.size() - 1; i >= 0; i--) {
        newCodes += string("1") + codes[i];
    }
    generateGrayCodeRec(nBits - 1, newCodes);

}