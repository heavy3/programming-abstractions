//
// exprcalc.h
//
// This interface exports the ExpressionCalculator class, providing
// a four-function calculator supporting:
//
//    addition, subtraction, multiplication, division
//
// Order of operations is from left-to-right (contrary to conventional
// precedance which would usually evaluate multiplication and division
// before addition and subtraction).
//
// Variable assignment is allowed:
//
//    > pi = 3.1415926535
//    > r = 1.5
//    > area = pi * r * r
//    > area
//    7.06858
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/24/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#ifndef exprcalc_h
#define exprcalc_h

#include <string>
#include "error.h"
#include "map.h"
#include "set.h"
#include "stack.h"
#include "strlib.h"
#include "tokenscanner.h"

extern const std::string PROMPT;
extern const Set<char> ARITHMETIC_OPERATORS;

class ExpressionCalculator {
    
public:
    ExpressionCalculator();
    ExpressionCalculator(std::string expr);
    
    void setExpr(std::string expr);
    std::string eval();
    std::string eval(std::string expr);
    
private:
    std::string expr;
    TokenScanner scanner;
    Stack<std::string> operands;
    Stack<char> operators;
    
    // Make the symbol table static so variable definitions are visible
    // to recursive evaluations especially when referencing variables
    // on the rhs of assignment expressions.
    //
    // NB: Maybe a constructor that references an existing symbol table
    //     would be a better option since it seems wrong-ish to have
    //     all objects of this class share the same variable map.
    
    static Map<std::string,std::string> variables;
};

#endif // exprcalc_h
