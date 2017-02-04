//
// main.cpp
//
// This program calculates the sum of numbers from 1 to 100 using
// using two different methods of O(n) and O(1) time complexity, 
// respectively.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 1, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/12/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[]) {

    // Iterative method, O(n) time complexity.

    int sum = 0;
    for (int i = 1; i <= 100; i++) {
        sum += i;
    }
    std::cout << "Sum of numbers from 1 to 100 = " << sum << std::endl;

    // Gauss' method, O(1) time complexity.
    // 1 + 100
    // 2 +  99
    // 3 +  98
    // ..
    // 50 + 51
    // 101 * 50
    
    sum = (100 + 1) * (100/2);
    std::cout << "Elegant sum of numbers from 1 to 100 = " << sum << std::endl;
    return 0;
}
