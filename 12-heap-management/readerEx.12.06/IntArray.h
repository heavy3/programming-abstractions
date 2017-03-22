//
// IntArray.h
//
// This file exports an interface for a simple integer array class
// that attempts to improve upon the standard C++ array by tracking
// array size and index values to avoid writing beyond the bounds of 
// the array.  Support for [] operator and deep copying are also 
// provided.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 12, Exercise 6
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/13/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#ifndef IntArray_h
#define IntArray_h

#include <iostream>
#include "error.h"

//
// Class: IntArray
// ---------------
// This class models an integer array that avoids the problem of out-of-bounds
// references of native arrays.  The fundamental operations of the this array
// are similar to native arrays, though they may not be copied nor assigned,
// severely limiting their practical benefit (for reasons of pedagogy).

class IntArray {
public:
    IntArray(int n);
    ~IntArray();
    IntArray(const IntArray & src);
    IntArray & operator=(const IntArray & rhs);
    int size() const;
    int get(int k) const;
    void put(int k, int value);
    int & operator[](int k);
    void deepcopy(const IntArray & src);

// ---------------
// Private Section
// ---------------
    
private:
    
// Private constants
    
    static const std::string E_OUT_OF_BOUNDS;
    
// Instance variables
    
    int * array;     // Pointer to heap-based array memory.
    int capacity;    // Max number of elements that array may hold.
};

#endif // IntArray_h
