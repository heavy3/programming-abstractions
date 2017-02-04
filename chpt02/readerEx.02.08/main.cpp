//
// main.cpp
//
// This program implements a function, findEaster(), which computes the month
// and day for Easter Sunday for a given year according to an algorithm
// attributed to Carl Friedrich Gauss.
//
// TODO: Unfortunately, this algorithm only works for 18th and 19th centuries.
//       Generalize it to other centuries.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/18/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdlib>

// Function prototypes

void findEaster(int year, std::string & month, int & day);
void error(std::string msg);

// Main program

int main(int argc, char * argv[]) {
    
    int year = 1753;
    std::string month;
    int day;

    findEaster(year, month, day);
    std::cout << "In the year " << year << ", Easter fell on "
              << month << " " << day << std::endl;
    return 0;
}

// Function definitions

//
// Function: findEaster
// Usage: findEaster(year, month, day);
// ------------------------------------
// Returns the month and day for Easter in a given year.
// Easter is the first Sunday after the first full moon following
// the vernal equinox.
//
// The algorithm is courtesy Carl Friedrich Gauss, first published
// in 1800:
//
// 1. Divide the number of the year for which one wishes to calculate Easter
//    by 19, by 4, and by 7, and call the remainders of these divisions
//    a, b, and c, respectively.  If the division is even, set the remainder
//    to 0; the quotients are not taken into account.  Precisely the same is
//    true of the following divisions.
//
// 2. Divide the value 19a + 23 by 30 and call the remainder d.
//
// 3. Finally, divide 2b + 4c + 6d +3, or 2b +4c +6d +4, choosing the
//    former for years between 1700 and 1799 and the latter for years between
//    1800 and 1899, by 7 and call the remainder e.
//
// When (d + e) <= 9, Easter falls on March 22 + d + e,
// When (d + e) > 9,  Easter falls on April d + e - 9.
//

void findEaster(int year, std::string & month, int & day) {
    int a, b, c, d, e;
    
    a = year % 19;
    b = year % 4;
    c = year % 7;
    
    d = (19 * a + 23) % 30;
    
    if ((1700 <= year) && (year <= 1799)) {
        e = (2 * b + 4 * c + 6 * d + 3) % 7 ;
    } else if ((1800 <= year) && (year <= 1899)) {
        e = (2 * b + 4 * c + 6 * d + 4) % 7;
    } else {
        error("This routine only works for years 1700 - 1899");
    }
    
    if ((d + e) <= 9) {
        month = "March";
        day = 22 + d + e;
    } else {
        month = "April";
        day = d + e - 9;
    }
}

//
// Function: error
// Usage: error("Exiting.  Something bad happened.");
// --------------------------------------------------
// Returns control to the OS with standard failure 
// exit code, EXIT_FAILURE, defined in <cstdlib>.
//

void error(std::string msg) {
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}
