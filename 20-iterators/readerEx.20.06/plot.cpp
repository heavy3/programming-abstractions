//
// plot.cpp
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 06
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// Most of this code comes from Figure 20-2
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/31/17
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//

#include "plot.h"

//
// Function: plot
// Usage: plot(gw, fn, minX, maxX, minY, maxY);
// --------------------------------------------
// Plots the specified function (which must map one double to another double)
// on the screen.  The remaining arguments indicate the range of values in the
// x and y directions respectively.
//

void plot(GWindow& gw,
          ExpressionFunction fn,
          double minX,
          double maxX) {
    
    double minY;
    double maxY;
    
    reasonableYInterval(minY, maxY, minX, maxX, fn);
    plot(gw, fn, minX, maxX, minY, maxY);
}

void plot(GWindow& gw,
          ExpressionFunction fn,
          double minX,
          double maxX,
          double minY,
          double maxY) {
    
    double width = gw.getWidth();
    double height = gw.getHeight();
    
    int nSteps = int(width);
    double dx = (maxX - minX) / double(nSteps);
    
    // Scale coordinates of first point to screen coordinates
    // (reflecting the y-coodinate about the x-axis since positive y
    // direction is toward the bottom of the window, opposite to
    // Cartesian convention).
    
    double sx0 = 0; // far left side of view port
    double sy0 = height - (fn(minX) - minY) / (maxY - minY) * height;
    
    for (int i = 1; i < nSteps && sy0 >= 0; i++) {
        double x = minX + i * dx;
        double y = fn(x);
        
        double sx1 = (x - minX) / (maxX - minX) * width;
        double sy1 = height - (y - minY) / (maxY - minY) * height;
        
        gw.drawLine(sx0, sy0, sx1, sy1);
        
        sx0 = sx1;
        sy0 = sy1;
    }
}

// Function: reasonableYInterval
// Usage: reasonable(minY, maxY, minX, maxX, fn);
// ----------------------------------------------
// With monotonically increasing or decreasing functions, the usual
// heuristic of establishing minY and maxY by evaluating the function
// at minX and maxX may be acceptable when minY and maxY are not otherwised
// specified.
//
// However, for periodic functions, it's likely that the local function
// minima and maxima do -not- occur at the minX and maxX values
// respectively.
//
// So we detect for that and scale minY and maxY by an order of magnitude,
// centered about the x-axis, if those values are within 10% of each other
// otherwise.
//
// TODO: Calculate local minima and maxima over the X-interval and use
//       -that- to drive minY and maxY.

void reasonableYInterval(double& minY,
                         double& maxY,
                         double minX,
                         double maxX,
                         ExpressionFunction& fn) {
    minY = fn(minX);
    maxY = fn(maxX);
    
    if (abs(maxY - minY) <= .1) {
        maxY = max(minY, maxY) * 5;
        minY = -maxY;
    }
}
