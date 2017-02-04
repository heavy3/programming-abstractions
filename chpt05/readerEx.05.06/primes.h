//
// primes.h
//
// This file exports some functions relating to the Sieve of Eratosthenes
// algorithm for generating a list of prime numbers from 2 to N.
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

#ifndef _primes_h
#define _primes_h

#include <iostream>
#include "vector.h"
using namespace std;

// Constants and types

extern const int LOWEST_PRIME; /* Lower bound must be smallest prime, 2. */
extern const string E_INVALID_RANGE; /* Lower bound exceeds upper bound. */

//
// Function: printPrimes
// Usage: printPrimes(primes);
// ---------------------------
// Print a vector of prime numbers to the console.
//
// Example:
//
//     Primes
//     {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59}
//

void printPrimes(Vector<int> & primes);

//
// Function: reportSievePrimes
// Usage: reportSievePrimes(upperBound, primes);
// ----------------------------------------------------------
// Reports the results of running the sievePrimes function against a vector
// of integers.
//
// Example:
//
//     168 primes found over the range (2, 1000).
//

void reportSievePrimes(int upperBound, Vector<int> & primes);

//
// Function: sievePrimes
// Usage: if (sievePrimes(upperBound, primes)) ...
//        Vector<int> primes = sievePrimes(1000);
// ------------------------------------------------------------
// Populates a vector with prime numbers from 2 to the upper bound specified.
//
// The two usage patters are:
//
// 1. Returns true if input parameters are valid; resultant primes are
//    populated in a pass-by-reference vector.
//
// 2. Returns a vector of primes.  Invalid input parameters
//    (e.g., lowest prime > upperBound) trigger a call to error and
//    processing halts.
//

bool sievePrimes(int upperBound, Vector<int> & primes);
Vector<int> sievePrimes(int upperBound);

#endif
