//
// main.cpp
//
// This program implements a function,
//
//    drawTickMarks(gw, x1, y1, lineLength, tickLenth);
//
// which uses recursion to render an inch-long ruler segmented into
// 1/16ths of varying heights.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 14
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/13/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include "gwindow.h"
#include "random.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.14\n";
const std::string DETAIL = "Ruler Tick Marks";
const std::string BANNER = HEADER + DETAIL;
const double HALF_INCH_TICK = 0.2;  // Length of tick mark in inches (approx).
const double SIXTEENTHS_TICK = HALF_INCH_TICK / 8.0;

//
// It's hard to render a line exactly 1" long.  We'd need to query the 
// current screen resolution and display dimensions to calculate pixels per
// inch.  Since the Stanford graphics window API doesn't seem to export
// primitives for gathering that info, just estimate the number of pixels
// in one inch of display length.
//

const int PIXELS_PER_INCH = 96;     // Expedient hack.

// Prototypes

void drawTickMarks(GWindow &gw, int x, int y, int lineLength, double tickLength);

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;

    GWindow gw;
    
    // Calculate coordinates for 1" line centered in the graphics window.
    
    int lineLength = PIXELS_PER_INCH;
    int x1 = (gw.getWidth() - lineLength) / 2;
    int y1 = gw.getHeight() / 2;
    int x2 = x1 + lineLength;        // Horizontal line.
    int y2 = y1;
    
    // Draw 1" line.
    
    gw.drawLine(x1, y1, x2, y2);
    
    double tickLenth = HALF_INCH_TICK;
    drawTickMarks(gw, x1, y1, lineLength, tickLenth);
    
    return 0;
}

//
// Function: drawTickMarks
// Usage: drawTickMarks(gw, x, y, lineLength, tickLength);
// -------------------------------------------------------
// Draws tick marks along a line segment to create a ruled appearance.
// Assuming a 1" line segment is passed in, the routine will delineate
// down to 16th of an inch tick marks.
//
// The length of a tick mark diminishes by half with each smaller
// gradation.
//

void drawTickMarks(GWindow &gw, int x, int y, int lineLength, double tickLength) {
    if (tickLength < SIXTEENTHS_TICK) {
        return;
    }
    
    int xMid = x + lineLength/2;
    gw.drawLine(xMid, y, xMid, y - tickLength * PIXELS_PER_INCH);
    
    lineLength /= 2;
    tickLength /= 2.0;
    drawTickMarks(gw, x, y, lineLength, tickLength);
    drawTickMarks(gw, xMid, y, lineLength, tickLength);
}
