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
const char PERIOD = '.';

// Function prototypes

string getExtension(string filename);
string getRoot(string filename);
string defaultExtension(string filename, string ext);
int getLastDotPos(string filename);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    
    assert(defaultExtension("Shakespeare", ".txt") == "Shakespeare.txt");
    assert(defaultExtension("library.h", ".cpp") == "library.h");
    assert(defaultExtension("library.h", "*.cpp") == "library.cpp");
    
    cout << "Unit tests passed." << endl;
    return 0;
}

// Function definitions

// Function: defaultExtension
// Usage: string filename = defaultExtension("Shakespeare", ".txt");
//        string filename = defaultExtension("Shakespeare.exe", "*.txt");
// ----------------------------------------------------------------------
// Returns a filename with the second string appended as an extension if
// either of these conditions hold:
//
//    * the first string is missing an extension
//    * the first character of the extension is a wildcard '*'
//

string defaultExtension(string filename, string ext) {
    string result = filename;
    
    if (ext[0] == '*') {
        string root = getRoot(filename);
        result = root + ext.substr(1);
    } else {
        string fileExt = getExtension(filename);
        if (fileExt == "") {
            result = filename + ext;
        }
    }
    return result;
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

// Function: getRoot
// Usage: string base = getRoot("myfile.txt");
// -------------------------------------------
// Returns the basename of the file (with any extension truncated).

string getRoot(string filename) {
    string result = filename;
    int dotPos = getLastDotPos(filename);
    return (dotPos == string::npos) ? result : filename.substr(0, dotPos);
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
