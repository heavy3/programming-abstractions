//
// main.cpp
//
// This program exercises the Shape class, extended to include a
// contains() member function suitable for hit detection for lines,
// rectangles, and ellipses.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/23/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include "shape.h"
#include "gevents.h"

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 19.03\n";
const std::string DETAIL = "Inheritance: Extend Shape with contains() method.";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main(int argc, char ** argv) {
    std::cout << BANNER << std::endl << std::endl;
    
    GWindow gw;
    double width = gw.getWidth();
    double height = gw.getHeight();
    gw.setWindowTitle(DETAIL);
    
    // Instantiate a square
    
    double sqSize = min(width, height) / 4;
    double sqX = (width - sqSize) / 2;
    double sqY = (height - sqSize) / 4;
    Square sq(sqX, sqY, sqSize);
    sq.draw(gw);
    
    // Instantiate an ellipse
    
    double elHeight = min(width, height) / 4;
    double elWidth = 2*elHeight;
    double elX = (width - elWidth) / 2;
    double elY = 3*sqY;
    Oval el(elX, elY, elWidth, elHeight);
    el.draw(gw);
    
    // Instantiate a line
    
    Line line(sqX, sqY, elX, elY + elHeight/2);
    line.draw(gw);

    // Click on the shape to turn it red
    
    while (true) {
        GMouseEvent e = waitForEvent(MOUSE_EVENT);
        if (e.getEventType() == MOUSE_PRESSED) {
            double mX = e.getX();
            double mY = e.getY();
            
            if (sq.contains(mX, mY)) {
                sq.setColor("RED");
            } else {
                sq.setColor("BLACK");
            }
            sq.draw(gw);
            
            if (el.contains(mX, mY)) {
                el.setColor("RED");
            } else {
                el.setColor("BLACK");
            }
            el.draw(gw);
            
            if (line.contains(mX, mY)) {
                line.setColor("RED");
            } else {
                line.setColor("BLACK");
            }
            line.draw(gw);
        }
    }
    return 0;
}
