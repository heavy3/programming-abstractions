//
// main.cpp
//
// This program finds the roots of the quadratic equation:
//
//    ax² + bx + c = 0
//
// If a is 0 or if the equation has no real roots, the program prints an
// error message and exits.
//
// The implementation is a reimplementation of the quadratic program in
// Figure 2-3, but using explicit pointers instead of call-by-reference idioms
// to return values from the functions getCoefficients and solveQuadratic.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 11, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code is largely derived from Figure 2-3.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 6/16/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "error.h"
using namespace std;

// Function Prototypes

void getCoefficients(double * pA, double * pB, double * pC);
void solveQuadratic(double a, double b, double c, double * pX1, double * pX2);
void printRoots(double x1, double x2);
void error(string msg);

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 11.05\n";
const std::string DETAIL = "Refactor Quadratic Solver to use pointers: Fig 2-3";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    double a, b, c, r1, r2;
    getCoefficients(&a, &b, &c);
    solveQuadratic(a, b, c, &r1, &r2);
    printRoots(r1, r2);
    return 0;
}

// Function: getCoefficients
// Usage: getCoefficients(a, b, c);
// --------------------------------
// Reads in the coefficients of a quadratic equation into the reference
// parameters a, b, and c.

void getCoefficients(double * pA, double * pB, double * pC) {
    cout << "Enter coefficients for the quadratic equation:" << endl;
    cout << "a: ";
    cin >> *pA;
    cout << "b: ";
    cin >> *pB;
    cout << "c: ";
    cin >> *pC;
}

// Function: solveQuadratics
// Usage: solveQuadratics(a, b, c, x1, x2);
// ----------------------------------------
// Solves a quadratic equation for the coefficients a, b, c.  The roots
// are returned in the reference paramters x1 and x2.

void solveQuadratic(double a, double b, double c, double * pX1, double * pX2) {
    if (a == 0) error("The coefficient a must be nonzero.");
    double disc = b * b - 4 * a * c;
    if (disc < 0) error("This equation has no real roots.");
    double sqrtDisc = sqrt(disc);
    *pX1 = (-b + sqrtDisc)/ (2*a);
    *pX2 = (-b - sqrtDisc)/ (2*a);
}

// Function: printRoots
// Usage: printRoots(x1, x2);
// --------------------------
// Display x1 and x2, which are the roots of the quadratic equation.

void printRoots(double x1, double x2) {
    if (x1 == x2) {
        cout << "There is a double root at " << x1 << endl;
    } else {
        cout << "The roots are " << x1 << " and " << x2 << endl;
    }
}