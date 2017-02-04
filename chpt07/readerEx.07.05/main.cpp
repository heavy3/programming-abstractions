//
// main.cpp
//
// This program implements an iterative Fibonacci number generator.
// Fibonacci numbers are defined by the recurrence relation:
//
//    fib(n) = n                    if n < 2
//           = fib(n-1) + fib(n-2)  otherwise
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 7, Exercise 5
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

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 7.05\n";
const std::string DETAIL = "Fibonacci Number Generator (Iterative)";
const std::string BANNER = HEADER + DETAIL;

int fib(unsigned int n);

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    while (true) {
        int n = getInteger("Enter n: ");
        std::cout << "fib(" << n << ") = " << fib(n) << std::endl;
    }
    return 0;
}

//
// Function: fib
// Usage: int num = fib(7);  // num = 13
// -------------------------------------
// This implements an iterative Fibonacci number generator.
// Fibonacci numbers are defined by the recurrence relation:
//
// fib(n) = n                    if n < 2
//        = fib(n-1) + fib(n-2)  otherwise
//

int fib(unsigned int n) {
    if (n < 2) {
        return n;
    } else {
        int f1 = 0;
        int f2 = 1;
        int fn;
        
        // Iterate n-1 times.
        
        for (int i = 1; i < n; i++) {
            fn = f1 + f2;
            f1 = f2;
            f2 = fn;
        }
        return fn;
    }
}
