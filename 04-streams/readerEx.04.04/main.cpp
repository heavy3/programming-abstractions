//
// main.cpp
//
// This program counts the number characters, words, and lines
// in a file.
//
// For example:
//
//    CS106B Programming Abstractions in C++: Ex 4.04
//    Count characters, words, and lines in a file.
//
//    Input file: Lear.txt
//    Chars: 254
//    Words:  43
//    Lines:   6
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 4, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/17/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "error.h"
using namespace std;

// Constants and Types

const string HEADER = "CS106B Programming Abstractions in C++: Ex 4.04\n";
const string DETAIL = "Count characters, words, and lines in a file.";
const string BANNER = HEADER + DETAIL;
const string PROMPT = "Input file: ";
const string E_NO_FILE = "File not found.  Please try again.";

// Function prototypes

string promptForFile(ifstream & stream, string prompt);


// Main program

int main(int argc, char * argv[]) {
	cout << BANNER << endl << endl;
	ifstream istream;

	string fname = promptForFile(istream, PROMPT);

	int lineCount = 0;
	int wordCount = 0;
	int charCount = 0;

	string currentLine;
	while (getline(istream, currentLine)) {
		++lineCount;
        // Add +1 for truncated newline character.
        charCount += currentLine.length() + 1;
		istringstream lineStream(currentLine);
		string word;
		while (lineStream >> skipws >> word) { ++wordCount; }
	}
	istream.close();
    
    // Exercise wants us to float the second column right
    // but minimally distant from the first.
    //
    // Assume max field width is the number of digits in the
    // character count.  Strignify that count to compute the
    // widest display field.
    
    ostringstream ostream;
    ostream << charCount;
    int width = ostream.str().length();
    
    cout << "Chars: " << right << setw(width) << charCount << endl;
    cout << "Words: " << right << setw(width) << wordCount << endl;
    cout << "Lines: " << right << setw(width) << lineCount << endl;
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
