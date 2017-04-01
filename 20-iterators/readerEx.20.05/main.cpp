//
// main.cpp
//
// Add support to the arithmetic expression parser for evaluating single
// variable expressions for specific values of that variable.
//
// For example:
//
//    "2 * x + 3"(7) = 2 * 7 + 3 = 17
//
// This achieved through a functor which contructs an expression tree for
// a given string-formed expression and then overloads the () operator so
// specific values for the variable may be passed in at runtime for evaluation.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 05
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/1/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "error.h"
#include "exp.h"
#include "parser.h"
#include "tokenscanner.h"
using namespace std;

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 20.05\n";
const std::string DETAIL = "Single-variable function expression evaluation";
const std::string BANNER = HEADER + DETAIL;
const std::string PROMPT = "=> ";

// Function prototypes

ostream& operator<<(ostream& os, Expression* exp);
void repl();

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    // Test data
    
    Vector<double> xvals;
    xvals += 7;
    xvals += -7;
    xvals += 0;
    xvals += 19.5;
    
    std::string expStr = "2 * x + 3";
    ExpressionFunction fn(expStr);
    
    cout << "Given: f(x) = " << expStr << endl << endl;
    for (int i = 0; i < xvals.size(); i++) {
        cout << "       f(" << left << xvals[i] << ") ";
        cout << "= " << fn(xvals[i]) << endl;
    }
    
    //repl();
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
