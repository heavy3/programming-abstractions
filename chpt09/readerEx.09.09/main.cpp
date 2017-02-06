//
// main.cpp
//
// This program implements a recursive function:
//
//    bool formsDominoChain(Vector<Domino> & dominos);
//
// which returns true if it is possible to build a chain consisting of 
// every domino in the vector.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/3/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "set.h"
#include "vector.h"
#include "error.h"
#include "simpio.h"
#include "random.h"
#include "Dominos.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.09\n";
const std::string DETAIL = "Searching for a Domino Chain with recursion.";
const std::string BANNER = HEADER + DETAIL;
const int MAX_DOMINOS = 27;
const std::string PROMPT = "Number of dominos to draw (0 < # < 27)?: ";

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    Dominos dominos(DOUBLE6);   // Create a set of dominos.
    Domino d1(2,5), d2(5,5), d3(2,6);

    int drawNum;  // Number of dominos to draw from the boneyard.
    
    while ((drawNum = getInteger(PROMPT)) > 0) {
        if (drawNum > MAX_DOMINOS) break;
        
        Vector<Domino> hand = dominos.drawHand(drawNum);
        // Vector<Domino> hand; hand += d1; hand += d2; hand += d3;
        dominos.reportHand(hand);
        
        if (formsDominoChain(hand)) {
            cout << "Found chain: " << hand << endl;
        } else {
            cout << "No chain found for: " << hand << endl;
        }
    }
    return 0;
}