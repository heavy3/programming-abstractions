//
// Domino.cpp
//
// This file implements a domino class.  It includes comparison
// operators, allowing a client of the class to model a collection
// of domios with a set.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 1.b
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 12/4/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <sstream>
#include "Domino.h"

extern const int DOMINO_RADIX = 7;

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

int Domino::getRank() {
    int msd;
    int lsd;
    
    // 
    // When ranking a domino, adopt the convention that the
    // side with the fewest dots is the most significant digit
    // while the other side is the least significant digit.
    //
    // We do this to ensure, for example, that [6|0] and [0|6] have 
    // the same computed rank.
    //
    // (Huh ... we could enforce this in the constructor since a 
    //  domino models nicely as an immutable object.)
    //
    
    if (leftDots < rightDots) {
        msd = leftDots;
        lsd = rightDots;
    } else {
        msd = rightDots;
        lsd = leftDots;
    }
    
    // Return the unique, ranked value for this domino (e.g., 0 to 27).
    
    return msd * DOMINO_RADIX + lsd;
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

bool operator==(Domino d1, Domino d2) {
    return d1.getRank() == d2.getRank();
}

bool operator<(Domino d1, Domino d2) {
    return d1.getRank() < d2.getRank();
}

bool operator<=(Domino d1, Domino d2) {
    return d1.getRank() <= d2.getRank();
}

bool operator>(Domino d1, Domino d2) {
    return d1.getRank() > d2.getRank();
}

bool operator>=(Domino d1, Domino d2) {
    return d1.getRank() >= d2.getRank();
}

bool operator!=(Domino d1, Domino d2) {
    return !(d1 == d2);
}

int compareDominos(Domino d1, Domino d2) {
    if (d1 < d2) return -1;
    if (d1 == d2) return 0;
    return 1;  // d1 > d2
}
