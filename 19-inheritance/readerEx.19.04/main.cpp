//
// main.cpp
//
// This program exercises the ShapeList class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This is largely derived from Figure 19-8 in the text.
// --------------------------------------------------------------------------
//

#include <iostream>
#include "gwindow.h"
#include "shape.h"
#include "shapelist.h"

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 19.04\n";
const std::string DETAIL = "Inheritance: ShapeList Class";
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
    shapes.moveBackward(op);
    pause(1000);
    shapes.draw(gw);
    shapes.moveToFront(op);
    pause(1000);
    shapes.draw(gw);
    pause(1000);
    shapes.free();
    shapes.clear();

    return 0;
}
