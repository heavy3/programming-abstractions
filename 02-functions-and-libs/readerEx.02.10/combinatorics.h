//
// combinatorics.h
//
// This file exports some functions related to discrete mathematics,
// namely:
//
//    combinations(n, k)
//    permutations(n, k)
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 10
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/19/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#ifndef _combinatorics_h
#define _combinatorics_h

#include <string>
#include <cstdlib>

//
// Function: error
// Usage: error("Goodbye, world.");
// ----------------------
// Returns control to the operating system with <code>EXIT_FAILURE</code>, 
// defined in <code><cstdlib></code>.
//

void error(std::string msg);

//
// Function: combinations
// Usage: int answer = combinations(n, k);
// --------------------------------------------------
// Returns the number of <code>combinations</code> associated with choosing n
// items k at a time, without regard to order, according to the standard 
// formula:<p>
// <code>C(n, k) = n! / (k! * (n - k)!)</code>
//

long combinations(unsigned n, unsigned k);

//
// Function: permutations
// Usage: int answer = permutations(n, k);
// --------------------------------------------------
// Returns the number of <code>permutations</code> associated with choosing n 
// items k at a time, with order of selection considered significant, according 
// to the standard formula:<p>
// <code>P(n, k) = n! / (n - k)!</code>
//

long permutations(unsigned n, unsigned k);

//
// Function: fact
// Usage: long l = fact(5);
// ------------------------
// Returns the factorial of an unsigned integer according to the formula:
// <code>fact(n) = n * (n-1) * ... * 3 * 2 * 1</code>
//
long fact(unsigned n);

#endif