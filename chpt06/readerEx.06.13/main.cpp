//
// main.cpp
//
// This program exercises the EzTokenScanner class, including a saveToken()
// method which I've implemented.
//
// The saveToken method essentially pushes a scanned token, back into the
// input buffer so that a subsequent call to *.nextToken() will return
// the saved token.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 13
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This exercise extends the token scanner source in Figure 6-10 of the
// course reader to add a saveToken() method.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/25/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include "eztokenscanner.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.13\n";
const std::string DETAIL = "Token Scanner";
const std::string BANNER = HEADER + DETAIL;

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    std::string str = "Scan me, please!";
    std::cout << "Input string: " + str << std::endl;
    
    bool oneShot = true;
    
    EzTokenScanner scanner(str);
    while (scanner.hasMoreTokens()) {
        std::string token = scanner.nextToken();
        std::cout << "[" + token + "]" ;
        if (token == "me" && oneShot ) {
            scanner.saveToken(token);
            scanner.saveToken("injected");
            oneShot = false;
        }
    }
    
    return 0;
}
