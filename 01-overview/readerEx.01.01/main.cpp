//
// main.cpp
//
// This program converts degrees Celsius to degrees Fahrenheit
// according to the formula: 
//
//    F = (9/5)C + 32
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 1, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/12/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[]) {
    double tempC;
    double tempF;
    
    std::cout << "I convert temperature from degrees Celsius to degrees Fahrenheit." << std::endl;
    std::cout << "Enter a temparature in degrees C: ";
    std::cin >> tempC;
    tempF = (9.0/5)*tempC + 32;
    std::cout << "Temperature in degrees F: " << tempF << std::endl;
    return 0;
}
