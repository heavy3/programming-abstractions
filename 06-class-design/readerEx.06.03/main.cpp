//
// main.cpp
//
// This program exercises a watered-down implementation of a GRectangle
// class that (mostly) meets the interface defined in the Stanford C++
// Library.
//
// Typical output:
//
//    CS106B Programming Abstractions in C++: Ex 6.3
//    Implement a simple GRectangle Class from gtypes.h
//
//    (10, 15, 500, 300)
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 12/7/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include "GRectangle.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.3";
const std::string DETAIL = "Implement a simple GRectangle Class from gtypes.h";

int main(int argc, char * argv[]) {
    
    std::cout << HEADER << std::endl;
    std::cout << DETAIL << std::endl << std::endl;
    
    double x = 10.0;
    double y = 15.0;
    double width = 500;
    double height = 300;
    
    std::cout << GRectangle(x, y, width, height) << std::endl;
    
    return 0;
}