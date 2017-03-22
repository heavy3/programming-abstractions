//
// Domino.cpp
//
// This file implements a minimal class that models a single domino.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 12/3/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <sstream>
#include "Domino.h"

Domino::Domino() {
    leftDots = 0;
    rightDots = 0;
}

Domino::Domino(int lDots, int rDots) {
    leftDots = lDots;
    rightDots = rDots;
}

int Domino::getLeftDots() {
    return leftDots;
}

int Domino::getRightDots() {
    return rightDots;
}

std::string Domino::toString() {
    std::ostringstream os;
    
    os << "[" << leftDots << "|" << rightDots << "]";
    return os.str();
}

std::ostream & operator<<(std::ostream & output, Domino d) {
    output << d.toString();
    return output;
}
