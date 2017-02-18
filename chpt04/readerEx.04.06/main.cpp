//
// main.cpp
//
// This program implements getRoot and getExtension functions for splitting
// a filename into its component parts (based upon a similar offering
// described in Stanford's filelib.h).
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 4, Exercise 6
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/17/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "error.h"
using namespace std;

// Constants and Types

const string HEADER = "CS106B Programming Abstractions in C++: Ex 4.06\n";
const string DETAIL = "filelib function: defaultExtension()";
const string BANNER = HEADER + DETAIL;
const string PROMPT = "Input file: ";
const char PERIOD = '.';

// Function prototypes

string getExtension(string filename);
string defaultExtension(string filename, string ext);
int getLastDotPos(string filename);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    
    assert(defaultExtension("Shakespeare", ".txt") == "Shakespeare.txt");
    assert(defaultExtension("library.h", ".cpp") == "library.h");
    
    cout << "Unit tests passed." << endl;
    return 0;
}

// Function definitions

// Function: defaultExtension
// Usage: string filename = defaultExtension("Shakespeare", ".txt");
// -----------------------------------------------------------------
// Returns a filename with an extension appended if the first argument
// appears to be missing an extension.

string defaultExtension(string filename, string ext) {
    string result = filename;
    string fileExt = getExtension(filename);
    if (fileExt == "") {
        return filename + ext;
    }
    return filename;
}

// Function: getExtension
// Usage: string ext = getExtension("myfile.txt");
// -----------------------------------------------
// Returns the file extension associated with a filename (if it has one).

string getExtension(string filename) {
    string result;
    int dotPos = getLastDotPos(filename);
    return (dotPos == string::npos) ? result : filename.substr(dotPos);
}

// Function: getLastDotPos
// Usage: int pos = getLasDotPos("somefile.thing.ext");
// ----------------------------------------------------
// Returns the index of the last period in the input string.
// Otherwise it returns the constant string::npos if no
// period is found.

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
