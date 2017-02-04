//
// main.cpp
//
// This program displays a histogram for data read from a file and then banded
// into the ranges 0-9, 10-19, 20-29, and so forth, up to the range containing
// only 100.
//
// For example, typical output looks like this:
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
// Chapter 5, Exercise 4
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
string histochars(Vector<int>, int lowerBound, int delta, char symbol = '*');
void histogram(Vector<int>& data, int minVal, int maxVal, int delta,
               char symbol = '*');
double mean(Vector<double>& data);
double stddev(Vector<double>& data);
bool openFile(ifstream& infile, string fileName);
bool pickFile(string prompt, string& fileName);
void printVector(Vector<int> & vec);
void readVector(istream & is, Vector<int> & vec);

// Main program

int main(int argc, const char * argv[]) {
    ifstream infile;
    
    banner();
    if (getInputStream(infile, "Enter filename of ints to historgram: ")) {
        Vector<int> data;
        int delta = 10;
        int minVal = 0;
        int maxVal = 100;
        
        readVector(infile, data);
        //printVector(data);
        histogram(data, minVal, maxVal, delta);
        
    } else {
        cout << "No file specified.";
    }
    infile.close();
    return 0;
}

// Function definitions

//
// Function: histogram
// Usage: histogram(dataVector, minVal, maxVal, delta);
// ----------------------------------------------------
// Displays a simple histogram representation of a integers to the console.
// Data is passed in as a vector along with the minimum, maximum, and
// size of a given range.  Typical chart looks like this:
// 
// 00s:
// 10s:
// 20s:
// 30s:
// 40s: *
// 50s: *
// 60s:
// 70s: **
// 80s: *****
// 90s: **
// 100: *
//

void histogram(Vector<int>& data, int minVal, int maxVal, int delta, char symbol) {
    for (int i = minVal; i < maxVal; i += delta) {
        cout << setfill('0') << setw(2) << i << "s: ";
        cout << histochars(data, i, delta) << endl;
    }
    cout << maxVal << ": " << histochars(data, maxVal, delta) << endl;
}

//
// Function: histochars
// Usage: string stars = histochars(dataVector, lowerbound, delta);
// ----------------------------------------------------------------
// Returns a string of stars (or whatever character symbol is specified) 
// representing the number of integers in the data vector which fall within 
// the half-open interval:
//
//    lowerBound <= val < lowerBound + delta
//
// For example, all values from 0 to 9 found in the data would be represented
// with a star character in the returned string given a lower bound of 0 and
// a range delta of 10 units.
//

string histochars(Vector<int> data, int lowerBound, int delta, char symbol) {
    string stars;
    
    for (int val: data) {
        if ((val >= lowerBound) && (val < (lowerBound + delta))) {
            stars += symbol;
        }
    }
    return stars;
}

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
// Error is thrown if an empty vector is passed in.
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
    cout << "This program displays a histogram of int values "
         << "read from a file." << endl << endl;
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