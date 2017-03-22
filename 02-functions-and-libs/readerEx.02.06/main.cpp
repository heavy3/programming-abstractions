//
// main.cpp
//
// This program implements a predicate function, isPrime(n), that returns 
// true if an integer n is prime and false otherwise.  As a test, the program
// lists the prime numbers in the range 1 to 100.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 6
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/18/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>

//
// Constants
//

const int LOWER_LIMIT = 1;      // Test for primes over this range of
const int UPPER_LIMIT = 100;    // integers.

// Function prototypes

bool isPrime(int num);
void testIsPrime(int low, int high);
int error(std::string msg);

// Main program

int main(int argc, char * argv[]) {
    
    testIsPrime(LOWER_LIMIT, UPPER_LIMIT);
    testIsPrime(-(UPPER_LIMIT), -(LOWER_LIMIT));
    return 0;
}

// Function definitions

//
// Function: isPrime
// Usage: bool b = isPrime(17);
// ----------------------------
// Returns true if a given integer is prime.
// Assumes test integer is positive; exits with a fail message otherwise.
//

bool isPrime(int num) {
    int divisorCount = 0;
    
    if (num < 0) {
        error("My lame version of isPrime requires positive integers.");
    }
    
    for (int i = 1; i <= num; i++) {
        if ((num % i) == 0) {
            divisorCount++;
        }
    }
    
    return (divisorCount == 2);
}

//
// Function: testIsPrime
// Usage: testIsPrime(low = 1, high = 100);
// ----------------------------------------
// Writes primes to console output stream, one to a line, over
// an input test range of intergers.
//

void testIsPrime(int low, int high) {
    std::cout << "Testing for primes over the range: [" << low
              << ", " << high << "]" << std::endl;
    for (int i = low; i <= high; i++) {
        if (isPrime(i)) {
            std::cout << std::setw(2) << i << std::endl;
        }
    }
}

//
// Function: error
// Usage: error("I don't support negative numbers.");
//

int error(std::string msg) {
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}