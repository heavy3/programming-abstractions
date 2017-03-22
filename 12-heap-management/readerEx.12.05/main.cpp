//
// main.cpp
//
// This program exercises an IntArray class which overrides the [] bracket
// operator for more traditional array syntax.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 11, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/13/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "error.h"
#include "IntArray.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 12.05\n";
const std::string DETAIL = "IntArray Class plus [] operator";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    IntArray ia(10);
    IntArray ia2 = ia;
    assert(ia.size() == 10);
    for (int i = 0; i < 10; i++) {
        assert(ia.get(i) == 0);
        assert(ia[i] == 0);
    }
    
    for (int i = 0; i < 5; i++) {
        ia.put(i, i+1);
        assert(ia.get(i) == i+1);
        ia[i] = i + 1;
        assert(ia[i] == i+1);
    }
    
    assert(ia.size() == 10);
    for (int i = 5; i < 10; i++) {
        assert(ia.get(i) == 0);
        assert(ia[i] == 0);
    }
    
    assert(ia[1] == 2);
    ia[1] = 42;
    assert(ia[1] == 42);
    
    cout << "IntArray passed unit tests." << endl;
    return 0;
}