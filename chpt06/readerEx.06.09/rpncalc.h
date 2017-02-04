//
// rpncalc.h
//
// This file exports an interface for a Reverse Polish Notation calculator
// that operates upon rational numbers.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This class reimplements the source code from Figure 5-04 of the course
// reader.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/19/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#ifndef rpncalc_h
#define rpncalc_h

#include <iostream>
#include <cctype>
#include <string>
#include "error.h"
#include "simpio.h"
#include "stack.h"
#include "strlib.h"
#include "rational.h"

// Function prototypes

void applyOperator(char op, Stack<Rational> & operandStack);
void helpCommand();
void runcalc();

#endif // rpncalc_h
