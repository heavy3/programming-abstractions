//
// main.cpp
//
// This program implements the function,
//
//    int countSubsetSumWays(Set<int> & set, int target);
//
// which returns the number of ways a target value can be produced
// by summing a subset of numbers from a given set.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 4
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
#include "Vector.h"
#include "Set.h"
#include "error.h"

// Prototypes

bool subsetSumExists(Set<int> & set, int target);
int countSubsetSumWays(Set<int> & set, int target);

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.04\n";
const std::string DETAIL = "Count Subset Sums";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    Set<int> s;
    s += 1, 3, 4, 5;
    
    int t = 5;  // target sum we seek from a combination of elements in s
    
    std::cout << "Subset sum count of " << t << " from " << s << " is "
              << countSubsetSumWays(s, t) << std::endl;
    
    return 0;
}

//
// Function: subsetSumExists
// Usage: if (subsetSumExists(set, sum)) { ... }
// ---------------------------------------------
// Predicate function returns true if one or more elements of the set can
// be combined (without repetition) to yield the target sum.
//

bool subsetSumExists(Set<int> & set, int target) {
    if (set.isEmpty()) {
        return target == 0;
    } else {
        int element = set.first();
        Set<int> rest = set - element;
        return subsetSumExists(rest, target) ||
               subsetSumExists(rest, target - element);
    }
}

//
// Function: countSubsetSumWays
// Usage: int count = countSubsetSumWays(set, targetSum);
// ------------------------------------------------------
// Returns the number of ways in which you can produce the target value
// by choosing a subset of the specified set.
//
// The key insight is count should be 1 when the target sum
// is at 0.  This might happen if empty set is passed in and sum is 0
// or if target has been reduced to 0 through recursion.
//

int countSubsetSumWays(Set<int> & set, int target) {
    
    if (set.isEmpty()) {
        return (target == 0) ? 1 : 0;
    } else {
        int element = set.first();
        Set<int> rest = set - element;
        return countSubsetSumWays(rest, target) +
               countSubsetSumWays(rest, target - element);
    }
}
