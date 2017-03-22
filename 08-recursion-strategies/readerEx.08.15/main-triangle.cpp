//
// main.cpp
//
// This program renders a Koch snowflake (i.e., fractal triangle).
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 15
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code comes from Figure 8-5 of the course reader.
// --------------------------------------------------------------------------
//
// Created on 3/13/16.
//

#include <iostream>
#include <cmath>
#include "gwindow.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.15\n";
const std::string DETAIL = "Koch Snowflake (fractal triangle)";
const std::string BANNER = HEADER + DETAIL;

const double SIZE = 200;        // Size of the order 0 fractal in pixels.
const int ORDER = 4;            // Order of the fractal snowflake.       

// Prototypes

GPoint drawFractalLine(GWindow & gw, 
                        GPoint pt,
                        double r, 
                        double theta, 
                        int order);

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;

    GWindow gw;
    double cx = gw.getWidth() / 2;
    double cy = gw.getHeight() / 2;
    
    // 
    // The 0th-order (equilateral) Koch triangle has height of:
    //
    //    h = sqrt(3) * side
    //
    // For esthetic reasons, the top (horizontal) line of the triangle is
    // positioned above vertical center that is 1/3rd the height of the
    // triangle itself.
    //
    // This yields higher-order Koch snowflakes that are nicely centered.
    //
    
    GPoint pt(cx - SIZE / 2, cy - sqrt(3.0) * SIZE / 6);
    pt = drawFractalLine(gw, pt, SIZE, 0, ORDER);
    pt = drawFractalLine(gw, pt, SIZE, -120, ORDER);
    pt = drawFractalLine(gw, pt, SIZE, +120, ORDER);
    
    return 0;
}

//
// Function: drawFactalLine
// Usage: GPoint end = drawFractalLine(gw, pt, r, theta, order);
// -------------------------------------------------------------
// Draws a fractal edge starting from pt and extending r units in direction
// theta.  If order > 0, the edge is divided into four fractal edges of the
// next lower order.  The function returns the endpoint of the line.
//

GPoint drawFractalLine(GWindow & gw, 
                        GPoint pt, double r,
                        double theta, 
                        int order) {
    
    if (order == 0) {
        return gw.drawPolarLine(pt, r, theta);
    } else {
        pt = drawFractalLine(gw, pt, r / 3, theta, order - 1);
        pt = drawFractalLine(gw, pt, r / 3, theta + 60, order - 1);
        pt = drawFractalLine(gw, pt, r / 3, theta - 60, order - 1);
        return drawFractalLine(gw, pt, r / 3, theta, order -1);
    }
}