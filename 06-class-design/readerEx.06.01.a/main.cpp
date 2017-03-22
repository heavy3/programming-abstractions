//
// main.cpp
//
// This program exercises the Domino class and interface by
// creating a set of dominos and printing them to the console.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 12/3/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "Domino.h"

int main(int argc, const char * argv[]) {
    
    Domino dominoSet[28];
    int index = 0;
    
    // Create a set of dominos.
    
    for (int left = 0; left <= 6; left++) {
        for (int right = left; right <= 6; right++) {
            Domino d(left,right);
            dominoSet[index++] = d;
        }
    }
    
    // Print the dominos.
    
    for (Domino d : dominoSet) {
        std::cout << d.toString() << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
