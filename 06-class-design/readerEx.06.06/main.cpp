// TODO: FIX ME!  Does not handle delta between dates correctly.
//
// main.cpp
//
// This program exercises the Calendar interface exported in calendar.h.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 6
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 12/17/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "calendar.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.6\n";
const std::string DETAIL = "Extend the calendar.h interface even more.";
const std::string BANNER = HEADER + DETAIL;

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    Date moonLanding1(JULY, 20, 1969);
    Date moonLanding2(20, JULY, 1969);
    
    Date earlier(JULY, 20, 1969);
    Date sameAsEarlier(JULY, 20, 1969);
    Date later(JULY, 21, 1969);
    Date later2(AUGUST, 19, 1969);
    
    if (earlier < later) {
        std::cout << "[PASS] " << earlier << " is earlier than "
                  << later << std::endl;
    } else {
        std::cout << "[FAIL] " << earlier << " is later than "
                  << later << std::endl;
    }
    
    if (earlier < later2) {
        std::cout << "[PASS] " << earlier << " is earlier than "
        << later2 << std::endl;
    } else {
        std::cout << "[FAIL] " << earlier << " is later than "
        << later2 << std::endl;
    }

    if (earlier == sameAsEarlier) {
        std::cout << "[PASS] " << earlier << " is same as "
                  << sameAsEarlier << std::endl;
    } else {
        std::cout << "[FAIL] " << earlier << " is later than "
                  << sameAsEarlier << std::endl;
    }
    
    if (later > earlier) {
        std::cout << "[PASS] " << later << " is later than "
                  << earlier << std::endl;
    } else {
        std::cout << "[FAIL] " << later << " is earlier than "
                  << earlier << std::endl;
    }
    
    if (earlier != later) {
        std::cout << "[PASS] " << earlier << " is not equal to "
                  << later << std::endl;
    } else {
        std::cout << "[FAIL] " << earlier << " is equal to "
                  << later << std::endl;
    }

    // Add overloaded '<<' operator.
    
    std::cout << std::endl << moonLanding1 << std::endl;
    std::cout << moonLanding2 << std::endl;
    
    Date date(DECEMBER, 31, 1898);
    //Date date(FEBRUARY, 28, 1900);

    std::cout << toEpochDay(date) << std::endl;
    std::cout << toDate(1) << std::endl;
    std::cout << toDate(2) << std::endl;
    std::cout << toDate(0) << std::endl;
    std::cout << toDate(-1) << std::endl;
    
    return 0;
}
