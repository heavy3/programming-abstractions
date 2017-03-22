//
// main.cpp
//
// This program implements a simple calculator that evaluates expressions
// from left to right.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 11
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/23/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cctype>
#include "tokenscanner.h"
#include "filelib.h"
#include "error.h"
#include "set.h"
#include "stack.h"
#include "strlib.h"
#include <string>
#include "simpio.h"

// Function prototypes

Set<char> setFromString(std::string str);
bool strContains(std::string str, char ch);

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.11\n";
const std::string DETAIL = "Expression Calculator";
const std::string BANNER = HEADER + DETAIL;
const std::string PROMPT = "> ";
const   Set<char> ARITHMETIC_OPERATORS = setFromString("+-*/");

// Helper function to create a set from a string of characters.

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

class ExpressionCalculator {
 
public:
    ExpressionCalculator() {
        this->expr = "";
        scanner.setInput(this->expr);
        scanner.ignoreWhitespace(); // Do not treat whitespace as a token.
        scanner.scanNumbers();
    }
    
    ExpressionCalculator(std::string expr) {
        this->expr = expr;
        scanner.setInput(this->expr);
        scanner.ignoreWhitespace(); // Do not treat whitespace as a token.
        scanner.scanNumbers();
    }
    
    void setExpr(std::string expr) {
        this->expr = expr;
        scanner.setInput(this->expr);
    }
    
    std::string eval(std::string expr) {
        setExpr(expr);
        return eval();
    }
    
    std::string eval() {
        double result;
        
        while (scanner.hasMoreTokens()) {
            std::string token = scanner.nextToken();
            
            if (isnumber(token[0])) {
                operands.push(token);
            } else {
                if (!ARITHMETIC_OPERATORS.contains(token[0])) {
                    error("Invalid operator: " + string(1, token[0]));
                }
                operators.push(token[0]);
            }
            
            if ((operands.size() == 2) && (operators.size() == 1)) {
                
                double r2 = stringToReal(operands.pop());
                double r1 = stringToReal(operands.pop());
                
                char op = operators.pop();
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
                operands.push(realToString(result));
            }
        }
        return operands.pop();
    }
    
private:
    std::string expr;
    TokenScanner scanner;
    Stack<std::string> operands;
    Stack<char> operators;
};

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    std::cout << "Allows simple left-to-right evaluation of arithmetic\n";
    std::cout << "expressions of numbers and operators (+,-,*,/).\n\n";
    
    // Test Data
    //
    // std::string input = "4+9-2*16+1/3*6-67+8*2-3+26-1/34+3/7+2-5";
    // std::string input = "20.4 - 3.6 * 2.5";
    // std::string input = "2 ~ 2";
    
    ExpressionCalculator calc;
    std::string input;
    
    while ((input = getLine(PROMPT)) != "") {
        std::cout << calc.eval(input) << std::endl;
    }
    return 0;
}
