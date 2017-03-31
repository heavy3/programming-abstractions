//
// main.cpp
//
// Generalize the plot function to a library with interfaces for function
// pointers and function objects.  Use it to render some growth curves
// that reflect common big-O complexity classes.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 02
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/31/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include "error.h"
#include "plot.h"
#include "map.h"
using namespace std;

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 20.02\n";
const std::string DETAIL = "Iterators: Plot Complexity Curves";
const std::string BANNER = HEADER + DETAIL;
const std::string TITLE = "Plot Complexity Classes";
const double MIN_X = 1;
const double MAX_X = 15;
const double MIN_Y = 0;
const double MAX_Y = 50;
const double CONSTANT_Y = 0.5;

typedef double(*FunctionPtr)(double);
typedef std::string ColorStr;

// Function prototypes

void multiPlot(const Map<FunctionPtr,ColorStr>& map,
               double minX,
               double minY,
               double maxX,
               double maxY,
               string title);

double constant(double x);
double linear(double x);
double nlogn(double x);
double quadratic(double x);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    cout << "Legend" << endl;
    cout << string(50, '-') << endl;
    cout << setw(8) << "Color"   << setw(20) << "Growth"      << endl;
    cout << string(50, '-') << endl;
    cout << setw(8) << "black"   << setw(20) << "constant"    << endl;
    cout << setw(8) << "red"     << setw(20) << "log"         << endl;
    cout << setw(8) << "green"   << setw(20) << "linear"      << endl;
    cout << setw(8) << "cyan"    << setw(20) << "nlogn"       << endl;
    cout << setw(8) << "blue"    << setw(20) << "quadratic"   << endl;
    cout << setw(8) << "magenta" << setw(20) << "exponential" << endl;
    
    Map<FunctionPtr, ColorStr> growthCurves;
    
    growthCurves[constant] = "BLACK";
    growthCurves[log] = "RED";
    growthCurves[linear] = "GREEN";
    growthCurves[nlogn] = "CYAN";
    growthCurves[quadratic] = "BLUE";
    growthCurves[exp2] = "MAGENTA";
    
    multiPlot(growthCurves, MIN_X, MAX_X, MIN_Y, MAX_Y, TITLE);
    
    return 0;
}

// Function: multiPlot
// Usage: multiPlot(curveMap, minX, maxX, minY, maxY, "Title");
// ------------------------------------------------------------
// This function plots a collection of curves in a graphics window,
// scaling the desired domain and range to the physical dimensions of
// the window.
//
// The curves are function pointers to the analytic methods for computing
// a y-value for a given x-value.  The curves are also keys that
// map to corresponding color strings used in rendering the graphs.

void multiPlot(const Map<FunctionPtr,ColorStr>& map,
               double minX,
               double maxX,
               double minY,
               double maxY,
               string title) {
    GWindow gw;
    gw.setWindowTitle(TITLE);
    
    for (FunctionPtr fn: map) {
        string color = map[fn];
        gw.setColor(color);
        plot(gw, fn, minX, maxX, minY, maxY);
    }
}

double constant(double x) {
    double y;
    return y = CONSTANT_Y;
}

double linear(double x) {
    double y;
    return y = x;
}

double nlogn(double x) {
    double y;
    return y = x * log(x);
}

double quadratic(double x) {
    double y;
    return y = pow(x, 2);
}
