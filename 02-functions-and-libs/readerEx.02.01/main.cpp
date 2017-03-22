//
// main.cpp
//
// This program reads in a temperature in degrees Celsius and uses a function
// to convert to degrees Fahrenheit for display according to the conversion
// formula:
//
//    F = (9/5)C + 32
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/17/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>

// Constants and Types

const double C2F_SCALE = 9.0/5.0; // Number of degrees F per degree C.
const int    C2F_XLATE = 32;      // Freezing offset between C and F scales.

// Function prototypes

double fahrenheit(double celcius);

// Main program

int main(int argc, char * argv[]) {
    
    double degC;
    
    std::cout << "I convert temperature from degrees Celsius to degrees "
              << "Fahrenheit." << std::endl;
    std::cout << "Enter a temparature in degrees C: ";
    std::cin >> degC;
    std::cout << "Temperature in degrees F: " << fahrenheit(degC) << std::endl;

    return 0;
}

// Function definitions

//
// Function: fahrenheit
// Usage: degF = fahrenheit(double degC);
// ----------------------
// Returns the temperature in degrees Fahrenheit given degrees Celsius.
//

double fahrenheit(double degC) {
    return C2F_SCALE * degC + C2F_XLATE;
}