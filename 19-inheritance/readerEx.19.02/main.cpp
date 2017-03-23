//
// main.cpp
//
// This program exercises the Shape class, newly extended with a Square
// subclass.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 2
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

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 19.02\n";
const std::string DETAIL = "Inheritance: Extend Shape with Square";
const std::string BANNER = HEADER + DETAIL;

// Prototypes


// Main program

int main(int argc, char ** argv) {
    std::cout << BANNER << std::endl << std::endl;
    
    GWindow gw;
    double width = gw.getWidth();
    double height = gw.getHeight();
    
    double sqSize = min(width, height) / 2;
    double sqX = (width - sqSize) / 2;
    double sqY = (height - sqSize) / 2;
    Square sq(sqX, sqY, sqSize);
    sq.draw(gw);
    return 0;
}
