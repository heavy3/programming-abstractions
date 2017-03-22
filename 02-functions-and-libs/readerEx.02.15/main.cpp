//
// main.cpp
//
// This programs simulates flipping a coin repeatedly and continues until 
// three consecutive heads are tossed. The program then displays total 
// number of of coin flips.
//
// For example:
//
//    heads
//    tails
//    heads
//    tails
//    heads
//    heads
//    heads
//    It took 7 flips to get 3 consecutive heads.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 15
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/23/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "random.h"

using namespace std;

// Constants

const int CONSECUTIVE_HEADS = 3;

// Function prototypes

int headsInARow(int desiredCount);

// Main program

int main() {
    
    int numFlips = headsInARow(CONSECUTIVE_HEADS);
    cout << "It took " << numFlips << " to get " << CONSECUTIVE_HEADS
         << " consecutive heads.";
    return 0;
}

//
// Function: headsInARow
// Usage: int numFlips = headsInARow(3);
// -------------------------------
// Functions returns the number of coin flips before the
// desired number of consecutive heads occured.
//

int headsInARow(int desiredCount) {
    int numFlips = 0;
    int winningStreak = 0;
    
    while (winningStreak < desiredCount) {
        numFlips++;
        if (randomChance(0.5)) {
            winningStreak++;
            cout << "heads" << endl;
        } else {
            winningStreak = 0;
            cout << "tails" << endl;
        }
    }
    
    return numFlips;
}