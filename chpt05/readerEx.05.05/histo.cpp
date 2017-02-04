//
// histo.cpp
//
// This file implements the interface exported by histo.h.  It renders
// a character-based histogram of integer data stored in a vector.
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

#include "histo.h"

// Constants and types

extern const char HISTO_SYMBOL = '*';

// Private function prototypes

string histochars(Vector<int>, 
                    int lowerBound, 
                    int delta, 
                    char symbol = HISTO_SYMBOL);

// Function definitions

//
// Implementation notes: histogram
// -------------------------------
// Iterates across a range of displayable subdivisions, looking for data
// from the input vector that corresponds to a given division.  Much of the work
// is farmed out to a private method called histochars() which returns
// the histogram string or characters for a given subdivision, representing
// each in-range integer from the vector with a '*' (or other specified symbol).
//
// The maximum value is handled as an edge case with it's own single-value
// display range for data equal to that value.
//
// Optional headers and footers may be passed in for use as titles or
// captions.
//

void histogram(Vector<int>& data, 
                int minVal, 
                int maxVal, 
                int delta,
                char symbol, 
                string header, 
                string footer) {
    
    if (header.length()) {
        cout << endl << header << endl << endl;
    }
    
    for (int i = minVal; i < maxVal; i += delta) {
        cout << setfill('0') << setw(2) << i << "s: ";
        cout << histochars(data, i, delta) << endl;
    }
    cout << maxVal << ": " << histochars(data, maxVal, delta) << endl;

    if (footer.length()) {
        cout << endl << footer << endl;
    }
}

//
// Implementation notes: histochars
// --------------------------------
// Returns a string of stars (or whatever character symbol is specified) 
// representing the number of integers in the data vector which fall within 
// the half-open interval:
//
//    lowerBound <= val < lowerBound + delta
//
// For example, all values from 0 to 9 found in the data would be represented
// with a star character in the returned string given a lower bound of 0 and
// a range delta of 10 units.
//

string histochars(Vector<int> data, int lowerBound, int delta, char symbol) {
    string stars;
    
    for (int val: data) {
        if ((val >= lowerBound) && (val < (lowerBound + delta))) {
            stars += symbol;
        }
    }
    return stars;
}