//
// main.cpp
//
// This program uses the pigeonhole principle to find a duplicate value from
// a vector of N elements having range from 1 to (N-1) with O(N) compute
// complexity and O(1) space complexity and without changing any of the values
// in the vector.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 10, Exercise 14
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// I resorted to googling on this:
// http://www.geeksforgeeks.org/find-duplicates-in-on-time-and-constant-extra-space/
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 6/9/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include "vector.h"
#include "error.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 10.14\n";
const std::string DETAIL = "Find Duplicate Element in O(N) time & O(1) Space";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

int findDuplicate(Vector<int> & v);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    Vector<int> v;
    
    // Test data
    
    //v += 1, 2, 3;
    //v += 2, 1, 1;
    //v += 2, 2, 1;
    //v += 2, 3, 4, 1, 2;
    //v += 3, 1, 1, 1;
    
    v += 1, 2, 3, 1, 2;
    
    cout << "input: " << v << " first duplicate: " << findDuplicate(v) << endl;
    return 0;
}

//
// Function: findDuplicate
// Usage: int dup = findDuplicate(vec);
// Attribution: http://www.geeksforgeeks.org/find-duplicates-in-on-time-and-constant-extra-space/
// -----------------------------------------------------------------------------
// Returns the first duplicate value found within a vector of integers
// in the range from 1 to (n-1), where n is the size of the vector.
//
// Returns -1 if no duplicate value is found.
//
// Performance is O(n) and space complexity is O(1).
//
// The strategy is to consider the values in the vector as edges in
// a directed graph that point specific nodes.
//
// Nodes are marked as visited by (temporarily) negating the associated
// value of the vector at the node's index.
//
// As we iterate over each node in sequence and interpret node values
// as edges to secondary nodes, we can detect if the secondary nodes have
// already been visited if their associated value is already negative.
//
// In that case, the current edge can be normalized back to it's original
// 1-based lower bound to yield the duplicate value ... since another edge
// of the same value must have already been encountered in order to negate
// the node value.

int findDuplicate(Vector<int> & v) {
    int firstDuplicate = -1;
    
    for (int node = 0; node < v.size(); node++) {
        if (v[node] == 0) {
            error("Input data must be positive integers only.");
        }
    
        int edge = abs(v[node]) - 1;    // Down-bias edge to valid index range.
        int nextNode = v[edge];
        
        if (nextNode > 0) {
            v[edge] = -v[edge];
        } else {
            firstDuplicate = ++edge;    // Up-bias edge to recovers duplicate.
            break;
        }
    }
    
    // Restore vector to unmutated form.
    
    for (int node = 0; node < v.size(); node++) {
        v[node] = abs(v[node]);
    }
    
    return firstDuplicate;
}
