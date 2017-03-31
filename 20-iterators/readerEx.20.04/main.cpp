//
// main.cpp
//
// Add support to the arithmetic expression parser for evaluating function
// expressions such as sqrt(2).
//
// See: parser.cpp readT() for enhancement.
//
// From a BNF perspective, I've extended the grammar of a term to include:
//
//    T -> fn(E)
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 04
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/31/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
#include "error.h"
#include "exp.h"
#include "parser.h"
#include "tokenscanner.h"
using namespace std;

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 20.04\n";
const std::string DETAIL = "Arithmetic expression parser with functions.";
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

// Function: repl
// Usage: repl();
// --------------
// Performs a 'read', 'evaluate', 'print', 'loop' sequence until 'quit'
// is read from the input.

void repl() {
    
    // Add support for evaluating functions that take a double and return
    // a double.  TODO: May want to have a default fnTable built as part
    // of the EvaluationContext constructor to unburden the client.
    
    Map<std::string, doubleFn> fnTable;
    fnTable["sin"] = sin;
    fnTable["cos"] = cos;
    fnTable["sqrt"] = sqrt;
    EvaluationContext context(fnTable);
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
            exp = parseExp(scanner, fnTable);
            double value = exp->eval(context);
            cout << value << endl;
        } catch (ErrorException& ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
        if (exp != NULL) delete exp;
    }
}
