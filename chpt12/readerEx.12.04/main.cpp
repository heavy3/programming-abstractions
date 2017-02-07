//
// main.cpp
//
// This program exercises an IntArray class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 12, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/12/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "error.h"
#include "IntArray.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 12.04\n";
const std::string DETAIL = "IntArray Class";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    IntArray ia(10);
    assert(ia.size() == 10);
    for (int i = 0; i < 10; i++) {
        assert(ia.get(i) == 0);
    }
    
    for (int i = 0; i < 5; i++) {
        ia.put(i, i+1);
        assert(ia.get(i) == i+1);
    }
    
    assert(ia.size() == 10);
    for (int i = 5; i < 10; i++) {
        assert(ia.get(i) == 0);
    }
    cout << "IntArray passed unit tests." << endl;

    // Try doing something that will force an error.
    // Eventually shizzle like this will be wrapped in try/catch block.
    
    ia.put(10, 11); // Try putting 11 at 11th slot.  Should throw error.
    return 0;
}