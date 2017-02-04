// main.cpp
//
// This program uses a stack to reverse a list of integers entered by the user
// at the console.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/05/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "stack.h"
#include "simpio.h"
using namespace std;

// Constants and types

const string LINE   = string(50, '-');
const string SENTINEL = "0";
const string PROMPT = "Enter a list of integers, ending with: " + SENTINEL;
const string NUM_PROMPT = "? ";
const string REPORT = "Those integers in reverse order are:";

// Function prototypes

void banner();
void getNums(Stack<int> & stack);
void showReverseNums(Stack<int> & stack);

// Main program

int main(int argc, char * argv[]) {
    banner();
    cout << LINE << endl << endl;
    
    Stack<int> stack;
    getNums(stack);
    showReverseNums(stack);

    return 0;
}

// Function definitions

//
// Function: banner
// Usage: banner(upperBound)
// -------------------------
// Function displays the purpose of the program on the console.
//

void banner() {
    cout << "This program uses a stack to reverse a list of integers."
              << endl;
}

//
// Function: getNums
// Usage: getNums(stack)
// ---------------------
// Populates a pass-by-reference stack of integers entered by the user at the
// console until a sentinel value is entered.
//

void getNums(Stack<int> & stack) {
    int myInt = -1;
    
    cout << PROMPT << endl;
    while (myInt != stringToInteger(SENTINEL)) {
        myInt = getInteger(NUM_PROMPT);
        if (myInt) stack.push(myInt);
    }
}

//
// Function: showReverseNums
// Usage: showReverseNums(stack)
// -----------------------------
// Pops off the contents of a stack, displaying each value on a separate line
// of the console.
//

void showReverseNums(Stack<int> & stack) {
    if (stack.size()) {
        cout << REPORT << endl;
        while (!stack.isEmpty()) {
            int myInt = stack.pop();
            cout << setw(4) << myInt << endl;
        }
    } else {
        cout << "No numbers provided to reverse.";
    }
}
