//
// main.cpp
//
// This program helps illustrate the state of the heap & stack at the
// end of initPair.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 12, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file uses the code from Figure 12-15.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/13/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

/*
 State of heap/stack at breakpoint in initPair() below.
-------------------------------------------------------------------------------
    Heap                             Stack
    ----                             -----

                              +---------------+
                              |      *---------- dom ---------------+
                              +---------------+                     |
                          +----------*        |  list               |
+---------------+         |   +===============+                     |
|   2   |   1   | [1]     |   |  [initPair]   |                     |
+-------+-------+         |   +===============+                     |
|   1   |   2   | [0] <---+----------*        |  array          _   |
+-------+-------+             +---------------+  onetwo.leftDots |  |
leftDots rightDots            |  2   |   1    |  onetwo.rightDots|<-+
                              +===============+                 -
                              |    [main]     |
                              +===============+
-------------------------------------------------------------------------------
*/

#include <iostream>
#include "error.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 12.07\n";
const std::string DETAIL = "Code for Heap/Stack Diagram #1";
const std::string BANNER = HEADER + DETAIL;

// Types

struct Domino {
    int leftDots;
    int rightDots;
};

// Prototypes

void initPair(Domino list[], Domino & dom);

// Main program

int main() {
    cout << BANNER << endl;
    
    Domino onetwo;
    onetwo.leftDots = 1;
    onetwo.rightDots = 2;
    Domino *array = new Domino[2];
    initPair(array, onetwo);
    return 0;
}

// Function: initPair
// Usage: initPair(array, domino);
// -------------------------------
// Initializes a 2-element array of dominos.

void initPair(Domino list[], Domino & dom) {
    list[0] = dom;
    list[1].leftDots = dom.rightDots;
    list[1].rightDots = dom.leftDots;
    dom = list[1];
    cout << "break point" << endl;
}
