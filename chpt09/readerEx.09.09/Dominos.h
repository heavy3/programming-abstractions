//
// Dominos.h
//
// This file exports an interface that models a set of dominos.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/3/16
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#ifndef Dominos_h
#define Dominos_h

#include <iostream>
#include "Domino.h"
#include "set.h"
#include "random.h"

enum DominosSetT {
        DOUBLE6,
        DOUBLE9,
        DOUBLE12
};

class Dominos {
    
public:
    Dominos(DominosSetT setType = DOUBLE6);
    
    Vector<Domino> drawHand(int num = 7);
    void reportHand(Vector<Domino> hand);
    std::string toString();
    DominosSetT getSetType();
    
private:
    Set<Domino> dominos;
    DominosSetT setType;
    Set<Domino> createSet(DominosSetT setType = DOUBLE6);
};

ostream & operator<<(ostream & os, Dominos d);
bool formsDominoChain(Vector<Domino> & dominos);
bool foundChain(Vector<Domino> & chain, Vector<Domino> unordered);
Vector<Domino> permutesOf(Domino d);

#endif // Dominos_h