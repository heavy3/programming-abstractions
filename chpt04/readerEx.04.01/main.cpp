// 
// main.cpp
//
// This program displays a formatted trigonometric table of sines and cosines
// over the domain -90° to +90°.
//
// For examaple:
//
// CS106B Programming Abstractions in C++: Ex 4.01
// Formatted Trigonometric Tables
//
//  theta°| sin(theta) | cos(theta) |
// -------+------------+------------+
//   -90  | -1.0000000 |  0.0000000 |
//   -75  | -0.9659258 |  0.2588190 |
//   -60  | -0.8660254 |  0.5000000 |
//   -45  | -0.7071068 |  0.7071068 |
//   -30  | -0.5000000 |  0.8660254 |
//   -15  | -0.2588190 |  0.9659258 |
//     0  |  0.0000000 |  1.0000000 |
//    15  |  0.2588190 |  0.9659258 |
//    30  |  0.5000000 |  0.8660254 |
//    45  |  0.7071068 |  0.7071068 |
//    60  |  0.8660254 |  0.5000000 |
//    75  |  0.9659258 |  0.2588190 |
//    90  |  1.0000000 |  0.0000000 |
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 4, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/16/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 4.01\n";
const std::string DETAIL = "Formatted Trigonometric Tables";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

void printTableHeader(string str1, string str2, string str3);
void printTableRow(int theta, double sin, double cos);

// Main program

int main(int argc, const char * argv[]) {
    cout << BANNER << endl << endl;
    printTableHeader("theta°", "sin(theta)", "cos(theta)");
    for (int theta = -90; theta <= 90; theta += 15) {
        double thetaRadians = (theta * M_PI) / 180;
        printTableRow(theta, sin(thetaRadians), cos(thetaRadians));
    }
    
    return 0;
}

void printTableHeader(string str1, string str2, string str3) {
    cout << " " << str1 << "| " << str2 << " | " << str3 << " |" << endl;
    cout << "-------+------------+------------+" << endl;
}

void printTableRow(int theta, double sin, double cos) {
    cout << right << setw(5)  << theta << "  | " << fixed << setprecision(7)
         << setw(10) << sin << " | "
         << setw(10) << cos << " | "
         << endl;
}
