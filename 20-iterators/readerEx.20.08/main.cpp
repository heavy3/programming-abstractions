//
// main.cpp
//
// Implement the function:
//
//    template <typename IterType>
//    IterType max_element(IterType start, IterType end);
//
// from the <algorithm> library, which returns an iterator pointing to the
// largest element in the specified iterator range.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 08
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/4/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "error.h"
#include "vector.h"

using namespace std;

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 20.08\n";
const std::string DETAIL = "STL Programming: max_element()";
const std::string BANNER = HEADER + DETAIL;

typedef Vector<int> CollType;
typedef CollType::iterator IterType;

// Function prototypes

IterType max_element(IterType start, IterType end);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    CollType v;
    v += 8;
    v += 6;
    v += 7;
    v += 5;
    v += 3;
    v += 0;
    v += 9;
    v += 8;
    
    IterType it = max_element(v.begin(), v.end());
    
    cout << "Max element = " << *it << " from v = " << v << endl;
    return 0;
}

// Function: max_element
// Usage: IterType it = max_element(coll.begin(), coll.end());
// -----------------------------------------------------------
// Returns an iterator pointing to the maximum element within the bounds
// of a begin and end iterator for a collection.

IterType max_element(IterType start, IterType end) {
    IterType maxSoFar = start;
    
    for (IterType it = start; it < end; it++) {
        if (*it > *maxSoFar) {
            maxSoFar = it;
        }
    }
    return maxSoFar;
}
