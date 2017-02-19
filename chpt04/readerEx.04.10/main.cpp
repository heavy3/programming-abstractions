//
// main.cpp
//
// This program copies the contents of an input text file to an output
// text file, but with tab characters expanded to spaces.
//
// For example:
//
//    CS106B Programming Abstractions in C++: Ex 4.10
//    Replace tabs with spaces for an input text stream.
//
//    Input file: input.txt
//    Output file: output.txt
//    Length of tab stop: 8
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 4, Exercise 10
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/19/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include "strlib.h"
#include "error.h"

using namespace std;

// Constants and Types

const string HEADER = "CS106B Programming Abstractions in C++: Ex 4.10\n";
const string DETAIL = "Replace tabs with spaces for an input text stream.";
const string BANNER = HEADER + DETAIL;
const string I_PROMPT = "Input file: ";
const string O_PROMPT = "Output file: ";
const string T_PROMPT = "Length of tab stop: ";
const string E_NO_FILE = "File not found.  Please try again.";
const string E_BAD_OPEN = "Unable to open file.";
const char SPACE = ' ';
const char TAB = '\t';
const char NEWLINE = '\n';

// Function prototypes

string promptForInFile(ifstream & stream, const string prompt);
string promptForOutFile(ofstream & stream, const string prompt);
void expandTabs(istream & is, ostream & os, const int tablen);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    ifstream is;
    ofstream os;
    string fname = promptForInFile(is, I_PROMPT);
    string oname = promptForOutFile(os, O_PROMPT);
    string line;
    cout << T_PROMPT;
    getline(cin, line);
    int tablen = stringToInteger(trim(line));
    expandTabs(is, os, tablen);
    is.close();
    os.close();
    return 0;
}

// Function definitions

// Function: expandTabs
// Usage: expandTabs(inStream, outStream, tablen);
// --------------------------------------------------------
// This function expands tabs encountered on the input stream
// into spaces on the output stream.  All others characters
// pass through from input to output stream unchanged.

void expandTabs(istream & is, ostream & os, const int tablen) {
    int pos = 0;
    int ch;
    while ((ch = is.get()) != EOF) {
        int space = tablen - (pos % tablen);
        switch(ch) {
            case TAB:
                for (int i = 0; i < space; i++) {
                    os << SPACE;
                }
                pos += space;
                break;

            case NEWLINE:
                os << NEWLINE;
                pos = 0;
                break;

            default:
                ++pos;
                os << char(ch);
        }
    }
}

// Function: promptForFile
// Usage: string filename = promptForFile(inputFstream, "Input file: ");
// ---------------------------------------------------------------------
// Prompts the user for a filename to open and associate with a pass-by-
// reference input file stream.  Upon success, returns the name of the
// file.  Otherwise spins in a loop until a valid filename is provided
// and the stream is attached to that file.

string promptForInFile(ifstream & stream, const string prompt) {
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

// TODO: Fix repeated code.

string promptForOutFile(ofstream & stream, const string prompt) {
    while (true) {
        string fname;
        cout << prompt;
        getline(cin, fname);
        stream.open(fname.c_str());
        if (!stream.fail()) return fname;
        stream.clear();
        error(E_BAD_OPEN);
    }
}
