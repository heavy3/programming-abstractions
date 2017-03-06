//
// RandomSubsets.cpp
//
// Given a set of integers, returns a uniformly-random subset of that set.
//
// Example output:
//
//     CS106B Programming Abstractions: Assignment 3.0B
//     Random Subset
//
//     Displays a random subset of a set of integers.
//
//     A random subset of the set of integers from 0 to 99
//
//     {2, 4, 5, 7, 9, 15, 16, 17, 24, 30, 33, 35, 41, 42, 43, 45,
//     46, 50, 58, 59, 61, 66, 67, 68, 69, 73, 75, 77, 79, 82, 83,
//     85, 87, 88, 93, 96, 98, 99}
//
// --------------------------------------------------------------------------
// Assignment: 3, Part 0B Random Subsets
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/110%20Assignment%203.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/6/2017.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "set.h"
#include "random.h"
using namespace std;

// Constants and Types

const int SET_SIZE = 100;

const string HEADER = "CS106B Programming Abstractions: Assignment 3.0B\n";
const string DETAIL = "Random Subset";
const string BANNER = HEADER + DETAIL;

Set<int> randomSubsetOf(Set<int> & s);
void seedMasterSet(Set<int> & s, int size);

int main() {
    cout << BANNER << endl << endl;
    cout << "Displays a random subset of a set of integers." << endl << endl;
    
    Set<int> s;
    
    seedMasterSet(s, SET_SIZE);
    Set<int> rs = randomSubsetOf(s);
    
    cout << "A random subset of the set of integers from 0 to " << SET_SIZE -1;
    cout << endl << endl;
    
    cout << rs << endl;
    
    return 0;
}

Set<int> randomSubsetOf(Set<int> & s) {
    Set<int> subset;
    
    for (int elem: s) {
        if (randomChance(0.5)) {
            subset.add(elem);
        }
    }
    return subset;
}

void seedMasterSet(Set<int> & s, int size) {
    for (int i = 0; i < size; i++) {
        s.add(i);
    }
}
