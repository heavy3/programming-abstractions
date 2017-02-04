//
// primes.cpp
//
// This file implements the interface for primes.h, an API for generating
// a list of primes using the Sieve of Eratosthenese algorithm.
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

// Constants and types

extern const int LOWEST_PRIME = 2;
extern const string E_INVALID_RANGE =
    "sievePrimes: Invalid range. lowest prime > upper bound.";

// Private function prototypes

void initSieve(Vector<int> & candidates, int upperBound);
bool ismultiple(int x, int y);

// Function definitions

//
// Implementation notes: printPrimes
// ---------------------------------
// Dumps the string version of a vector to the output console.
//

void printPrimes(Vector<int> & primes) {
    cout << primes.toString() << endl << endl;
}

//
// Implementation notes: reportSievePrimes
// ---------------------------------------
// Dumps a summary of the primes found for the specified range to the 
// output console.
//

void reportSievePrimes(int upperBound, Vector<int> & primes) {
    cout << primes.size() << " primes found over the range ("
         << LOWEST_PRIME << ", " << upperBound << ")."
         << endl << endl;
}

//
// Implementation notes: sievePrimes
// ---------------------------------
// The outside loop iterates over a shrinking vector of prime number candidates
// from smallest element to largest and is responsible for handing the
// inside loop the next (incrementally larger) prime.
//
// The inside loop is responsible for shrinking the list of candidate primes
// by removing all multiples of a given prime number from the vector but 
// iterating in the opposite direction from largest candidate to smallest.
//
// This way, the outside iterator always has a valid next index, advancing
// through ever-increasing prime numbers at the low end of the list while
// providing inputs to the inside iterator's prime-multiple removal algorithm
// which shrinks the balance of the list until only prime numbers remain.
//
// If the LOWEST_PRIME happens to exceed the specified upper bound,
// false is returned, since the algorithm will fail otherwise.
//

bool sievePrimes(int upperBound, Vector<int> & primes) {
    
    if (LOWEST_PRIME > upperBound) return false;
    
    initSieve(primes, upperBound);
    #ifdef DEBUGGING
    printPrimes(primes);
    #endif
    for (int largestKnownPrime: primes) {
        int last = primes.size() - 1;
        for (int i = last; primes[i] > largestKnownPrime; i--) {
            int N = primes[i];
            if (ismultiple(N, largestKnownPrime)) {
                primes.remove(i);
            }
        }
    }
    return true;
}

// 
// Implementation notes: sievePrimes
// ---------------------------------
// Overloads the sievePrimes function, returning a vector of primes instead
// of a pass-by-reference vector in the parameter list as described above.
//
// Control is passed to error() if the lower bound exceeds the upper bound.
//
// Calls the lower-level primitive of the same name to populate the prime
// vector.
//

Vector<int> sievePrimes(int upperBound) {
    Vector<int> primes;
    
    if (!sievePrimes(upperBound, primes)) {
        error(E_INVALID_RANGE);
    }
    return primes;
}

// ----------------------------
// Private function definitions 
// ----------------------------

//
// Function: ismultiple
// Usage: if (ismultiple(10, 2)) { ... }
// -------------------------------------
// Returns true if the first number is a multiple of the second.
//

bool ismultiple(int N, int n) {
    return (N < n) ? false : (N % n == 0);
}

//
// Function: initSieve
// Usage: initSieve(intVector, int upperBound);
// --------------------------------------------
// Populates a pass-by-reference vector with integers over a closed
// interval:
//
//    2 <= val <= upperBound
//
// as a starting point for the sieve of Eratosthenes algorithm.
//

void initSieve(Vector<int> & primeCandidates, int upperBound) {
    primeCandidates.clear();
    for (int i = LOWEST_PRIME; i <= upperBound; i++) {
        primeCandidates.add(i);
    }
}
