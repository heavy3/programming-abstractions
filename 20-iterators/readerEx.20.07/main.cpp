//
// main.cpp
//
// Numerically integrate a continuous, single-valued function over an interval
// along the x-axis by subdividing the region under the curve into a given
// number of rectangles whose height corresponds to the value of the function
// at the midpoint of the rectangle.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 07
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/3/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include "map.h"
#include "error.h"

using namespace std;

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 20.07\n";
const std::string DETAIL = "Integrate f(x) over an x-interval using rectangles";
const std::string BANNER = HEADER + DETAIL;
const std::string PROMPT = "=> ";
const double PI = M_PI;

typedef double (*doubleFn)(double);
typedef Map<std::string, doubleFn> FnMap;

// Function prototypes

double integrate(doubleFn fn, double minX, double maxX, int nRect);
double constantFn(double x);
double linearFn(double x);
void testDriver(const FnMap& testFunctions, double minX, double maxX);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    // Test integrate() against a few different continuous functions.
    
    FnMap testFunctions;
    testFunctions["1"] = constantFn;
    testFunctions["x"] = linearFn;
    testFunctions["sin(x)"] = sin;
    
    double minX = 0;
    double maxX = PI;
    testDriver(testFunctions, minX, maxX);
    
    return 0;
}

void testDriver(const FnMap& testFunctions, double minX, double maxX) {
    cout << "The following functions are numerically integrated" << endl;
    cout << "over the domain x = [0, pi], subdividing the region" << endl;
    cout << "under the curve with varying numbers of rectangles." << endl;
    cout << endl;
    
    cout << "nRect   f(x)   ⎰f(x)" << endl;
    for (std::string fnDesc: testFunctions) {
        cout << string(25, '-') << endl;
        doubleFn fn = testFunctions[fnDesc];
        for (int nRect = 20; nRect <= 100; nRect += 20) {
            
            double value = integrate(fn, minX, maxX, nRect);
            
            cout << setw(3) << right << nRect  << "   ";
            cout << setw(6) << right << fnDesc << "   ";
            cout << value   << endl;
        }
    }
}

// Function: integrate
// Usage: area = integrate(sin, 0, PI, 20);
// ----------------------------------------
// Returns an approximation to the area under a curve for a given function
// over a specified domain and subdivision factor (number of rectangles).
//
// Approximates the area under a curve by the summation of one or more
// rectangles of fixed width and variable height.  The height corresponds
// to the value of the fn evaluated at the midpoint of the rectangle.

double integrate(doubleFn fn, double minX, double maxX, int nRect) {
    if (nRect == 0) error("integrate: Division by 0. Need non-zero nRect.");
    double result = 0;
    double dx = abs(maxX - minX)/double(nRect);
    for (int i = 0; i < nRect; i++) {
        double xMid = i * dx + dx/2.0;
        double rectHeight = fn(xMid);
        double rectArea = dx * rectHeight;
        result += rectArea;
    }
    return result;
}

double constantFn(double x) {
    return 1;
}

double linearFn(double x) {
    return x;
}
