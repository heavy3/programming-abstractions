//
// main.cpp
//
// This program implements a roll operator for stacks, similar to that provided
// in the PostScript® language.
//
// The top element is popped off the stack and effectively inserted n-units down
// in the stack, creating the effect of a 1-unit rotation of the top n elements. 
//
// Multi-unit rotation is specified with the k variable seen in this
// prototype:
//
//    void roll(Stack<char> & stack, int n, int k)
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 15
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
#include "error.h"
#include "stack.h"
using namespace std;

// Function prototypes

void banner();
void roll(Stack<char> & stack, int n, int k);
int rollIndex(size_t startIndex, size_t nItems, int rollDelta);

// Constants

const string LINE = string(50, '-') + '\n';
const string E_INDEX_OUT_OF_BOUNDS = "Start index out of bounds.";

// Main program

int main(int argc, char * argv[]) {
    banner();
    cout << LINE;
    
    // Populate a stack with some dummy data.
    //
    // [ a ]
    // [ b ]
    // [...]
    // [ j ]
    //
    
    Stack<char> stack;
    for (char ch = 'j'; ch >= 'a'; ch--) {
        stack.push(ch);
    }
    cout << "before: " << stack << endl;
    
    roll(stack, 4, 1);
    cout << "after:  " << stack << endl;
    
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
    cout << "This program implements a roll() primitive for rotating the top "
         << "n items in a stack k units."
         << endl;
}

//
// Function: roll
// Usage: roll(stack, nItems, k);
// ------------------------------
// Rotates the top n items of a stack by k units.
//
// Throws an error if n or k are negative or if one attempts
// to rotate more items than currently in the stack.
//

void roll(Stack<char> & stack, int n, int k) {
    if (((n < 0) || (k < 0)) ||
         (n > stack.size())) {
        error("roll: argument out of range");
    }
    
    //
    // Example: Roll first 4 items of a stack 1 unit:
    //
    // Before                          After
    // [ a ]   Desired target state:   [ b ]
    // [ b ]                           [ c ]
    // [ c ]                           [ d ]
    // [ d ]                           [ a ]
    // [...]                           [...]
    //
    // To get there, pop 4 items from stack into a reording vector in
    // rolled, reversed order (suitable for repopulating the stack later):
    //
    // [a b c d] --roll--> [b c d a] --reverse-->> [a d c b]
    //

    Vector<char> reordered(n);
    for (int i = 0; i < n; i++) {
        if (!stack.isEmpty()) {
            int rolledIndex = rollIndex(i, n, -k); // -k for backward roll.
            int reversedRolledIndex = (n - 1) - rolledIndex;
            
            reordered[reversedRolledIndex] = stack.pop();
        }
    }
    
    //
    // Repopulate stack with reordered items:
    //
    // [a d c b]      Yielding:        [ b ]
    //  |                              [ c ]
    //  v                              [ d ]
    // [...]                           [ a ]
    //                                 [...]
    //
    
    for (int item: reordered) {
        stack.push(item);
    }
}

//
// Function: rollIndex
// Usage: int ri = rollIndex(i, nItems, delta);
// -------------------------------------------------------------
// Returns a list index that has been moved forward or backward by 
// delta units from a starting position in a list of n items.
//
// Returned index is 0-based, thus constrained between 0 and n-1.
//
// Delta may have negative value for specifying a backward rotation.
//
// An error is thrown if the start index is too large for the number 
// of items specified.
//

int rollIndex(size_t i, size_t n, int delta) {
    if (i >= n) {
        error("rollIndex: " + E_INDEX_OUT_OF_BOUNDS);
    }
    
    int ri = (int(i) + delta) % int(n);
    if (ri < 0) ri += n;  // Backward roll. Normalize to non-negative value.
    return ri;
}
