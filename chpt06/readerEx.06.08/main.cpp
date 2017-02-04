//
// main.cpp
//
// This program and related Rational class extend the source
// provided in the course reader to add various operations
// (i.e., relational, shorthand assignment, increment/decrement).
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This class extends the source code from Figure 6-07 in the reader.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/19/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include "rational.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.8\n";
const std::string DETAIL = "Extending Rational Numbers Class";
const std::string BANNER = HEADER + DETAIL;

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    Rational oneHalf(1, 2);
    Rational oneHalfPrime(1, 2);
    
    Rational oneThird(1, 3);
    Rational oneSixth(1, 6);
    
    Rational sum = oneHalf + oneThird + oneSixth;
    
    std::cout << oneHalf << " + " << oneThird << " + " << oneSixth << " = ";
    std::cout << sum << std::endl;
    
    bool same = (oneHalf == oneThird);
    std::cout << "Is " << oneHalf << " == " << oneThird << " ? " << same;
    std::cout << std::endl;
    
    same = (oneHalf == oneHalfPrime);
    std::cout << "Is " << oneHalf << " == " << oneHalfPrime << " ? " << same;
    std::cout << std::endl;
    
    bool different = (oneHalf != oneThird);
    std::cout << "Is " << oneHalf << " != " << oneThird << " ? " << different;
    std::cout << std::endl;
    
    different = (oneHalf != oneHalfPrime);
    std::cout << "Is " << oneHalf << " != " << oneHalfPrime << " ? " << different;
    std::cout << std::endl;
    
    std::cout << (sum += ++oneHalf) << std::endl;
    std::cout << oneHalf++ << std::endl;
    std::cout << oneHalf-- << std::endl;
    std::cout << oneHalf << std::endl;
    std::cout << --oneHalf << std::endl;
    std::cout << oneHalf << std::endl;

    return 0;
}
