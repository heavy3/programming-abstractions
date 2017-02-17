//
// main.cpp
//
// This program implements getRoot and getExtension functions for splitting
// a filename into its component parts (based upon a similar offering
// described in Stanford's filelib.h).
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 4, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/17/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "error.h"
#include "vector.h"
using namespace std;

// Constants and Types

const string HEADER = "CS106B Programming Abstractions in C++: Ex 4.05\n";
const string DETAIL = "Filename functions: getRoot() and getExtension()";
const string BANNER = HEADER + DETAIL;
const string PROMPT = "Input file: ";
const string E_NO_FILE = "File not found.  Please try again.";
const char PERIOD = '.';

// Function prototypes

string promptForFilename(string prompt);
string getRoot(string filename);
string getExtension(string filename);
int getLastDotPos(string filename);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    
    Vector<string> testStrings;
    testStrings += ".h.ello";
    testStrings += "root.ext";
    testStrings += "noext";

    //string fname = promptForFilename(PROMPT);

    for (string fname : testStrings) {
        cout << "filename: " << fname << endl;
        cout << "root: " << getRoot(fname) << endl;;
        cout << " ext: " << getExtension(fname) << endl << endl;
    }
    return 0;
}

// Function definitions

string promptForFilename(string prompt) {
    string fname;
    cout << prompt;
    getline(cin, fname);
    return fname;
}

string getRoot(string filename) {
    string result = filename;
    int dotPos = getLastDotPos(filename);
    return (dotPos == string::npos) ? result : filename.substr(0, dotPos);
}

string getExtension(string filename) {
    string result;
    int dotPos = getLastDotPos(filename);
    return (dotPos == string::npos) ? result : filename.substr(dotPos);
}

int getLastDotPos(string filename) {
    int dotPos = string::npos;
    
    for (int i = filename.length() - 1; i >= 0; i--) {
        if (filename[i] == PERIOD) {
            dotPos = i;
            break;
        }
    }
    return dotPos;
}
