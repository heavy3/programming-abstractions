//
// main.cpp
//
// This program calculates the expected distance between the sun and
// the kth planet according to the rule proposed by the 18th century
// astronomer, Johann Daniel Titius (known as the Titius-Bode Law).
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 7, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/28/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "simpio.h"
#include "strlib.h"
#include "Vector.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 7.03\n";
const std::string DETAIL = "Titius-Bode Planetary Distance Calculation";
const std::string BANNER = HEADER + DETAIL;

// Planets known at the time the T-B Distance Calculation was formulated in
// the 18th century.  Pad the array with an entry at the beginning to
// allow 1-based referencing that follows planet numbering convention.

const std::string PlanetNames[] = {
    "pad", "Mercury", "Venus", "Earth", "Mars", "Asteroids", "Jupiter", "Saturn", "Uranus" };

enum PlanetNumT {
    Mercury = 1,
    Venus,
    Earth,
    Mars,
    Asteroids,
    Jupiter,
    Saturn,
    Uranus
};

float getTitiusBodeDistance(PlanetNumT k);
unsigned int getBodeSequenceValue(PlanetNumT k);
PlanetNumT operator++(PlanetNumT & n, int);

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    std::cout << "Planet    "   << " Distance (AU)*" << std::endl;
    std::cout << std::string(10,'-') << " " + std::string(13,'-') << std::endl;
    
    for (PlanetNumT k = Mercury; k <= Uranus; k++) {
        float auDist = getTitiusBodeDistance(k);
        std::cout << setw(10) << left << PlanetNames[k] << " "
                  << setprecision(1)  << std::fixed
                  << setw(5) << right << auDist << std::endl;
    }
    
    std::cout << std::endl << "* 1 Astronomical Unit (AU) = ~ 93 million miles" << std::endl;
    return 0;
}

//
// Function: getTitiusBodeDistance(PlanetNumT n)
// Usage: float d = getTitiusBodeDistance(3); // Avg dist from Sun to Earth in
//                                            // Astronomical Units (AU)
//                                            // 1 AU ~= 93 million miles
// -------------------------------------------------------------------------
// This function returns the approximate astronomical units between the Sun
// and a given planet according to the Titius-Bode Law,
//
//    d(k) = (4 + b(k)) / 10, 
//
//           where   k  = planet number (1 = closest to Sun)
//                 b(k) = sequence value (see getBodeSequenceValue for details)
//

float getTitiusBodeDistance(PlanetNumT kthPlanet) {
    unsigned int b_subk = getBodeSequenceValue(kthPlanet);
    return (4 + b_subk) / 10.0;
}

//
// Function: getBodeSequenceValue
// Usage: unsigned int b = getBodeSequenceValue(3);
// ------------------------------------------------
// Returns the sequence number required for the Titius-Bode distance
// calculation defined by the following recurrence relation:
//
//     b(k) = 1           if k = 1
//          = 3           if k = 2
//          = 2 * b(k-1)  if k > 2
//

unsigned int getBodeSequenceValue(PlanetNumT k) {
    if (k == 0) return 0;
    if (k == 1) return 1;
    if (k == 2) return 3;
    
    return 2 * getBodeSequenceValue(PlanetNumT(k - 1));
}

//
// Function: ++ (PlanetNumT suffix operator)
// Usage: for (PlanetNumT pn = Mercury, pn <= Uranus, pn++) ...
// ------------------------------------------------------------
// Allow for enumeration across the known planets at the time
// the Titius-Bode equation was formulated.
//

PlanetNumT operator++(PlanetNumT & n, int) {
    PlanetNumT old = n;
    n = PlanetNumT(n + 1);
    return old;
}
