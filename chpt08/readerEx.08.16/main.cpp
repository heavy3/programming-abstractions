//
// main.cpp
//
// A program to draw a fractal H on the graphics window.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 16
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

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.16\n";
const std::string DETAIL = "Fractal H";
const std::string BANNER = HEADER + DETAIL;

const double SIZE = 100;        // Size of the order-0 fractal in pixels.
const int ORDER = 4;            // Order of the fractal-H.               

// Prototypes

void drawFractalH(GWindow & gw, int cx, int cy, int SIZE, int ORDER);
void drawH(GWindow & gw, int top, int mid, int bottom, int left, int right);

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;

    GWindow gw;
    double cx = gw.getWidth() / 2;
    double cy = gw.getHeight() / 2;
    
    drawFractalH(gw, cx, cy, SIZE, ORDER);
    
    return 0;
}

//
// Function: drawFractalH
// Usage: drawFractalH(gw, cx, cy, SIZE, ORDER);
// ---------------------------------------------
// Draws an H-fractal at the center of the graphics window.
//

void drawFractalH(GWindow & gw, int cx, int cy, int SIZE, int ORDER) {
    
    SIZE /= 2;
    int top = cy - SIZE;
    int mid = cy;
    int bottom = cy + SIZE;
    int left = cx - SIZE;
    int right = cx + SIZE;
    
    if (ORDER == 0) {
        drawH(gw, top, mid, bottom, left, right);
        return;
    } else {
        drawH(gw, top, mid, bottom, left, right);
        ORDER -= 1;
        drawFractalH(gw, left, top, SIZE, ORDER);
        drawFractalH(gw, right, top, SIZE, ORDER);
        drawFractalH(gw, left, bottom, SIZE, ORDER);
        drawFractalH(gw, right, bottom, SIZE, ORDER);
    }
}

// 
// Function: drawH
// Usage: drawH(gw, top, mid, bottom, left, right);
// ------------------------------------------------
// Draws a capital H in the graphics window using the 
// 3 vertical and 2 horizontal dimensions specified.
//

void drawH(GWindow & gw, int top, int mid, int bottom, int left, int right) {
    gw.drawLine(left,  top, left,  bottom);    // Left side of H
    gw.drawLine(right, top, right, bottom);    // Right side of H
    gw.drawLine(left,  mid, right, mid);       // Crossbar
}