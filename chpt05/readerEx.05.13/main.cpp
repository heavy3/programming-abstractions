//
// main.cpp
//
// This program implements the function, reverseQueue:
//
//      void reverseQueue(Queue<string> & queue);
//
// which reverses the elements in the queue.
//
// Typical output:
//
//    This program reverses a queue of strings.
//    --------------------------------------------------
//    Original: {"0", "1", "2", "3", "4", "5", "6"}
//    Reversed: {"6", "5", "4", "3", "2", "1", "0"}
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 13
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/05/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "stack.h"
#include "queue.h"
#include "strlib.h"
using namespace std;

// Constants and types

const string LINE = string(50, '-') + '\n';
const int Q_SIZE = 7;       // Load 7 items into the queue.

// Function prototypes

void banner();
void display(string, Queue<string> q);
void fillQueue(Queue<string> & queue, int numItems);
void reverseQueue(Queue<string> & queue);

// Main program

int main(int argc, char * argv[]) {
    banner();
    cout << LINE;
    
    Queue<string> q;
    
    fillQueue(q, Q_SIZE);
    display("Original: ", q);
    
    reverseQueue(q);
    display("Reversed: ", q);

    return 0;
}

// Function definitions

//
// Function: banner
// Usage: banner(upperBound)
// -------------------------
// Displays the purpose of the program on the console.
//

void banner() {
    cout << "This program reverses a queue of strings."
         << endl;
}

//
// Function: display
// Usage: display(queue, "my queue: ");
// ------------------------------------
// Displays contents of a queue of strings to the console, prepended with
// some header text.
//

void display(string header, Queue<string> q) {
    cout << header << q << endl;
}

//
// Function: fillQueue
// Usage: fillQueue(queue, numItems);
// ----------------------------------
// Fill a pass-by-reference queue with a specified number of
// sequentially increasing integer strings.
//

void fillQueue(Queue<string> & q, int numItems) {
    for (int i = 0; i < numItems; i++) {
        q.enqueue(integerToString(i));
    }
}

//
// Function: reverseQueue
// Usage: reverseQueue(queue);
// ---------------------------
// Reverses a pass-by-reference queue of strings.
//

void reverseQueue(Queue<string> & q) {
    Stack<string> s;
    
    // Unload queue to stack.
    
    while (q.size()) {
        s.push(q.dequeue());
    }
    
    // Reload queue from stack, effecting the reversal.
    
    while (s.size()) {
        q.enqueue(s.pop());
    }
}