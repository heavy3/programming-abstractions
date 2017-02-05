//
// main.cpp
//
// The program implements the function, countHanoiMoves(n), which computes
// the number of moves required to solve the Towers of Hanoi puzzle for
// n disks.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/4/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "simpio.h"
#include "error.h"

// Constant
 
const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.01\n";
const std::string DETAIL = "Count Towers of Hanoi Moves";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

int countHanoiMoves(int n);
void moveTower(int n, char start, char finish, char tmp,
                     int & count, bool showMoves = false);
void moveDisk(char start, char finish, int & count, bool showMoves);

// Main program

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    while (true) {
        int n = getInteger("Enter number of disks: ");
        if (n < 0) break;
        int numMoves = countHanoiMoves(n);
        std::cout << "Total moves: " << numMoves << std::endl;
    }
    return 0;
}

//
// Function: countHanoiMoves
// Usage: int count = countHanoiMoves(n);
// --------------------------------------
// Returns the number of moves it takes to transfer n disks stacked in 
// order of increasing radius from top to bottom from a start tower to a 
// destination tower using a temporary tower such that a larger disk is
// never placed atop a smaller disk.
//

int countHanoiMoves(int n) {
    int count = 0;
    moveTower(n, 'A', 'B', 'C', count);
    return count;
}

//
// Function: moveTower
// Usage: moveTower(n, 'A', 'B', 'C', count);
// ------------------------------------------
// Recursive function for solving the Towers of Hanoi problem.
//

void moveTower(int n, char start, char finish, char tmp, int & count,
                    bool showMoves) {
    if (n == 0) return;
    if (n == 1) {
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
