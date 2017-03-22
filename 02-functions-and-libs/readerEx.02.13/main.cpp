//
// main.cpp
//
// This program simulates radioactive decay of a sample of 10,000 atoms
// of material.  Each atom has a 50 percent chance of decaying in a year.
// The output shows the number of atoms remaining at the end of successive
// years.
//
// For example:
//
//    There are 10000 atoms initially.
//    There are 4957 atoms at the end of year 1.
//    There are 2484 atoms at the end of year 2.
//    ...
//    There are 2 atoms at the end of year 11.
//    There are 0 atoms at the end of year 12.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 13
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/22/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "random.h"

using namespace std;

// Constants and Types

const int SAMPLE_SIZE = 10000;

// Function prototypes

int halfLifeAtoms(int& sampleSize);

// Main program

int main() {
    int remainingAtoms = SAMPLE_SIZE;
    int year = 0;
    
    cout << "There are " << SAMPLE_SIZE << " atoms initially." << endl;
    while (halfLifeAtoms(remainingAtoms)) {
        cout << "There are " << remainingAtoms << " atoms at the end of year "
             << ++year << "." << endl;
    }
    cout << "There are " << remainingAtoms << " atoms at the end of year "
         << ++year << "." << endl;
    return 0;
}

//
// Function: halfLifeAtoms
// Usage: halfLifeAtoms(sampleSize);
// ---------------------------------
// Returns the number of atoms that remain in a sampleSize after a decay
// period.  Each atom has a 50% chance of decay in a given period.
//

int halfLifeAtoms(int & sampleSize) {
    int startSize = sampleSize;
    
    //
    // Iterate across all the samples and see which have
    // decayed to nothingness.  Update the remaining sample size
    // accordingly.
    //

    for (int sample = 1; sample <= startSize; sample++) {
        if (randomChance(0.5)) {
            --sampleSize;
        }
    }
    
    return sampleSize;
}