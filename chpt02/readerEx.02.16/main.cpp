//
// main.cpp
//
// This program uses the Standford C++ Library (circa ~2013) to 
// draw a 6-color rainbow in a graphics window.
//
// Starting at the top, the six stripes in the rainbow are:
//
//    red, orange, yellow, green, blue, magenta
//
// against a cyan background.
//
// This requires a java runtime and spl.jar for rendering the graphics.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 16
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/24/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <iomanip>
#include "gwindow.h"
using namespace std;

//#define TESTING                   // Uncomment for debug text in console.

// Constants and types

//
// Define the rainbow colors of interest as an enumerated type for use in
// a simple iterator.
//

enum RainbowColor {
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    MAGENTA,
    CYAN,       // Strictly speaking, not a rainbow color, but programmatically
                // handy for rendering a final arc of sky under the rainbow.
    LASTCOLOR   // Token to mark the end of the list should we want to add 
                // more colors.
};

const double ASPECT_RATIO = 2.3;    // Approximate length:width ratio in 
                                    // problem.
const int WINDOW_WIDTH_PX = 500;    // Desired physical window width in pixels.
const int WINDOW_HEIGHT_PX = WINDOW_WIDTH_PX / ASPECT_RATIO;
const int COLORS_IN_RAINBOW = LASTCOLOR - 1;
//
// Give each band a thickness such that all 6 colors plus a top and bottom 
// band of sky color can fit above the vertical midpoint of the window as 
// presented in the reader.
//
const int COLOR_THICKNESS_PX = (WINDOW_HEIGHT_PX / 1.6) / (COLORS_IN_RAINBOW + 2);
const double RADIANS = 1.4;         // Model rainbow arc with fractional circle
                                    // with this arc length.
const RainbowColor SKYBLUE = CYAN;

//
// According to:http://mathcentral.uregina.ca/QQ/database/QQ.09.09/h/wayne1.html
//
// we can calculate the radius of the circle we're using to model the
// rainbow if we know the chord length (which is just the window length 
// in our case) and the desired visible arc length.
//

const double RADIUS_PX = WINDOW_WIDTH_PX / (2 * sin(RADIANS/2.0));

//
// Specify baseline position and diameter of the circle representing the 
// top-most color in the rainbow.
//
// The X0 and Y0 variables are the upper lefthand coordinates of the circle's 
// bounding box relative to the viewing window which clips the circle to an arc.
//
const int CIRCLE_X0 = 0 - (2 * RADIUS_PX - WINDOW_WIDTH_PX) / 2.0;
const int CIRCLE_Y0 = 0 + COLOR_THICKNESS_PX;
const int CIRCLE_DIAMETER0 = RADIUS_PX * 2;  // Diameter of outermost (or red) circle.

// Function prototypes

RainbowColor operator++(RainbowColor& rc, int);
string stringizeColor(RainbowColor color);
void getCircleCoordinates(RainbowColor rc, int& x0, int& y0,
                        int& width, int& height);

// Main function

int main() {
    
    int x0, y0, width, height;
    
    GWindow gw(WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX);
    gw.setWindowTitle("Rainbow");
    
    gw.setColor(stringizeColor(SKYBLUE));
    gw.fillRect(0, 0, WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX);
    
    for (RainbowColor rc = RED; rc < LASTCOLOR; rc++) {
        getCircleCoordinates(rc, x0, y0, width, height);
        gw.setColor(stringizeColor(rc));
        gw.fillOval(x0, y0, width, height);
    }
    
    return 0;
}

//
// Function: stringizeColor
// Usage: string stringColor = stringizeColor(RED);
// ------------------------------------------------
// Returns the literal name of the specified RainbowColor as a string
// for use with the Stanford GWindow interface.
//
// This function should be updated if colors are added or deleted from the
// RainbowColor enumerated type.
//

string stringizeColor(RainbowColor c) {
    string s;
    switch (c) {
        case RED:       s = "RED";     break;
        case ORANGE:    s = "ORANGE";  break;
        case YELLOW:    s = "YELLOW";  break;
        case GREEN:     s = "GREEN";   break;
        case BLUE:      s = "BLUE";    break;
        case MAGENTA:   s = "MAGENTA"; break;
        case CYAN:      s = "CYAN";    break;
        default:        s = "UNKOWN_COLOR";
    }
    return s;
}

//
// Function: operator++
// Usage: for (RainbowColor rc = RED; rc < LAST; rc++) ...
// -------------------------------------------------------
// Post-increment operator for iterating across a list of rainbow colors
// in display order from top to bottom.
//

RainbowColor operator++(RainbowColor& rc, int) {
    RainbowColor old = rc;
    rc = RainbowColor(old + 1);
    return rc;
}

//
// Function: getCircleCoordinates
// Usage: getCircleCoordinates(RED, x0, y0, width, height);
// --------------------------------------------------------
// The rainbow swaths are modeled as a set of
// concentric circles that are clipped by a viewing window.
//
// This function returns the dimensions of the bounding
// box for one such circle of color.
//
// To achieve the concentric effect, the dimensions scale 
// inward toward the center as we progress from outter colors
// like red, to inner colors like blue and magenta.
//

void getCircleCoordinates(RainbowColor rc, int& x0, int& y0,
                        int& width, int& height) {
    x0 = (rc * COLOR_THICKNESS_PX)/2.0 + CIRCLE_X0;
    y0 = (rc * COLOR_THICKNESS_PX) + CIRCLE_Y0;
    height = width = CIRCLE_DIAMETER0 - (rc * COLOR_THICKNESS_PX);
    
    #ifdef TESTING
    cout << setw(8) << stringizeColor(rc) << ": ";
    cout << "(x0, y0) = " << "(" << setw(4) << x0 << ", "  << setw(3) << y0 << ")  ";
    cout << "w = " << width << "  h = " << height << endl;
    #endif
}