//
// main.cpp
//
// This program renders a Yin-Yang symbol using the Stanford C++ Graphics
// interface.  Java runtime environment and spl.jar required for rendering.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 18
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/25/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include "gmath.h"
#include "gwindow.h"
using namespace std;

//#define DEBUGGING   // Uncomment for debug text sent to console.

// Constants and Types

enum SymbolColor {
    LIGHT,
    DARK
};

const string LIGHT_COLOR = "GREEN";
const string DARK_COLOR  = "BLUE";
const int SYMBOL_WIDTH = 400;                    // Diameter of symbol in pixels.
const int RADIUS = SYMBOL_WIDTH / 2;             // Radius of symbol in pixels.
const int EYE_WIDTH = 30;                        // Size of tailchaser's eye.
const int MARGIN = 10;                           // Distance between symbol and
                                                 // edge of window in pixels.
const int WINDOW_WIDTH = 2 * (RADIUS + MARGIN);  // Relate the size of the
                                                 // display window to size of
const int WINDOW_HEIGHT = 2 * (RADIUS + MARGIN); // symbol.
const int WINDOW_CENTER_X = WINDOW_WIDTH / 2;    // X offset to cartesian origin
const int WINDOW_CENTER_Y = WINDOW_HEIGHT / 2;   // Y offset to cartesian origin
const int FLIP_Y_POLARITY = -1;                  // +Y screen axis points down.
const int NUM_BODY_CIRCLES = 50;                 // Number of circles to model
                                                 // tailchaser.

// Function prototypes

void drawTailChasers(GWindow gw, SymbolColor color, int quality);
double getTailchaserBodyX0(double r, double R, double theta);
double getTailchaserBodyY0(double r, double R, double theta);
double getCircleTangentXsubT(double r, double theta);
double getCircleTangentYsubT(double r, double theta);
double toXScreenCoord(double xCartesianCoord);
double toYScreenCoord(double yCartesianCoord);
SymbolColor operator++(SymbolColor& color, int);
string toString(SymbolColor color);

// Main program

int main(int argc, char * argv[]) {
    GWindow gw(WINDOW_WIDTH, WINDOW_HEIGHT);
    double x0 = MARGIN;
    double y0 = MARGIN;
    SymbolColor color = DARK;

    gw.setWindowTitle("yin yang");

    //
    //  Begin with a solid black circle.
    //
    gw.setColor(toString(color));
    gw.fillOval(x0, y0, SYMBOL_WIDTH, SYMBOL_WIDTH);
    
    //
    //  Draw the constrasting-color tailchaser as a
    //  sequence of closely-spaced, filled circles.
    //
    color++;
    drawTailChasers(gw, color, NUM_BODY_CIRCLES);
    
    //
    //  Outline the symbol with a black circle to
    //  complete the symbol rendering.
    //
    color++;
    gw.setColor(toString(color));
    gw.drawOval(x0, y0, SYMBOL_WIDTH, SYMBOL_WIDTH);
    
    return 0;
}

// Function definitions

//
// Function: drawTailChasers
// Usage: drawTailChasers(window, "WHITE", 20);
// -------------------------------------------
// We model the body of the tailchaser as a set of closely spaced,
// filled circles that hug the inside of a larger circle in a 180 degree arc 
// that sweeps from north to south.
//
// The circles are rendered with the help of a graphics package that
// describes the location of a circle in terms of a bounding box which
// circumscribes the circle and has a reference origin in the upper
// lefthand corner of the box.
//
// Generate a sequence of (x, y) points tangent to the outer circle
// of the yin yang symbol, starting at the top and tracing a 180 degree
// arc along the right side.
//
// Calculate the bounding boxes for the circular body segments
// of one tailchaser.  Render those circles as we sweep along a semi-
// circular path to create a half-arc with rounded ends.  Adding
// the head of the contrasting tail-chaser at the end of the arc
// yields the effect two tail-chasers.
//

void drawTailChasers(GWindow gw, SymbolColor color, int quality) {
    double R = (gw.getWidth() / 2.0) - MARGIN;
    double r = R / 2.0;
    int numBodyCircles = quality;
    double x0, y0;
    
    gw.setColor(toString(color));
    double deltaRadians = PI / numBodyCircles;
    
    // 
    // Draw tail-chaser body.
    //
    
    for (double theta = PI/2 ; theta >=  -PI/2; theta -= deltaRadians) {
    
        x0 = getTailchaserBodyX0(r, R, theta);
        y0 = getTailchaserBodyY0(r, R, theta);
        
        gw.fillOval(x0, y0, R, R);
        
    }
    
    // 
    // Draw head of contrasting tail-chaser.
    //
    // Translate to the left slightly to correct for white outline artifact
    // left over from previous step, otherwise there would be an "eclipse"
    // effect to the head.
    //
    
    color++;
    gw.setColor(toString(color));
    gw.fillOval(x0 - 1, y0, R, R);
    
    //
    // Draw black and white eyes of the tail-chasers.
    //
    
    gw.fillOval(WINDOW_CENTER_X - EYE_WIDTH/2,
                WINDOW_CENTER_Y - RADIUS/2 - EYE_WIDTH/2,
                EYE_WIDTH, EYE_WIDTH);
    
    color++;
    gw.setColor(toString(color));
    gw.fillOval(WINDOW_CENTER_X - EYE_WIDTH/2,
                WINDOW_CENTER_Y + RADIUS/2 - EYE_WIDTH/2,
                EYE_WIDTH, EYE_WIDTH);

}

//
// Function: getCircleTangentXsubT
// Usage: double XsubT = getCircleTangentXsubT(radius, theta);
// ----------------------------------------------------------
// Returns the x-coordinate of a point along the perimeter of the
// circular yin-yang symbol at the polar coordinate (radius, theta).
//

double getCircleTangentXsubT(double rPixels, double thetaRadians) {
    return rPixels * cos(thetaRadians);
}

//
// Function getCircleTangentYsubT
// Usage: double YsubT = getCircleTangentYsubT(radius, theta);
// ----------------------------------------------------------
// Returns the y-coordinate of a point along the perimeter of the
// circular yin-yang symbol at the polar coordinate (radius, theta).
//

double getCircleTangentYsubT(double rPixels, double thetaRadians) {
    return rPixels * sin(thetaRadians);
}

//
// Function getTailchaserBodyX0
// Usage: double x0 = getTailchaserBodyX0(tcRadius, symbolRadius, thetaRadians);
// ---------------------------------------------------------------------------
// This function returns the x-coordinate of the origin to the bounding box for
// one tailchaser body circle tangent and interior to the larger symbol circle.
//
// The calling routine will typically iterate over a 180 degree arc starting
// in the north, at the top of the yin-yang symbol, and sweeping clockwise down
// to the south.  With each call to this routine:
//
//    1. The angle of interest is passed in.
//
//    2. A corresponding tangent is calculated where the tailchaser's body
//       touches the outer circle.
//
//    3. A smaller circle for the tailchaser's body at that tangent is 
//       modeled with a bounding box in the form of an origin coordinate 
//       which is calculated and returned.
//
// The translation between the tangent point and x-coordinate of the bounding
// box for circles making up the tailchaser's body varies sinusoidally from 
// 1 to 2 to 1 radial units as we sweep from north to east to south along the 
// outer symbol circle with theta.  Use some trigonometry to model that.
//

double getTailchaserBodyX0(double r, double R, double theta) {
    
    //
    // Point of tangency to the outer circle determines which circle segment
    // of the tailchaser's body we're locating.
    //
    
    double xsubT = getCircleTangentXsubT(R, theta);
    
    #ifdef DEBUGGING
    cout << "xsubT(R, theta) = xSubT(" << R << ", " << theta << ") = raw("
         << xsubT << ") = " << toXScreenCoord(xsubT) << endl;
    #endif
    
    //
    // We can derive the bounding box origin for the smaller interior circle
    // we're rendering from:
    //
    // 1. the radius of the tailchaser's head
    // 2. the place where the rendered circle is tangent to the larger circle
    //
    // Devise a radial scaling factor that varies from 1 to 2 to 1
    // as tangets to the symbol sweep from north to south.
    // (In the north, the tangent is 1 radial unit to the right of the x-coord
    // of the bounding box.  In the east, it's 2 units to the right.  In the 
    // south, the tangent is back to 1 unit away.)
    //
    
    double tcRadiusScaling = 1 + sin(theta + PI/2);
    
    #ifdef DEBUGGING
    cout << "radScaling(r, theta) = radScaling(" << r << ", " << theta << ") = "
         << tcRadiusScaling << endl;
    #endif
    
    //
    // Return the x-coordinate of the bounding box origin for the tailchaser's
    // body circle at tangent xsubT.
    //
    
    double translate = r * tcRadiusScaling;
    
    #ifdef DEBUGGING
    cout << "xlate(r, radScaling) = xlate(" << r << ", " << tcRadiusScaling
         << ") = " << translate << endl;
    
    cout << "raw(x0) = " << xsubT - translate << endl;
    cout << "x0 = " << toXScreenCoord(xsubT - translate) << endl;
    #endif

    return toXScreenCoord(xsubT - translate);
}

//
// Function getTailchaserBodyY0
// Usage: double y0 = getTailchaserBodyY0(tcRadius, symbolRadius, thetaRadians);
// -----------------------------------------------------------------------
// This function returns the y-coordinate of the origin to the bounding box for
// one tailchaser body circle tangent and interior to the larger symbol circle.
//
// The calling routine will typically iterate over a 180 degree arc starting
// in the north, at the top of the yin-yang symbol, and sweeping clockwise down
// to the south.  With each call to this routine:
//
//    1. The angle of interest is passed in.
//
//    2. A corresponding tangent is calculated where the tailchaser's body
//       touches the outer circle.
//
//    3. A smaller circle for the tailchaser's body at that tangent is
//       modeled with a bounding box in the form of an origin coordinate
//       which is calculated and returned.
//
// The translation between the tangent point and y-coordinate of the bounding
// box for circles making up the tailchaser's body varies sinusoidally from
// 0 to 2 radial units as we sweep clockwise from north to south along the
// outer symbol circle with theta.  Use some trigonometry to model that.
//

double getTailchaserBodyY0(double r, double R, double theta) {
    
    //
    // Point of tangency to the outer circle determines which circle segment
    // of the tailchaser's body we're locating.
    //
    
    double ysubT = getCircleTangentYsubT(R, theta);
    
    #ifdef DEBUGGING
    cout << "ysubT(R, theta) = ySubT(" << R << ", " << theta << ") = raw("
         << ysubT << ") = " << toYScreenCoord(ysubT) << endl;
    #endif

    //
    // We can derive the bounding box origin for the smaller interior circle
    // we're rendering from:
    //
    // 1. the radius of the tailchaser's head
    // 2. the place where the rendered circle is tangent to the larger circle
    //
    // Devise a radial scaling factor that varies from 0 to 2
    // as tangets to the symbol sweep from north to south.
    // (In the north, the tangent is 0 radial units below the y-coord
    // of the bounding box.  In the east, it's 1 unit below.  In the
    // south, the tangent is 2 radial units below.)
    //
    
    double tcRadiusScaling = 1 + cos(theta + PI/2);
    
    #ifdef DEBUGGING
    cout << "radScaling(r, theta) = radScaling(" << r << ", " << theta << ") = "
         << tcRadiusScaling << endl;
    #endif
    
    //
    // Return the y-coordinate of the bounding box origin for the tailchaser's
    // body circle at tangent xsubT.
    //
    
    double translate = r * tcRadiusScaling;
    
    #ifdef DEBUGGING
    cout << "ylate(r, radScaling) = ylate(" << r << ", " << tcRadiusScaling
         << ") = " << translate << endl;
    
    cout << "raw(y0) = " << ysubT - translate << endl;
    cout << "y0 = " << toYScreenCoord(ysubT + translate) << endl;
    #endif
    
    return toYScreenCoord(ysubT + translate);
}

//
// Function: toXScreenCoord
// Usage: double xSreenCoord = toXScreenCoord(xCartesianCoord);
// ------------------------------------------------------------
// This function converts a Cartesian position in the x-dimension to a 
// horizontal screen position for use by rendering primitives.
//
// We do most of the math assuming a conventional Cartesian coordinate system 
// with origin centered in the middle of the screen, but the Stanford graphics 
// package for rendering things like circles expects screen coordinates with 
// origin (0, 0) at the upper lefthand corner.
//

double toXScreenCoord(double xCartesianCoord) {
    double cartesianOrigin = WINDOW_CENTER_X;
    double screenCoord = cartesianOrigin + xCartesianCoord;

    return screenCoord;
}

//
// Function: toYScreenCoord
// Usage: double ySreenCoord = toYScreenCoord(yCartesianCoord);
// ------------------------------------------------------------
// This function converts a Cartesian position in the y-dimension to a vertical 
// screen position for use by rendering primitives.
//
// We do most of the math assuming a conventional Cartesian coordinate system 
// with centered in the middle of the screen, but the Stanford graphics package 
// for things like circles expects screen coordinates with origin (0, 0) at the 
// upper corner and positive y-axis pointing downward.
//

double toYScreenCoord(double yCartesianCoord) {
    double cartesianOrigin = WINDOW_CENTER_Y;
    double screenCoord = cartesianOrigin + (FLIP_Y_POLARITY) * yCartesianCoord;
    
    return screenCoord;
}

//
// Operator: color++
// Usage: SymbolColor nextColor = currentColor++;
// ----------------------------------------------
// Returns the opposite color of the current color.
//

SymbolColor operator++(SymbolColor& color, int) {
    SymbolColor old = color;
    color = SymbolColor((old + 1) % 2);
    return color;
}

string toString(SymbolColor color) {
    string strColor;
    
    switch (color) {
        case LIGHT:
            strColor = LIGHT_COLOR;
            break;
        case DARK:
            strColor = DARK_COLOR;
            break;
        default:
            strColor = "unknown_color";
            break;
    }
    return strColor;
}