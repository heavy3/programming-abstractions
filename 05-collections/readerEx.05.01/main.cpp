// 
// main.cpp
//
// This program implements the overloaded functions:
//
//    void readVector(istream & is, Vector<int> & vec); 
//    void readVector(istream & is, Vector<double> & vec);
//    void readVector(istream & is, Vector<string> & vec);
//
// each of which read lines from an input stream to populate
// a vector until a blank line or end-of-file is encountered.
//
// For example:
//
// If readVector is presented with the following input data,
// a vector is populated with the first 4 values on the initial
// call.
//
//    SquareAndCubeRoots.txt
//    ----------------------
//    1.0000
//    1.4142
//    1.7321
//    2.0000
//
//    1.0000
//    1.2599
//    1.4422
//    1.5874
//    1.7100
//    1.8171
//    1.9129
//    2.0000
//
// On the second call, the vector is initialized with the
// next 8 entries.  On the third call, an empty vector is returned.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 1
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
#include "vector.h"
#include "strlib.h"
using namespace std;

// Function prototypes

void banner();
bool getInputStream(ifstream& inf, string prompt);
bool openFile(ifstream& infile, string fileName);
bool pickFile(string prompt, string& fileName);
void printVector(Vector<double> & vec);
void printVector(Vector<int> & vec);
void printVector(Vector<string> & vec);
void readVector(istream & is, Vector<double> & vec);
void readVector(istream & is, Vector<int> & vec);
void readVector(istream & is, Vector<string> & vec);

// Main program

int main(int argc, const char * argv[]) {
    ifstream infile;
    
    banner();
    if (getInputStream(infile, "Enter filename to initialize a vector of doubles: ")) {
        Vector<double> dblVec;
        readVector(infile, dblVec);
        printVector(dblVec);
        readVector(infile, dblVec);
        printVector(dblVec);
        readVector(infile, dblVec);
        printVector(dblVec);
    } else {
        cout << "No file specified.";
    }
    infile.close();
    
    if (getInputStream(infile, "Enter filename to initialize a vector of ints: ")) {
        Vector<int> intVec;
        readVector(infile, intVec);
        printVector(intVec);
        readVector(infile, intVec);
        printVector(intVec);
        readVector(infile, intVec);
        printVector(intVec);
    } else {
        cout << "No file specified.";
    }
    infile.close();
    
    if (getInputStream(infile, "Enter filename to initialize a vector of strings: ")) {
        Vector<string> strVec;
        readVector(infile, strVec);
        printVector(strVec);
        readVector(infile, strVec);
        printVector(strVec);
        readVector(infile, strVec);
        printVector(strVec);
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
    cout << "This program initializes 3 different types of Vectors:\n\n";
    cout << "\t" << "Vector<double>" << endl;
    cout << "\t" << "Vector<int>" << endl;
    cout << "\t" << "Vector<string>" << endl;
    cout << "by prompting for files from which to initialize the vectors.";
    cout << endl << endl;
}

//
// Function: readVector
// Usage: readVector(inputstream, doubleVector);
//        readVector(inputstream, intVector);
//        readVector(inputstream, stringVector);
// ---------------------------------------------
// Populates a pass-by-reference vector with input from a data file.
// Input records are structured one vector element per line.
// Entries are read until a blank line or end-of-file condition is
// reached.
//

void readVector(istream & is, Vector<double> & vec) {
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

void readVector(istream & is, Vector<int> & vec) {
    string line;
    
    vec.clear();
    while (true) {
        if (getline(is, line)) {
            if (line == "") {
                break;
            }
            vec.add(stringToInteger(line));
        } else {
            break;
        }
    }
}

void readVector(istream & is, Vector<string> & vec) {
    string line;
    
    vec.clear();
    while (true) {
        if (getline(is, line)) {
            if (line == "") {
                break;
            }
            vec.add(line);
        } else {
            break;
        }
    }
}

//
// Function: printVector
// Usage: printVector(vec);
// ------------------------
// Overloaded functions print a vector of doubles, or ints, or strings to the
// console.
//

void printVector(Vector<double> & vec) {
    cout << vec.toString() << endl << endl;
}

void printVector(Vector<int> & vec) {
    cout << vec.toString() << endl << endl;
}

void printVector(Vector<string> & vec) {
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