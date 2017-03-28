//
// parser.h
//
// This file defines the interface for a simple, recursive descent parser
// which can parse arithmetic expressions.
//
// I've extended this to be a templated class to support various numeric
// formats.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 07
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/27/17
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//

#ifndef parser_h
#define parser_h

#include "tokenscanner.h"

template <typename NumType>
class Parser {

public:
    
    // Function: parseExp
    // Usage: Expression* exp = parseExp(scanner);
    // -------------------------------------------
    // This function returns the next expression scanned from one or more
    // input tokens.

    Expression<NumType>* parseExp(TokenScanner& scanner);

    // Function: readE
    // Usage: Expression* exp = readE(scanner, precedence);
    // ----------------------------------------------------
    // This function returns the next expression found from a token scanner,
    // using token precedence to resolve ambiguity in the grammar.

    Expression<NumType>* readE(TokenScanner& scanner, int prec);

    // Function: readT
    // Usage: Expression* term = readT(scanner);
    // -----------------------------------------
    // This functions scans an input term, which is either an iteger, identifier, or
    // subexpression enclosed in paretheses.

    Expression<NumType>* readT(TokenScanner& scanner);

    // Function: precedence
    // Usage: int prec = precedence(token);
    // ------------------------------------
    // Checks the token against the defined operators and returns the corresponding
    // precedence value.  A value of 0 implies the token is -not- an operator.

    int precedence(string token);
};

#include "parser.cpp"
#endif // parser_h
