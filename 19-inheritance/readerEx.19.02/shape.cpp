//
// shape.cpp
//
// This program implements the Shape class hierarchy.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/23/17.
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//

#include "shape.h"

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
    gw.drawLine(x, y, x + dx, y + dy);
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
    gw.drawRect(x, y, width, height);
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
    gw.drawOval(x, y, width, height);
}
