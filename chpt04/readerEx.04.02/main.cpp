//
// main.cpp
//
// This program uses stream manipulators to format a windchill table
// to look like this:
//
// -----------------------------------------------------------------------------
// Figure 2-17 Wind chill as a function of temperature and wind speed, v mph
// -----------------------------------------------------------------------------
// Temperature (deg F)
// (MPH)  40  35  30  25  20  15  10   5   0  -5 -10 -15 -20 -25 -30 -35 -40 -45
// v=  5  36  31  25  19  13   7   1  -5 -11 -16 -22 -28 -34 -40 -46 -52 -57 -63
// v= 10  34  27  21  15   9   3  -4 -10 -16 -22 -28 -35 -41 -47 -53 -59 -66 -72
// v= 15  32  25  19  13   6   0  -7 -13 -19 -26 -32 -39 -45 -51 -58 -64 -71 -77
// v= 20  30  24  17  11   4  -2  -9 -15 -22 -29 -35 -42 -48 -55 -61 -68 -74 -81
// v= 25  29  23  16   9   3  -4 -11 -17 -24 -31 -37 -44 -51 -58 -64 -71 -78 -84
// v= 30  28  22  15   8   1  -5 -12 -19 -26 -33 -39 -46 -53 -60 -67 -73 -80 -87
// v= 35  28  21  14   7   0  -7 -14 -21 -27 -34 -41 -48 -55 -62 -69 -76 -82 -89
// v= 40  27  20  13   6  -1  -8 -15 -22 -29 -36 -43 -50 -57 -64 -71 -78 -84 -91
// v= 45  26  19  12   5  -2  -9 -16 -23 -30 -37 -44 -51 -58 -65 -72 -79 -86 -93
// v= 50  26  19  12   4  -3 -10 -17 -24 -31 -38 -45 -52 -60 -67 -74 -81 -88 -95
// v= 55  25  18  11   4  -3 -11 -18 -25 -32 -39 -46 -54 -61 -68 -75 -82 -89 -97
// v= 60  25  17  10   3  -4 -11 -19 -26 -33 -40 -48 -55 -62 -69 -76 -84 -91 -98
// -----------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 4, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/16/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#define USE_LIB_ERROR // If defined, build against StanfordCPPLib/{error.h, libStanfordCPPLib.a}
                      // else build against locally defined error().

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <string>
#include <iomanip>


#ifdef USE_LIB_ERROR
#include "error.h"
#endif

// Constants and Types

// Function prototypes

double windChill(double t, double v);
void windChillTable();
#ifndef USE_LIB_ERROR
void error(std::string msg);
#endif

// Main program

int main(int argc, char * argv[]) {
    windChillTable();
    return 0;
}

// Function definitions

//
// Function: windChill
// Usage: perceivedTemp = windChill(temp, velocity);
// ----------------------------------------------------------
// Returns the wind chill, or perceived temperature,
// for a given temperature and wind velocity in mph.
// All temperatures are in Fahrenheit.
//
// Fails with an error message if passed-in temp is greater
// than 40 degrees.
//

double windChill(double t, double v) {
    double wc = 0.0;
    if (v == 0.0)
        wc = t;
    else if (t > 40)
        error("Wind Chill is undefined for temperatures greater than 40 deg F.");
    else
        wc = 35.74 + 0.6215 * t - 35.75 * pow(v, 0.16)
                   + 0.4275 * t * pow(v, 0.16);
    return wc;
}

#ifndef USE_LIB_ERROR

//
// Function: error
// Usage: error("I died for some reason.");
// ----------------------------------------
// Writes an error message to standard error and halts execution.

void error(std::string msg) {
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}

#endif

//
// Function: windChillTable
// Usage: windChillTable();
// ------------------------
// Generate a table of wind chill temperatures across a range of
// temperatures and wind velocities.
//

void windChillTable() {
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
    std::cout << "   Figure 2-17 Wind chill as a function of temperature and wind speed, v mph"  << std::endl;
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
    std::cout << "                           Temperature (deg F)" << std::endl;
    
    std::cout << "(MPH)";
    for (int t = 40; t >= -45; t -= 5) {
        std::cout << std::setw(4) << t;
    }
    std::cout << std::endl;
    
    for (int v = 5; v <= 60; v += 5) {
        std::cout << "v=" << std::setw(3) << v << " ";
        for (int t = 40; t >= -45; t -= 5) {
            double wc = windChill(t, v);
            std::cout << std::setw(3) << std::fixed << std::setprecision(0)
                      << wc << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
}
