//
// main.cpp
//
// This program computes an approximation to the constant, pi,
// using the area of a quarter circle that has been approximated by
// subdivision into 10,000 rectangles which are summed.  The midpoint of 
// each rectangle intersects with the arc of the circle.  The 
// height of the rectangle for a given position x along the horizontal 
// axis is given by:
//
//    h(x) = sqrt(r^2 - x^2), where r = radius of the circle.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 1, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/14/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <cmath>
#include <iostream>
using namespace std;

const int NUM_RECTANGLES = 10000;
const int RADIUS = 2;
const double RECTANGLE_WIDTH = RADIUS / double(NUM_RECTANGLES);

double height(double x);

int main(int argc, char * argv[]) {
    double area = 0;
    double midPoint;
    double midPointHeight;
    
    for (int i = 1; i <= NUM_RECTANGLES; i++) {
        midPoint = (2/double(NUM_RECTANGLES))*(i - 0.5);
        midPointHeight = height(midPoint);
        area += RECTANGLE_WIDTH * midPointHeight;
    }
    
    cout << "Approximating pi by the area summation of " << NUM_RECTANGLES << " rectangles that subdivide a quarter circle yields" << endl;
    cout << "pi ~= " << area << endl;
    
    return 0;
}

// Returns height of a point along a circle provided
// the x coordinate is positive and less than or equal
// to the radius of the circle.

double height(double x) {
    return (sqrt(RADIUS*RADIUS - x*x));
}
