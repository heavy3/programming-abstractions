//
// main.cpp
//
// This program performs a random character substitution on an input file
// of text.  Punctuation and whitespace are unaffected.
//
// For example:
//
//    CS106B Programming Abstractions in C++: Ex 4.07
//    Randomize text from a file.
//
//    Input file: Troilus.txt
//    Fg, Ydwaq; rli pidy tocxf db ljnsynyt lwje
//    Sx ixgvstbhlg fy oyr yd Keff owg muyhk
//    Gjhkxzwj beee Naumz:
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 4, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/18/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include <fstream>
#include <cctype>
#include "error.h"
#include "random.h"

using namespace std;

// Constants and Types

const string HEADER = "CS106B Programming Abstractions in C++: Ex 4.07\n";
const string DETAIL = "Randomize text from a file.";
const string BANNER = HEADER + DETAIL;
const string PROMPT = "Input file: ";
const string E_NO_FILE = "File not found.  Please try again.";

// Function prototypes

string promptForFile(ifstream & stream, const string prompt);
int randomAlphaOffset(const int refIndex);
char randomChar(const char ch);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    ifstream inStream;
    string fname = promptForFile(inStream, PROMPT);
    int ch;
    while ((ch = inStream.get()) != EOF) {
        char rch = randomChar(ch);
        cout << rch;
    }
    return 0;
}

// Function definitions

// Function: promptForFile
// Usage: string filename = promptForFile(inputFstream, "Input file: ");
// ---------------------------------------------------------------------
// Prompts the user for a filename to open and associate with a pass-by-
// reference input file stream.  Upon success, returns the name of the
// file.  Otherwise spins in a loop until a valid filename is provided
// and the stream is attached to that file.

string promptForFile(ifstream & stream, const string prompt) {
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

// Function: randomChar
// Usage: char rch = randomChar(ch);
// ---------------------------------
// Randomizes an alpha character into another alpha character.
// Case is preserved.
//
// All other characters are passed through without modification.

char randomChar(const char ch) {
    int chOffset;
    char result;
    
    if (isalpha(ch)) {
        if (isupper(ch)) {
            chOffset = ch - 'A';
            int rOffset = randomAlphaOffset(chOffset);
            result = 'A' + rOffset;
        } else {
            chOffset = ch - 'a';
            int rOffset = randomAlphaOffset(chOffset);
            result = 'a' + rOffset;
        }
    } else {
        result = ch;
    }
    return result;
}

// Function: randomAlphaOffset
// Usage: int rOffset = randomAlphaOffset(17);
// --------------------------------------------
// Takes a value between 0 and 25 and returns a different
// number between 0 and 25 based upon a random number
// generator.

int randomAlphaOffset(const int chOffset) {
    int result;
    const int ALPHABET_LENGTH = 26;
    
    // Take care to avoid the chance of returning the
    // exact same index as was passed in (which might
    // happen if we made the range 0 - 26 and
    // we happened to hit either extreme).
    
    int randomOffset = randomInteger(1, ALPHABET_LENGTH - 1);
    result = (randomOffset + chOffset) % ALPHABET_LENGTH;
    return result;
}
