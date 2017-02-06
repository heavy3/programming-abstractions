//
// Dominos.cpp
//
// This implements a Dominos class.
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

#include "Dominos.h"

//
// Constructor: Dominos
// Usage: Dominos dominos();
//        Dominos dominos(DOUBLE6);
// --------------------------------
// Instantiates a Dominos object.  Nominally, this embodies a set of dominos
// from which domino hands can be drawn for game play.
//

Dominos::Dominos(DominosSetT theSetType) {
    setType = theSetType;
    dominos = createSet(setType);
}

//
// Function: drawHand
// Usage: Vector<Domino> hand = drawHand(dominoSet, 7);
// ----------------------------------------------------
// Returns a vector of randomly selected dominos from a set of dominos.
// The number to draw is a parameter option.
//

Vector<Domino> Dominos::drawHand(int num) {
    Vector<Domino> hand;
    Vector<Domino> boneyard;
    
    if (num > 0) {
        for (Domino d : dominos) boneyard += d;
        for (int i = 0; i < num; i++) {
            int r = randomInteger(0, boneyard.size() - 1);
            hand.add(boneyard[r]);
            boneyard.remove(r);
        }
    }
    return hand;
}

//
// Function: reportHand
// Usage: reportHand(hand);
// ------------------------
// Displays the hand of dominos to permute on the output stream.
//

void Dominos::reportHand(Vector<Domino> hand) {
    string be, bones;
    
    if (hand.size() == 1) {
        be = "is";
        bones = "domino";
    } else {
        be = "are";
        bones = "dominos";
    }
    cout << "Here " << be << " " << hand.size() << " randomly selected ";
    cout << bones << ":" << endl << endl;
    cout << hand << endl << endl;
}

//
// Method: toString
// Usage: std::string str = toString(dominoSet);
// ---------------------------------------------
// Returns a string-form of a Dominos.
//

std::string Dominos::toString() {
    std::string str;
    
    for (Domino d : dominos) {
        str += d.toString() + " ";
    }
    return str;
}

//
// Method: getSetType
// Usage: DominosSetType setType = getSetType();
// ---------------------------------------------
// Domino sets come in multiple flavors (e.g., Double-6, Double-9).
// Returns the set type.
//

DominosSetT Dominos::getSetType() {
    return setType;
}

//
// Operator: <<
// Usage: cout << dominoSet << endl;
// ---------------------------------
// Displays a set of dominos on the output stream.
//

ostream & operator<<(ostream & os, Dominos d) {
    os << d.toString();
    return os;
}

//
// Function: createSet
// Usage: Set<Domino> bones = createSet();
//        Set<Domino> bones = createSet(DOUBLE9);
// ----------------------------------------------
// Returns a set of dominos.  A standard 'Double 6' set
// is returned if no set type or an unrecognized set type
// is specified.
//

Set<Domino> Dominos::createSet(DominosSetT setType) {
    Set<Domino> results;
    int radix;
    
    switch (setType) {
        case DOUBLE9:
            radix = 10;
            break;
            
        case DOUBLE12:
            radix = 13;
            break;
            
        case DOUBLE6:
        default:
            radix = 7;
            break;
    }
    
    for (int left = 0; left < radix; left++) {
        for (int right = left; right < radix; right++) {
            Domino d(left, right, radix);
            results.add(d);
        }
    }
    
    return results;
}

//
// Function: formsDominoChain
// Usage: if (formsDominoChain(someDominos)) { . . . }
// ---------------------------------------------------
// Returns true if a collection of dominos can be strung together in a chain.
//

bool formsDominoChain(Vector<Domino> & dominos) {
    bool result = false;
    Vector<Domino> chain;
    Vector<Domino> unordered(dominos);
    
    if ((result = foundChain(chain, unordered))) {
        dominos = chain;
    }
    return result;
}

//
// Function: foundChain
// Usage: if ((result = foundChain(chain, unorderedDominos)) { . . . }
// -------------------------------------------------------------------
// Returns true if a vector of unordered dominos, represented by
// the 'left' parameter, can be strung together in a chain.
// In that case, the ordered dominos are returned in a pass-by-reference
// vector called 'sofar'.
//
// Recursion is used to explore potential permutations of dominos
// until the first workable arrangement is found or all
// arrangements are exhausted with no solution.
//

bool foundChain(Vector<Domino> & sofar, Vector<Domino> left) {

    // Base case: no unordered dominos to sort
    
    if (left.size() == 0) {
        return true;
    }
    
    // Recursive case
    
    // cout << "sofar: " << solution << endl;
    // cout << "left:  " << remaining << endl;
    for (int i = 0; i < left.size(); i++) {
        Domino nthDomino = left[i];
        for (Domino d : permutesOf(nthDomino)) {
            int sIndex;
            if (d.chainsTo(sofar, sIndex)) {
                sofar.insert(sIndex, d);
                left.remove(i);
                if (foundChain(sofar, left)) {
                    return true;
                }
                left.insert(i, nthDomino);
                sofar.remove(sIndex);
            }
        }
    }

    // Trigger recursive backtracking or fail if all permutes
    // have been tried with no solution.

    return false;
}

//
// Function: permutesOf
// Usage: for (Domino d : permutesOf(nthDomino)) { . . . }
// -------------------------------------------------------------------
// Returns a vector of the two unique orientations of a given
// domino.
//

Vector<Domino> permutesOf(Domino d) {
    Vector<Domino> result;
    
    result += d;
    result += d.flipped();
    
    return result;
}