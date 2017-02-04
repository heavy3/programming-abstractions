//
// labelgen.h
//
// This file specifies a label generator that allows the client to define
// arbitrary sequences of labels, each of which consists of a prefix string
// (e.g., "Figure " or "P") coupled with an integer used as a sequence number.
//
// Because the client may want different sequences to be active simultaneously,
// it makes sense to define the label generator as a LabelGenerator class. To
// initialize a new generator, the client provides the prefix string and the
// initial index as arguments to the LabelGenerator constructor. Once the
// generator has been created, the client can return new labels in the sequence
// by calling nextLabel on the LabelGenerator.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 1/27/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#ifndef labelgen_h
#define labelgen_h

#include "error.h"

class LabelGenerator {

public:
    
//
// Constructor: LabelGenerator
// Usage: LabelGenerator label("Figure", 1);
// -----------------------------------------------
// Creates a label generator object initialized with the given
// string prefix and sequence number.
//
    LabelGenerator(std::string prefix, int index);
    
//
// Method: nextLabel
// Usage: cout << label.nextLabel() << endl;
// -----------------------------------------
// Returns the next label in the sequence by appending the string prefix
// and incremented sequence number.
//
    
    std::string nextLabel();
    
//
// Method: toString
// Usage: std::string label.toString();
// ------------------------------------
// Returns the string form of a label by appending the current values
// of the label generator's prefix string and sequence number.
//
    
    std::string toString();
    
private:
    
// Instance variables
    
    std::string prefix;
    int index;
    
}; // end Date

#endif // labelgen_h
