//
// main.cpp
//
// This program implements the sqrt() function for taking the square root
// of a number using the following successive approximation technique:
//
// 1. Begin by guessing that the square root is x / 2. Call that guess g.
//
// 2. The actual square root must lie between g and x / g.
//    At each step in the successive approximation, generate a new guess 
//    by averaging g and x / g.
//
// 3. Repeat step 2 until the values g and x / g are as close together as the 
//    machine precision allows. In C++, the best way to check for this 
//    condition is to test whether the average is equal to either of the values 
//    used to generate it.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/18/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <string>

// Function prototypes

double sqrt(double n);
bool refineGuess(double n, double & nextGuess);
void error(std::string msg);

// Main program

int main(int argc, char * argv[]) {
    
    double n = 10;
    
    double answer = sqrt(n);
    
    std::cout << "sqrt(" << n << ") = " << answer << std::endl;
    
    return 0;
}

// Function definitions

//
// Function: sqrt
// Usage: double d = sqrt(9);
// --------------------------
// Returns square root of a number.
//

double sqrt(double n) {
    double guess = n / 2.0;
    
    if (n < 0) {
        error("square root of a negative number is undefined");
    }
    if (n == 0) {
        return 0;  // short circuit to avoid divide by 0;
    }
    while(refineGuess(n, guess)) continue;
    return guess;
}

//
// Function: refineGuess
// Usage: double n = 9;
//        double nextGuess = n/2.0;
//        while (refineGuess(n, nextGuess)) continue;
// ---------------------------------------------------
// Returns true once a successive approximation algorithm
// has convered on an answer to within the accuracy supported
// by the machine.
//
// When the routine converges on an answer and returns boolean
// false, the pass by reference variable, g, will hold the
// approximate solution.
//

bool refineGuess(double x, double  & g) {
    double nextG = (g + x/g) / 2.0;
    if (nextG == g) {
        return false;
    } else {
        g = nextG;
        return true;
    }
}

//
// Function: error
// Usage: error("Something bad happened.  Exitting.");
// ---------------------------------------------------
// Exits program with the standard failure code EXIT_FAILURE
// defined in <cstdlib>.
//

void error(std::string msg) {
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}