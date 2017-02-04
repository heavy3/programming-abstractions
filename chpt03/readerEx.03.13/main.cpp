//
// main.cpp
//
// This program implements the function, createOrdinalForm(n), which returns
// the ordinal version of a number according to the following rules:
//
// Numbers ending in   are appended with
// -----------------   -----------------
//    11 - 13                  th
//          1                  st
//          2                  nd
//          3                  rd
//    default                  th
//
// For example:
//
//    createOrdinalForm(103); // returns "103rd"
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 13
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/6/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
using namespace std;

// Function prototypes

bool testCreateOrdinalForm(int n, string expected);
string createOrdinalForm(int n);
string getOrdinalSuffix(int n);
string integerToString(int n);

// Main program

int main(int argc, const char * argv[]) {
    testCreateOrdinalForm(11, "11th");
    testCreateOrdinalForm(1, "1st");
    testCreateOrdinalForm(12, "12th");
    testCreateOrdinalForm(13, "13th");
    testCreateOrdinalForm(99, "99th");
    testCreateOrdinalForm(103, "103rd");
    testCreateOrdinalForm(-1, "-1st");
    return 0;
}

// Function definitions

//
// Function: testCreateOrdinalForm
// Usage: testCreateOrdinalForm(11, "11th");
// Console output: [PASS]   11 -->   11th
// --------------------------------------------------------------------
// Returns true if expected conversion from cardinal to ordinal matches
// actual conversion.
//

bool testCreateOrdinalForm(int n, string expected) {
    string p = "[PASS]";
    string f = "[FAIL]";
    string result = createOrdinalForm(n);
    
    if (result == expected) {
        cout << p << " " << setw(4) << n << " --> " << setw(6) << result << endl;
    } else {
        cout << f << " " << setw(4) << n << " --> " << setw(6) << result
             << " (expected " << expected << ")" << endl;
    }
    
    return (result == expected);
}

//
// Function: createOrdinalForm
// Usage: string ordinal = createOrdinalForm(103); // returns "103rd"
// ------------------------------------------------------------------
// Returns the ordinal form of an integer as a string.
//

string createOrdinalForm(int n) {
    return integerToString(n) + getOrdinalSuffix(n);
}

//
// Function: getOrdinalSuffix
// Usage: string suffix = getOrdinalSuffix(11); // returns "th"
// ------------------------------------------------------------
// Returns the English ordinal suffix for a given cardinal number as a string.
//
// The algorithm examines the two least significant digits of the number
// so exceptions for numbers ending in 11, 12, and 13 are handled appropriately.
//

string getOrdinalSuffix(int n) {
    string suffix;
    
    int lastTwo = abs(n) % 100; // Use absolute value for neg number edge case.
    
    switch (lastTwo) {
        case 11: case 12: case 13:
            suffix = "th";
            break;
            
        case 1:
            suffix = "st";
            break;
            
        case 2:
            suffix = "nd";
            break;
            
        case 3:
            suffix = "rd";
            break;
            
        default:
            suffix = "th";
            break;
    }
    return suffix;
}

//
// Function: integerToString
// Usage: string str = integerToString(103); // returns "103"
// ----------------------------------------------------------
// Returns the string form of an integer.
//

string integerToString(int n) {
    string result;
    string sign = (n < 0) ? "-" : "";
    
    while (true) {
        int onesDigit = abs(n) % 10;
        if ((onesDigit == 0) && (abs(n) < 10)) {
            break;
        }
        result = char(onesDigit + '0') + result;
        n /= 10;
    }
    return sign + result;
}