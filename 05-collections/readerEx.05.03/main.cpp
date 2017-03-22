//
// main.cpp
//
// This program implements a standard deviation function
//
//     double stddev(Vector<double> & data);
//
// which computes the standard deviation of a complete distribution.
//  
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 3
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
#include <iomanip>
#include <cmath>
#include "vector.h"
#include "strlib.h"

using namespace std;

// Function prototypes

void banner();
bool getInputStream(ifstream& inf, string prompt);
double mean(Vector<double>& data);
double stddev(Vector<double>& data);
bool openFile(ifstream& infile, string fileName);
bool pickFile(string prompt, string& fileName);
void printVector(Vector<double> & vec);
void readVector(istream & is, Vector<double> & vec);

// Main program

int main(int argc, const char * argv[]) {
    ifstream infile;
    
    banner();
    if (getInputStream(infile, "Enter filename of doubles for stddev calc: ")) {
        Vector<double> dblVec;
        readVector(infile, dblVec);
        printVector(dblVec);
        cout << setw(10) << "Mean: "   << setprecision(3) << setw(5)
             << mean(dblVec)   << endl;
        cout << setw(10) << "Stddev: " << setprecision(3) << setw(5)
             << stddev(dblVec) << endl;
    } else {
        cout << "No file specified.";
    }
    infile.close();
    return 0;
}

// Function definitions

//
// Function: stddev
// Usage: double sd = stddev(data);
// --------------------------------
// Returns the standard deviation of a complete distribution of doubles 
// passed by reference in a vector.
//

double stddev(Vector<double>& data) {
    double result = 0;
    
    if (!data.size()) {
        error("stddev: Divide by 0.  Empty data vector passed in.");
    }
    
    double xavg = mean(data);
    double summation = 0;
    
    for (double xi: data) {
        summation += pow((xavg - xi),2);
    }
    result = sqrt(summation/data.size());
    return result;
}

//
// Function: mean
// Usage: double avg = mean(vectorOfDoubles);
// ------------------------------------------
// Returns the mean value of doubles stored in a vector.
// Error is thrown and processing halts if an empty vector is passed in.
//

double mean(Vector<double>& data) {
    double sum = 0;
    
    if (!data.size()) {
        error("mean(): Divide by 0.  Empty data vector passed in.");
    }
    
    for (double datum: data ) {
        sum += datum;
    }
    return sum / data.size();
}

//
// Function: banner
// Usage: banner()
// ---------------
// Function displays the purpose of the program on the console.
//

void banner() {
    cout << "This program computes the standard deviation of a distribution " ;
    cout << "of values stored in a file and exercises some vector idioms in ";
    cout << "in the process.";
    cout << endl << endl;
}

//
// Function: readVector
// Usage: readVector(inputstream, doubleVector);
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
