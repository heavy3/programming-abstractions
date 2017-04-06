//
// main.cpp
//
// Reimplement the operator==() method for the Set class to use map iteration.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/6/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cassert>
#include "error.h"
#include "set.h"
using namespace std;

// Constants and types

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 20.12\n";
const std::string DETAIL = "Reimplement Set<type> operator==() method";
const std::string BANNER = HEADER + DETAIL;

// Function prototypes

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    Set<int> s1, s2;
    
    s1.add(8);s1.add(6);s1.add(7);s1.add(5);s1.add(3);s1.add(0);s1.add(9);
    s2.add(8);s2.add(6);s2.add(7);s2.add(5);s2.add(3);s2.add(0);
    
    assert(s1 != s2);
    s2.add(9);
    assert(s1 == s2);
    
    cout << "All unit tests passed." << endl;
    return 0;
}
