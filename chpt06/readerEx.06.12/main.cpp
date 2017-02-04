//
// main.cpp
//
// This program implements a simple calculator that evaluates expressions
// from left to right.  It extends the program from Exercise 6.11 to 
// support variables in expressions.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/24/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "simpio.h"
#include "exprcalc.h"

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.12\n";
const std::string DETAIL = "Expression Calculator";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    std::cout << "Allows simple left-to-right evaluation of arithmetic\n";
    std::cout << "expressions of numbers, variables, and operators (+,-,*,/).\n\n";
    
    ExpressionCalculator calc;
    std::string input;
    
    while ((input = getLine(PROMPT)) != "") {
        std::string output = calc.eval(input);
        if (!output.empty()) {
            std::cout << output << std::endl;
        }
    }
    return 0;
}
