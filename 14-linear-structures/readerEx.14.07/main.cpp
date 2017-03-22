//
// main.cpp
//
// This is a test driver for array-based and linked list based implementations
// of a queue that supports a reverse() method that does not require additional
// memory allocation.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/6/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "ArrayQueue.h"
#include "LinkedListQueue.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 14.07\n";
const std::string DETAIL = "Array-based & List-based Queue with reverse method";
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
    
    // Test queues with character data.
    
    ArrayQueue<char> cQueue;
    LinkedListQueue<char> lQueue;
    
    if (runCharQueueUnitTest(cQueue)) {
        std::cout << "ArrayQueue<char> unit test succeeded" << std::endl;
    }

    if (runCharQueueUnitTest(lQueue)) {
        std::cout << "LinkedListQueue<char> unit test succeeded" << std::endl;
    }
    std::cout << std::endl;
    
    // Test queues with integer data.
    
    ArrayQueue<int> ciQueue;
    LinkedListQueue<int> liQueue;
    
    if (runIntQueueUnitTest(ciQueue)) {
        std::cout << "ArrayQueue<int> unit test succeeded" << std::endl;
    }
    if (runIntQueueUnitTest(liQueue)) {
        std::cout << "LinkedListQueue<int> unit test succeeded" << std::endl;
    }
    std::cout << std::endl;
    
    // Test queues with string data.
    
    ArrayQueue<string> csQueue;
    LinkedListQueue<string> lsQueue;
    
    if (runStrQueueUnitTest(csQueue)) {
        std::cout << "ArrayQueue<string> unit test succeeded" << std::endl;
    }
    if (runStrQueueUnitTest(lsQueue)) {
        std::cout << "LinkedListQueue<string> unit test succeeded" << std::endl;
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
    cq.enqueue('a');                        // Add something to it.
    assert(!cq.isEmpty());                  // Shouldn't be empty anymore.
    assert(cq.size() == 1);                 // Should be precisely 1 in size.
    cq.enqueue('b');                        // Add more stuff to queue.
    cq.enqueue('c');                        // ..
    assert(cq.peek() == 'a');               // Attempt to peek.
    assert(cq.size() == 3);                 // Size should be unaffected by peek
    std::cout << cq.toString() << std::endl;//
    cq.reverse();                           //
    std::cout << cq.toString() << std::endl;//
    assert(cq.dequeue() == 'c');            // Take stuff off queue.
    assert(cq.dequeue() == 'b');            // ..
    assert(cq.dequeue() == 'a');            // ..
    assert(cq.isEmpty());
    return true;
}

// Function: runIntQueueUnitTest
// Usage: bool passed = runIntQueueUnitTest(queue);
// -------------------------------------------------
// Returns true if unit test passes all assertions.

template <typename QueueType>
bool runIntQueueUnitTest(QueueType & iq) {
    assert(iq.isEmpty());                 // Check it is empty.
    iq.enqueue(1);                        // Add something to it.
    assert(!iq.isEmpty());                // Shouldn't be empty anymore.
    assert(iq.size() == 1);               // Should be precisely 1 in size.
    iq.enqueue(2);                        // Add more stuff to queue.
    iq.enqueue(3);                        // ..
    assert(iq.peek() == 1);               // Attempt to peek.
    assert(iq.size() == 3);               // Size should be unaffected by peek
    std::cout << iq.toString() << std::endl;//
    iq.reverse();
    std::cout << iq.toString() << std::endl;
    iq.reverse();
    assert(iq.dequeue() == 1);            // Take stuff off queue.
    assert(iq.dequeue() == 2);            // ..
    assert(iq.dequeue() == 3);            // ..
    assert(iq.isEmpty());
    return true;
}

// Function: runStrQueueUnitTest
// Usage: bool passed = runStrQueueUnitTest(queue);
// ------------------------------------------------
// Returns true if unit test passes all assertions.

template <typename QueueType>
bool runStrQueueUnitTest(QueueType & cq) {
    assert(cq.isEmpty());                   // Check it is empty.
    cq.enqueue("A");                        // Add something to it.
    assert(!cq.isEmpty());                  // Shouldn't be empty anymore.
    assert(cq.size() == 1);                 // Should be precisely 1 in size.
    cq.enqueue("B");                        // Add more stuff to queue.
    cq.enqueue("C");                        // ..
    assert(cq.peek() == "A");               // Attempt to peek.
    assert(cq.size() == 3);                 // Size should be unaffected by peek
    std::cout << cq.toString() << std::endl;//
    cq.reverse();                           //
    std::cout << cq.toString() << std::endl;//
    cq.reverse();
    assert(cq.dequeue() == "A");            // Take stuff off queue.
    assert(cq.dequeue() == "B");            // ..
    assert(cq.dequeue() == "C");            // ..
    assert(cq.isEmpty());
    return true;
}
