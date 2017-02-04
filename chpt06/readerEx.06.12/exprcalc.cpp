//
//  exprcalc.cpp
//
//  This file implements the ExpressionCalculator class.
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

#include "exprcalc.h"

// Function prototypes

Set<char> setFromString(std::string str);
bool strContains(std::string str, char ch);

// Constants

const std::string PROMPT = "> ";
const Set<char> VALID_ARITH_OPS = setFromString("+-*/");

// Class implementation

Map<std::string,std::string> ExpressionCalculator::variables;

// Constructors

ExpressionCalculator::ExpressionCalculator() {
    this->expr = "";
    scanner.setInput(this->expr);
    scanner.ignoreWhitespace(); // Do not treat whitespace as a token.
    scanner.scanNumbers();
}

ExpressionCalculator::ExpressionCalculator(std::string expr) {
    this->expr = expr;
    scanner.setInput(this->expr);
    scanner.ignoreWhitespace(); // Do not treat whitespace as a token.
    scanner.scanNumbers();
}

void ExpressionCalculator::setExpr(std::string expr) {
    this->expr = expr;
    scanner.setInput(this->expr);
}

std::string ExpressionCalculator::eval(std::string expr) {
    setExpr(expr);
    return eval();
}

//
// Implementation notes: eval
// --------------------------
// This method parses and evaluates an arithmetic expression represented
// as a string already associated with the scanner via the constructor
// or possibly the setExpr method.
//
// Two stacks and a symbol table are used to hold operands, operators and 
// variable values, respectively.
//
// Evaluation is generally immediate when two operands and an operator are
// detected on the stacks.  However, variable assignment involves a recursive
// evaluation of the complete rhs before associating the resultant value
// with the variable name in the map.
//

std::string ExpressionCalculator::eval() {
    double result;
    
    while (scanner.hasMoreTokens()) {
        std::string token = scanner.nextToken();
        TokenType tokenType = scanner.getTokenType(token);
        
        switch (tokenType) {
            case NUMBER:
                operands.push(token);
                break;
                
            case WORD: {
                // Variable reference or assignment?
                
                // Use existence and relative position of '=' within the
                // expression to decide.
                
                int equalPos = expr.find('=');
                int varPos = expr.find(token);
                
                if ((equalPos == string::npos) || (varPos > equalPos)) {
                    
                    // Assume variable reference.
                    
                    if (ExpressionCalculator::variables.containsKey(token)) {
                        operands.push(ExpressionCalculator::variables.get(token));
                    } else {
                        error("Undefined variable: " + token);
                    }
                    
                } else {
                    
                    // Assume variable assignment.
                    
                    // Push name of variable onto the operand stack until the
                    // rhs has been evaluated.
                    
                    operands.push(token);
                }}
                break;
                
            case OPERATOR:
                if (VALID_ARITH_OPS.contains(token[0])) {
                    operators.push(token[0]);
                } else if (token[0] == '=') {
                    
                    // Evaluate rhs of variable assignment and update
                    // symbol table with the name:value pair.
                    
                    ExpressionCalculator rhsCalc;
                    
                    std::string rhs = rhsCalc.eval(expr.substr(scanner.getPosition()));
                    std::string var = operands.pop();
                    
                    ExpressionCalculator::variables.put(var, rhs);
                    
                    // Drain the rest of the tokens in the scanner since
                    // these have been evaluated in the rhs calculator.
                    
                    while (scanner.hasMoreTokens()) token = scanner.nextToken();
                    
                } else error("Invalid operator: " + string(1, token[0]));
                break;
                
            default:
                error("Unexpected input: " + token);
                break;
        }

        // Combine operands with some math.
        
        if ((operands.size() == 2) && (operators.size() == 1)) {
            
            char op = operators.pop();
            
            double r2 = stringToReal(operands.pop());
            double r1 = stringToReal(operands.pop());
            
            switch (op) {
                case '+':
                    result = r1 + r2;
                    break;
                    
                case '-':
                    result = r1 - r2;
                    break;
                    
                case '*':
                    result = r1 * r2;
                    break;
                    
                case '/':
                    result = r1 / r2;
                    break;
                    
                default:
                    error("Invalid operator: " + string(1, op));
                    break;
            }
            
            // Push the result /back/ onto the stack as the left hand
            // operand in a subsequent evaluation.
            
            operands.push(realToString(result));
        }
    }
    
    // Figure out what to return.
    
    // Only return stuff when the input expression is well-formed,
    // leaving a single value on the operand stack as the result.
    
    if (operands.size() == 1) {
        std::string strResult = operands.pop();
        
        if (ExpressionCalculator::variables.containsKey(strResult)) {
            
            // User typed in a variable on a line by itself.
            // Return the value of that variable.
            //
            // E.g., > pi
            //       3.14159
            
            return ExpressionCalculator::variables[strResult];
        }
        
        // Assume we're returning a numeric result in string form.
        
        return strResult;
    }
    
    // Don't return anything on a variable assignment per the specs
    // or if the input expression is not well-formed.
    //
    // E.g.  > pi = 3.14159
    //       >
    
    return "";
}

// Free functions

Set<char> setFromString(string str) {
    Set<char> set;
    
    for (int i = 0; i < str.length(); i++) {
        set.add(str[i]);
    }
    return set;
}

bool strContains(std::string str, char ch) {
    return (str.find(string(1,ch)));
}
