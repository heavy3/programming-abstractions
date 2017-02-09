//
// main.cpp
//
// This program exercises a template class Pair<T1, T2> which represents a
// pair of values, possibly differing in type.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/5/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "pair.h"

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 14.03\n";
const std::string DETAIL = "Template class Pair<T1,T2>";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    Pair<std::string, int> myPair1;
    
    std::cout << "Default constructed pair values: myPair1" << std::endl;
    std::cout << "v1 = " << myPair1.first() << "  v2 = " << myPair1.second();
    std::cout << std::endl << std::endl;
    
    Pair<std::string, double> myPair2("PI", 3.14159);
    
    std::cout << "Parameter constructed pair values: myPair2" << std::endl;
    std::cout << "v1 = " << myPair2.first() << "  v2 = " << myPair2.second();
    std::cout << std::endl << std::endl;
    
    Pair<std::string, std::string> myPair3("phone", "867-5309");
    
    std::cout << "Parameter constructed pair values: myPair3" << std::endl;
    std::cout << "v1 = " << myPair3.first() << "  v2 = " << myPair3.second();
    std::cout << std::endl << std::endl;
    
    Pair<std::string, std::string> myPair4 =
    *new Pair<std::string, std::string>("phone", "867-5309");
    
    std::cout << "Parameter constructed pair values: myPair4 (heap)" << std::endl;
    std::cout << "v1 = " << myPair4.first() << "  v2 = " << myPair4.second();
    std::cout << std::endl;
    
    return 0;
}
