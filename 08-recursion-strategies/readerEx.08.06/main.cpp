//
// main.cpp
//
// This program implements the function:
//
//    bool isMeasurable(int target, Vector<int> & weights)
//
// which determines if it is possible to measure the desired target
// amount with a given set of weights.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 6
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/9/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "Vector.h"
#include "Set.h"

// Prototypes

bool isMeasurable(int target, Vector<int> & weights);
Set<int> measurableSums(Vector<int> & weights);

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.06\n";
const std::string DETAIL = "Is Measurable?";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    //
    // Collection of weights to measure stuff.
    //
    // Use a vector (instead of a set) so we may have multiples of a given
    // denominations.
    //
    
    Vector<int> weights;
    weights += 1, 3;
    
    /* Quantity we're attempting to measure. */
    
    int target = 2;
    
    std::cout << "Given weights " << weights << " can we measure ";
    std::cout << target << "? Ans: " << (boolalpha) << isMeasurable(target, weights);
    std::cout << std::endl;
    
    return 0;
}

//
// Function: isMeasurable
// Usage: bool b = isMeasurable(quantity, weights);
// ------------------------------------------------
// Predicate function that returns true if a given quantity can be measured
// with a collection of discrete weights.
//

bool isMeasurable(int target, Vector<int> & weights) {
    Set<int> measurable = measurableSums(weights);
    return measurable.contains(target);
}

//
// Function: measurableSums
// Usage: Set<int> validSums = measurableSums(weights);
// ----------------------------------------------------
// Returns a set of quantities that can be directly or indirectly measured
// using a collection of sample weights.
//

Set<int> measurableSums(Vector<int> & weights) {
    Set<int> sums;      // All measurable quantities we can weight.
    int vectorSum = 0;  // Sum of weights in the passed-in vector.
    
    for (int i = 0; i < weights.size(); i++) {
        
        // Add up all the weight in the current collection of weights.
        
        vectorSum += weights[i];
        
        // 
        // What sums are possible with the remaining weights if we
        // exclude one?  (Reduce scale of problem to one of the same form.)
        //
        
        Vector<int> fewerWeights(weights);
        fewerWeights.remove(i);
        
        Set<int> smallerSums;
        smallerSums += measurableSums(fewerWeights); // Recurse to fewer wts.
        
        // 
        // Can we shift the excluded weight to the left side of the scale
        // and use the difference of left and right weights to cleverly measure
        // a new quantity?
        //
        
        for (int rhsWeight : smallerSums) {
            int lhsWeight = weights[i];
            
            int cleverWeight = rhsWeight - lhsWeight;
            if (cleverWeight > 0) {
                sums += cleverWeight;
            }
        }
        sums += smallerSums;
    }
    
    sums += vectorSum;
    return sums;
}
