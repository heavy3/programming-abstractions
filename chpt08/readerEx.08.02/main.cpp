//
// TODO: I don't think I've really captured the spirit of this question
//       since I still have a test for n == 1.  So maybe think more and
//       fix that.
//
// main.cpp
//
// This program modifies the recursive moveTower() function from the 
// text to test for n == 0 as it's base case (instead of simply n == 1).
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program largely borrows from the code in Figure 8-1 of the course
// reader.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/4/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <iomanip>
#include <string>
#include "simpio.h"
#include "error.h"

// Constants
 
const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.02\n";
const std::string DETAIL = "Tweak Towers of Hanoi Implementation (test n == 0)";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

void moveTower(unsigned int n, char start, char finish, char tmp,
                     int & count, bool showMoves = true);
void moveDisk(char start, char finish, int & count, bool showMoves);

// Main program

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    while (true) {
        int N = getInteger("Enter number of disks: ");
        if (N < 0) break;
        unsigned int n = N;
        int count = 0;
        moveTower(n, 'A', 'B', 'C', count);
        std::cout << "Total moves: " << count << std::endl;
    }
    return 0;
}

//
// Function: moveTower
// Usage: moveTower(n, 'A', 'B', 'C', count);
// ------------------------------------------
// Recursive function for solving the Towers of Hanoi problem.
//

void moveTower(unsigned int n, char start, char finish, char tmp, int & count,
               bool showMoves) {
    if (n == 0) {
        return;
    } else if (n == 1) {
        moveDisk(start, finish, count, showMoves);
    } else {
        moveTower(n - 1, start, tmp, finish, count);
        moveDisk(start, finish, count, showMoves);
        moveTower(n - 1, tmp, finish, start, count);
    }
}

//
// Function: moveDisk
// Usage: moveDisk('A', 'B', count);
// ---------------------------------
// Moves a single disk from a start tower to a destination tower.
// Increments a counter so we can track overall effort required to
// move n disks.
//

void moveDisk(char start, char finish, int & count, bool showMoves) {
    if (showMoves) {
        std::cout << start << " -> " << finish << std::endl;
    }
    count++;
}
