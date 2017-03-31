//
// main.cpp
//
// Add a simpler interface for the plot() function which computes the
// min and max Y values based upon the known min and max X values and the
// function itself.
//
// For simple renderings of a single function over an X interval, there's
// some virtue in not having to think about the Y limits provided the
// function is defined over the entire X interval.
//
// However, you'll encounter grief when trying to render constant value
// functions since the delta in Y for min and max values of X will be 0,
// by definition.  This eliminates the ability to render anything interesting
// in this case.
//
// This approach also means that comparative plots of different functions
// in the same graphics window will be misleading since the vertical scale will
// mean different things for different functions over the same X interval.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 03
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

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 20.03\n";
const std::string DETAIL = "Iterators: Simpler plot Prototype";
const std::string BANNER = HEADER + DETAIL;
const std::string TITLE = "Plot Complexity Classes";
const double MIN_X = 1;
const double MAX_X = 8;
const double CONSTANT_Y = 10;


typedef std::string ColorStr;

// Function prototypes

void multiPlot(const Map<doubleFn, ColorStr>& map,
               double minX,
               double maxX,
               string title);

double constant(double x);
double linear(double x);
double nlogn(double x);
double quadratic(double x);
void plot(GWindow& gw,
          double (*fn)(double),
          double minX,
          double maxX);

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
    
    Map<doubleFn, ColorStr> growthCurves;
    
    growthCurves[constant] = "BLACK";
    growthCurves[log] = "RED";
    growthCurves[linear] = "GREEN";
    growthCurves[nlogn] = "CYAN";
    growthCurves[quadratic] = "BLUE";
    growthCurves[exp2] = "MAGENTA";
    
    multiPlot(growthCurves, MIN_X, MAX_X, TITLE);
    
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

void multiPlot(const Map<doubleFn,ColorStr>& map,
               double minX,
               double maxX,
               string title) {
    
    for (doubleFn fn: map) {
        GWindow gw;
        gw.setWindowTitle(TITLE);
        string color = map[fn];
        gw.setColor(color);
        pause(1000);
        plot(gw, fn, minX, maxX);
        pause(1000);
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
