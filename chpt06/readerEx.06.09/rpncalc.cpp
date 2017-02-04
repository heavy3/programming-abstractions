//
// rpncalc.cpp
//
// This file implements a simple RPN calculator that mutates rational
// numbers.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This class reimplements the source code from Figure 5-04 of the course
// reader.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/19/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include "rpncalc.h"

//
// Function: applyOperator
// Usage: applyOperator(op, operandStack);
// ---------------------------------------
// This function applies the operator to the top two elements on the
// operand stack.  Because the elements on the stack are popped in reverse
// order, the right operand is popped before the left operand.
//

void applyOperator(char op, Stack<Rational> & operandStack) {
    Rational result(0);
    if (operandStack.size() < 2) {
        error("Too few operands on stack for that operation.");
    }
    Rational rhs = operandStack.pop();
    Rational lhs = operandStack.pop();
    switch (op) {
        case '+' : result = lhs + rhs; break;
        case '-' : result = lhs - rhs; break;
        case '*' : result = lhs * rhs; break;
        case '/' : result = lhs / rhs; break;
        default:
            error("Illegal operator: " + string(1, op));
    }
    cout << result << endl;
    operandStack.push(result);
}

//
// Function: helpCommand
// Usage: helpCommand();
// ---------------------
// This function generates a help message for the user.
//

void helpCommand() {
    cout << "Enter expressions in Reverse Polish Notation," << endl;
    cout << "in which operators follow the operands to which" << endl;
    cout << "they apply.  Each line consists of a number, an" << endl;
    cout << "operator, or one of the following commands:" << endl;
    cout << "  Q -- Quit the program" << endl;
    cout << "  H -- Display this help message" << endl;
    cout << "  C -- Clear the calculator stack" << endl;
}

//
// Function: run
// Usage: run();
// -------------
// Runs the calculator in the forground, responding to keyboard events
// until the user presses 'Q' to quit or hits 'Enter' on an empty line.
//

void runcalc() {
    cout << "RPN Calculator Simulation (type H for help)" << endl;
    Stack<Rational> operandStack;
    while (true) {
        string line = getLine("> ");
        if (line.length() == 0) line = "Q";
        char ch = toupper(line[0]);
        if (ch == 'Q') {
            break;
        } else if (ch == 'C') {
            operandStack.clear();
        } else if (ch == 'H') {
            helpCommand();
        } else if (isdigit(ch)) {
            operandStack.push(stringToReal(line));
        } else {
            applyOperator(ch, operandStack);
        }
    }
}