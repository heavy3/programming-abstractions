//
// main.cpp
//
// This file contains a unit test driver for a priority queue.  The underlying
// queue uses a linked list implementation while separate priority channels into
// the overall queue are managed through a set which identifies existing
// channels along with a map that relates the priority value to a queue
// of items at that priority.  The set helps maintain queue ordering.
//
// Strictly speaking, I'm going rogue here, since the problem statement
// suggests a pure linked list implementation.  However, my ADT mash-up
// was relatively easy to implement and provides O(logN) runtime performance
// when adding new priorities to the queue versus the O(N) performance of a
// pure linked list.  This comes at the expense, I think, of O(logN) dequeue
// since the map iterator appears to use a binary search tree implementation
// whereas a pure linked list approach would be O(1) for dequeue.  I think
// I still win since enqueue + dequeue is what matters and 2logN < O(N) + O(1),
// right? :-).  For memory utilization both approaches are O(N), though the
// linked list is better by a constant factor (~3x) since I incur overhead for
// the set and map.
//
// Example:
//
//     CS106B Programming Abstractions in C++: Ex 14.08
//     Unit test driver for priority queue.
//
//     PriorityQueue<char> unit test succeeded
//
//     PriorityQueue<int> unit test succeeded
//
//     PriorityQueue<string> unit test succeeded
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/23/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "PriorityQueue.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 14.08\n";
const std::string DETAIL = "Unit test driver for priority queue.";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

template <typename QueueType>
bool runCharQueueUnitTest(QueueType & stk);

template <typename QueueType>
bool runStrQueueUnitTest(QueueType & stk);

template <typename QueueType>
bool runIntQueueUnitTest(QueueType & stk);

// Main program

int main() {
    std::cout << BANNER << std::endl << std::endl;
    
    // Test queue with character data.
    
    PriorityQueue<char> chpQueue;
    
    if (runCharQueueUnitTest(chpQueue)) {
        std::cout << "PriorityQueue<char> unit test succeeded" << std::endl;
    }
    std::cout << std::endl;
    
    // Test queue with integer data.
    
    PriorityQueue<int> ipQueue;
    
    if (runIntQueueUnitTest(ipQueue)) {
        std::cout << "PriorityQueue<int> unit test succeeded" << std::endl;
    }
    std::cout << std::endl;
    
    // Test queues with string data.
    
    PriorityQueue<string> strpQueue;
    
    if (runStrQueueUnitTest(strpQueue)) {
        std::cout << "PriorityQueue<string> unit test succeeded" << std::endl;
    }
    std::cout << std::endl;
    
    return 0;
}

// Function: runCharQueueUnitTest
// Usage: bool passed = runCharQueueUnitTest(queue);
// -------------------------------------------------
// Returns true if unit test passes all assertions.

template <typename QueueType>
bool runCharQueueUnitTest(QueueType & cq) {
    assert(cq.isEmpty());                   // Check it is empty.
    cq.enqueue('a', 2.0);                    // Add something to it.
    assert(!cq.isEmpty());                  // Shouldn't be empty anymore.
    assert(cq.size() == 1);                 // Should be precisely 1 in size.
    cq.enqueue('b', 1.0);                   // Add more stuff to queue.
    cq.enqueue('c', 3.0);                   // ..
    assert(cq.peek() == 'b');               // Attempt to peek.
    assert(cq.size() == 3);                 // Size should be unaffected by peek
    QueueType copycq = cq;                  // Exercise deep copy.
    assert(cq.dequeue() == 'b');            // Take stuff off queue.
    assert(cq.dequeue() == 'a');            // ..
    assert(cq.dequeue() == 'c');            // ..
    assert(cq.isEmpty());
    assert(copycq.dequeue() == 'b');        // Take stuff off copied queue.
    assert(copycq.dequeue() == 'a');        // ..
    assert(copycq.dequeue() == 'c');        // ..
    assert(copycq.isEmpty());
    return true;
}

// Function: runIntQueueUnitTest
// Usage: bool passed = runIntQueueUnitTest(queue);
// -------------------------------------------------
// Returns true if unit test passes all assertions.

template <typename QueueType>
bool runIntQueueUnitTest(QueueType & iq) {
    assert(iq.isEmpty());                 // Check it is empty.
    iq.enqueue(1, 2.0);                   // Add something to it.
    assert(!iq.isEmpty());                // Shouldn't be empty anymore.
    assert(iq.size() == 1);               // Should be precisely 1 in size.
    iq.enqueue(2, 1.0);                   // Add more stuff to queue.
    iq.enqueue(3, 3.0);                   // ..
    assert(iq.peek() == 2);               // Attempt to peek.
    assert(iq.size() == 3);               // Size should be unaffected by peek
    QueueType copyiq = iq;                // Exercise deep copy.
    assert(iq.dequeue() == 2);            // Take stuff off queue.
    assert(iq.dequeue() == 1);            // ..
    assert(iq.dequeue() == 3);            // ..
    assert(iq.isEmpty());
    assert(copyiq.dequeue() == 2);        // Take stuff off copied queue.
    assert(copyiq.dequeue() == 1);        // ..
    assert(copyiq.dequeue() == 3);        // ..
    assert(copyiq.isEmpty());
    return true;
}

// Function: runStrQueueUnitTest
// Usage: bool passed = runStrQueueUnitTest(queue);
// ------------------------------------------------
// Returns true if unit test passes all assertions.

template <typename QueueType>
bool runStrQueueUnitTest(QueueType & cq) {
    assert(cq.isEmpty());                   // Check it is empty.
    cq.enqueue("A", 2.0);                   // Add something to it.
    assert(!cq.isEmpty());                  // Shouldn't be empty anymore.
    assert(cq.size() == 1);                 // Should be precisely 1 in size.
    cq.enqueue("B", 1.0);                   // Add more stuff to queue.
    cq.enqueue("C", 3.0);                   // ..
    assert(cq.peek() == "B");               // Attempt to peek.
    assert(cq.size() == 3);                 // Size should be unaffected by peek
    QueueType copycq = cq;                  // Exercise deep copy.
    assert(cq.dequeue() == "B");            // Take stuff off queue.
    assert(cq.dequeue() == "A");            // ..
    assert(cq.dequeue() == "C");            // ..
    assert(cq.isEmpty());
    assert(copycq.dequeue() == "B");        // Take stuff off copied queue.
    assert(copycq.dequeue() == "A");        // ..
    assert(copycq.dequeue() == "C");        // ..
    assert(copycq.isEmpty());
    return true;
}
