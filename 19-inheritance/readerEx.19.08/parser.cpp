//
// parser.cpp
//
// This file implements the parser interface.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 07
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code mainly comes from Figure 19-14.
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/27/17
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)

#include <iostream>
#include <string>
#include "error.h"
#include "parser.h"
#include "exp.h"
#include "strlib.h"
#include "tokenscanner.h"
using namespace std;

// Implementation notes: parseExp
// ------------------------------
// This code reads an expression and then checks for unexpected trailing tokens.

template <typename NumType>
Expression<NumType>* Parser<NumType>::parseExp(TokenScanner& scanner) {
    Expression<NumType>* exp = readE(scanner, 0);
    if (scanner.hasMoreTokens()) {
        error("Unexpected token \"" + scanner.nextToken() + "\"");
    }
    return exp;
}

// Implementation notes: readE
// ---------------------------
// This method returns the next expression found from a token scanner,
// using token precedence to resolve ambiguity in the grammar.
//
// At each level, the parser reads operators and subexpressions
// until it finds an operator whose precedence is greater than the
// prevailing one.  In this case, readE calls itself to read the subexpression
// as a unit.

template <typename NumType>
Expression<NumType>* Parser<NumType>::readE(TokenScanner& scanner, int prec) {
    Expression<NumType>* exp = readT(scanner);
    string nextToken;
    while (true) {
        nextToken = scanner.nextToken();
        int tprec = precedence(nextToken);
        if (tprec <= prec) break;
        // Non-zero precedence implies token is an operator
        // (as opposed to constant or variable name).
        std::string op = nextToken;
        Expression<NumType>* lhs = exp;
        Expression<NumType>* rhs = readE(scanner, tprec);
        exp = new CompoundExp<NumType>(op, lhs, rhs);
    }
    scanner.saveToken(nextToken);
    return exp;
}

// Implementation notes: readT
// ---------------------------
// This functions scans a term, which is either an iteger, identifier, or
// subexpression enclosed in paretheses.

template <typename NumType>
Expression<NumType>* Parser<NumType>::readT(TokenScanner& scanner) {
    Expression<NumType>* exp;
    
    string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    switch(type) {
        case WORD:
            exp = new IdentifierExp<NumType>(token);
            break;
        case NUMBER: {
            NumType val;
            stringstream ss;
            ss << token;
            ss >> val;
            exp = new ConstantExp<NumType>(val);
            }break;
        default:
            if (token != "(") error("Illegal term in expression");
            exp = readE(scanner, 0);
            if (scanner.nextToken() != ")") {
                error("Unbalanced parentheses in expression");
            }
            break;
    }
    return exp;
}

// Implementation notes: precedence
// --------------------------------
// Checks the token against the defined operators and returns the corresponding
// precedence value.  A value of 0 implies the token is -not- an operator.

template <typename NumType>
int Parser<NumType>::precedence(string token) {
    if (token == "=") return 1;
    if (token == "+" || token == "-") return 2;
    if (token == "*" || token == "/" || token == "%") return 3;
    return 0;
}
