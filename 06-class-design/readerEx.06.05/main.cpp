//
// main.cpp
//
// Exercises the calendar class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 12/16/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "calendar.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.5\n";
const std::string DETAIL = "Extend the Ex 2.11 calendar.h interface.";
const std::string BANNER = HEADER + DETAIL;

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    Date moonLanding(JULY, 20, 1969);
    std::cout << moonLanding.toString() << std::endl;
    
    return 0;
}
