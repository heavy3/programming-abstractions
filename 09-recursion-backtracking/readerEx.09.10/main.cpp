//
// main.cpp
//
// This program implements a function:
//
//    int cutStock(Vector<int> & requests, int stockLength);
//
// which uses recursion to compute the minimum number of fixed-length
// pipes needed for a construction job.  What we know up front are the
// number of individually sized "cut pipe" lengths needed for the job 
// and the length of the stock pipe available from the supplier.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 10
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/5/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "vector.h"
#include "error.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.10\n";
const std::string DETAIL = "Cuttin'pipe & Minimizin'cost";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

int cutStock(Vector<int> & requests, int stockLength);
bool isEnoughStock(Vector<int> & requests, int stockLength, Vector<int> & onHand);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    
    Vector<int> pipes; pipes += 4, 3, 4, 1, 7, 8;
    int stockLength = 10;
    
    int minStockPipes = cutStock(pipes, stockLength);
    
    cout << "Lengths of pipe needed for the job: " << pipes << endl;
    cout << "Minimum number of " << stockLength << "-unit stock length pipes ";
    cout << "needed: " << minStockPipes << endl;
    
    return 0;
}

//
// Function: cutStock
// Usage: int minStockPipes = cutStock(pipes, stockLength);
// --------------------------------------------------------
// Returns the minimum number of stock-length pipes required to create
// a collection of smaller, variable-lengthed pipes.
//
// This is a wrapper for the recursive isEnoughStock function.
// If if the stock is too small for the requested pipe lengths, control
// is passed to an error handler that displays a message on the console
// then abruptly halts.
//

int cutStock(Vector<int> & requests, int stockLength) {
    int estimate = 0;       // Minimum estimated stock-length pipes needed.
    Vector<int> onHand;     // Pieces available for cutting.
    Vector<int> desired(requests);
    
    // Sanity check the order.
    
    for (int request : requests) {
        if (request > stockLength) {
            /* No amount of creative cutting will fix this problem. ;-) */
            error("Unable to satisfy your order with current stock length.");
            return false;
        }
    }
    
    // 
    // To calculate the _minimum_ amount of stock required, start with a
    // small estimate that will fail and gradually increase it until all
    // requests for cut pipe are met.
    //
    
    while (!isEnoughStock(desired, stockLength, onHand)) {
        estimate++;
        onHand += stockLength;
    }
    return estimate;
}

//
// Function: isEnoughStock
// Usage: while (!isEnoughStock(requests, stockLength, onHand) { . . . }
// ---------------------------------------------------------------------
// Returns true if the supply of on-hand pipe is adequate to meet the
// requests for pipes of various length.
//
// Uses recursion with backtracking to explore how various pipe lengths can
// be made from stock pipe and (subsequent) remnants of stock.
//
// The elementary case is no remaining requests for cut pipe.
//
// The recursive case selects the nth pipe request (leaving n - 1)
// and iterates over the available stock to make that pipe.
//
// If sufficiently long stock is found and the remaining n - 1 requests
// can be (recursively) met with the reduced stock on-hand, then we return 
// true.
//
// Otherwise, we backtrack to an earlier invocation and reconsider our choice 
// of stock.  Maybe there is other adequate stock closer in size to the
// request that would leave more stock for subsequent requests.
//
// If we exhaust all the ways in which requests can be mapped onto available
// stock, then return false.  In that case, the caller, typically a wrapper 
// function, decides if more stock should be added for meeting the requests.
//

bool isEnoughStock(Vector<int> & requests, int stockLength,
                   Vector<int> & onHand) {
    // Base case
    
    if (requests.size() == 0) {
        return true;
    }
    
    // Recursive case
    
    int nthPipe = requests.size() - 1;
    int nthLength = requests[nthPipe];
    requests.remove(nthPipe);
    for (int p = 0; p < onHand.size(); p++) {
        if (nthLength <= onHand[p]) {
            onHand[p] -= nthLength;
            if (isEnoughStock(requests, stockLength, onHand)) {
                return true;
            }
            onHand[p] += nthLength;
        }
    }
    requests.insert(nthPipe, nthLength);
    return false;   // Trigger backtracking.
}