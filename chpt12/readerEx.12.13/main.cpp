//
// main.cpp
//
// This program provides unit tests for a const correct version of the 
// Rational math class from Chapter 6, Exercise 8.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 12, Exercise 13
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/20/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "rational.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 12.13\n";
const std::string DETAIL = "Const correct Rational Numbers Class";
const std::string BANNER = HEADER + DETAIL;

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    Rational oneHalf(1, 2);
    assert(oneHalf.toString() == "1/2");
    Rational oneHalfPrime(1, 2);
    
    Rational oneThird(1, 3);
    assert(oneThird.toString() == "1/3");
    
    Rational oneSixth(1, 6);
    assert(oneSixth.toString() == "1/6");
    
    Rational sum = oneHalf + oneThird + oneSixth;
    assert(sum.toString() == "1");
    
    std::cout << oneHalf << " + " << oneThird << " + " << oneSixth << " = ";
    std::cout << sum << std::endl;
    
    bool same = (oneHalf == oneThird);
    std::cout << "Is " << oneHalf << " == " << oneThird << " ? " << same;
    std::cout << std::endl;
    assert(same == false);
    
    same = (oneHalf == oneHalfPrime);
    std::cout << "Is " << oneHalf << " == " << oneHalfPrime << " ? " << same;
    std::cout << std::endl;
    assert(same == true);
    
    bool different = (oneHalf != oneThird);
    std::cout << "Is " << oneHalf << " != " << oneThird << " ? " << different;
    std::cout << std::endl;
    assert(different == true);
    
    different = (oneHalf != oneHalfPrime);
    std::cout << "Is " << oneHalf << " != " << oneHalfPrime << " ? " << different;
    std::cout << std::endl;
    assert(different == false);
    
    std::cout << sum << std::endl;
    std::cout << ++oneHalf << std::endl;
    std::cout << (sum += oneHalf) << std::endl;
    assert(sum.toString() == "5/2");
    
    assert(oneHalf++.toString() == "3/2");
    assert(oneHalf--.toString() == "5/2");
    assert(oneHalf.toString() == "3/2");
    assert((--oneHalf).toString() == "1/2");
    std::cout << oneHalf << std::endl;
    
    std::cout << "Passed unit tests" << std::endl;

    return 0;
}
