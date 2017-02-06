//
// Domino.h
//
// This file exports an interface that models a domino.
//
// It introduces the notion of rank and provides a full set of
// comparison operators.  This opens up the possibility of modeling
// a collection of dominos with a set.
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

extern const int DOMINO_RADIX;

class Domino {
public:
    Domino();
    Domino(int leftDots, int rightDots);
    
    int getLeftDots();
    int getRightDots();
    int getRank();
    
    std::string toString();
    
private:
    int leftDots;
    int rightDots;
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
