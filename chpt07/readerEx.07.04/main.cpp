//
// main.cpp
//
// This program uses recursion to compute the greatest common divisor 
// of two numbers according to an algorithm from the Greek mathematician, 
// Euclid, of the third century BCE:
//
// • If x is evenly divisible by y, then y is the greatest common divisor.
//
// • Otherwise, the greatest common divisor of x and y is always equal to the
//   greatest common divisor of y and the remainder of x divided by y.
//
// For example:
//
//    CS106B Programming Abstractions in C++: Ex 7.04
//    Euclid's GCD Algorithm (Recursive Edition)
//
//    Enter first integer:  49
//    Enter second integer: 35
//    GCD = 7
//
//    Enter first integer:  32
//    Enter second integer: 33
//    GCD = 1
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 7, Exercise 4
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

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 7.04\n";
const std::string DETAIL = "Euclid's GCD Algorithm (Recursive Edition)";
const std::string BANNER = HEADER + DETAIL;

int gcd(int x, int y);
int gcdI(int x, int y);

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    while (true) {
        int x = getInteger("Enter first integer:  ");
        int y = getInteger("Enter second integer: ");
        
        std::cout << "GCD = " << gcd(x, y) << std::endl << std::endl;
    }
    return 0;
}

//
// Function: gcd
// Usage: divisor = gcd(12, 4);
// ----------------------------
// Returns the greatest common divisor of two integers with
// a recursive implementation of Euclid's GCD algorithm from
// Elements, Book VII, proposition 2.
//

int gcd(int x, int y) {
    
    if (y == 0) {
        error("Unable to compute GCD.");
    }
    int r = x % y;
    if (r == 0) return y;
    
    return gcd(y, r);
}

//
// Function: gcdI
// Usage: int gcd = gcdI(x, y);
// --------------------------------------
// Iterative version of Euclid's GCD algorithm.
//
// Returns the greatest common divisor between two integers.
//

int gcdI(int x, int y) {
    int r = x % y;
    
    while (r != 0) {
        x = y;
        y = r;
        r = x % y;
    }
    return y;
}
