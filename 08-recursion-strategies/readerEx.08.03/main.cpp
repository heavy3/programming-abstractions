//
// main.cpp
//
// This program solves the Towers of Hanoi problem by using an
// explicit stack of task for mutating the towers as desired.
// 
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/6/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "simpio.h"
#include "Stack.h"
#include "error.h"

// Constants
 
const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.03\n";
const std::string DETAIL = "Towers of Hanoi with Task Stack";
const std::string BANNER = HEADER + DETAIL;

struct task_T {
    unsigned int n; // num disks to move
    char start;     // label for start tower (move disks from here to finish)
    char finish;    // label for destination tower
    char tmp;       // label for temporary tower
};

// Prototypes

void moveTower(Stack<task_T> & moveTasks);
void moveDisk(char start, char finish, bool showMoves=true);

// Main program

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    Stack<task_T> moveTasks;
    
    while (true) {
        int n = getInteger("Enter number of disks: ");
        if (n < 0) break;
        moveTasks.push((task_T){n, 'A', 'B', 'C'});
        moveTower(moveTasks);
    }
    return 0;
}

//
// Function: moveTower
// Usage: moveTower(moveTasks);
// ------------------------------------------
// Uses a task stack, instead of recursion, to move n disks from
// a start tower to a finish tower with the help of a temporary tower.
//

void moveTower(Stack<task_T> & moveTasks) {
    while (!moveTasks.isEmpty()) {
        task_T t = moveTasks.pop();
        
        if (t.n == 0) {
            break;
        } else if (t.n == 1) {
            moveDisk(t.start, t.finish);
        } else {
            task_T t1 = {t.n - 1, t.start, t.tmp, t.finish};
            task_T t2 = {1, t.start, t.finish};
            task_T t3 = {t.n - 1, t.tmp, t.finish, t.start};
            
            // Push t1 last since it will get popped off first.
            
            moveTasks.push(t3);
            moveTasks.push(t2);
            moveTasks.push(t1);
        }
    }
}

//
// Function: moveDisk
// Usage: moveDisk('A', 'B');
// ---------------------------------
// Moves a single disk from a start tower to a destination tower.
//

void moveDisk(char start, char finish, bool showMoves) {
    if (showMoves) {
        std::cout << start << " -> " << finish << std::endl;
    }
}
