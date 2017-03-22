//
// Domino.h
// 
// This file exports a minimal interface that models a domino.
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

#ifndef Domino_h
#define Domino_h

#include <string>
#include <iostream>

class Domino {
public:
    Domino();
    Domino(int leftDots, int rightDots);
    
    int getLeftDots();
    int getRightDots();
    
    std::string toString();
    
private:
    int leftDots;
    int rightDots;
};

std::ostream & operator<<(std::ostream & os, Domino d);
#endif // Domino_h
