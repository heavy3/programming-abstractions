//
// main.cpp
//
// List the variables in an expression in alphabetical order on the console.
//
// I came up with two ways to solve this.  The first approach recursively
// walks the unevaluated expression tree, in search of IdentifierExp nodes.
//
// The second approach iterates over the symbol table in the context object,
// but requires that the expression first be evaluated so the symbol table is
// fully populated before iteration.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 09
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/29/17 and 4/01/17
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
void testListVariablesFunctor();

template <typename NumType>
void testListVariablesFunction(std::string expStr);

template <typename NumType>
void getVariables(Set<std::string>& varNames, Expression<NumType>* exp);

template <typename NumType>
void listVariables(Expression<NumType>* exp);

template <typename NumType>
void getVariables(Set<std::string>& varNames, Expression<NumType>* exp);

// Main program

int main() {
    cout << BANNER << endl;
    
    testListVariablesFunction<double>("z = 3 * x * x - 4 * x - 2 * a + y");
    testListVariablesFunctor<double>();
    
    return 0;
}

// Function: testListVariablesFunction
// Usage: testListVariablesFunction("3 * x * x - 4 * x - 2 * a + y");
// -----------------------------------------------------------------
// Lists the variables in the expression string to the console in alpahbetical
// order.
//
// This is accomplished by parsing the expression string to create
// an expression tree that is walked by listVariables() in search
// of unique variable names (wrappered in IdentifierExp leaf nodes).

template <typename NumType>
void testListVariablesFunction(std::string expStr) {
    cout << endl;
    cout << "Solve approach: Walk unevaulated expression tree." << endl;
    cout << string(60, '-') << endl;
    cout << "Given the expression: " << expStr << endl << endl;
    cout << "The variables in the final expression are:" << endl;

    try {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(expStr);
        
        Parser<NumType> parser;
        Expression<NumType>* exp = parser.parseExp(scanner);

        listVariables(exp);
    } catch (ErrorException& ex) {
        cerr << "Error: " << ex.getMessage() << endl;
    }
}

// Function: listVariables
// Usage: listVariables(expPtr);
// -----------------------------
// Lists the variable names within an expression tree to the console
// in lexigraphical order.

template <typename NumType>
void listVariables(Expression<NumType>* exp) {
    Set<std::string> varNames;
    
    getVariables(varNames, exp);
    for (std::string varName: varNames) {
        cout << varName << endl;
    }
}

// Function: getVariables
// Usage: getVariables(nameSet, expPtr);
// -------------------------------------
// Returns the variable names referenced in an expression as a
// set of strings passed by reference back to the client.

template <typename NumType>
void getVariables(Set<std::string>& varNames, Expression<NumType>* exp) {
    if (exp == NULL) return;
    
    ExpressionType et = exp->getType();
    switch (et) {
        case CONSTANT:
            break;
        case IDENTIFIER:
            varNames.add(exp->getIdentifierName());
            break;
        case COMPOUND:
            getVariables(varNames, exp->getLHS());
            getVariables(varNames, exp->getRHS());
            break;
        default:
            error("getVariables: Unknown expression type");
    }
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

// Function: testListVariablesFunctor
// Usage: testListVariablesFunctor<double>();
// --------------------------------
// This test harness drives a function object instantiated as listVariables().

template <typename NumType>
void testListVariablesFunctor() {
    cout << endl;
    cout << "Solve approach: Iterate symbol table of eval'd expression." ;
    cout << endl;
    cout << string(60, '-') << endl;
    cout << "Given the expressions: " << endl;
    
    Parser<NumType> parser;
    EvaluationContext<NumType> context;
    TokenScanner scanner;
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
