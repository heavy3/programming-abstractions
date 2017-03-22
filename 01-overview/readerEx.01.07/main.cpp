//
// main.cpp
//
// This program reads a list of integers representing exam
// scores and computes the average.  Here, we use a sentinel of -1
// (instead of 0) to avoid exiting too early if a student actually
// scored 0 on the exam.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 1, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/13/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
using namespace std;

const int SENTINEL = -1;

int main(int argc, const char * argv[]) {
    
    cout << "This program averages a list of numbers." << endl;
    cout << "Use " << SENTINEL << " to signal the end." << endl;
    int total = 0;
    int numNumbers = 0;
    
    while (true) {
        int value;
        cout << " ? ";
        cin >> value;
        if (value == SENTINEL) break;
        total += value;
        numNumbers += 1;
    }
    
    if (numNumbers > 0)
        cout << "Average is " << total/double(numNumbers) << endl;
    else
        cout << "Nothing to average :-/";
    return 0;
    
}
