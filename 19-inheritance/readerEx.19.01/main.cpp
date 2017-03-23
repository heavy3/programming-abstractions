//
// main.cpp
//
// This program exercises the Employee class hierarchy.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/23/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "Employee.h"

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 19.01\n";
const std::string DETAIL = "Inheritance: Employee Class Hierarchy";
const std::string BANNER = HEADER + DETAIL;

// Prototypes


// Main program

int main(int argc, char ** argv) {
    std::cout << BANNER << std::endl << std::endl;
    
    HourlyEmployee donald("Donald", 96.153846, 2080);
    std::cout << "Donald makes $" << donald.getPay() << " per yr." << std::endl;
    
    SalariedEmployee peter("Peter", 10000000);
    std::cout << "Peter makes $" << std::setprecision(0);
    std::cout << std::fixed;
    std::cout << peter.getPay()  << " per yr." << std::endl;
    
    CommissionedEmployee charlie("Charlie", 500000, 0.10, 1000000);
    std::cout << "Charlie makes $" << std::setprecision(0);
    std::cout << std::fixed;
    std::cout << charlie.getPay()  << " per yr." << std::endl;
    
}
