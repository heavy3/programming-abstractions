//
// main.cpp
//
// This file contains a unit test driver for the Vector class.
//
// For example:
//
//     CS106B Programming Abstractions in C++: Ex 14.10
//     Unit test driver for Vector class.
//
//     Vector = {a, b, c}
//     Vector<char> unit test succeeded
//
//     Vector = {1, 2, 3}
//     Vector<int> unit test succeeded
//
//     Vector = {"one", "two", "three"}
//     Vector<string> unit test succeeded
//
// TODO: Add more tests especially for operator overloads.
//
//       Adding stuff to the end of the vector is expensive, O(N),
//       since underlying implementation doesn't manage a tail pointer.
//       Would likely need doubly linked list to fix that.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 10
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/1/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "LinkedListVector.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 14.10\n";
const std::string DETAIL = "Linked-list vector implementation.";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

bool runCharVectorUnitTest(Vector<char> & v);
bool runIntVectorUnitTest(Vector<int> & v);
bool runStrVectorUnitTest(Vector<string> & v);

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

bool runCharVectorUnitTest(Vector<char> & v) {
    assert(v.isEmpty());                   // Check it is empty.
    v.add('a');                            // Add something to it.
    assert(!v.isEmpty());                  // Shouldn't be empty anymore.
    assert(v.size() == 1);                 // Should be precisely 1 in size.
    v.add('c');                            // Add more stuff.
    v.insert(1, 'b');                      // Insert something.
    assert(v.get(1) == 'b');               // Get a value.
    assert(v.size() == 3);                 // Size should be unaffected get.
    cout << "Vector = " << v << endl;      // toString and << overload test.
    Vector<char> copyv = v;                // Exercise deep copy.
    v.remove(2);                           // Remove the third thing.
    v.remove(0);                           // Remove the first thing.
    assert(v.get(0) == 'b');               // Should just have second thing.
    v.clear();                             // Clear remaining elements.
    v.clear();                             // Double clear should be ok.
    assert(v.isEmpty());                   // Verify empty.
    assert(copyv.size() == 3);             // Verify size of copied vector.
    copyv[1] = 'B';                        // Mutate with bracket operator.
    assert(copyv.get(1) == 'B');           // Verify content.
    Vector<char> initMe;
    initMe += 'A', 'B', 'C', 'D';          // Exercise += and comma operators.
    assert(initMe.size() == 4);            //
    Vector<char> initMe2;                  //
    initMe2 += 'E';
    Vector<char> catMe;                    // Exercise vector concatenation.
    catMe = initMe + initMe2;
    assert(catMe.size() == 5);
    int iteration = 0;
    for (char ch: catMe) {
        ++iteration;
    }
    assert(catMe.size() == iteration);
    return true;
}

// Function: runIntVectorUnitTest
// Usage: bool passed = runIntVectorUnitTest(vector);
// -------------------------------------------------
// Returns true if unit test passes all assertions.

bool runIntVectorUnitTest(Vector<int> & v) {
    assert(v.isEmpty());                    // Check it is empty.
    v.add(1);                               // Add something to it.
    assert(!v.isEmpty());                   // Shouldn't be empty anymore.
    assert(v.size() == 1);                  // Should be precisely 1 in size.
    v.add(3);                               // Add more stuff.
    v.insert(1, 2);                         // Insert something.
    assert(v.get(1) == 2);                  // Get a value.
    assert(v.size() == 3);                  // Size should be unaffected get.
    cout << "Vector = " << v << endl;       // toString and << overload test.
    Vector<int> copyv = v;                  // Exercise deep copy.
    v.remove(2);                            // Remove the third thing.
    v.remove(0);                            // Remove the first thing.
    assert(v.get(0) == 2);                  // Should just have second thing.
    v.clear();                              // Clear remaining elements.
    v.clear();                              // Double clear should be ok.
    assert(v.isEmpty());                    // Verify empty.
    assert(copyv.size() == 3);              // Verify size of copied vector.
    copyv[1] = 10;                          // Mutate with bracket operator.
    assert(copyv.get(1) == 10);             // Verify content.
    Vector<int> initMe;
    initMe += 0, 1, 2, 3;                   // Exercise += and comma operators.
    assert(initMe.size() == 4);             //
    Vector<int> initMe2;                    //
    initMe2 += 4;
    Vector<int> catMe;                      // Exercise vector concatenation.
    catMe = initMe + initMe2;
    assert(catMe.size() == 5);
    int iteration = 0;
    for (int val: catMe) {
        ++iteration;
    }
    assert(catMe.size() == iteration);
    return true;
}

// Function: runStrVectorUnitTest
// Usage: bool passed = runStrVectorUnitTest(vector);
// ------------------------------------------------
// Returns true if unit test passes all assertions.

bool runStrVectorUnitTest(Vector<string> & v) {
    assert(v.isEmpty());                    // Check it is empty.
    v.add("one");                           // Add something to it.
    assert(!v.isEmpty());                   // Shouldn't be empty anymore.
    assert(v.size() == 1);                  // Should be precisely 1 in size.
    v.add("three");                         // Add more stuff.
    v.insert(1, "two");                     // Insert something.
    assert(v.get(1) == "two");              // Get a value.
    assert(v.size() == 3);                  // Size should be unaffected get.
    cout << "Vector = " << v << endl;       // toString and << overload test.
    Vector<string> copyv = v;               // Exercise deep copy.
    v.remove(2);                            // Remove third thing.
    v.remove(0);                            // Remove first thing.
    assert(v.get(0) == "two");              // Should just have second thing.
    v.clear();                              // Clear remaining elements.
    v.clear();                              // Double clear should be ok.
    assert(v.isEmpty());                    // Verify empty.
    assert(copyv.size() == 3);              // Verify size of copied vector.
    copyv[1] = "One";                       // Mutate with bracket operator.
    assert(copyv.get(1) == "One");          // Verify content.
    Vector<string> initMe;
    initMe += "uno","dos","tres","quatro";  // Exercise += and comma operators.
    assert(initMe.size() == 4);             //
    Vector<string> initMe2;                 //
    initMe2 += "cinqo";
    Vector<string> catMe;                   // Exercise vector concatenation.
    catMe = initMe + initMe2;
    assert(catMe.size() == 5);
    int iteration = 0;
    for (string str: catMe) {
        ++iteration;
    }
    assert(catMe.size() == iteration);
    return true;
}
