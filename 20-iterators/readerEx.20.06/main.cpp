//
// main.cpp
//
// Integrate ExpressionFunction from exercise 5 with the plot function in
// exercise 3 to create a generalized function renderer with user-supplied
// functions.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 06
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
#include "parser.h"
#include "tokenscanner.h"
#include "plot.h"
#include "gwindow.h"
#include <cmath>
#include "exp.h"

using namespace std;

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 20.06\n";
const std::string DETAIL = "Expression parsing, evaluation, and rendering.";
const std::string BANNER = HEADER + DETAIL;
const std::string PROMPT = "=> ";
const double PI = M_PI;

// Function prototypes

ostream& operator<<(ostream& os, Expression* exp);
void repl();

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    // Test data
    
//    Vector<double> xvals;
//    xvals += 7;
//    xvals += -7;
//    xvals += 0;
//    xvals += 19.5;
//    
//    std::string expStr = "2 * x + 3";
//    ExpressionFunction fn(expStr);
//    
//    cout << "Given: f(x) = " << expStr << endl << endl;
//    for (int i = 0; i < xvals.size(); i++) {
//        cout << "       f(" << left << xvals[i] << ") ";
//        cout << "= " << fn(xvals[i]) << endl;
//    }
    
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

    EvaluationContext context;
    TokenScanner scanner;
    
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    
    cout << "I plot single-variable functions, f(x), over a given x interval.";
    cout << endl << endl;
    cout << "For example:" << endl << endl;
    cout << "minX = -PI" << endl;
    cout << "maxX = PI" << endl;
    cout << "minY = -2" << endl;
    cout << "maxY = 2" << endl;
    cout << "sin(2 * x) + cos(3 * x)" << endl << endl;
    
    double minX = -PI;
    double maxX =  PI;
    double minY = -2;
    double maxY = 2;
    
    context.setValue("minX", minX);
    context.setValue("maxX", maxX);
    context.setValue("minY", minY);
    context.setValue("maxY", maxY);
    context.setValue("PI", PI);
    
    GWindow gw;
    while (true) {
        Expression* exp = NULL;
        try {
            string line;
            cout << PROMPT;
            getline(cin, line);
            if (line == "") continue;
            if (line == "quit") break;
            if (line == "clear") {
                gw.clear();
            } else {
                scanner.setInput(line);
                if (line.find('=') == string::npos) {
                    ExpressionFunction fn(line);
                    gw.setWindowTitle(line);
                    if (context.isDefined("minY") &&
                        context.isDefined("maxY")) {
                        plot(gw,
                             fn,
                             context.getValue("minX"),
                             context.getValue("maxX"),
                             context.getValue("minY"),
                             context.getValue("maxY"));
                    } else {
                        plot(gw,
                             fn,
                             context.getValue("minX"),
                             context.getValue("maxX"));
                    }
                } else {
                    fnTable = context.getFnTable();
                    exp = parseExp(scanner, fnTable);
                    double value = exp->eval(context);
                    cout << value << endl;
                }
            }
        } catch (ErrorException& ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
        if (exp != NULL) delete exp;
    }
}
