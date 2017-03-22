//
// combinatorics.cpp
//
// This file implements some functions related to discrete mathematics,
// namely:
//
//    combinations(n, k)
//    permutations(n, k)
//
// TODO: Split the test driver out into its own main.cpp file.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 10
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
#include "combinatorics.h"

#define _TEST_COMBINATORICS  // Define this to test entrypoint via main().

// Function definitions

//
// Implementation notes: error
// ---------------------------
// This function writes out the error message to the cerr stream and then exits
// the program.  The EXIT_FAILURE constant is defined in <cstdlib> to
// represent a standard failure code.
//

void error(std::string msg) {
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}

//
// Implementation notes: permutations
// ----------------------------------
// Returns the number of permutations for choosing n items k at a time according 
// to this formula:
//
//      P(n, k) = n! / (n - k)!
//
// Implementation is optimized (and overflow avoided) by using the denominator 
// as a guide to factoring a clever form of one.
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
// Implementation notes: combinations
// ----------------------------------
// Returns the number of combinations for choosing n items k at a time according
// to this formula:
//
//      C(n, k) = n! / (k! * (n - k)!)
//
// Implementation is optimized (and overflow avoided) by using the denominator
// as a guide to factoring a clever form of one.
//

long combinations(unsigned nItems, unsigned chooseK) {
    long numeratorProduct = 1;
    long denominatorProduct;
    
    //
    // Given that C(6, 2) = 6 * 5 * 4 * 3 * 2 * 1 / (2 * 1) * (4 * 3 * 2 * 1)
    //
    // we can avoid evaluation of the denominator and corresponding
    // portions of the numerator by factoring out a clever form of
    // one, leaving just a partial factorial in the numerator to evaluate:
    //
    // P(6, 2) = (4 * 3 * 2 * 1) * (6 * 5) = 6 * 5 = 15
    //           -------------------------   -----
    //           (4 * 3 * 2 * 1) * (2 * 1)     2
    //
    
    for (int i = nItems; i > (nItems - chooseK); i--) {
        numeratorProduct *= i;
    }
    denominatorProduct = (numeratorProduct == 1) ? 1 : fact(chooseK);
    
    return numeratorProduct / denominatorProduct;
}

//
// Implementation notes: fact
// --------------------------
// Returns the factorial of an unsigned integer.
//

long fact(unsigned n) {
    long product = 1;
    
    for (int i = 1; i <= n; i++) {
        product *= i;
    }
    
    return product;
}

#ifdef _TEST_COMBINATORICS

//
// Implementation notes: testPermutations
// --------------------------------------
// Provides a simple test driver for permutations(n, k) that
// writes pass/fail message to cout if the expected answer
// matches the actual answer.
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

//
// Implementation notes: testCombinations
// --------------------------------------
// Provides a simple test driver for combinations(n, k) that
// writes pass/fail message to cout if the expected answer
// matches the actual answer.
//

bool testCombinations(unsigned n, unsigned k, long expectedAnswer) {
    long actualAnswer = combinations(n, k);
    if (actualAnswer == expectedAnswer ) {
        std::cout << "[PASS] C(" << n << ", " << k << ") = " << expectedAnswer
        << std::endl;
        return true;
    } else {
        std::cout << "[FAIL] C(" << n << ", " << k << ") = " << actualAnswer
        << " (expecting " << expectedAnswer << ")" << std::endl;
        return false;
    }
}

// Main program

int main(int argc, char * argv[]) {
    
    unsigned nItems = 6;
    unsigned chooseK = 2;
    long expectedAnswer = 30;
    
    testPermutations(nItems, chooseK, expectedAnswer);
    
    nItems = 6;
    chooseK = 2;
    expectedAnswer = 15;
    testCombinations(nItems, chooseK, expectedAnswer);
    
    nItems = 6;
    chooseK = 0;
    expectedAnswer = 1;
    testPermutations(nItems, chooseK, expectedAnswer);
    
    nItems = 6;
    chooseK = 0;
    expectedAnswer = 1;
    testCombinations(nItems, chooseK, expectedAnswer);
    
    nItems = 0;
    chooseK = 2;
    expectedAnswer = 1;
    testPermutations(nItems, chooseK, expectedAnswer);
    
    nItems = 0;
    chooseK = 2;
    expectedAnswer = 1;
    testCombinations(nItems, chooseK, expectedAnswer);
    
    return 0;
}

#endif
