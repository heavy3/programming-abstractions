//
// main.cpp
//
// This program converts distances provided in meters to distances
// in feet and inches using the following conversion factors:
//
//    1 inch = 0.0254 meters 
//    1 foot = 12 inches
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 1, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/12/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[]) {
    
    const double meterPerInch = 0.0254;
    const double inchPerFoot  = 12.0;
    
    double totalMeters;
    
    std::cout << "Enter a distance in meters that i'll convert to feet and inches." << std::endl;
    std::cout << "Meters: ";
    std::cin  >> totalMeters;
    
    double totalInches = totalMeters / meterPerInch;
    double totalFeet   = totalInches / inchPerFoot;
    
    int fracFeet = int(totalFeet);
    double fracInches = (totalFeet - fracFeet) * double(inchPerFoot);
    
    std::cout << "Equals: " << fracFeet << "' " << fracInches << "\"" << std::endl;
    
    return 0;
}
