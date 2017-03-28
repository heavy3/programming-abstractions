//
// main.cpp
//
// Alter arithmetic interpreter to use doubles instead of integers.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 08
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/27/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
#include "error.h"
#include "exp.h"
#include "parser.h"
#include "tokenscanner.h"
using namespace std;

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 19.08\n";
const std::string DETAIL = "Inheritance: Arithmetic interpreter w/ doubles.";
const std::string BANNER = HEADER + DETAIL;
const std::string PROMPT = "=> ";

void repl();

// Main program

int main() {
    cout << BANNER << endl << endl;
    cout << "Enter an arithmetic expression to evaluate (e.g., x = 3.5 + 4)";
    cout << endl;
    
    repl(); // read, evaluate, print, loop

    return 0;
}

void repl() {
    EvaluationContext<double> context;
    TokenScanner scanner;
    Parser<double> parser;
    
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    
    while (true) {
        Expression<double>* exp = NULL;
        try {
            string line;
            cout << PROMPT;
            getline(cin, line);
            if (line == "quit") break;
            scanner.setInput(line);
            exp = parser.parseExp(scanner);
            double value = exp->eval(context);
            cout << value << endl;
        } catch (ErrorException& ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
        if (exp != NULL) delete exp;
    }
}
