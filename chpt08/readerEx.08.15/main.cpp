//
// main.cpp
//
// This program renders a fractal coastline between two points.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 15
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// The program extends the Koch snowflake code of Figure 8-5.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/13/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <cmath>
#include "gwindow.h"
#include "random.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.15b\n";
const std::string DETAIL = "Fractal Coastline";
const std::string BANNER = HEADER + DETAIL;

const double SIZE = 200;        // Size of the order 0 fractal in pixels.
const int ORDER = 3;            // Order of the fractal coastline.

// Prototypes

GPoint drawFractalCoast(GWindow & gw, 
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
    pt = drawFractalCoast(gw, pt, SIZE, 0, ORDER);
    
    return 0;
}

//
// Function: drawFactalCoast
// Usage: GPoint end = drawFractalCoast(gw, pt, r, theta, order);
// -------------------------------------------------------------
// Draws a fractal edge starting from pt and extending r units in direction
// theta.  If order > 0, the edge is divided into four fractal edges of the
// next lower order.  The function returns the endpoint of the line.
//

GPoint drawFractalCoast(GWindow & gw, 
                        GPoint pt, 
                        double r,
                        double theta, 
                        int order) {
    
    if (order == 0) {
        return gw.drawPolarLine(pt, r, theta);
    } else {
        int sign = (randomChance(0.5)) ? 1 : -1;
        
        pt = drawFractalCoast(gw, pt, r / 3, theta, order - 1);
        pt = drawFractalCoast(gw, pt, r / 3, theta + (sign) * 60, order - 1);
        pt = drawFractalCoast(gw, pt, r / 3, theta - (sign) * 60, order - 1);
        return drawFractalCoast(gw, pt, r / 3, theta, order -1);
    }
}
