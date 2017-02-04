//
// main.cpp
//
// This program exercises the EzTokenScanner class, including a scanNumbers()
// method which I've implemented according to Figure 6-15 in the course
// reader.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 14
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This exercise extends the token scanner source in Figure 6-10 of the
// course reader to add scanNumbers(), scanStrings(), and saveToken() methods.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/25/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include "eztokenscanner.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.15\n";
const std::string DETAIL = "Token Scanner (extended with scanNumbers)";
const std::string BANNER = HEADER + DETAIL;

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    //std::string str = "Scan me, \"pl'ea'se\"!";
    std::string str = "This string 1739 3.1416 3.0E+9 numbers";
    std::cout << "Input string: " + str << std::endl;
    
    EzTokenScanner scanner(str);
    scanner.scanStrings();
    scanner.scanNumbers();
    while (scanner.hasMoreTokens()) {
        std::string token = scanner.nextToken();
        std::cout << "[" + token + "]" ;
    }
    
    return 0;
}
