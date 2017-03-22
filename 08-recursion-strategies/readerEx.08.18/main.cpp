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
#include "simpio.h"
#include "gwindow.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.18\n";
const std::string DETAIL = "Sierpinski Triangle";
const std::string BANNER = HEADER + DETAIL;

const std::string EDGE_PROMPT = "Enter the edge lenth of the outer triangle.";
const int MIN_EDGE = 50;
const int MAX_EDGE = 300;

const std::string ORDER_PROMPT = "Enter the order of the Sierpinski triangle.";
const int MIN_ORDER = 0;
const int MAX_ORDER = 5;

const std::string HELP = "Something reasonable would be in the range of: ";

// Prototypes

void drawSierpinski(GWindow & gw, GPoint top, double edgeLength, int order);

void drawTriangle(GWindow & gw,
                  GPoint & nextTopL,
                  GPoint & nextTopR,
                  const GPoint & top,
                  double edgeLength);

int getReasonableInteger(const std::string prompt,
                         const std::string guidance,
                         int minInt,
                         int maxInt);

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl << std::endl;
    
    int edge = getReasonableInteger(EDGE_PROMPT, HELP, MIN_EDGE, MAX_EDGE);
    int order = getReasonableInteger(ORDER_PROMPT, HELP, MIN_ORDER, MAX_ORDER);
    
    GWindow gw;
    double tHeight = sqrt(3.0) * (edge / 2);
    
    GPoint top(gw.getWidth()/2, (gw.getHeight() - tHeight)/2);
    
    drawSierpinski(gw, top, edge, order);

    return 0;
}

//
// Function: drawSierpinski
// Usage: drawSierpinski(gw, top, edgeLength, order);
// --------------------------------------------------
// Draws a Sierpinski triangle in a graphics window.
//

void drawSierpinski(GWindow & gw, GPoint top, double edge, int order) {
    
    GPoint topL, topR;
    drawTriangle(gw, topL, topR, top, edge);
    
    if (order == 0) {
        return;
    } else {
        // Draw half-sized triangles.
        drawSierpinski(gw, top,  edge/2, order - 1);
        drawSierpinski(gw, topL, edge/2, order - 1);
        drawSierpinski(gw, topR, edge/2, order - 1);
    }
}

//
// Function: drawTriangle
// Usage: drawTriangle(gw, top, edge);
// -----------------------------------
// Draws an equilateral triangle in a graphics window with the tip
// oriented at the top.
//

void drawTriangle(GWindow & gw,
                  GPoint & nextTopL,
                  GPoint & nextTopR,
                  const GPoint & top,
                  double edge) {
    
    GPoint pt;
    double halfEdge = edge/2;
    
    // Render triangle in clockwise fashion, starting from the top.
    //
    // Harvest the midpoints of the sloping sides and return these
    // back to the client as the tops of smaller left and right
    // triangles.
    
    nextTopR = gw.drawPolarLine(top, halfEdge,  -60);
    pt = gw.drawPolarLine(nextTopR, halfEdge,  -60);
    pt = gw.drawPolarLine(pt, edge, -180);
    nextTopL = gw.drawPolarLine(pt, halfEdge,   60);
    pt = gw.drawPolarLine(nextTopL, halfEdge,   60);
}

//
// Function: getReasonableInteger
// Usage: int i = getReasonableInteger(prompt, guidance, minInt, maxInt);
// ----------------------------------------------------------------------
// Prompts user to enter an integer within a certain range.  Returns
// with the valid integer.
//

int getReasonableInteger(const std::string prompt,
                         const std::string guidance,
                         int minInt,
                         int maxInt) {
    int result;
    
    while (true) {
        std::cout << prompt << std::endl;
        std::cout << guidance << minInt << " to " << maxInt << std::endl;
        result = getInteger("value: ");
        if (result < minInt || result > maxInt) continue;
        break;
    }
    return result;
}
