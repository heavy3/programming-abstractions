//
// main.cpp
//
// This program renders a Wallinger fractal tree.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 17
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/14/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include "gwindow.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.17\n";
const std::string DETAIL = "Wallinger's Fractal Tree";
const std::string BANNER = HEADER + DETAIL;

const double SIZE = 100;            // Size of the order 0 fractal in pixels.
const int ORDER = 8;                // Order of the fractal tree.            
const double GROWTH_RATIO = 1.618;  // (a + b) / b :: b : a, the golden ratio.
const int THETA = 45;               // Angle at which branching occurs.

// Prototypes

void drawFractalTree(GWindow & gw, GPoint base, int branchAngle, int order);
GPoint drawTrunk(GWindow gw, GPoint base, int length, int orientation);
void drawBranches(GWindow gw, GPoint base, int length, int theta, int branchAngle, int order);

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    GWindow gw;
    GPoint base(gw.getWidth()/2, gw.getHeight());
    
    drawFractalTree(gw, base, THETA, ORDER);
    return 0;
}

//
// Function: drawFractalTree
// Usage: drawFractalTree(gw, basePt, branchAngle, recursiveOrder);
// ----------------------------------------------------------------
// Draws a bifurcating fractal tree with relative branch lengths
// determined by a constant growth ratio.
//

void drawFractalTree(GWindow & gw, GPoint base,
                     int branchAngle, int order) {
    
    int theta  = 90;   // Orient trunk upward relative to base point.
    int length = SIZE;
    
    GPoint top = drawTrunk(gw, base, length, theta);
    drawBranches(gw, top, length, theta, branchAngle, order);
}

//
// Function: drawTrunk
// Usage: drawTrunk(gw, base, length, orientation);
// ------------------------------------------------
// Draws the trunk from a base point along a specified orientation.
// Returns the top of the trunk for use in subsequent branch drawing.
//

GPoint drawTrunk(GWindow gw, GPoint base, int length, int orientation) {
    return gw.drawPolarLine(base, length, orientation);
}

//
// Function: drawBranches
// Usage: drawBranches(gw, base, length, orientation, branchAngle, order);
// -----------------------------------------------------------------------
// Recursively renders bifurcating branches relative to some reference point, 
// initially the top of the trunk.  The number of branching generations
// is controlled by the the order variable.
//
// Recursive Insight:
//
// The top of a large branch forms the base of two smaller branches
// rotated relative to the orientation of the larger branch.
//
// The ratio of large branch length to small branch length is a constant ratio.
// The branch orientation is tracked over successive generations and
// is rotated by a specified angle.
//

void drawBranches(GWindow gw, GPoint base, int length, int orientation,
                  int branchAngle, int order) {
    if (order == 0) {
        return;
    } else {
        order--;
        length = int(length / GROWTH_RATIO);
        
        int thetaL = orientation + branchAngle;
        int thetaR = orientation - branchAngle;
        
        GPoint topL = gw.drawPolarLine(base, length, thetaL);
        GPoint topR = gw.drawPolarLine(base, length, thetaR);
    
        drawBranches(gw, topL, length, thetaL, branchAngle, order);
        drawBranches(gw, topR, length, thetaR, branchAngle, order);
    }
}
