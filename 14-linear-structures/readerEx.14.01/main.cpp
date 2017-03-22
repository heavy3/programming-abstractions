//
// main.cpp
//
// This program implements a template version of the swap function.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/4/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 14.01\n";
const std::string DETAIL = "Templatized swap() function.";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

template <typename ValueType>
void swap(ValueType & x, ValueType & y) {
    ValueType tmp;
    
    tmp = x;
    x = y;
    y = tmp;
}

// Main program

int main(int argc, char ** argv) {
    std::cout << BANNER << std::endl << std::endl;
    
    int xNum = 4;
    int yNum = 2;
    
    std::cout << "before: xNum = " << xNum << " yNum = " << yNum << std::endl;
    swap(xNum, yNum);
    std::cout << "after:  xNum = " << xNum << " yNum = " << yNum << std::endl;
    
    std::cout << std::endl;
    
    std::string xStr = "four";
    std::string yStr = "two";
    
    std::cout << "before: xStr = " << xStr << " yStr = " << yStr << std::endl;
    swap(xStr, yStr);
    std::cout << "after:  xStr = " << xStr << " yStr = " << yStr << std::endl;
}
