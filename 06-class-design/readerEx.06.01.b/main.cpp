//
// main.cpp
//
// This program uses the Domino class to create a set of dominos.
//
// For example:
//
//    CS106B Programming Abstractions in C++: Chpt 6, Ex 1.b
//    Printing a set of dominos modeled with a class
//
//    [6|6]  [5|6]  [5|5]  [4|6]  [4|5]  [4|4]  [3|6]  [3|5]
//    [3|4]  [3|3]  [2|6]  [2|5]  [2|4]  [2|3]  [2|2]  [1|6]
//    [1|5]  [1|4]  [1|3]  [1|2]  [1|1]  [0|6]  [0|5]  [0|4]
//    [0|3]  [0|2]  [0|1]  [0|0]
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 1.b
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 12/4/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "set.h"
#include "Domino.h"

const string HEADER = "CS106B Programming Abstractions in C++: Chpt 6, Ex 1.b";
const string DETAIL = "Printing a set of dominos modeled with a class\n";

Set<Domino> createSet();
void printSet(Set<Domino> set);

int main(int argc, const char * argv[]) {
    
    std::cout << HEADER << std::endl;
    std::cout << DETAIL << std::endl;
    
    Set<Domino> dominos = createSet();
    printSet(dominos);
    
    return 0;
}

//
// Function: createSet
// Usage: Set<Domino> dominoSet = createSet();
// -------------------------------------------
// Returns a set of 28 dominos.
//

Set<Domino> createSet() {
    Set<Domino> set(compareDominos);
    
    for (int left = 0; left < DOMINO_RADIX; left++) {
        for (int right = left; right < DOMINO_RADIX; right++) {
            Domino d(left,right);
            set.add(d);
        }
    }
    return set;
}

//
// Function: printSet
// Usage: printSet(dominoSet);
// ---------------------------
// Displays a representation of a set of dominos to the console.
//

void printSet(Set<Domino> set) {
    for (Domino d : set) {
        std::cout << d.toString() << "  ";
    }
    std::cout << std::endl;
}
