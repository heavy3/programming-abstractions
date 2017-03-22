//
// main.cpp
//
// This program recursively calculates n^k (n raised to the kth power).
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 7, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/27/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. 
//

#include <iostream>
#include "simpio.h"
#include "strlib.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 7.02\n";
const std::string DETAIL = "Compute n^k (n raised to the k power)";
const std::string BANNER = HEADER + DETAIL;

int raiseToPower(int n, int k);

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    while (true) {
        int n = getInteger("Enter n: ");
        int k = getInteger("Enter k: ");
        std::cout << n << "^" << k << " = " << raiseToPower(n, k);
        std::cout << std::endl << std::endl;
    }
    return 0;
}

//
// Function: raiseToPower
// ----------------------
// This function returns the value of an integer raised to a given
// exponent.
//

int raiseToPower(int n, int k) {
    if (k == 0) return 1;
    if (k == 1) return n;
    
    return n * raiseToPower(n, k-1);
}
