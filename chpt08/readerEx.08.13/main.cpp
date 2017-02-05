//
// main.cpp
//
// This program extends the Mondrian program from Figure 8-4 so
// it fills in some of the rectanular regions with randomly chosen
// colors.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 13
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program extends the code from Figure 8-4.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/12/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include "gwindow.h"
#include "random.h"
using namespace std;

//
// Define the rainbow colors of interest as an enumerated type for use in
// a simple iterator.
//

enum MondrianColor {
    RED,
    YELLOW,
    WHITE1,
    BLUE,
    GRAY,
    WHITE2,
    
    // Strictly speaking, not a rainbow color, but programmatically
    // handy for rendering a final arc of sky under the rainbow.
    
    LASTCOLOR // Token to mark the end of the list should
              // should we want to add more colors to our Mondrian facsimiles.
};

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.13\n";
const std::string DETAIL = "Mondrian Madness";
const std::string BANNER = HEADER + DETAIL;

const double MIN_AREA = 10000;  // Smallest square that will be split
const double MIN_EDGE = 20;     // Smallest edge length allowed

// Prototypes

void subdivideCanvas(GWindow & gw, double x, double y,
                                   double width, double height);
string stringizeColor(MondrianColor color);

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;

    GWindow gw;
    subdivideCanvas(gw, 0, 0, gw.getWidth(), gw.getHeight());
    waitForClick();
    return 0;
}

//
// Function: subdivideCanvas
// Usage: subdivideCanvas(gw, x, y, width, height);
// ------------------------------------------------
// Decomposes the specified rectangular region on the canvas recursively
// by splitting that rectangle randomly along its larger dimension.  The
// recursion continues until the area falls below the constant MIN_AREA.
//

void subdivideCanvas(GWindow & gw, double x, double y,
                                   double width, double height) {
    
    if ((width * height) >= MIN_AREA) {
        MondrianColor mcolor = MondrianColor(randomInteger(0, LASTCOLOR - 1));
        gw.setColor(stringizeColor(mcolor));
        gw.fillRect(x, y, width, height);
        gw.setColor("BLACK");
        if (width > height) {
            double mid = randomReal(MIN_EDGE, width - MIN_EDGE);
            subdivideCanvas(gw, x, y, mid, height);
            subdivideCanvas(gw, x + mid, y, width - mid, height);
            gw.drawLine(x + mid, y, x + mid, y + height);
        } else {
            double mid = randomReal(MIN_EDGE, height - MIN_EDGE);
            subdivideCanvas(gw, x, y, width, mid);
            subdivideCanvas(gw, x, y + mid, width, height - mid);
            gw.drawLine(x, y + mid, x + width, y + mid);
        }
    }
}

//
// Function: stringizeColor
// Usage: string stringColor = stringizeColor(RED);
// ------------------------------------------------
// Returns the literal name of the specified color as a string
// for use with the Stanford GWindow interface.
//
// This function should be updated if colors are added or deleted from the
// MondrianColor enumerated type.
//

string stringizeColor(MondrianColor c) {
    string s;
    switch (c) {
        case RED:       s = "RED";     break;
        case YELLOW:    s = "YELLOW";  break;
        case WHITE1:    s = "WHITE";   break;
        case BLUE:      s = "BLUE";    break;
        case GRAY:      s = "GRAY";    break;
        case WHITE2:    s = "WHITE";   break;
        default:        s = "UNKOWN_COLOR";
    }
    return s;
}
