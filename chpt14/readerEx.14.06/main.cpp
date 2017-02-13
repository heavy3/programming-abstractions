//
// main.cpp
//
// Test driver array-based ring buffer queue that allows for complete
// memory utilization before capacity is expanded.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 6
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/6/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "queue.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 14.06\n";
const std::string DETAIL = "Array-based Queue with full memory utilization";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main() {
    cout << BANNER << endl << endl;;
    
    Queue<char> cq;                         // Declare an empty char Queue.
    assert(cq.isEmpty());                   // Check it is empty.
    cq.enqueue('a');                        // Add something to it.
    assert(!cq.isEmpty());                  // Shouldn't be empty anymore.
    assert(cq.size() == 1);                 // Should be precisely 1 in size.
                                            // Fill queue array to capacity.
    for (int i = 1; i < cq.getInitialCapacity(); i++) {
        cq.enqueue('a' + i);
    }
    assert(cq.peek() == 'a');               // Attempt to peek.
                                            // Size should be unaffected by peek
    assert(cq.size() == cq.getInitialCapacity());
    
                                            // Enqueue a capacity+1 element,
                                            // triggering the underlying array
                                            // to double in size.
                                            // Typically means 11 elements in Q.
    
    cq.enqueue('a' + cq.getInitialCapacity());
    assert(cq.getCapacity() == 2*cq.getInitialCapacity());
    
    assert(cq.dequeue() == 'a');            // Take stuff off queue.
    assert(cq.dequeue() == 'b');            // ..
    assert(cq.dequeue() == 'c');            // ..
    assert(cq.size() == cq.getInitialCapacity() - 2); // 11 - 3 = 8
    assert(!cq.isEmpty());                  // Should not be empty.
    cq.clear();
    assert(cq.isEmpty());                   // Should be empty queue now.
    cout << "Queue<char> unit test succeeded" << endl;
    
    return 0;
}
