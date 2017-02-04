//
// main.cpp
//
// This program illustrates the use of the histo.h interface
// which exports a character-based histogram rendering function
// with several options.
//
// Data are read from a file and then banded for display.
//
// For example, typical output looks like this
//
//      00s:
//      10s:
//      20s:
//      30s:
//      40s: *
//      50s: *
//      60s:
//      70s: **
//      80s: *****
//      90s: **
//      100: *
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/27/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <string>
#include "histo.h"

// Function prototypes

void banner();
bool getInputStream(ifstream& inf, string prompt);
bool openFile(ifstream& infile, string fileName);
bool pickFile(string prompt, string& fileName);
void printVector(Vector<int> & vec);
void readVector(istream & is, Vector<int> & vec);

// Main program

int main(int argc, const char * argv[]) {
    ifstream infile;
    
    banner();
    if (getInputStream(infile, "Enter filename of ints to histogram: ")) {
        Vector<int> data;

        readVector(infile, data);
        #ifdef DEBUGGING
        printVector(data);
        #endif
        int delta = 10;
        int minVal = 0;
        int maxVal = 100;
        string header = "Histogram 1";
        string footer = "Footer";
        histogram(data, minVal, maxVal, delta, HISTO_SYMBOL, header, footer);
        
        delta = 20;
        minVal = 40;
        maxVal = 100;
        ostringstream oss;
        oss << "Range: " << minVal << " - " << maxVal;
        header = "Histogram 2";
        footer = oss.str();
        histogram(data, minVal, maxVal, delta, '*', header, footer);
        
    } else {
        cout << "No file specified.";
    }
    infile.close();
    return 0;
}

// Function definitions

//
// Function: banner
// Usage: banner()
// ---------------
// Function displays the purpose of the program on the console.
//

void banner() {
    cout << "This program displays a histogram of int values read from a file."
         << endl << endl;
}

//
// Function: readVector
// Usage: readVector(inputstream, intVector);
// ---------------------------------------------
// Populates a pass-by-reference vector with input from a data file.
// Input records are structured one vector element per line.
// Entries are read until a blank line or end-of-file condition is
// reached.
//

void readVector(istream & is, Vector<int> & vec) {
    string line;
    
    vec.clear();
    while (true) {
        if (getline(is, line)) {
            if (line == "") {
                break;
            }
            vec.add(stringToReal(line));
        } else {
            break;
        }
    }
}

//
// Function: printVector
// Usage: printVector(vec);
// ------------------------
// Print a vector of ints to the console.
//

void printVector(Vector<int> & vec) {
    cout << vec.toString() << endl << endl;
}

//
// Function: getInputStream
// Usage: getInputStream(infile, "Enter a file name: ");
// -----------------------------------------------------
// Returns true if user enters a filename which can be opened.  Otherwise
// it continues to prompt the user until they enter a valid filename or an
// empty filename (by hitting 'Enter').  In the case of the later, false is
// returned.
//

bool getInputStream(ifstream& infile, string prompt) {
    string fileName;
    
    while (true) {
        if (pickFile(prompt, fileName)) {
            if (openFile(infile, fileName)) break;
        } else {
            cout << "No file selected.  Goodbye.";
            return false;
        }
    }
    return true;
}

//
// Function: openFile
// Usage: bool opened = openFile(in, fileName);
// --------------------------------------------
// Returns true and readies the input filestream for reading if the named file
// is successfully opened.  Otherwise false is return and related message is
// written to the error console.
//

bool openFile(ifstream& infile, string fileName) {
    bool result;
    
    infile.open(fileName.c_str());
    if (infile.fail()) {
        cerr << "Unable to open " << fileName << endl;
        infile.clear();
        result = false;
    } else {
        result = true;
    }
    return result;
}

//
// Function: pickFile
// Usage: if (pickFile("Enter a file to process: ", fileName)) {...}
// -----------------------------------------------------------------
// Prompts user for the name of a file to process.  Returns true if a
// non-empty filename is entered, otherwise returns false, signaling
// the user's desire to not pick a file.
//

bool pickFile(string prompt, string& fileName) {
    
    cout << prompt;
    getline(cin, fileName);
    return (fileName != "");
}