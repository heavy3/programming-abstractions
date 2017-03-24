//
// shapelist.cpp
//
// This file implements a ShapeList class that maintains a list of shapes.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/23/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include "shapelist.h"

static const int npos = -1; // Value returned if find() method fails.

// Methods: moveToFront, moveToBack, moveForward, moveBackward
// Usage: shapes.moveToFront(sp);
//        shapes.moveToBack(sp);
//        shapes.moveForward(sp);
//        shapes.moveBackward(sp);
// -------------------------------
// Changes the position of sp in the ShapeList.  The first two methods
// move the shape all the way to the specified end.  The last two move
// it one position in the indicated direction, if possible.  The method
// signals an error if sp is not in the ShapeList.

void ShapeList::moveToFront(Shape *sp) {
    int spi = find(sp);
    if (spi == -1) error("ShapeList::moveToFront: shape pointer not found");
    
    Shape *tmp = (*this)[spi];
    (*this).remove(spi);
    (*this).add(tmp);
}

void ShapeList::moveToBack(Shape *sp) {
    int spi = find(sp);
    if (spi == -1) error("ShapeList::moveToBack: shape pointer not found");
    
    Shape *tmp = (*this)[spi];
    (*this).remove(spi);
    (*this).insert(0, tmp);
}

void ShapeList::moveForward(Shape *sp) {
    int spi = find(sp);
    if (spi == -1) error("ShapeList::moveForward: shape pointer not found");
    
    if (spi < this->size() - 1) {
        Shape *tmp = (*this)[spi];
        (*this)[spi] = (*this)[spi+1];
        (*this)[spi+1] = tmp;
    }
}

void ShapeList::moveBackward(Shape *sp) {
    int spi = find(sp);
    if (spi == -1) error("ShapeList::moveBackward: shape pointer not found");
    
    if (spi > 0) {
        Shape *tmp = (*this)[spi];
        (*this)[spi] = (*this)[spi-1];
        (*this)[spi-1] = tmp;
    }
}

// Method: draw
// Usage: shapes.draw(gw);
// -----------------------
// Draws the shapes in the ShapeList on the graphics window.  The shapes
// are drawn from back to front, so that shapes closer to the front seem
// to cover those further back.

void ShapeList::draw(GWindow& gw) const {
    for (Shape *sp: *this) {
        sp->draw(gw);
    }
}

void ShapeList::free() {
    for (int i = 0; i < this->size(); i++) {
        delete (*this)[i];
    }
}

int ShapeList::find(Shape *sp) const {
    int spi = npos;
    for (int i = 0; i < this->size(); i++) {
        if (sp == (*this)[i]) {
            spi = i;
            break;
        }
    }
    return spi;
}
