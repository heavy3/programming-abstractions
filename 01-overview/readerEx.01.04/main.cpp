//
// main.cpp
//
// This program reads a positive integer N and computes the sum of
// the first N odd integers.
//
// For example:
//
//    N = 4 results in the display of 16 (1 + 3 + 5 + 7).
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 1, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/13/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[]) {

    int N;
    
    std::cout << "Display sum of first N odd integers." << std::endl;
    std::cout << "Enter N: ";
    std::cin >> N;
    
    int sum = 0;
    for (int i = 0, nextOdd = 1; i < N; i++, nextOdd += 2) {
        sum += nextOdd;
    }
    std::cout << "Sum = " << sum <<  std::endl;
    
    return 0;
}