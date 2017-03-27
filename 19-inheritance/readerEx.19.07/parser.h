//
// parser.h
//
// This file defines the interface for a simple, recursive descent parser
// which can parse arithmetic expressions.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 07
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//

#ifndef parser_h
#define parser_h

#include "tokenscanner.h"

// Function: parseExp
// Usage: Expression* exp = parseExp(scanner);
// -------------------------------------------
// This function returns the next expression scanned from one or more
// input tokens.

Expression* parseExp(TokenScanner& scanner);

// Function: readE
// Usage: Expression* exp = readE(scanner, precedence);
// ----------------------------------------------------
// This function returns the next expression found from a token scanner,
// using token precedence to resolve ambiguity in the grammar.

Expression* readE(TokenScanner& scanner, int prec);

// Function: readT
// Usage: Expression* term = readT(scanner);
// -----------------------------------------
// This functions scans an input term, which is either an iteger, identifier, or
// subexpression enclosed in paretheses.

Expression* readT(TokenScanner& scanner);

// Function: precedence
// Usage: int prec = precedence(token);
// ------------------------------------
// Checks the token against the defined operators and returns the corresponding
// precedence value.  A value of 0 implies the token is -not- an operator.

int precedence(string token);

#endif // parser_h
