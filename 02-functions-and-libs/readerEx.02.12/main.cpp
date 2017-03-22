//
// main.cpp
//
// This program generates a set of random real number between 0 and 1 and 
// then displays the average after a number of trials specified by 
// the user.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/22/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "random.h"
using namespace std;

// Function prototypes

double randomAverage(int numTrials);

// Main program

int main() {
    int numTrials;
    double average;
    
    cout << "Welcome to RandomAverage" << endl;
    cout << "------------------------" << endl;
    cout << "This program repeatedly generates a random real number between "
         << endl;
    cout << "0 and 1 and displays the average after a specified number of"
         << endl;
    cout << "trials entered by the user." << endl << endl;
    cout << "Enter number of trials: ";
    cin >> numTrials;
    
    average = randomAverage(numTrials);
    cout << "Average = " << average << endl;
    
    return 0;
}

//
// Function: randomAverage
// Usage: randomAverage(numTrials);
// --------------------------------
// Returns the average of numTrials over random real numbers
// between 0 and 1.
//

double randomAverage(int numTrials) {
    double sum = 0;
    
    for (int i = 0; i < numTrials; i++) {
        double random = randomReal(0, 1);
        sum += random;
        cout << "Random real = " << random << endl;
    }
    
    return sum / double(numTrials);
}