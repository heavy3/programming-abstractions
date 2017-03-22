//
// main.cpp
//
// The program implements a predicate function, isPerfect, that takes 
// an integer n and returns true if n is 'perfect' in the Greek sense, 
// and false otherwise.
//
// Perfect numbers are equal to the sum of their proper divisors
//
// For example,  6 is perfect because  6 = 1 + 2 + 3
//              28 is perfect because 28 = 1 + 2 + 4 + 7 + 14
//
// The program will generate all the perfect numbers in the range 1 to 9999.
//
// For example:
//
// 6
// 28
// 496
// 8128
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/18/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>

// Constants and Types

const int LOWER_LIMIT = 1;      // Test for perfect numbers in this range.
const int UPPER_LIMIT = 9999;

// Function prototypes

bool isPerfect(int num);
int sumProperDivisors(int num);

// Main program

int main(int argc, char * argv[]) {
    std::cout << "This program generates a list of 'perfect numbers' "
              << "found in this range of integers:" << std::endl;
    std::cout << LOWER_LIMIT << " < x <= " << UPPER_LIMIT << std::endl;
    for (int i = LOWER_LIMIT; i <= UPPER_LIMIT; i++) {
        if (isPerfect(i))
            std::cout << std::setw(4) << i << std::endl;
    }
    return 0;
}

// Function definitions

//
// Function: isPerfect
// Usage: if (isPerfect(num = 28)) cout << num << " is perfect." << endl;
// ----------------------------------------------------------------------
// Returns true if the proper divisors of a number sum to the number itself.
// A proper divisor is any divisor less than the number iteself.
//

bool isPerfect(int num) {
    return (sumProperDivisors(num) == num);
}

//
// Function: sumProperDivisors
// Usage: int sum = sumProperDivisors(28);
// ---------------------------------------
// Returns the sum of the proper divisors for a given integer.
// Edge case is 1 since it has no proper divisors.
//

int sumProperDivisors(int num) {
    int sum = 0;
    
    for (int divisor = 1; divisor < num; divisor++) {
        if (num % divisor == 0) {
            sum += divisor;
        }
    }
    
    return sum;
}