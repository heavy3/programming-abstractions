//
// Combinations.h
//
// This interface exports a function for computing the combinations function,
//
//     c(n, k) = n! / (k! (n-k)!)
//
// from the domain of discrete mathematics.
//
// Relatedly, it provides a simple routine for displaying the results
// of c(n,k) for successively larger values of n and k in a form known as
// Pascal's Triangle.
//
// --------------------------------------------------------------------------
// Assignment: 1, Part 2 Combinations
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/050%20Assignment%201.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/15/2015.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#ifndef Combinations_h
#define Combinations_h

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

// Types and constants

enum ImplementationT {  // Specify which algorithmic approach is used.
    recursion,
    iteration
};

// Function prototyes

//
// Function: c
// Usage: unsigned ans = c(5, 2);  // Returns 10 using recursion.
//        unsigned ans = c(5, 2, iteration);   // Uses iteration.
// -----------------------------------------------------------------------
// Returns the number of combinations one can choose k things from a
// collection of n items without replacement.
//
// The order in which the items are chosen is insignificant by definition.
//
// An optional parameter is provided to select whether a recursive or iterative
// approach is used in the underlying implementation.
//
// Defaults to recursion.
//

unsigned c(unsigned nItems, unsigned chooseK,
                      ImplementationT impl = recursion);

//
// Function: showPascalsTriangle
// Usage: showPascalsTriangle(5);  // Displays first 5 rows to the console.
// --------------------------------------------------------------------------
// Displays the first n-rows of Pascal's Triangle to the console.
//
// Pascal's Triangle provides a visual representation of the combinations
// function for various values of n and k (starting with c(n=0, k=0) at the top
// with n increasing downward and k increasing from left to right).
//
// Limitations: Formatting is approximate and suitable for n <= 15.
//
// Typical output for first 5 rows:
//
//          Pascal's  Triangle
//          ------------------
//                   1
//                 1    1
//               1    2    1
//             1    3    3    1
//          1    4    6    4    1
//
// Notice the value of successive entries in the triangle is the sum of
// adjacent entries in the row above.
//

void showPascalsTriangle(unsigned rows);

//
// Function: cnkToString
// Usage: cout << cnkToString(n, k);         // outputs "c(n, k)"
//        cout << cnkToString(n, k, answer); // outputs "c(n, k) = answer";
// ------------------------------------------------------------------------
// Converts numeric values associated with c(n, k) to string form.
//
// In the case of c(n, k) = answer, an optional formatting width modifier
// may be passed in to improve visual alignment of numeric values in the output.
//

string cnkToString(unsigned n, unsigned k);
string cnkToString(unsigned n, unsigned k, unsigned answer, unsigned width = 4);

#endif // Combinations_h
