//
// main.cpp
//
// This program implements a recursive function, digitalRoot(n), which
// returns the digital root of its argument.
//
// The digital root is computed by repeatedly summing the digits of
// a given number until you have a single digit.
//
// For example, the digital root of 1729 is 1:
//
//    1729 -> 1 + 7 + 2 + 9 -> 19 -> 1 + 9 > 10 -> 1 + 0 -> 1
//
// Typical interaction with the program:
//
//    CS106B Programming Abstractions in C++: Ex 7.08
//    Digital Root
//
//    This program sums the digits of a non-negative integer.
//    Enter non-negative integer: 99
//    Digital root: 9
//    Enter non-negative integer: 10
//    Digital root: 1
//    Enter non-negative integer: 111
//    Digital root: 3
//    Enter non-negative integer: 1729
//    Digital root: 1
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 7, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/28/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "simpio.h"
#include "error.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 7.08\n";
const std::string DETAIL = "Digital Root";
const std::string BANNER = HEADER + DETAIL;

int digitalRoot(int n);

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    std::cout << "This program sums the digits of a non-negative integer.\n";
    
    while (true) {
        int n = getInteger("Enter non-negative integer: ");
        if (n < 0) break;
        std::cout << "Digital root: " << digitalRoot(n) << std::endl;
    }
    return 0;
}

//
// Function: digitalRoot
// Usage: int r = digitalRoot(99); // r = 9 + 9 = 18 = 1 + 8 = [ 9 ]
// -----------------------------------------------------------------
// Returns the digital root of a non-negative integer using a recursive
// strategy.
//

int digitalRoot(int n) {
    if (n < 10) {
        return n;
    } else {
        int ones = n % 10;
        return digitalRoot(digitalRoot(n/10) + ones);
    }
}
