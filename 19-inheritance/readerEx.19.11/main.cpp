//
// main.cpp
//
// Add unary minus (-) operator to the arithmetic expression parser.
//
// See: parser.cpp readT() for enhancement.
//
// From a BNF perspective, I've extended the grammar of a term to include:
//
//    T -> -T
//
// As implemented, this gives highest precedence to unary operators.
// There may be a nicer implementation that allows unary precedence to be
// expressed numerically (as with the binary operators).
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 11
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/29/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
#include "error.h"
#include "exp.h"
#include "parser.h"
#include "tokenscanner.h"
using namespace std;

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 19.11\n";
const std::string DETAIL = "Inheritance: Add unary minus to arithmetic parser";
const std::string BANNER = HEADER + DETAIL;
const std::string PROMPT = "=> ";

// Function prototypes

ostream& operator<<(ostream& os, Expression* exp);
void repl();

// Main program

int main() {
    cout << BANNER << endl << endl;
    repl();
    return 0;
}

// Operator: <<
// Usage: cout << expressionPtr << endl;
// -------------------------------------
// Overrides the insertion stream operator to enable console output of
// a string-form of an arithmetic expression.

ostream& operator<<(ostream& os, Expression* exp) {
    os << exp->toString();
    return os;
}

void repl() {
    EvaluationContext context;
    TokenScanner scanner;
    
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    
    while (true) {
        Expression* exp = NULL;
        try {
            string line;
            cout << PROMPT;
            getline(cin, line);
            if (line == "quit") break;
            scanner.setInput(line);
            exp = parseExp(scanner);
            double value = exp->eval(context);
            cout << value << endl;
        } catch (ErrorException& ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
        if (exp != NULL) delete exp;
    }
}
