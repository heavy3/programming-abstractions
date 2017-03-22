//
// main.cpp
//
// This program reimplements the expression calculator from Chapter 6, ex. 11
// so both the input and output values are represented in hexadecimal.
//
// For example:
//
//    ----------------HexCalculator-----------------
//    > 2 + 2
//    4
//    > 9 + 3
//    C
//    > 10000 - 1
//    FFFF
//    > 6 * 7
//    2A
//    > FEED - CAFE
//    33EF
//    ----------------------------------------------
//
// TODO: Split this up into an interface and a test driver.
//       Fix code duplication between isNumber and stringToInteger.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 11, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 6/16/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <ostream>
#include "vector.h"
#include <cctype>
#include <iostream>
#include <string>
#include "filelib.h"
#include "error.h"
#include "set.h"
#include "stack.h"
#include "strlib.h"
#include "simpio.h"
#include "tokenscanner.h"

using namespace std;

// Function prototypes

std::string integerToString(int n, const int base);
std::string integerToStringR(const int n, const int base);
std::string integerToStringRec(const int n, const int base);
int stringToInteger(const std::string str, const int base);
void exitOnBadBase(const int base);
bool isNumber(const std::string str, const int base);
Set<char> setFromString(std::string str);
bool strContains(std::string str, char ch);

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 11.04\n";
const std::string DETAIL = "Hex Expression Calculator";
const std::string BANNER = HEADER + DETAIL;
const std::string DIGITS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const int MAX_BASE = DIGITS.size();
const int MIN_BASE = 2;
const std::string PROMPT = "> ";
const Set<char> ARITHMETIC_OPERATORS = setFromString("+-*/");
const int DFLT_BASE = 16;

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

// Function: integerToString
// Usage: string str = integerToString(42, 16); // returns "2A"
// -------------------------------------------------------------
// Returns the string form of an integer.
//
// The input base must fall within the supported range:
//
//      MIN_BASE <= base <= MAX_BASE
//
// otherwise an error message is displayed and the program exits.

std::string integerToString(int n, const int base) {
    std::string result;
    std::string sign = (n < 0) ? "-" : "";
    
    exitOnBadBase(base);
    while (true) {
        int onesDigit = abs(n) % base;
        if ((onesDigit == 0) && (abs(n) < base)) {
            break;
        }
        result = DIGITS[onesDigit] + result;
        n /= base;
    }
    return sign + result;
}

// Function: integerToStringR
// Usage: cout << integerToStringR(42, 16) << endl;
// ------------------------------------------------
// Returns the string form of an integer.
//
// The input base must fall within the supported range:
//
//      MIN_BASE <= base <= MAX_BASE
//
// otherwise an error message is displayed and the program exits.
//
// This is a wrapper function for the recursive primitive, integerToStringRec.
// It sets up the call to the primitive and manages the sign for negative
// input values.

std::string integerToStringR(const int n, const int base) {
    std::string result;
    std::string sign = (n < 0) ? "-" : "";
    
    result = integerToStringRec(abs(n), base);
    return sign + result;
}

// Function: integerToStringRec
// Usage: string str = integerToStringRec(42, 16); // returns "2A"
// ---------------------------------------------------------------
// This function uses recursion to return the string-form of a
// non-negative integer.
//
// The input base must fall within the supported range:
//
//      MIN_BASE <= base <= MAX_BASE
//
// otherwise an error message is displayed and the program exits.
//
// Implementation Notes
//
// The input number is decomposed into higher order digits and a
// ones-digit, with the latter easily converted to a single-character
// string on a subsequent recursive call.
//
// The results of that conversion are appended to the
// recursively-converted higher-order digits to form a complete string.

std::string integerToStringRec(const int n, const int base) {
    if (n < 0) {
        error("integerToStringRec: Input number must be non-negative.");
    }
    exitOnBadBase(base);
    if (n < base) {
        return std::string(1, DIGITS[n]);
    }
    return integerToStringRec(n / base, base) + integerToString(n % base, base);
}

// Function: exitOnBadBase
// Usage: exitOnBadBase(base);
// ---------------------------
// If the input base is outside the supported range specified by the constants
// MIN_BASE and MAX_BASE, an error message is displayed and the program exits.

void exitOnBadBase(const int base) {
    ostringstream oss;
    
    if (base < MIN_BASE || base > MAX_BASE) {
        oss << "Input base of " << base << " is out of range. ";
        oss << "Must be (" << MIN_BASE << " <= base <= " << MAX_BASE << ").";
        error(oss.str());
    }
}

// Function: stringToInteger
// Usage: int myInt = stringToInteger("111111", 2); // myInt = 63
// --------------------------------------------------------------
// Converts a string representation of a number under a specified base
// to an integer.
//
// Exits if input cannot be represented under the given base or the
// specified base is less than the MIN_BASE minimum.
//
// Limitations: Doesn't check for overflow.

int stringToInteger(const std::string str, const int base) {
    if (str.size() < 1) error("stringToInteger: Empty input string.");
    if (base < MIN_BASE) error("stringToInteger: Invalid base.");
    
    int result = 0;
    int place = 1;
    int sign = 1;
    
    for (int i = (str.size() - 1); i >= 0; i--) {
        char digit = toupper(str[i]);
        int n = 0;
        if (isalpha(digit)) {
            n = digit - 'A';
        } else if (isnumber(digit)){
            n = digit - '0';
        } else {
            // Handle case where left-most character is '-' or '+'.
            if (i == 0 && digit == '-' && str.size() > 1) {
                sign = -1;
            } else if (i == 0 && digit == '+' && str.size() > 1) {
                sign = 1;
            } else {
                error("stringToInteger: Out of range.");
            }
        }
        if (n >= base) {
            error("stringToInteger: Invalid input string for specified base.");
        }
        result += n * place;
        place *= base;
    }
    
    if (sign < 0) {
        return -result;
    } else {
        return result;
    }
}

// Function: isNumber
// Usage: if (isNumber("DEADBEEF", 16)) { . . . }
// ----------------------------------------------
// Returns true if the input string can be interpreted as a number under
// the specified base.
//
// Similar to stringToInteger(str, base) except without the runtime
// exit to the operating system if the str isn't convertible to a number.
//
// TODO: Do something about the replicated code with stringToInteger.

bool isNumber(const std::string str, const int base) {
    
    if (str.size() < 1) return false;
    if (base < MIN_BASE) return false;
    
    int sign = 1;
    
    bool result = true;
    for (int i = (str.size() - 1); i >= 0; i--) {
        char digit = toupper(str[i]);
        int n = 0;
        if (isalpha(digit)) {
            n = digit - 'A';
        } else if (isnumber(digit)){
            n = digit - '0';
        } else {
            // Handle case where left-most character is '-' or '+'.
            if (i == 0 && digit == '-' && str.size() > 1) {
                sign = -1;
            } else if (i == 0 && digit == '+' && str.size() > 1) {
                sign = 1;
            } else {
                result = false; // out of range
            }
        }
        if (n >= base) {
            result = false; // out of range for specified base
        }
    }
    return result;
}

// Class: ExpressionCalculator
// ---------------------------
// Defines a class that can be instantiated to evaluate hexadecimal
// arithmetic expressions.

class ExpressionCalculator {
    
public:

    //
    // Constructor: ExpressionCalculator
    // Usage: ExpressionCalculator calc;
    //        calc.eval("2 + 2");
    //
    //        ExpressionCalculator("2 + 2");
    //        calc.eval();
    // -------------------------------------
    // Creates an arithmetic expression calculator.  The expression may be
    // set in a subsequent call to the eval member function or specified
    // within the constructor itself.
    //
    // Evaluation of the expression is made through a call to the eval
    // member function.
    
    ExpressionCalculator() {
        this->expr = "";
        this->base = DFLT_BASE;
        scanner.setInput(this->expr);
        scanner.ignoreWhitespace(); // Do not treat whitespace as a token.
    }
    
    ExpressionCalculator(std::string expr) {
        this->expr = expr;
        this->base = DFLT_BASE;
        scanner.setInput(this->expr);
        scanner.ignoreWhitespace(); // Do not treat whitespace as a token.
    }
    
    // Method: setExpr
    // Usage: setExpr("2+2");
    // ----------------------
    // Sets the instance variable, expr, to the string to evaluate.
    // Readies the input scanner as well.
    
    void setExpr(std::string expr) {
        this->expr = expr;
        scanner.setInput(this->expr);
    }
    
    // Method: setBase
    // Usage: setBase(16);
    // -------------------
    // Sets the base used to evaluate input and express output.
    // This defaults to the constant, DFLT_BASE;
    
    void setBase(int base) {
        this->base = base;
    }
    
    // Method: isOperator
    // Usage: isOperator('+');
    // -----------------------
    // Returns true if the input character corresponds to a valid operator
    // as defined by the string ARITHMETIC_OPERATORS.
    
    bool isOperator(char ch) {
        return ARITHMETIC_OPERATORS.contains(ch);
    }
    
    // Method: eval
    // Usage: std::string result = eval("2 + 2");
    //
    //        setExpr("2+2");
    //        std::string result = eval();
    // ------------------------------------------
    // Returns the results of evaluating the input expression string.
    // Output is expressed as a string.
    //
    // These results are influenced by the base instance variable which
    // defaults to DFLT_BASE unless otherwise specified.
    
    std::string eval(std::string expr) {
        setExpr(expr);
        return eval();
    }
    
    std::string eval() {
        int result;
        
        while (scanner.hasMoreTokens()) {
            std::string token = scanner.nextToken();
            
            if (isNumber(token, base)) {
                operands.push(token);
            } else {

                if (isOperator(token[0])) {
                    operators.push(token[0]);
                } else {
                    error("Invalid operator: " + string(1, token[0]));
                }
            }
            
            if ((operands.size() == 2) && (operators.size() == 1)) {
                
                int i2 = stringToInteger(operands.pop(), base);
                int i1 = stringToInteger(operands.pop(), base);
                
                char op = operators.pop();
                switch (op) {
                    case '+':
                        result = i1 + i2;
                        break;
                        
                    case '-':
                        result = i1 - i2;
                        break;
                        
                    case '*':
                        result = i1 * i2;
                        break;
                        
                    case '/':
                        result = i1 / i2;
                        break;
                        
                    default:
                        error("Invalid operator: " + string(1, op));
                        break;
                }
                operands.push(integerToString(result, base));
            }
        }
        return operands.pop();
    }
    
private:
    std::string expr;
    TokenScanner scanner;
    Stack<std::string> operands;
    Stack<char> operators;
    int base;
};

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    std::cout << "Allows simple left-to-right evaluation of arithmetic\n";
    std::cout << "expressions of operators (+,-,*,/) and hexadecimal numbers.\n";
    
    // Test Data
    //std::string input = "2 + 2";
    //std::string input = "9 + 3";
    //std::string input = "10000 - 1";
    //std::string input = "6 * 7";
    //std::string input = "FEED - CAFE";
    
    ExpressionCalculator calc;
    std::string input;
    
    while ((input = getLine(PROMPT)) != "") {
        std::cout << calc.eval(input) << std::endl;
    }
    return 0;
}
