//
// main.cpp
//
// This program finds the longest line in a text file.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 4, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/16/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "error.h"
using namespace std;

// Constants and Types

const string HEADER = "CS106B Programming Abstractions in C++: Ex 4.03\n";
const string DETAIL = "Find longest line in a file.";
const string BANNER = HEADER + DETAIL;
const string PROMPT = "Enter a filename to analize: ";
const string E_NO_FILE = "File not found.  Please try again.";

// Function prototypes

string promptForFile(ifstream & stream, string prompt);


// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    ifstream istream;

    string fname = promptForFile(istream, PROMPT);

    string currentLine;
    string longestLine;
    while (getline(istream, currentLine)) {
        if (currentLine.length() > longestLine.length()) {
            longestLine = currentLine;
        }
    }
    istream.close();
    if (longestLine.length() > 0) {
        cout << "Longest line: " << longestLine << endl;
    }
    return 0;
}

// Function definitions

string promptForFile(ifstream & stream, string prompt) {
    while (true) {
        string fname;
        cout << prompt;
        getline(cin, fname);
        stream.open(fname.c_str());
        if (!stream.fail()) return fname;
        stream.clear();
        cerr << E_NO_FILE << endl;
    }
}
