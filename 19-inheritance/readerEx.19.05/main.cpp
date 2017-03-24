//
// main.cpp
//
// This program exercise the ShapeList class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file extends code from Figure 19-8 in the text.
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/24/17.
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include "gwindow.h"
#include "gevents.h"
#include "shape.h"
#include "shapelist.h"

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 19.05\n";
const std::string DETAIL = "Inheritance: ShapeList Class + shapeAt()";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main(int argc, char ** argv) {
    std::cout << BANNER << std::endl << std::endl;
    
    GWindow gw;
    gw.setWindowTitle(DETAIL);
    double width = gw.getWidth();
    double height = gw.getHeight();
    
    Rect *rp = new Rect(width / 4, height / 4, width / 2, height / 2);
    Oval *op = new Oval(width / 4, height / 4, width / 2, height / 2);
    rp->setColor("BLUE");
    op->setColor("GRAY");
    
    ShapeList shapes;
    
    shapes.add(new Line(0, height / 2, width / 2, 0));
    shapes.add(new Line(width / 2, 0, width, height / 2));
    shapes.add(new Line(width, height / 2, width / 2, height));
    shapes.add(new Line(width / 2, height, 0, height / 2));
    shapes.add(rp);
    shapes.add(op);
    pause(1000);
    shapes.draw(gw);
    pause(1000);

    // Exercise shapes.getShapeAt() member function.
    //
    // Click on the shape to flash it green, but maintain
    // render order within the shape list.
    
    while (true) {
        GMouseEvent e = waitForEvent(MOUSE_EVENT);
        if (e.getEventType() == MOUSE_PRESSED) {
            double mX = e.getX();
            double mY = e.getY();
            
            Shape *gsp = shapes.getShapeAt(mX, mY);
            if (gsp != NULL) {
                std::string origColor = gsp->getColor();
                gsp->setColor("GREEN");
                gsp->draw(gw);
                pause(1000);
                gsp->setColor(origColor);
                pause(1000);
                shapes.draw(gw);
            }
        }
    }
    
    shapes.free();
    shapes.clear();

    return 0;
}
