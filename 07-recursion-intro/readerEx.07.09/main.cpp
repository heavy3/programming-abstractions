//
// main.cpp
//
// This program implements a recursive algorithm for computing
// the combinations formula from statistics:
//
//    c(n, k) =        n!
//               -------------
//               k! * (n - k)!
//
// For example:
//
//    CS106B Programming Abstractions in C++: Ex 7.09
//    Recursive Combinations, c(n, k)
//
//    This program sums the digits of a non-negative integer.
//    Enter n for c(n, k): 6
//    Enter k for c(n, k): 2
//    c(6, 2) = 15
//    Enter n for c(n, k): 7
//    Enter k for c(n, k): 3
//    c(7, 3) = 35
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 7, Exercise 9
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

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 7.09\n";
const std::string DETAIL = "Recursive Combinations, c(n, k)";
const std::string BANNER = HEADER + DETAIL;

int c(int n, int k);

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    std::cout << "Recursive computing of the comination function c(n, k).\n";
    
    while (true) {
        int n = getInteger("Enter n for c(n, k): ");
        int k = getInteger("Enter k for c(n, k): ");
        if (n < 0) break;
        std::cout << "c(" << n << ", " << k << ") = " << c(n, k) << std::endl;
    }
    return 0;
}

//
// Function: c
// Usage: int nChoosek = c(n, k);
// ------------------------------
// Returns the number of combinations of choosing n items, k at a time
// using just recursion and addition (no looping or multiplication).
//
// Uses the recurrence relations:
//
//      c(n, k) = c(n-1, k-1) + c(n-1, k)   if n > 2
//              = 1                         otherwise
//
// The algorithm leverages the fact that the value of an 
// interior entry in Pascal's triangle is the sum of the two 
// digits above:
//
//      c(6, 2) = 5 + 10 = 15
//
//                     1
//                 .  .  .
//         1     5    10   10   5     1
//      1     6    15   20   15    6    1
//                 --

int c(int n, int k) {
    if (n < 2 || k == 0 || k == n) {
        return 1;
    }
    int rowAbove = n - 1;
    int colLeft  = k - 1;
    int colRight = k;
    
    return c(rowAbove, colLeft) + c(rowAbove, colRight);
}
