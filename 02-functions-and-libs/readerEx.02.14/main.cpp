//
// main.cpp
//
// This program computes an approximate value of pi using a Monte Carlo
// integration technique.
//
// For example, typical output looks like this:
//
//    -------------------------------------------
//    After 10000 trials, I estimate pi ~= 3.1352
//
// For estimates that are accurate to 3 decimal places, try 1 million trials.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 14
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/23/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cmath>
#include "random.h"
using namespace std;

// Constants

const unsigned NUM_TRIALS = 10000;  // Number of times the dart is thrown.

// Function prototypes

bool dartLandedInCircle();
double estimatePI(unsigned numTrials);

// Main function

int main() {
    
    cout << "After " << NUM_TRIALS << " trials, I estimate pi ~= "
         << estimatePI(NUM_TRIALS) << endl;
    
    return 0;
}

//
// Function: estimatePI
// Usage: double pi = estimatePI(10000);
// -------------------------------------
// Returns an estimated value of pi using a Monte Carlo integration technique
// that models the universe as someone repeatedly throwing darts at a 2x2 square
// dartboard which circumscribes a unit circle.
//
// By constraining the position of the dart on the playing field to random
// x and y values that cannot stray outside the square, we compute a
// probability that the dart's (x, y) position falls within the circle over a 
// suitably large number of trials.  That probability happens to approximate 
// pi / 4, allowing us to calcuate an approximate value of pi itself.
//
// Here's the underlying model that relates probability to pi:
//
// Area(unit_circle)       = pi * r^2 =  pi * 1 = pi
// Area(enclosing_square)  = length^2 =   2 * 2 = 4
//
// Probability(lands_in_circle) = Area(unit_circle)         pi
//                               ---------------------- = -----
//                               Area(enclosing_square)     4
//
// Computed probability, P, from many trials:
//
// P(lands_in_circle) = throws_that_landed_in_circle / total_throws
//
// Therefore pi ~= P(lands_in_circle) * 4
//

double estimatePI(unsigned numTrials) {
    int landedInCircle = 0;
    
    for (int throws = 1; throws <= numTrials; throws++) {
        if (dartLandedInCircle()) landedInCircle++;
    }
    
    return (landedInCircle / double(numTrials)) * 4.0;
}

//
// Function: dartLandedInCircle
// Usage: if (dartLandedInCircle()) landedInCircle++;
// ------------------------------------------------------
// This function simulates someone throwing a dart at of some unit length
// perfectly enclosed by a square centered about the origin of a cartesan
// coordinate system in their garage.
//
// Returns true if the simulated throw resulted in the dart landing
// within the inscribed circle (x^2 + y^2 <= 1^2)
//

bool dartLandedInCircle() {
    double x = randomReal(-1, 1);
    double y = randomReal(-1, 1);
    
    return ((x * x + y * y) <= 1);
}