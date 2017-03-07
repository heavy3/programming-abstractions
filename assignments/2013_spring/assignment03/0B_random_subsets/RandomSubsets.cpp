//
// RandomSubsets.cpp
//
// Given a set of integers, returns a uniformly-random subset of that set.
// Avoids looping constructs when building the random subset.
//
// Example output:
//
//     CS106B Programming Abstractions: Assignment 3.0B
//     Random Subset
//
//     Displays a random subset of a set of integers.
//
//     A random subset of the set of integers from 1 to 100
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
void recurRandomSubsetOf(Set<int> & subset, Set<int> & s);
void seedMasterSet(Set<int> & s, int size);

int main() {
    cout << BANNER << endl << endl;
    cout << "Displays a random subset of a set of integers." << endl << endl;
    
    Set<int> s;
    
    seedMasterSet(s, SET_SIZE);
    Set<int> rs = randomSubsetOf(s);
    
    cout << "A random subset of the set of integers from 1 to " << SET_SIZE;
    cout << endl << endl;
    
    cout << rs << endl;
    
    return 0;
}

// Function: randomSubsetOf
// Usage: Set<int> subset = randomSubsetOf(fullset);
// ---------------------------------------------------
// Returns a randomized subset of elements from the full set.
//
// Serves as a wrapper for a function which uses procedural recursion
// to build the subset but at the expense of the full set which is
// emptied in the process.
//
// The client remains blissfully unaware of the carnage since we operate
// upon a copy of the passed-in set.

Set<int> randomSubsetOf(Set<int> & s) {
    Set<int> subset;
    Set<int> master(s);
    
    recurRandomSubsetOf(subset, master);
    return subset;
}

// Function: recurRandomSubsetOf
// Usage: recurRandomSubsetOf(subset, fullset);
// --------------------------------------------
// Uses recursion (instead of iteration) to populate a random selection
// of elements from the second set into the first.

void recurRandomSubsetOf(Set<int> & subset, Set<int> & fullset) {
    
    // base case: full set is empty
    if (fullset.size() == 0) return;
    
    // recursive case
    //
    // reduce the size of the full set by one element
    // flip a coin to decide if this element should be added to the subset
    
    int ri = fullset.first();
    if (randomChance(0.5)) {
        subset.add(ri);
    }
    fullset.remove(ri);
    
    // perform the same operation on the n-1 sized fullset
    
    recurRandomSubsetOf(subset, fullset);
}

void seedMasterSet(Set<int> & s, int size) {
    if (s.size() == SET_SIZE) return;
    s.add(size);
    seedMasterSet(s, --size);
}
