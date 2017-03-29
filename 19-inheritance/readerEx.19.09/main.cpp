//
// main.cpp
//
// List the variables in an expression in alphabetical order on the console.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 09
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

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 19.09\n";
const std::string DETAIL = "Inheritance: listVariables(expr_ptr)";
const std::string BANNER = HEADER + DETAIL;
const std::string PROMPT = "=> ";

template <typename NumType>
void testExpression();

template <typename NumType>
void listVariables(Expression<NumType>* exp);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    testExpression<double>();
    return 0;
}

// Function Object: ListVariables
// Usage: ListVariables listVariables = ListVariables(context);
//        listVariables(expr_ptr);
// -------------------------------
// This class defines a function object that takes an expression
// and lists the variables associated with that expression in
// alphabetical order on the console.
//
// Expressions with variables imply an evaluation context is associated
// with that expression.  Without it, the parseExp would fail since any
// variables included with the expression would be seen as undefined.
//
// But how to give listVariables visibility into the context given that
// it is only supposed to take the expression pointer as a variable?
//
// The strategy I use borrows a concept of 'function object' from Chapter 20
// that class-wrappers the context as private data that is initialized
// when the ListVariables object is constructed.  This allows subsequent
// expressions to be passed into listVariables and evaluated against that
// pre-requisite context, yielding a fully populated symbol table which can
// then be harvested for variable names through a getter and then displayed
// to the console.
//
// There's probably a more direct approach that probably doesn't involve
// evaluating the expression first, but function objects and closures
// have been on my mind lately and that symbol table looked juicey and
// sweet.

template <typename NumType>
class ListVariables {
public:
    ListVariables(EvaluationContext<NumType> context) {
        this->context = context;
    }
    
    // Operator: ()
    // ------------------------------
    // Defines the behavior of a ListVariables object when it is
    // called as a function.
    //
    // For example, given the expression: 3 * x * x - 4 * x - 2 * a + y
    // (with values for a, x, and y previously established) prints on the
    // console:
    //
    // +--------
    // |a
    // |x
    // |y
    
    void operator()(Expression<NumType>* exp) {
        exp->eval(context);
        for (std::string var: context.getVariables()) {
            std::cout << var << std::endl;
        }
    }
    
private:
    EvaluationContext<NumType> context;
};

// Function: testExpression
// Usage: testExpression<double>();
// --------------------------------
// This test harness drives a function object instantiated as listVariables().
//
template <typename NumType>
void testExpression() {
    EvaluationContext<NumType> context;
    TokenScanner scanner;
    Parser<NumType> parser;
    
    cout << "Given the expressions: " << endl;
    cout << endl;
    
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    
    Vector<std::string> vExpr;
    vExpr += "a = 3";
    vExpr += "y = 5";
    vExpr += "x = 42";
    vExpr += "z = 3 * x * x - 4 * x - 2 * a + y";
    
    for (int i = 0; i < vExpr.size(); i++) {
        cout << vExpr[i] << endl;
        Expression<NumType>* exp = NULL;
        try {
            scanner.setInput(vExpr[i]);
            exp = parser.parseExp(scanner);
            exp->eval(context);
            if (i == vExpr.size() - 1) {
                cout << endl;
                cout << "The variables in the final expression are:" << endl;
                
                ListVariables<NumType> listVariables(context);
                listVariables(exp);
            }
        } catch (ErrorException& ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
        if (exp != NULL) delete exp;
    }
}
