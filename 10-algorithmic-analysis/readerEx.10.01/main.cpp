//
// main.cpp
//
// This program implements an algorithm for raising a number
// to an integer power that runs in O(log N) time.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 10, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/27/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "error.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 10.01\n";
const std::string DETAIL = "Recursive O(logN) Exponent Implementation";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

double raiseToPower(double x, int n);
double raiseToEvenPower(double x, int n);
double raiseToPower_OrderN(double x, int n);

// Main program

int main() {
    cout << BANNER << endl << endl;

    int power = 16;
    double num = 2.0;
    
    cout << "O(logN) exponent: " << num << "^" << power << " ";
    cout << raiseToPower(num, power) << endl;
    
    cout << "O(N)    exponent: " << num << "^" << power << " ";
    cout << raiseToPower_OrderN(num, power) << endl;
  
    return 0;
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
//      depth <= log2(n)
//
// by ensuring the inputs to a secondary function, raiseToEvenPower,
// always include an even value of n.
//
//

double raiseToPower(double x, int n) {
    double result;
    
    if ((n % 2) == 0)
        result = raiseToEvenPower(x, n);
    else
        result = x * raiseToEvenPower(x, n-1);
    
    cout << " ";
    return result;
}

//
// Function: raiseToEvenPower
// Usage: double ans = raiseToEvenPower(num, 4);
// ---------------------------------------------
// Returns a double that has been raised to an even power using a
// recursive divide-and-conquer stategy for limiting recursive depth
// to O(log N).
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
// Uses an order N recursive algorithm for computing the value of a double
// raised to an integer power.
//

double raiseToPower_OrderN(double x, int n) {
    if (n == 0) {
        cout << " ";
        return 1.0;
    }
    
    cout << "*" ;
    return x * raiseToPower_OrderN(x, n-1);
}