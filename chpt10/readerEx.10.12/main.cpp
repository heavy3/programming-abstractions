//
// main.cpp
//
// This program implements an algorithm for computing the nth Fibonacci
// number in O(log N) time by leveraging the discovery in 1718 of the
// French mathematician, Abraham de Moivre, that
//
//    fib(n) ≃ 𝜑ⁿ/√5 (rounded to the nearest integer)
//
// where 𝜑 = (1 + √5)/2, a root of the quadratic equation 𝜑² - 𝜑 - 1 = 0
//
// Notes
//
// Using the golden ratio to compose a closed-form equation for fib(n)
// is really a beautiful bit of math.
//
// I discovered a bug in my raiseToPower function, borrowed from
// exercise 10.01 (now fixed), that didn't surface until I tried testing at 
// scale (which, sadly, was only raiseToPower(6) :-/).  My bug was missing the
// mutual recursion relationship required between raiseToPower and
// raiseToEvenPower.  Another way to put it is I mistakenly assumed
// n/2 was always an even number.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 10, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 5/12/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include "error.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 10.12\n";
const std::string DETAIL = "Recursive O(logN) Fibonacci Numbers";
const std::string BANNER = HEADER + DETAIL;

const int NUM_FIBS = 10;                // Compute fib(N) for N = 1 to NUM_FIBS
const double PHI = (1.0 + sqrt(5.0))/2; // Golden ratio.

// Prototypes

int fib(int n);
double raiseToPower(double x, int n);
double raiseToEvenPower(double x, int n);
double raiseToPower_OrderN(double x, int n);

// Main program

int main() {
    cout << BANNER << endl << endl;

    for (int n = 1; n <= NUM_FIBS; n++) {
        cout << "fib(" << setw(2) << n << ") = " << setw(2) << fib(n) << endl;
    }
    return 0;
}

// Function: fib
// Usage: int fibNum = fib(n);
// ---------------------------
// Returns the nth fibonacci number.
//
// Leverages the work of the French mathematician, Abraham de Moivre, who
// discovered that the nth fibonacci number could be computed in closed form.
//
// Here's the simplifying approximation of de Moivre's work that we use:
//
//      fib(n) ≃ 𝜑ⁿ/√5 (rounded to the nearest integer)
//
// Given that we can raise a value to an integer power in O(logN) time, we
// can therefore compute fib(N) in O(logN) time since it is simply a value
// raised to a power divided by a constant.
//
// This is a significant improvement over the O(N) time required had we
// computed fib(n) iteratively.
//

int fib(int n) {
    return round(raiseToPower(PHI, n)/sqrt(5.0));
}

//
// Function: raiseToPower
// Usage: double ans = raiseToPower(25.0, 2);
// ------------------------------------------
// Returns a double raised to an integer power.
//
// An efficient recursive strategy is employed to limit the depth of the
// recursion to:
//
//  depth <= log2(n)
//
// by ensuring the inputs to a secondary function, raiseToEvenPower,
// always include an even value of n.
//

double raiseToPower(double x, int n) {
    double result;
    
    if ((n % 2) == 0) {
        result = raiseToEvenPower(x, n);
    } else {
        result = x * raiseToEvenPower(x, n-1);
    }
    return result;
}

//
// Function: raiseToEvenPower
// Usage: double ans = raiseToEvenPower(num, 4);
// ---------------------------------------------
// Returns a double that has been raised to an even power using a
// recursive divide-and-conquer stategy for limiting recursive depth
// to O(logN).
//
// Is is the responsibility of the caller to ensure the power is even.
// This can be done by using the wrapper function, raiseToPower in the
// general case.
//

double raiseToEvenPower(double x, int evenN) {
    if (evenN == 0) return 1.0;
    if (evenN == 1) return x;
    
    double halfPower = raiseToPower(x, evenN/2);
    return halfPower * halfPower;
}

//
// Function: raiseToPower_OrderN
// Usage: double ans = raiseToPower_OrderN(num, 4);
// ------------------------------------------------
// Uses an order-N recursive algorithm for computing the value of a double
// raised to an integer power.
//

double raiseToPower_OrderN(double x, int n) {
    if (n == 0) {
        return 1.0;
    }
    return x * raiseToPower_OrderN(x, n-1);
}