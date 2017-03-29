//
// main.cpp
//
// This program implements a changeVariable function for altering the
// name of a given variable within an expression tree.
//
// A key constraint is to work entirely as a client of exp.h.
// So no messing with the internals of the Expression class hierarchy.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 10
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

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 19.10\n";
const std::string DETAIL = "Inheritance: changeVariable() in an expression";
const std::string BANNER = HEADER + DETAIL;

// Function prototypes

ostream& operator<<(ostream& os, Expression* exp);
void testExpression();
Expression* changeVariables(Expression* exp, string oldName, string newName);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    testExpression();
    return 0;
}

// Function: testExpression
// Usage: testExpression();
// --------------------------------
// This test harness drives a function object instantiated as listVariables().

void testExpression() {
    EvaluationContext context;
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    
    // Build up a non-trivial expression that includes variables.
    
    Vector<std::string> vExpr;
    vExpr += "a = 3";
    vExpr += "y = 5";
    vExpr += "x = 42";
    vExpr += "z = 3 * x * x - 4 * x - 2 * a + y"; // We'll mutate this last one.
    for (int i = 0; i < vExpr.size(); i++) {
        Expression* exp = NULL;
        try {
            scanner.setInput(vExpr[i]);
            exp = parseExp(scanner);
            exp->eval(context);
            if (i == vExpr.size() - 1) {
                cout << "Given: " << exp << endl << endl;
                cout << "change: 'x' to 'X'" << endl << endl;
                
                Expression* newExpr = changeVariables(exp, "x", "X");
                
                cout << "new: " << newExpr << endl;
                cout << "old: " << exp     << endl << endl;
                delete newExpr;
            }
        } catch (ErrorException& ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
        if (exp != NULL) delete exp;
    }
}

// Function: changeVariables
// Usage: Expression* newExpr = changeVariables(exp, "x", "X");
// ------------------------------------------------------------
// Returns a pointer to a newly allocated expression tree that matches
// an input expression tree but with all variables of a given name
// updated with a new name.
//
// Recursively walk the expression tree, allocating new
// nodes of the appropriate type as we proceed.  If we hit an identifier
// node, we check if its name should be changed.  Compound nodes imply
// the need to recursively handoff the left and right subexpressions
// for evaluation before building a new compound node with mutated subtrees.

Expression* changeVariables(Expression* exp, string oldName, string newName) {
    if (exp == NULL) return NULL;
    
    Expression* nexp;  // Polymorphic base type.  Use pointer to avoid slicing.
    
    switch (exp->getType()) {
        case CONSTANT:
            nexp = new ConstantExp(exp->getConstantValue());
            break;
            
        case IDENTIFIER:
            if (exp->getIdentifierName() == oldName) {
                // Rename the variable here.
                nexp = new IdentifierExp(newName);
            } else {
                // Preserve existing names of other variables.
                nexp = new IdentifierExp(exp->getIdentifierName());
            }
            break;
            
        case COMPOUND:
        default:
            // Recurse down the left and right sides of the compound expression.
            Expression* lexp = changeVariables(exp->getLHS(), oldName, newName);
            Expression* rexp = changeVariables(exp->getRHS(), oldName, newName);
            nexp = new CompoundExp(exp->getOp(), lexp, rexp);
            break;
    }
    return nexp;
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
