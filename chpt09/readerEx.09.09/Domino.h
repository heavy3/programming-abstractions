//
// Domino.h
//
// This file exports an interface that models a domino.
// It further elaborates the previous domino class of 
// exercise 6.1.a to include:
//
//    Domino.chainsTo()
//    Domino.flipped()
//    Domino.getRadix()
//    compareDominos()
//
// These will be useful in exercise 9.9, where we approximate
// game play by forming chains of dominos from a collection.
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

#ifndef Domino_h
#define Domino_h

#include <string>
#include <iostream>
#include "vector.h"

extern const int DFLT_DOMINO_RADIX;

class Domino {
public:
    Domino(int radix = DFLT_DOMINO_RADIX);
    Domino(int leftDots, int rightDots, int radix = DFLT_DOMINO_RADIX);
    
    int getLeftDots();
    int getRightDots();
    int getRadix();
    int getRank();
    Domino flipped();
    bool chainsTo(Vector<Domino> dominos, int & index);
    
    std::string toString();
    
private:
    int leftDots;
    int rightDots;
    int radix;
};

std::ostream & operator<<(std::ostream & os, Domino d);
bool operator==(Domino d1, Domino d2);
bool operator!=(Domino d1, Domino d2);
bool operator<(Domino d1, Domino d2);
bool operator<=(Domino d1, Domino d2);
bool operator>(Domino d1, Domino d2);
bool operator>=(Domino d1, Domino d2);
int compareDominos(Domino d1, Domino d2);

#endif // Domino_h
