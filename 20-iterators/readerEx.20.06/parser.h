//
// parser.h
//
// This file defines the interface for a simple, recursive descent parser
// for arithmetic expressions.
//
// It has been extended to support parsing functions such as sqrt, sin, cos
// by way of a function table which maps function names in string form to
// function pointers.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 04
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/31/17
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)

#ifndef parser_h
#define parser_h

#include <string>
#include "tokenscanner.h"
#include "exp.h"
#include "map.h"

// Function: parseExp
// Usage: Expression* exp = parseExp(scanner, fnTable);
// ----------------------------------------------------
// This function returns the next expression scanned from one or more
// input tokens.

Expression* parseExp(TokenScanner& scanner, Map<std::string, doubleFn>& fnTable);

// Function: readE
// Usage: Expression* exp = readE(scanner, fnTable, precedence);
// -------------------------------------------------------------
// This function returns the next expression found from a token scanner,
// using token precedence to resolve ambiguity in the grammar.

Expression* readE(TokenScanner& scanner,
                  Map<std::string, doubleFn>& fnTable,
                  int prec);

// Function: readT
// Usage: Expression* term = readT(scanner, fnTable);
// --------------------------------------------------
// This functions scans an input term, which is either an iteger, identifier,
// function with argument enclosed in parentheses, or subexpression enclosed in
// parentheses.

Expression* readT(TokenScanner& scanner,
                  Map<std::string, doubleFn>& fnTable);

// Function: precedence
// Usage: int prec = precedence(token);
// ------------------------------------
// Checks the token against the defined operators and returns the corresponding
// precedence value.  A value of 0 implies the token is -not- an operator.

int precedence(std::string token);

#endif // parser_h
