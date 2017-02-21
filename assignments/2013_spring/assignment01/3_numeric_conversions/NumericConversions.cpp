//
// NumericConversions.cpp
//
// This program implements the functions intToString and stringToInt.
//
// --------------------------------------------------------------------------
// Assignment: 1, Part 2 Combinations
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/050%20Assignment%201.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/15/2015.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
#include <cctype>
#include "strlib.h"
#include "error.h"
using namespace std;

// Function prototypes

string intToString(int n);
int stringToInt(string str);
bool isminus(char ch);
bool issign(char ch);

// Main program

int main() {

    cout << intToString(0) << endl;
    cout << intToString(-4234) << endl;
    int val = stringToInt("1.20");          // This should throw an error.
    cout << "val is " << val << endl;

    return 0;
}

//
// Function: intToString
// Usage: string str = intToString(103); // returns "103"
// ----------------------------------------------------------
// Returns the string form of an integer.
//

string intToString(int n) {
    string result;
    string sign = (n < 0) ? "-" : "";
    
    int onesDigit = abs(n) % 10;
    if ((abs(n) < 10)) {
        result = char(onesDigit + '0');
    } else {
        result = intToString(abs(n/10)) + intToString(abs(n)%10);
    }
    return sign + result;
}

//
// Function: stringToInt
// Usage: int num = stringToInt("-1234");   // returns -1234 as an integer.
// ------------------------------------------------------------------------
// Uses recursion to decompose a string into a higher-order string of
// digits plus a one's character.  The ones character is converted directly
// to a numeric value and added to the recursive decomposition of the
// higher-order string scaled by 10 with each recursive call.
//
// Any leading sign character is stripped off up front and used to set
// a static local factor (with value of 1 or -1) which is multiplied
// against the unsigned portion of the converted number prior to returning
// back to the caller.  Use of a static variable allows us to discern
// if the sign character has already been handled no matter where we are
// in the recursion stack.
//

int stringToInt(string strN) {
    static int signFactor = 0;
    static const string E_FORMAT = "stringToInt: Illegal integer format";
    int result = 0;
    
    if (strN.length() == 0) error(E_FORMAT);
    if (!signFactor) {
        if (issign(strN[0])) {
            signFactor = (isminus(strN[0])) ? -1 : 1;
            if (strN.length() < 2) error(E_FORMAT);
            result = stringToInt(strN.substr(1, strN.length()-1));
        } else {
            signFactor = 1;
            result = stringToInt(strN);
        }
    } else {
        if (!isnumber(strN[0])) error(E_FORMAT);
        if (strN.length() == 1) {
            return strN[0] - '0';
        } else {
            string strOnesDigit = string() + strN[strN.length()-1];
            string strTenAndUp = strN.substr(0, strN.length() - 1);
            return 10 * stringToInt(strTenAndUp) + stringToInt(strOnesDigit);
        }
    }
    return signFactor * result;
}

bool issign(char ch) {
    return (ch == '-' || ch == '+');
}

bool isminus(char ch) {
    return (ch == '-');
}

