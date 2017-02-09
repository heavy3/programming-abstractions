//
// main.cpp
//
// This file contains a unit test of two different stack implementations
// (i.e., array based and linked-list based) from the text.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/5/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "ArrayStack.h"
#include "LinkedListStack.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 14.04\n";
const std::string DETAIL = "Unit test driver for two stack implementations.";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

template <typename StackType>
bool runCharStackUnitTest(StackType & stk);

template <typename StackType>
bool runStrStackUnitTest(StackType & stk);

template <typename StackType>
bool runIntStackUnitTest(StackType & stk);

// Main program

int main() {
    std::cout << BANNER << std::endl << std::endl;
    
    // Test stacks with character data.
    
    ArrayStack<char> cStack;
    LinkedListStack<char> lStack;
    
    if (runCharStackUnitTest(cStack)) {
        std::cout << "ArrayStack<char> unit test succeeded" << std::endl;
    }
    if (runCharStackUnitTest(lStack)) {
        std::cout << "LinkedListStack<char> unit test succeeded" << std::endl;
    }
    std::cout << std::endl;
    
    // Test stacks with integer data.
    
    ArrayStack<int> ciStack;
    LinkedListStack<int> liStack;
    
    if (runIntStackUnitTest(ciStack)) {
        std::cout << "ArrayStack<int> unit test succeeded" << std::endl;
    }
    if (runIntStackUnitTest(liStack)) {
        std::cout << "LinkedListStack<int> unit test succeeded" << std::endl;
    }
    std::cout << std::endl;
    
    // Test stacks with string data.
    
    ArrayStack<string> csStack;
    LinkedListStack<string> lsStack;
    
    if (runStrStackUnitTest(csStack)) {
        std::cout << "ArrayStack<string> unit test succeeded" << std::endl;
    }
    if (runStrStackUnitTest(lsStack)) {
        std::cout << "LinkedListStack<string> unit test succeeded" << std::endl;
    }
    std::cout << std::endl;
    
    return 0;
}

// Function: runCharStackUnitTest
// Usage: bool passed = runCharStackUnitTest(stack);
// -------------------------------------------------
// Returns true if unit test passes all assertions.

template <typename StackType>
bool runCharStackUnitTest(StackType & cstk) {
    assert(cstk.size() == 0);               // Make sure its size is 0
    assert(cstk.isEmpty());                 // And that isEmpty is true
    cstk.push('A');                         // Push the character 'A'
    assert(!cstk.isEmpty());                // The stack is now not empty
    assert(cstk.size() == 1);               // And has size 1
    assert(cstk.peek() == 'A');             // Check that peek returns 'A'
    cstk.push('B');                         // Push the character 'B'
    assert(cstk.peek() == 'B');             // Make sure peek returns it
    assert(cstk.size() == 2);               // And that the size is now 2
    assert(cstk.pop() == 'B');              // Pop and test for the 'B'
    assert(cstk.size() == 1);               // Recheck the size
    assert(cstk.peek() == 'A');             // And make sure 'A' is on top
    cstk.push('C');                         // Test a push after a pop
    assert(cstk.size() == 2);               // Make sure size is correct
    assert(cstk.pop() == 'C');              // And that pop returns a 'C'
    assert(cstk.peek() == 'A');             // The 'A' is now back on top
    assert(cstk.pop() == 'A');              // Pop and test for the 'A'
    assert(cstk.size() == 0);               // And make sure size is 0
    for (char ch = 'A'; ch <= 'Z'; ch++) {  // Push the entire alphabet
        cstk.push(ch);                      //   one character at a time
    }                                       //   to test stack expansion
    assert(cstk.size() == 26);              // Make sure the size is 26
    for (char ch = 'Z'; ch >= 'A'; ch --) { // Pop the characters in
        assert(cstk.pop() == ch);           //   reverse order to make
    }                                       //   sure they're all there
    assert(cstk.isEmpty());                 // Ensure the stack is empty
    for (char ch = 'A'; ch <= 'Z'; ch++) {  // Push the alphabet again to
        cstk.push(ch);                      //   test that it works after
    }                                       //   expansion
    assert(cstk.size() == 26);              // Check size is again 26
    cstk.clear();                           // Check the clear method
    assert(cstk.size() == 0);               // And check if stack is empty
    cstk.clear();                           // Test clear with empty stack
    assert(cstk.size() == 0);
    return true;
}

// Function: runStrStackUnitTest
// Usage: bool passed = runStrStackUnitTest(stack);
// ------------------------------------------------
// Returns true if unit test passes all assertions.

template <typename StackType>
bool runStrStackUnitTest(StackType & cstk) {
    assert(cstk.size() == 0);               // Make sure its size is 0
    assert(cstk.isEmpty());                 // And that isEmpty is true
    cstk.push("A");                         // Push the character 'A'
    assert(!cstk.isEmpty());                // The stack is now not empty
    assert(cstk.size() == 1);               // And has size 1
    assert(cstk.peek() == "A");             // Check that peek returns 'A'
    cstk.push("B");                         // Push the character 'B'
    assert(cstk.peek() == "B");             // Make sure peek returns it
    assert(cstk.size() == 2);               // And that the size is now 2
    assert(cstk.pop() == "B");              // Pop and test for the 'B'
    assert(cstk.size() == 1);               // Recheck the size
    assert(cstk.peek() == "A");             // And make sure 'A' is on top
    cstk.push("C");                         // Test a push after a pop
    assert(cstk.size() == 2);               // Make sure size is correct
    assert(cstk.pop() == "C");              // And that pop returns a 'C'
    assert(cstk.peek() == "A");             // The 'A' is now back on top
    assert(cstk.pop() == "A");              // Pop and test for the 'A'
    assert(cstk.size() == 0);               // And make sure size is 0
    for (char ch = 'A'; ch <= 'Z'; ch++) {  // Push the entire alphabet
        cstk.push(string(1,ch));            //   one character at a time
    }                                       //   to test stack expansion
    assert(cstk.size() == 26);              // Make sure the size is 26
    for (char ch = 'Z'; ch >= 'A'; ch --) { // Pop the characters in
        assert(cstk.pop() == string(1,ch)); //   reverse order to make
    }                                       //   sure they're all there
    assert(cstk.isEmpty());                 // Ensure the stack is empty
    for (char ch = 'A'; ch <= 'Z'; ch++) {  // Push the alphabet again to
        cstk.push(string(1,ch));            //   test that it works after
    }                                       //   expansion
    assert(cstk.size() == 26);              // Check size is again 26
    cstk.clear();                           // Check the clear method
    assert(cstk.size() == 0);               // And check if stack is empty
    cstk.clear();                           // Test clear with empty stack
    assert(cstk.size() == 0);
    return true;
}

// Function: runIntStackUnitTest
// Usage: bool passed = runIntStackUnitTest(stack);
// ------------------------------------------------
// Returns true if unit test passes all assertions.

template <typename StackType>
bool runIntStackUnitTest(StackType & stk) {
    assert(stk.size() == 0);               // Make sure its size is 0
    assert(stk.isEmpty());                 // And that isEmpty is true
    stk.push(1);                           // Push the number 1
    assert(!stk.isEmpty());                // The stack is now not empty
    assert(stk.size() == 1);               // And has size 1
    assert(stk.peek() == 1);               // Check that peek returns 1
    stk.push(2);                           // Push the number 2
    assert(stk.peek() == 2);               // Make sure peek returns it
    assert(stk.size() == 2);               // And that the size is now 2
    assert(stk.pop() == 2);                // Pop and test for the 2
    assert(stk.size() == 1);               // Recheck the size
    assert(stk.peek() == 1);               // And make sure 1 is on top
    stk.push(3);                           // Test a push after a pop
    assert(stk.size() == 2);               // Make sure size is correct
    assert(stk.pop() == 3);                // And that pop returns a 3
    assert(stk.peek() == 1);               // The 1 is now back on top
    assert(stk.pop() == 1);                // Pop and test for the 1
    assert(stk.size() == 0);               // And make sure size is 0
    for (int i = 1; i <= 26; i++) {        // Push the first 26 numbers
        stk.push(i);                       //   one number at a time
    }                                      //   to test stack expansion
    assert(stk.size() == 26);              // Make sure the size is 26
    for (int i = 26; i >= 1; i--) {        // Pop the characters in
        assert(stk.pop() == i);            //   reverse order to make
    }                                      //   sure they're all there
    assert(stk.isEmpty());                 // Ensure the stack is empty
    for (int i = 1; i <= 26; i++) {        // Push the 1-26 again to
        stk.push(i);                       //   test that it works after
    }                                      //   expansion
    assert(stk.size() == 26);              // Check size is again 26
    stk.clear();                           // Check the clear method
    assert(stk.size() == 0);               // And check if stack is empty
    stk.clear();                           // Test clear with empty stack
    assert(stk.size() == 0);
    return true;
}
