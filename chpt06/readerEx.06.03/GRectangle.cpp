//
// GRectangle.cpp
//
// This file implements the GRectangle.h interface.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 12/7/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <sstream>
#include "GRectangle.h"

//
// Implementation notes: Constructors
// ----------------------------------
// Creates a GRectangle object with the specified components.  If these
// parameters are not supplied, the default constructor sets these fields
// to 0.
//

GRectangle::GRectangle() {};

GRectangle::GRectangle(double xc, double yc, double w, double h) {
    x = xc;
    y = yc;
    width = w;
    height = h;
}

//
// Implementation notes: accessors
// -------------------------------
// Returns the values of private instance variables of a GRectangle.
//

double GRectangle::getX() const {
    return x;
}

double GRectangle::getY() const {
    return y;
}

double GRectangle::getWidth() const {
    return width;
}

double GRectangle::getHeight() const {
    return height;
}

//
// Method: isEmpty
// Usage: if (r.isEmpty()) ...
// ---------------------------
// Returns true if the rectangle is empty (or not :).
//

bool GRectangle::isEmpty() const {
    return true;
}

//
// Method: contains
// Usage: if (r.contains(x, y)) ...
// --------------------------------
// Returns true if the rectangle contains the given point
// specified as distinct coordinates.
//

bool GRectangle::contains(double xc, double yc) const {
    double x2 = x + width;
    double y2 = y + height;
    
    return ((xc >= x) && (xc <= x2) && (yc >= y) && (yc <= y2));
}

//
// Implementation notes: toString
// ------------------------------
// Using an output string stream to convert GRectangle instance data
// into a string form.
//

std::string GRectangle::toString() const {
    std::ostringstream os;
    
    os << "(" << x << ", " << y << ", " << width << ", " << height << ")";
    return os.str();
}

//
// Implementation notes: operator<<
// --------------------------------
// Overloads the stream insertion operator to support displaying
// GRectangles.
//

std::ostream & operator<<(std::ostream & os, GRectangle rect) {
    return os << rect.toString();
}
