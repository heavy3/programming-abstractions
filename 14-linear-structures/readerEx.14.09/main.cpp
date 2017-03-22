//
// main.cpp
//
// This file contains a unit test driver for the Vector class.
//
// For example:
//
//     CS106B Programming Abstractions in C++: Ex 14.09
//     Unit test driver for Vector class.
//
//     Vector<char> unit test succeeded
//
//     Vector<int> unit test succeeded
//
//     Vector<string> unit test succeeded
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/28/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "vector.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 14.09\n";
const std::string DETAIL = "Unit test driver for Vector class.";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

template <typename VectorType>
bool runCharVectorUnitTest(VectorType & stk);

template <typename VectorType>
bool runStrVectorUnitTest(VectorType & stk);

template <typename VectorType>
bool runIntVectorUnitTest(VectorType & stk);

// Main program

int main() {
    std::cout << BANNER << std::endl << std::endl;
    
    // Test vector with character data.
    
    Vector<char> chVector;
    
    if (runCharVectorUnitTest(chVector)) {
        std::cout << "Vector<char> unit test succeeded" << std::endl;
    }
    std::cout << std::endl;
    
    // Test vector with integer data.
    
    Vector<int> iVector;
    
    if (runIntVectorUnitTest(iVector)) {
        std::cout << "Vector<int> unit test succeeded" << std::endl;
    }
    std::cout << std::endl;
    
    // Test vectors with string data.
    
    Vector<string> strVector;
    
    if (runStrVectorUnitTest(strVector)) {
        std::cout << "Vector<string> unit test succeeded" << std::endl;
    }
    std::cout << std::endl;
    
    return 0;
}

// Function: runCharVectorUnitTest
// Usage: bool passed = runCharVectorUnitTest(vector);
// -------------------------------------------------
// Returns true if unit test passes all assertions.

template <typename VectorType>
bool runCharVectorUnitTest(VectorType & cv) {
    assert(cv.isEmpty());                   // Check it is empty.
    cv.add('a');                            // Add something to it.
    assert(!cv.isEmpty());                  // Shouldn't be empty anymore.
    assert(cv.size() == 1);                 // Should be precisely 1 in size.
    cv.add('c');                            // Add more stuff.
    cv.insert(1, 'b');                      // Insert something.
    assert(cv.get(1) == 'b');               // Get a value.
    assert(cv.size() == 3);                 // Size should be unaffected get.
    VectorType copycv = cv;                 // Exercise deep copy.
    cv.remove(2);                           // Remove the third thing.
    cv.remove(0);                           // Remove the first thing.
    assert(cv.get(0) == 'b');               // Should just have second thing.
    cv.clear();                             // Clear remaining elements.
    assert(cv.isEmpty());                   // Verify empty.
    assert(copycv.size() == 3);             // Verify size of copied vector.
    copycv[1] = 'B';                        // Mutate with bracket operator.
    assert(copycv.get(1) == 'B');           // Verify content.
    return true;
}

// Function: runIntVectorUnitTest
// Usage: bool passed = runIntVectorUnitTest(vector);
// -------------------------------------------------
// Returns true if unit test passes all assertions.

template <typename VectorType>
bool runIntVectorUnitTest(VectorType & v) {
    assert(v.isEmpty());                    // Check it is empty.
    v.add(1);                               // Add something to it.
    assert(!v.isEmpty());                   // Shouldn't be empty anymore.
    assert(v.size() == 1);                  // Should be precisely 1 in size.
    v.add('3');                             // Add more stuff.
    v.insert(1, '2');                       // Insert something.
    assert(v.get(1) == '2');                // Get a value.
    assert(v.size() == 3);                  // Size should be unaffected get.
    VectorType copycv = v;                  // Exercise deep copy.
    v.remove(2);                            // Remove the third thing.
    v.remove(0);                            // Remove the first thing.
    assert(v.get(0) == '2');                // Should just have second thing.
    v.clear();                              // Clear remaining elements.
    assert(v.isEmpty());                    // Verify empty.
    assert(copycv.size() == 3);             // Verify size of copied vector.
    copycv[1] = 10;                         // Mutate with bracket operator.
    assert(copycv.get(1) == 10);            // Verify content.
    return true;
}

// Function: runStrVectorUnitTest
// Usage: bool passed = runStrVectorUnitTest(vector);
// ------------------------------------------------
// Returns true if unit test passes all assertions.

template <typename VectorType>
bool runStrVectorUnitTest(VectorType & v) {
    assert(v.isEmpty());                    // Check it is empty.
    v.add("one");                           // Add something to it.
    assert(!v.isEmpty());                   // Shouldn't be empty anymore.
    assert(v.size() == 1);                  // Should be precisely 1 in size.
    v.add("three");                         // Add more stuff.
    v.insert(1, "two");                     // Insert something.
    assert(v.get(1) == "two");              // Get a value.
    assert(v.size() == 3);                  // Size should be unaffected get.
    VectorType copycv = v;                  // Exercise deep copy.
    v.remove(2);                            // Remove third thing.
    v.remove(0);                            // Remove first thing.
    assert(v.get(0) == "two");              // Should just have second thing.
    v.clear();                              // Clear remaining elements.
    assert(v.isEmpty());                    // Verify empty.
    assert(copycv.size() == 3);             // Verify size of copied vector.
    copycv[1] = "One";                      // Mutate with bracket operator.
    assert(copycv.get(1) == "One");         // Verify content.
    return true;
}
