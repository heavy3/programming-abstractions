//
// main.cpp
//
// This program implements a recursive version of the function,
// integerToString().
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 7, Exercise 11
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/29/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "simpio.h"
#include "error.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 7.11\n";
const std::string DETAIL = "Recursive integerToString()";
const std::string BANNER = HEADER + DETAIL;

std::string integerToString(int n);

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    while (true) {
        int n = getInteger("Enter integer: ");
        if (n < 0) break;
        std::cout << integerToString(n) << std::endl << std::endl;
    }
    return 0;
}

//
// Function: integerToString
// Usage: str = integerToString(12345);  // str = "12345";
// -------------------------------------------------------
// This function returns the string-form of an integer.
//
// The input number is decomposed into higher order digits and a
// ones-digit, with the latter easily converted to a single-character
// string on a subsequent recursive call.
//
// The results of that conversion are appended to the 
// recursively-converted higher-order digits to form a complete string.
//

std::string integerToString(int n) {
    if (n < 10) {
        return std::string(1,'0' + n);
    }
    return integerToString(n / 10) + integerToString(n % 10);
}
