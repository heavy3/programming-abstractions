//
// main.cpp
//
// This program strips a set of characters out of an input text file
// and writes the result to an output file.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 4, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/18/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include "strlib.h"
#include "error.h"

using namespace std;

// Constants and Types

const string HEADER = "CS106B Programming Abstractions in C++: Ex 4.09\n";
const string DETAIL = "Banish letters from an input text stream.";
const string BANNER = HEADER + DETAIL;
const string I_PROMPT = "Input file: ";
const string O_PROMPT = "Output file: ";
const string C_PROMPT = "Letters to banish: ";
const string E_NO_FILE = "File not found.  Please try again.";
const string E_BAD_OPEN = "Unable to open file.";

// Function prototypes

string promptForInFile(ifstream & stream, const string prompt);
string promptForOutFile(ofstream & stream, const string prompt);
void removeChars(istream & is, ostream & os, const string & rmChars);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    ifstream is;
    ofstream os;
    string fname = promptForInFile(is, I_PROMPT);
    string oname = promptForOutFile(os, O_PROMPT);
    string chars;
    cout << C_PROMPT;
    getline(cin, chars);
    removeChars(is, os, chars);
    is.close();
    os.close();
    return 0;
}

// Function definitions

// Function: removeComments
// Usage: removeComments(inStream, outStream, rmChars);
// --------------------------------------------------------
// This function strips out the characters in the rmChars
// string from the input stream.  All other characters pass
// through to the output stream.

void removeChars(istream & is, ostream & os, const string & chars) {
    string normalizedChars = toLowerCase(chars);
    int ch;
    while ((ch = is.get()) != EOF) {
        int index = normalizedChars.find(tolower(ch));
        if (index != string::npos) continue;
        os.put(char(ch));
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
