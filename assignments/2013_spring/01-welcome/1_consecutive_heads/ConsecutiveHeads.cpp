//
// ConsecutiveHeads.cpp
//
// This program simulates flipping a coin repeatedly until three consecutive
// heads are tossed.  At that point, the program displays the total number of
// coin flips made.  The results of each coin flip and total coin
// flip count are written to the console.
//
// For example:
//
//     ---------------------------------------------
//     heads
//     tails
//     heads
//     tails
//     heads
//     heads
//     heads
//     It took 7 flips to get 3 consecutive heads.
//
// --------------------------------------------------------------------------
// Assignment: 1, Part 1 Consecutive Heads
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/050%20Assignment%201.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/14/2015.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "random.h"
using namespace std;

// Constants and types

enum CoinFlip {
    heads,
    tails
};

// Function prototypes

CoinFlip flipCoin();
unsigned flipTrials(CoinFlip desiredFlip, unsigned desiredConsecutive);
void show(CoinFlip outcome);
void show(unsigned totalFlips, CoinFlip desiredFlip, unsigned consecutiveFlips);

// Main program

int main() {
    unsigned consecutive = 3;
    CoinFlip desiredFlip = heads;
    
    unsigned count = flipTrials(desiredFlip, consecutive);
    show(count, desiredFlip, consecutive);
    
    return 0;
}

// Function definitions

// Function: flipCoin
// Usage: CoinFlip outcome = flipCoin();
// -------------------------------------
// Returns heads or tails outcome from simulated coin flip modeled
// with a pesudo-random number generator that returns true 50% of
// the time to indicate heads.

CoinFlip flipCoin() {
    return (randomChance(0.5)) ? heads : tails;
}

//
// Function: flipTrials
// Usage: unsigned totalFlips = flipTrials(heads, 3);
// ---------------------------------------------
// Returns the total number of coin flips until a desired outcome is
// reached n-times in a row.
//

unsigned flipTrials(CoinFlip desiredFlip, unsigned desiredConsecutive) {
    unsigned totalFlips = 0;
    unsigned consecutive = 0;
    CoinFlip outcome;
    
    if (desiredConsecutive == 0) {
        return 0;
    }
    
    while (consecutive < desiredConsecutive) {
        outcome = flipCoin();
        show(outcome);
        (outcome == desiredFlip) ? consecutive++ : consecutive = 0;
        totalFlips++;
    }
    
    return totalFlips;
}

//
// Function: show
// Usage: show(heads); // writes "heads" to console
// ------------------------------------------------
// Writes the outcome of a coin flip to the console along with a newline.
//

void show(CoinFlip outcome) {
    switch (outcome) {
        case heads:
            cout << "heads" << endl;
            break;
           
        case tails:
            cout << "tails" << endl;
            break;
            
        default:
            cout << "?" << endl;
            break;
    }
}

//
// Function: show
// Usage: show(totalFlips, desiredFlip, consecutiveCount);
// -------------------------------------------------------
// Reports the results of the coin flip trials, indicating how many
// simulated tosses were performed before the desired number of
// consecutive identical outcomes occurred.
//
// Output is sent to the console.
//

void show(unsigned totalFlips, CoinFlip desiredFlip,
          unsigned consecutiveFlips) {

    string flipStr = (desiredFlip == heads) ? "heads" : "tails";
    
    cout << "It took " << totalFlips << " flips to get "
         << consecutiveFlips << " consecutive " << flipStr << "." << endl;
}
