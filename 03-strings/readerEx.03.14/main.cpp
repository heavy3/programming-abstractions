//
// main.cpp
//
// The program implements the function
// 
//    string addCommas(string digits);
//
// which injects commas after every 3rd digit reading right to left.
//
// For example:
//
//    addCommas(1000000) returns "1,000,000"
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 14
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// The main driver comes from Chapter 3, exercise #14.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/6/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
using namespace std;

// Function prototypes

string addCommas(string digits);

int main() {
    while (true) {
        string digits;
        cout << "Enter a number: ";
        getline(cin, digits);
        if (digits == "") break;
        cout << addCommas(digits) << endl;
    }
    return 0;
}

//
// Function: addCommas
// Usage: string pretty = addCommas("1000"); // returns "1,000"
// ------------------------------------------------------------
// Returns a number with comma's added to separate groups of digits into 
// three. No provision is made for number strings with leading 0's.
// Implementation is a simple reverse iteration from right to left, adding
// a comma into the output string after every group of three.
//

string addCommas(string digits) {
    string result;
    string working;
    char signCh = '\0';
    int count = 0;
    
    if (digits.length() == 0) return result;
    
    // 
    // Strip off possible leading sign character which would otherwise
    // confuse the grouping algorithm below.  We'll add it back in the return.
    //
    
    switch (digits[0]) {
        case '+':
        case '-':
            signCh = digits[0];
            working = digits.substr(1, digits.length() - 1);  // Strip sign.
            break;
        default:
            working = digits;
    }
    
    for (int i = working.length() - 1; i >= 0 ; i--) {
        if (count == 3) {
            result = ',' + result;
            count = 0;
        }
        result = working[i] + result;
        count++;
    }
    return signCh + result;
}