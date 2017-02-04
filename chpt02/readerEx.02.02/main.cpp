//
// main.cpp
//
// This program is a reimplementation of a distance-conversion program
// from Chapter 1, Exercise 2, but using a function and employing 
// call-by-reference to return values through the parameter list.
//
// For example:
//
//    Enter a distance in meters to convert to feet and inches.
//    Meters: 1
//    Equals: 3' 3.37008"
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/17/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>

// Constants and Types

const double METERS_PER_INCH = 0.0254;
const double INCHES_PER_FOOT = 12.0;

// Function prototypes

void metersToFeetInches(double meters, int & feet, double & inches);

// Main program */

int main(int argc, char * argv[]) {
    double meters;
    int feet;
    double inches;
    
    std::cout << "Enter a distance in meters to convert to feet and inches."
              << std::endl;
    std::cout << "Meters: ";
    std::cin  >> meters;
    
    metersToFeetInches(meters, feet, inches);
    
    std::cout << "Equals: " << feet << "' " << inches << "\""
              << std::endl;
    return 0;
}

// Function definitions

//
// Function: metersToFeetInches
// Usage: metersToFeet(meters, feet, inches);
// ------------------------------------------
// Converts a distance in meters to whole feet and fractional inches via 
// pass-by-reference variables.
//

void metersToFeetInches(double meters, int & feet, double & inches) {
    double totalInches = meters / METERS_PER_INCH;
    double totalFeet   = totalInches / INCHES_PER_FOOT;
    
    feet = int(totalFeet);
    inches = (totalFeet - feet) * double(INCHES_PER_FOOT);
}
