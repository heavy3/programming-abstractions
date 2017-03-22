//
// sort.h
//
// This file exports a polymorphic interface and implementation of
// a Quicksort for all base types that supports the standard relational 
// operators.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file extends code from Figure 10-9.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/4/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#ifndef _sort_h
#define _sort_h

#include "Vector.h"

// Function: sort
// Usage: sort(vec);
// -----------------
// This function sorts the elements of the vector into increasing order using
// the Quicksort algorithm.

template <typename ValueType>
void sort(Vector<ValueType> & vec);

#include "sort.cpp"

#endif // _sort_h
