//
// main.cpp
//
// This program provides unit tests for the Rational math class
// from Chapter 6, Exercise 8.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 12, Exercise 12
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

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 12.12\n";
const std::string DETAIL = "Unit Test for 6.8 Extended Rational Numbers Class";
const std::string BANNER = HEADER + DETAIL;

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    // Verify basic constructor (Rational r(num, denom)).
    
    Rational oneHalf(1, 2);
    assert(oneHalf.toString() == "1/2");
    Rational oneHalfPrime(1, 2);
    
    Rational oneThird(1, 3);
    assert(oneThird.toString() == "1/3");
    
    Rational oneSixth(1, 6);
    assert(oneSixth.toString() == "1/6");
    
    // Verify rational addition (r1 + r2).
    
    Rational sum = oneHalf + oneThird + oneSixth;   // 1/2 + 1/3 + 1/6 = 1
    assert(sum.toString() == "1");
    
    // Verify rational subtraction (r2 - r1).
    
    sum = sum - oneHalf;                            // 1 - 1/2 = 1/2
    assert(sum.toString() == "1/2");
    
    // Verify rational multiplication (r1 * r2).
    
    Rational mult = oneHalf * oneThird;
    assert(mult.toString() == "1/6");
    
    // Verify rational division (r1 / r2).
    
    Rational div = mult / oneThird;
    assert(div.toString() == "1/2");
    
    // Verify comparison operator (r1 == r2).
    
    bool same = (oneHalf == oneThird);
    assert(same == false);                          // 1/2 != 1/3
    
    same = (oneHalf == oneHalfPrime);               // 1/2 == 1/2
    assert(same == true);
    
    // Verify pre-increment and += operators (++r, sum += r)
    
    sum += oneHalf + oneHalf;                       // 1/2 + 1/2 + 1/2
    assert(sum.toString() == "3/2");
    sum += ++oneHalf;                               // 3/2 + 1/2 + 1 = 6/2
    assert(sum.toString() == "3");
    
    // Verify pre-deccrement and -= operators (--r, sum -= r)
    
    Rational sum2 = sum;                            // 3
    sum2 -= --oneHalf;                              // 6/2 - 1/2 = 5/2
    assert(sum2.toString() == "5/2");
    
    // Verify post-increment and post-decrement operators (r++, r--).
    
    oneHalf++;                                      // 1/2 + 1 = 3/2
    assert(oneHalf++.toString() == "3/2");          // 3/2 ( + 1 post-inc)
    assert(oneHalf--.toString() == "5/2");          // 5/2 ( - 1 post-dec)
    assert(oneHalf.toString() == "3/2");            // 3/2
    assert((--oneHalf).toString() == "1/2");        // 3/2 - 2/2 = 1/2
    
    // Verify relation operators (<, <=, >, >=)
    
    assert(oneHalf > oneThird);
    assert(oneHalf >= oneHalfPrime);
    assert(oneThird < oneHalf);
    assert(oneThird <= oneThird);
    
    std::cout << "Passed unit tests" << std::endl;

    return 0;
}
