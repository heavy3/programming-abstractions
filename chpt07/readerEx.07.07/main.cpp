//
// main.cpp
//
// This program implements a recursive function, digitSum(n), which takes a
// non-negative integer and returns the sum of its digits.
//
// For example,
//
//    digitSum(1729) returns 19 (which is 1 + 7 + 2 + 9)
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 7, Exercise 7
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

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 7.07\n";
const std::string DETAIL = "Recursive Sum of Digits";
const std::string BANNER = HEADER + DETAIL;

int digitSum(unsigned int n);

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    std::cout << "This program sums the digits of a non-negative integer.\n";
    
    while (true) {
        int n = getInteger("Enter non-negative integer: ");
        if (n < 0) break;
        std::cout << "Digit sum = " << digitSum(n) << std::endl;
    }
    return 0;
}

//
// Function: digitSum
// Usage int sum = digitSum(15); // digit sum = 6;
// -----------------------------------------------
// Returns the sum of the digits in a non-negative integer using recursion.
//
// This algorithm breaks an integer down into two components using
// division by 10.  For example:
//
//         1729
//         ----
//         /  \
//        /    \
//     172      9
//
// Then 172 can be further decomposed using the same technique until
// you're left with single digit numbers.  At that point, the decomposition
// bottoms out and you can begin to unwind the recursion to sum
// the individual digits.
//

int digitSum(unsigned int n) {
    if (n < 10) {
        return n;
    } else {
        return digitSum(n/10) + (n % 10);
    }
}
