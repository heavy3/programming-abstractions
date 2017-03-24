//
// shape.cpp
//
// This program implements the Shape class hierarchy.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/23/17.
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//

#include "shape.h"
#include <cmath>

// Class: Shape (abstract)

void Shape::setLocation(double x, double y) {
    this->x = x;
    this->y = y;
}

void Shape::move(double x, double y) {
    this->x += x;
    this->y += y;
}

void Shape::setColor(std::string color) {
    this->color = color;
}

Shape::Shape() {
    color = "BLACK";
    x = y = 0;
}

Shape::Shape(double x, double y, std::string color) {
    this->color = color;
    this->x = x;
    this->y = y;
}

// Class: Line

Line::Line(double x1, double y1, double x2, double y2, std::string color):
                            Shape(x1, y1, color) {
    this->dx = x2 - x1;
    this->dy = y2 - y1;
}

void Line::draw(GWindow& gw) {
    gw.setColor(color);
    gw.drawLine(x, y, x + dx, y + dy);
}

// Function: contains
// Usage: if (line.contains(x, y)) { . . . }
// -----------------------------------------
// Returns true if a coordinate is proximate to a line
// (within the pixel proximity specified).

bool Line::contains(double x, double y, double pixelProximity) const {
    double dist = distToLine(x, y);
    return (abs(dist) <= pixelProximity);
}

// Function: distToLine
// Usage: double dist = distToLine(x, y);
// --------------------------------------
// Returns the distance from a point to a line.
//
// See:
// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line

double Line::distToLine(double x, double y) const {
    double dist;
    double x1 = this->x;
    double y1 = this->y;
    double x2 = this->x + dx;
    double y2 = this->y + dy;
    
    double n = abs((dy)*x - (dx)*y + x2*y1 - y2*x1);
    double d = sqrt(pow(dy, 2) + pow((dx),2));
    if (d == 0) error("distToLine: Not a line");
    
    dist = n / d;
    return dist;
}

// Class: Rect

Rect::Rect(double x,
           double y,
           double width,
           double height,
           std::string color): Shape(x, y, color) {
    this->width = width;
    this->height = height;
}

void Rect::draw(GWindow& gw) {
    gw.setColor(color);
    gw.fillRect(x, y, width, height);
}

bool Rect::contains(double x, double y, double pixelProximity) const {
    return ((this->x <= x) && (this->x + width >= x) &&
            (this->y <= y) && (this->y + height>= y));
}

// Class: Square

Square::Square(double x,
               double y,
               double size,
               std::string color): Rect(x, y, size, size, color) {
    // Empty
}

// Class: Oval

Oval::Oval(double x,
           double y,
           double width,
           double height,
           std::string color): Shape(x, y, color) {
    this->width = width;
    this->height = height;
}

void Oval::draw(GWindow& gw) {
    gw.setColor(color);
    gw.fillOval(x, y, width, height);
}

// Function: contains
// Usage: if (oval.contains(x, y)) { . . . }
// -----------------------------------------
// Returns true if a given coordinate resides within an oval.
//
// See:
// http://math.stackexchange.com/questions/76457/check-if-a-point-is-within-an-ellipse

bool Oval::contains(double x, double y, double pixelProximity) const {
    double rx = width/2;
    double ry = height/2;
    
    double h = this->x + rx;
    double k = this->y + ry;
    
    double e = pow(x-h, 2)/pow(rx,2) + pow(-1*(y-k), 2)/pow(ry,2);
    return (e <= 1);
}

// Class: Circle

Circle::Circle(double x,
               double y,
               double radius,
               std::string color):
            Oval(x - radius, y - radius, 2*radius, 2*radius, color) {
    // Empty
}
