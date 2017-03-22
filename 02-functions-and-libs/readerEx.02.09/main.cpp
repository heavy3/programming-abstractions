//
// main.cpp
//
// This program implements the classic permutation function from
// statistics:
//
//    P(n, k) = n! / (n - k)!
//
// in such a way as to avoid overflow for potentially large factorials.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/19/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdlib>

// Function prototypes

void error(std::string msg);
long permutations(unsigned n, unsigned k);
bool testPermutations(unsigned n, unsigned k, long expectedAnswer);

// Main program

int main(int argc, char * argv[]) {
    
    unsigned nItems = 6;
    unsigned chooseK = 2;
    long expectedAnswer = 30;
    testPermutations(nItems, chooseK, expectedAnswer);
    
    nItems = 6;
    chooseK = 0;
    expectedAnswer = 1;
    testPermutations(nItems, chooseK, expectedAnswer);
    
    nItems = 0;
    chooseK = 2;
    expectedAnswer = 1;
    testPermutations(nItems, chooseK, expectedAnswer);
    
    return 0;
}

// Function definitions

//
// Function: error
// Usage: error("Goodbye, bitter sweet existence.");
// ----------------------
// Returns control to the operating system with EXIT_FAILURE,
// defined in <cstdlib>
//

void error(std::string msg) {
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}

//
// Function: permutations
// Usage: int answer = permutations(nItems, chooseK);
// --------------------------------------------------
// This functions returns the number of permutations for
// for choosing n items k at a time with choice order
// considered significant as given in this formula:
//
//      P(n, k) = n! / (n - k)!
//
// Implementation is optimized (and overflow avoided)
// by using the denominator as a guide to factoring out
// a clever form of one.
//

long permutations(unsigned nItems, unsigned chooseK) {
    long numeratorProduct = 1;
    
    //
    // Given that P(6, 2) = 6 * 5 * 4 * 3 * 2 * 1 / 4 * 3 * 2 * 1
    //
    // we can avoid evaluation of the denominator and corresponding
    // portions of the numerator by factoring out a clever form of
    // one, leaving just a partial factorial in the numerator to evaluate:
    //
    // P(6, 2) = (4 * 3 * 2 * 1) * (6 * 5) = 6 * 5 = 30
    //           -------------------------   -----
    //           (4 * 3 * 2 * 1) *    1        1
    //
    
    for (int i = nItems; i > (nItems - chooseK); i--) {
        numeratorProduct *= i;
    }
    
    return numeratorProduct;
}

//
// Function: testPermutations
// Usage: (testPermutations(6, 2, 30)) ? std::cout << "pass" : std::cout "fail";
// -----------------------------------------------------------------------------
// Tests the permutation primitive by comparing the actual answer against
// an expected answer passed in as the final argument.
//

bool testPermutations(unsigned n, unsigned k, long expectedAnswer) {
    long actualAnswer = permutations(n, k);
    if (actualAnswer == expectedAnswer ) {
        std::cout << "[PASS] P(" << n << ", " << k << ") = " << expectedAnswer
                  << std::endl;
        return true;
    } else {
        std::cout << "[FAIL] P(" << n << ", " << k << ") = " << actualAnswer
                  << " (expecting " << expectedAnswer << ")" << std::endl;
        return false;
    }
}