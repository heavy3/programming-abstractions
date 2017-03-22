//
// labelgen.cpp
//
// This file implements the label generator interface.
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

#include <sstream>
#include <iostream>
#include <string>
#include "labelgen.h"

LabelGenerator::LabelGenerator(std::string thePrefix, int theIndex) {
    prefix = thePrefix;
    index = --theIndex; // Pre-decrement since nextLabel will increment.
}

std::string LabelGenerator::nextLabel() {
    index++;
    return toString();
}

std::string LabelGenerator::toString() {
    std::ostringstream oss;
    
    oss << prefix << index;
    return oss.str();
}
