//
// main.cpp
//
// This program implements the getReal and getLine functions associated
// with simpio.h.
//
// For example:
//
//    CS106B Programming Abstractions in C++: Ex 4.12
//    Implement getReal() and getLine()
//
//    Enter a real number: 3.14159
//    myDouble: 3.14159
//    Enter a line: hello dolly
//    myLine: hello dolly
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 4, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/19/17. (derivative work)
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string>
#include "error.h"

using namespace std;

// Constants and Types

const string HEADER = "CS106B Programming Abstractions in C++: Ex 4.12\n";
const string DETAIL = "Implement getReal() and getLine() from simpio.h";
const string BANNER = HEADER + DETAIL;
const string R_PROMPT = "Enter a real number: ";
const string L_PROMPT = "Enter a line: ";

// Function prototypes

double getReal(string prompt);
string getLine(string prompt);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    
    double myDouble = getReal(R_PROMPT);
    cout << "myDouble: " << myDouble << endl;
    
    string myLine = getLine(L_PROMPT);
    cout << "myLine: " << myLine << endl;
    
    return 0;
}

// Function definitions

double getReal(string prompt) {
    double value;
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);
        istringstream stream(line);
        stream >> value >> ws;
        if (!stream.fail() && stream.eof()) break;
        stream.clear();
        cerr << "Invalid real format. Try again." << endl;
    }
    return value;
}

string getLine(string prompt) {
    string line;
    cout << prompt;
    getline(cin, line);
    return line;
}
