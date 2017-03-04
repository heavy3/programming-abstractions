//
// main.cpp
//
// This file contains a unit test driver for the Grid class.
//
// For example:
//
//    CS106B Programming Abstractions in C++: Ex 14.11
//    Vector-based Grid implementation.
//
//    1  2  3
//    4  5  6
//
//    1  2
//    3  4
//    5  6
//
//    Grid<int> unit test succeeded
//
// TODO: Add more assertion testing.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 11
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/3/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "grid.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 14.11\n";
const std::string DETAIL = "Vector-based Grid implementation.";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

bool runGridUnitTest(Grid<int> & g);

// Main program

int main() {
    std::cout << BANNER << std::endl << std::endl;
    
    // Test grid with character data.
    
    Grid<int> grid;
    Grid<int> grid2(2, 3);
    
    if (runGridUnitTest(grid)) {
        std::cout << "Grid<int> unit test succeeded" << std::endl;
    }
    std::cout << std::endl;
    return 0;
}

// Function: runGridUnitTest
// Usage: bool passed = runGridUnitTest(grid);
// -------------------------------------------------
// Returns true if unit test passes all assertions.

bool runGridUnitTest(Grid<int> & g) {
    g.resize(2, 3);
    int count = 1;
    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < 3; c++) {
            g.set(r, c, count++);
        }
    }
    assert(g.get(1, 2) == 6);
    
    // Exercise stream insertion operator overload.
    
    cout << g << endl;
    
    // Alter dimensions (which will reset values to default for int type).
    
    g.resize(3, 2);
    
    // Re-initialize new grid.
    
    count = 1;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 2; c++) {
            g.set(r, c, count++);
        }
    }
    cout << g << endl;
    
    // Exercise deep copy.
    
    Grid<int> newG = g;
    g.set(0, 0, -1);
    cout << newG << endl;
    return true;
}
