//
// main.cpp
//
// This program modifies the RPN calculator from Chpater 5 to use rational
// instead of floating point numbers.
//
// Typical interaction:
//
//    CS106B Programming Abstractions in C++: Ex 6.9
//    RPN Calculator (enabled for rational numbers)
//
//    RPN Calculator Simulation (type H for help)
//    > 1
//    > 2
//    > /
//    1/2
//    > 1
//    > 3
//    > /
//    1/3
//    > 1
//    > 6
//    >/
//    1/6
//    > +
//    1/2
//    > +
//    1
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This class reimplements the source code from Figure 5-04 of the course
// reader.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/22/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include "rpncalc.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.9\n";
const std::string DETAIL = "RPN Calculator (enabled for rational numbers)";
const std::string BANNER = HEADER + DETAIL;

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    runcalc();
    
    return 0;
}
