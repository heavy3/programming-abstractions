//
// main.cpp
//
// This program exercises the EzTokenScanner class, including a scanStrings()
// method which I've implemented.
//
// The scanStrings method returns double or singly quoted strings as a
// single token when this attribute is in effect.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 14
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This exercise extends the token scanner source in Figure 6-10 of the
// course reader to add a scanStrings() & saveToken() methods.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/25/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include "eztokenscanner.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.14\n";
const std::string DETAIL = "Token Scanner";
const std::string BANNER = HEADER + DETAIL;

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    std::string str = "Scan me, \"pl'ea'se\"!";
    std::cout << "Input string: " + str << std::endl;
    
    EzTokenScanner scanner(str);
    scanner.scanStrings();
    while (scanner.hasMoreTokens()) {
        std::string token = scanner.nextToken();
        std::cout << "[" + token + "]" ;
    }
    
    return 0;
}
