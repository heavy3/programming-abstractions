//
// main.cpp
//
// This program implements versions of the functions:
// 
//    integerToString 
//    stringToInteger
//
// that support a second argument for specifying the numeric base
// which can be any integer from 2 through 36 (the 10 digits plus
// 26 letters).
//
// For example:
//
//    integerToString(42, 16)      returns "2A"
//    stringToInteger("111111", 2) returns  63
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 11, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 6/16/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cctype>
#include <ostream>
#include "vector.h"
#include "error.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 11.03\n";
const std::string DETAIL = "integerToString & stringToInteger with numeric base";
const std::string BANNER = HEADER + DETAIL;
const std::string DIGITS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const int MAX_BASE = DIGITS.size();
const int MIN_BASE = 2;

// Prototypes

std::string integerToString(int n, const int base);
std::string integerToStringR(const int n, const int base);
std::string integerToStringRec(const int n, const int base);
int stringToInteger(const std::string str, const int base);
void exitOnBadBase(const int base);

// Main program

int main() {
    std::cout << BANNER << endl << endl;
    
    int num = 42;
    int base = 16;
    
    std::cout << num << " base " << base << " = ";
    std::cout << integerToStringR(num, base);
    std::cout << std::endl;
    
    std::string str = "111111";
    base = 2;
    
    std::cout << str << " base " << base << " = ";
    std::cout << stringToInteger(str, base);
    std::cout << std::endl;
    return 0;
}

//
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
//

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

//
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
//

std::string integerToStringR(const int n, const int base) {
    std::string result;
    std::string sign = (n < 0) ? "-" : "";
    
    result = integerToStringRec(abs(n), base);
    return sign + result;
}

//
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
//

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

//
// Function: exitOnBadBase
// Usage: exitOnBadBase(base);
// ---------------------------
// If the input base is outside the supported range specified by the constants
// MIN_BASE and MAX_BASE, an error message is displayed and the program exits.
//

void exitOnBadBase(const int base) {
    ostringstream oss;
    
    if (base < MIN_BASE || base > MAX_BASE) {
        oss << "Input base of " << base << " is out of range. ";
        oss << "Must be (" << MIN_BASE << " <= base <= " << MAX_BASE << ").";
        error(oss.str());
    }
}

//
// Function: stringToInteger
// Usage: int myInt = stringToInteger("111111", 2); // myInt = 63
// --------------------------------------------------------------
// Converts a string representation of a number under a specified base
// to an integer.
//
// Exits if input cannot be represented under the given base or the
// specified base is less than the MIN_BASE minimum.
//

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
            if (i == 0 && digit == '-') {
                sign = -1;
            } else if (i == 0 && digit == '+') {
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
