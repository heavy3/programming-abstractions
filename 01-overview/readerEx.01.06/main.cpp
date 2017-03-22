//
// main.cpp
//
// This program reads a list of integers from the console until a sentinel
// value is detected.  It then determines the largest and second largest
// numbers entered and displays those on the console.
//
// For example:
//
//    This program finds the largest integer in a list.
//    Enter 0 to signal the end of the list.
//        ? 17
//        ? 42
//        ? 11
//        ? 0
//    The largest value was 42.
//    The second largest value was 17.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 1, Exercise 6
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
    int nextNum, largestNum = nextNum, secondLargestNum = nextNum;
    int pass = 0;
    
    std::cout << "This program finds the largest integer in a list." << std::endl;
    std::cout << "Enter 0 to signal the end of the list." << std::endl << "\t? ";
    
    while (true) {
        std::cin >> nextNum;
        if (nextNum == SENTINEL)
            break;
        else {
            pass += 1;
            switch (pass) {
                case 1:
                    largestNum = nextNum;
                    break;
                    
                case 2:
                    if (nextNum > largestNum) {
                        secondLargestNum = largestNum;
                        largestNum = nextNum;
                    } else {
                        secondLargestNum = nextNum;
                    }
                    break;
                    
                default:
                     // Avoid bumping up secondLargestNum if the largest num is entered more than once.
                    if (nextNum > largestNum) {
                        secondLargestNum = (secondLargestNum != largestNum) ? largestNum : secondLargestNum;
                        largestNum = nextNum;
                    } else if (nextNum > secondLargestNum){
                        secondLargestNum = (nextNum == largestNum) ? secondLargestNum : nextNum;
                    }
                    break;
            }
            std::cout << "\t? ";
        }
    }
    
    switch (pass) {
        case 0:
            std::cout << "Empty list :-/" << std::endl;
            break;
        case 1:
            std::cout << "Largest number is " << largestNum << std::endl;
            break;
        default:
            std::cout << "Largest number is " << largestNum << std::endl;
            std::cout << "Second largest number is " << secondLargestNum << std::endl;
            break;
    }
    return 0;
}