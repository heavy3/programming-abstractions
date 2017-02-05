//
// main.cpp
//
// This program renders a Sierpinski triangle.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 18
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/14/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <cmath>
#include "gwindow.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.18\n";
const std::string DETAIL = "Sierpinski Triangle";
const std::string BANNER = HEADER + DETAIL;

const int EDGE = 200;     // Size of the order-0 fractal in pixels.
const int ORDER = 5;      // Order of the fractal triangle.

// Prototypes

void drawSierpinski(GWindow & gw, GPoint top, int edgeLength, int order);
void drawTriangle(GWindow & gw, GPoint top, int edgeLength);

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    GWindow gw;
    double tHeight = sqrt(3.0) * (EDGE / 2);
    
    GPoint top(gw.getWidth()/2, (gw.getHeight() - tHeight)/2);
    
    drawSierpinski(gw, top, EDGE, ORDER);

    return 0;
}

//
// Function: drawSierpinski
// Usage: drawSierpinski(gw, top, edgeLength, order);
// --------------------------------------------------
// Draws a Sierpinski triangle in a graphics window.
//

void drawSierpinski(GWindow & gw, GPoint top, int edge, int order) {
    if (order == 0) {
        drawTriangle(gw, top, edge);
        return;
    }
    
    drawTriangle(gw, top, edge);

    edge /= 2;
    GPoint topL = gw.drawPolarLine(top, edge, -120);
    GPoint topR = gw.drawPolarLine(top, edge, -60);
    drawSierpinski(gw, top, edge, order - 1);
    drawSierpinski(gw, topL, edge, order - 1);
    drawSierpinski(gw, topR, edge, order - 1);
}

//
// Function: drawTriangle
// Usage: drawTriangle(gw, top, edge);
// -----------------------------------
// Draws an equilateral triangle in a graphics window with the tip
// oriented at the top.
//

void drawTriangle(GWindow & gw, GPoint top, int edge) {
    GPoint pt = top;
    
    pt = gw.drawPolarLine(pt, edge,  -60);
    pt = gw.drawPolarLine(pt, edge, -180);
    pt = gw.drawPolarLine(pt, edge,   60);
    
}