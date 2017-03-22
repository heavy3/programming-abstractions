//
// main.cpp
//
// This program exercises the LabelGenerator class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 1/27/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "labelgen.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.7\n";
const std::string DETAIL = "Implement LabelGenerator";
const std::string BANNER = HEADER + DETAIL;

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    LabelGenerator figureNumbers("Figure ", 1);
    LabelGenerator pointNumbers("P", 0);
     
    std::cout << "Figure numbers: ";
    for (int i = 0; i < 3; i++) {
        if (i > 0) std::cout << ", ";
        std::cout << figureNumbers.nextLabel();
    }
     
    std::cout << std::endl << "Point numbers: ";
    for (int i = 0; i < 5; i++) {
        if (i > 0) std::cout << ", ";
        std::cout << pointNumbers.nextLabel();
    }
     
    std::cout << std::endl << "More figures:  ";
    for (int i = 0; i < 3; i++) {
        if (i > 0) std::cout << ", ";
        std::cout << figureNumbers.nextLabel();
    }
     
    std::cout << std::endl;
     
    return 0;
    
}
