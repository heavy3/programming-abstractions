//
// main.cpp
//
// This program compares the efficiency of computing the Fibonacci
// sequence by two different methods.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 7, Exercise 6
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

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 7.06\n";
const std::string DETAIL = "Fibonacci Alg Comparison";
const std::string BANNER = HEADER + DETAIL;
const int MIN_INDEX = 0;  /* Index of first term to generate. */
const int MAX_INDEX = 12; /* Index of last term to generate.  */

int countFib1(int n, int & count);
int countFib2(int n, int & count);
int additiveSequence(int n, int t0, int t1, int & count);

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    std::cout << "This program compares the number of calls made by two\n";
    std::cout << "algorithms for computing the Fibonacci sequence.\n\n";
    
    std::cout << std::right   << std::setw(3) << "n";
    std::cout << std::setw(7) << "fib1" << std::setw(7) << "fib2" << std::endl;
    
    std::cout << std::right   << std::setw(3) << "--";
    std::cout << std::setw(7)  << "----" << std::setw(7) << "----" << std::endl;
    
    
    for (int i = MIN_INDEX; i <= MAX_INDEX; i++) {
        std::cout << std::right << std::setw(3) << i;
        int c1 = 0;
        int c2 = 0;
        countFib1(i, c1);
        countFib2(i, c2);
        std::cout << std::setw(7) << c1 << std::setw(7) << c2 << std::endl;
    }
    return 0;
}

//
// Function: countFib1
// Usage: int c = countFib1(n, int & count);
// -----------------------------------------
// Returns a count of recursive invocations required by the following
// algorithm to arrive at a solution:
//
//     fib(0) = 0
//     fib(1) = 1
//     fib(n) = fib(n - 1) + fib(n - 2)
//

int countFib1(int n, int & count) {
    count++;
    
    if (n < 2) {
        return n;
    } else {
        return countFib1(n - 1, count) + countFib1(n - 2, count);
    }
}

int countFib2(int n, int & count) {
    return additiveSequence(n, 0, 1, count);
}

//
// Function: additiveSequence
// Usage: int val = additiveSequence(n, t0, t1, count);
// ----------------------------------------------------
// Returns the nth value corresponding to the additive sequence given values
// for t0 and t1, the first two values in the sequence.
//

int additiveSequence(int n, int t0, int t1, int & count) {
    count++;
    
    if (n == 0) return t0;
    if (n == 1) return t1;
    return additiveSequence(n - 1, t1, t0 + t1, count);
}
