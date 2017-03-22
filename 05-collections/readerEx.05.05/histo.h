//
// histo.h
//
// This file provides an interface for displaying a character-based histogram 
// for integer data.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/27/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#ifndef _histo_h
#define _histo_h

#include <iostream>
#include <string>
#include <iomanip>
#include "vector.h"
using namespace std;

// Constants and types

extern const char HISTO_SYMBOL; // Default histogram char ('*' typically)

//
// Function: histogram
// Usage: histogram(dataVector, minVal, maxVal, delta, histoSymbol);
// -----------------------------------------------------------------
// Displays a simple histogram representation of integers to the console.
// Data are passed in as a vector of integers along with the minimum value,
// maximum value, and delta value that specifies the size of one subdivision.
//
// A typical chart looks like this:
//
// Histogram 1
//
// 00s:
// 10s:
// 20s:
// 30s:
// 40s: *
// 50s: *
// 60s:
// 70s: **
// 80s: *****
// 90s: **
// 100: *
//
// Footer
//

void histogram(Vector<int>& data, int minVal, int maxVal, int delta,
               char histoSymbol = HISTO_SYMBOL, string header = "",
               string footer = "");

#endif