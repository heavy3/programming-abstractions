//
// parser.cpp
//
// This file implements the parser interface.  It includes
// support for unary minus and functions like sqrt, cos, sin.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 04
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code mainly comes from Figure 19-14.
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/29/17 & 3/31/17
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <string>
#include "error.h"
#include "exp.h"
#include "parser.h"
#include "strlib.h"
#include "tokenscanner.h"
using namespace std;

// Implementation notes: parseExp
// ------------------------------
// This code reads an expression and then checks for unexpected trailing tokens.

Expression* parseExp(TokenScanner& scanner,
                     Map<std::string, doubleFn>& fnTable) {
    Expression* exp = readE(scanner, fnTable, 0);
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

Expression* readE(TokenScanner& scanner,
                  Map<std::string, doubleFn>& fnTable,
                  int prec) {
    Expression* exp = readT(scanner, fnTable);
    string nextToken;
    while (true) {
        nextToken = scanner.nextToken();
        int tprec = precedence(nextToken);
        if (tprec <= prec) break;
        // Non-zero precedence implies token is an operator
        // (as opposed to constant or variable name).
        std::string op = nextToken;
        Expression* lhs = exp;
        Expression* rhs = readE(scanner, fnTable, tprec);
        exp = new CompoundExp(op, lhs, rhs);
    }
    scanner.saveToken(nextToken);
    return exp;
}

// Implementation notes: readT
// ---------------------------
// This functions scans a term, which is either an iteger, identifier,
// subexpression enclosed in paretheses, or any of the above prefixed by
// a unary minus.
//
// T -> integer
// T -> varname
// T -> (E)
// T -> -T
// T -> fnname(E)

Expression* readT(TokenScanner& scanner,
                  Map<std::string, doubleFn>& fnTable) {
    Expression* exp;
    
    string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    switch(type) {
        case WORD:
            if (fnTable.containsKey(token)) {
                // This could be hardened with some validation testing
                // for a parenthetic argument.  We optimistically assume it is.
                Expression* argExp = readT(scanner, fnTable);
                exp = new FunctionExp(token, argExp);
            } else {
                exp = new IdentifierExp(token);
            }
            break;
        case NUMBER:
            exp = new ConstantExp(stringToReal(token));
            break;
        default:
            if (token == "-") {
                // Alias unary '-E' to '(-1)*E'
                Expression* minus1 = new ConstantExp(-1);
                string nexttoken = scanner.nextToken();
                TokenType nextType = scanner.getTokenType(nexttoken);
                switch(nextType) {
                    case NUMBER:
                        exp = new ConstantExp(-1 * stringToReal(nexttoken));
                        break;
                    case WORD:
                        // This could be hardened with some validation testing
                        // for a parenthetic argument.  We optimistically
                        // assume it is.
                        if (fnTable.containsKey(nexttoken)) {
                            Expression* argExp = readT(scanner, fnTable);
                            Expression* fexp = new FunctionExp(nexttoken,
                                                               argExp);
                            exp = new CompoundExp("*",
                                                  minus1,
                                                  fexp);
                        } else {
                            exp = new CompoundExp("*",
                                                  minus1,
                                                  new IdentifierExp(nexttoken));
                        }
                        break;
                    default:
                        scanner.saveToken(nexttoken);
                        Expression* rexp = readE(scanner, fnTable, 0);
                        exp = new CompoundExp("*", minus1, rexp);
                        break;
                }
            } else {
                // Parenthetic expression
                if (token != "(") error("Illegal term in expression");
                exp = readE(scanner, fnTable, 0);
                if (scanner.nextToken() != ")") {
                    error("Unbalanced parentheses in expression");
                }
            }
            break;
    }
    return exp;
}

// Implementation notes: precedence
// --------------------------------
// Checks the token against the defined operators and returns the corresponding
// precedence value.  A value of 0 implies the token is -not- an operator.

int precedence(string token) {
    if (token == "=") return 1;
    if (token == "+" || token == "-") return 2;
    if (token == "*" || token == "/" || token == "%") return 3;
    return 0;
}
