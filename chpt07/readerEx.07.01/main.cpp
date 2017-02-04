//
// main.cpp
//
// This program uses recursion to calculate the number of 
// cannonballs stacked in pyramid of height N
//
// For example, for a pyramid of height 4
//
//                  Top View
//
//          O       O       O       O
//              X       X       X
//          O       o       o       O
//              X      [x]      X
//          O       o       o       O
//              X       X       X
//          O       O       O       O
//
//  Level   # Balls
//  ------- -------
//  1       1
//  2       2^2 = 4
//  3       3^2 = 9
//  4       4^2 = 16
//  ----------------
//  Total  1+4+9+16 = 30 balls
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 7, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/27/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "simpio.h"
#include "strlib.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 7.01\n";
const std::string DETAIL = "Cannonball Pyramid";
const std::string BANNER = HEADER + DETAIL;

int numBalls(int height);

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    while (true) {
        int height = getInteger("Enter cannonball pyramid height: ");
        std::cout << "Number of cannonballs in pyramid: " << numBalls(height);
        std::cout << std::endl << std::endl;
    }
    
    return 0;
}

//
// Function: numBalls
// ------------------
// This function returns the number of cannonballs stacked in a pyramid
// of cannonballs of a given height.
//

int numBalls(int h) {
    if (h <= 0) return 0;
    if (h == 1) return 1;
    
    return h*h + numBalls(h-1);
}
