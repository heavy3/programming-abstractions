//
// main.cpp
//
// This program implements a function for rounding floating point numbers
// to integers.  A test driver is also included.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/17/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>

// Function prototypes

int roundToNearestInt(double number);
void testRoundToNearestInt(double number, int expectedAnswer);

// Main program

int main(int argc, char * argv[]) {
    
    testRoundToNearestInt(4.999, 5);
    testRoundToNearestInt(-4.999, -5);
    testRoundToNearestInt(1.5, 2);
    testRoundToNearestInt(-1.5, -2);
    testRoundToNearestInt(1.1, 1);
    testRoundToNearestInt(-1.1, -1);

    return 0;
}

// Function definitions

//
// Function: roundToNearestInt
// Usage: int num = roundToNearestInt(3.14159);
// --------------------------------------------
// Rounds a floating point number fo its nearest integer.
//

int roundToNearestInt(double number) {

    // Because truncation always moves toward zero, rounding negative 
    // numbers requires you to subtract 0.5, rather than adding it.
    //
    // For an interesting discussion, see:
    // http://mathforum.org/library/drmath/view/71202.html

    return (number < 0) ? (int(number - 0.5)) : (int(number + 0.5));
}

//
// Function: testRoundToNearestInt
// Usage: testRoundToNearestInt(3.14159, 3);
// -----------------------------------------
// Compares the expected rounded result with the actual output
// from roundToNearestInt() and reports PASS or FAIL to
// the standard output console stream, cout.
//

void testRoundToNearestInt(double number, int expectedAnswer) {
    int nearestInt = roundToNearestInt(number);
    (nearestInt == expectedAnswer) ? std::cout << "[PASS] " :
                                     std::cout << "[FAIL] " ;
    std::cout << number << " rounded to " << nearestInt << std::endl;
}
