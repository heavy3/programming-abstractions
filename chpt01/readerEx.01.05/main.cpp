//
// main.cpp
//
// This program reads a list of integers from the console until a sentinel
// value is detected.  It then determines the largest number entered and
// displays that on the console.
//
// Example:
//
//    This program finds the largest integer in a list.
//    Enter 0 to signal the end of the list.
//        ? 17
//        ? 42
//        ? 11
//        ? 0
//    The largest value was 42.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 1, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/13/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[]) {

    const int SENTINEL = 0;
    int nextNum, largestNum = nextNum;
    bool firstPass = true;
    
    std::cout << "This program finds the largest integer in a list." << std::endl;
    std::cout << "Enter 0 to signal the end of the list." << std::endl << "\t? ";
    
    while (true) {
        std::cin >> nextNum;
        if (nextNum == SENTINEL)
            break;
        else {
            largestNum = (firstPass) ? nextNum : ((nextNum > largestNum) ? nextNum : largestNum);
            firstPass = false;
        }
        std::cout << "\t? ";
    }
    
    if (!firstPass)
        std::cout << "Largest numbers is " << largestNum << std::endl;
    else
        std::cout << "Empty list :-/" << std::endl;
    return 0;
}