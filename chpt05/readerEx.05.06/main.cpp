//
// main.cpp
//
// This program uses the Sieve of Eratosthenes to generate a list of the 
// primes between 2 and 1000.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 6
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/27/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include "primes.h"

// Function prototypes

void banner(int upperBound);

// Main program

int main(int argc, const char * argv[]) {

    Vector<int> primes;
    int upperBound = 1000;
    banner(upperBound);

    cout << endl << "Exercising pass-by-reference function ..." << endl;
    
    if (sievePrimes(upperBound, primes)) {
        reportSievePrimes(upperBound, primes);
        printPrimes(primes);
    } else {
        cerr << E_INVALID_RANGE;
    }
    
    cout << endl << "Exercising vector copy function ..." << endl;
    
    primes.clear();
    primes = sievePrimes(upperBound);
    reportSievePrimes(upperBound, primes);
    printPrimes(primes);

    return 0;
}

// Function definitions

//
// Function: banner
// Usage: banner(upperBound)
// -------------------------
// Function displays the purpose of the program on the console.
//

void banner(int upperBound) {
    cout << "This program uses the sieve of Eratosthenes to generate "
         << "a list of primes from " << LOWEST_PRIME << " to " << upperBound << "."
         << endl << endl;
}