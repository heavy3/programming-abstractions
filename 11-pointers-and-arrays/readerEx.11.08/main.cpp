//
// main.cpp
//
// This program exercises an enhanced token scanner from the text 
// to read tokens from input streams as well as strings.
// 
// TODO: Test driver could use some decomposition to avoid duplicate code.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 11, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 6/21/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "eztokenscanner.h"

using std::cout;
using std::endl;
using std::string;
using std::ifstream;

// Constants

const string HEADER = "CS106B Programming Abstractions in C++: Ex 11.08\n";
const string DETAIL = "Extending TokenScanner to take input from streams.";
const string BANNER = HEADER + DETAIL;
const string INPUT_FILE = "input.txt";

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    
    // Test token scanner with file stream input; ignore whitespace.
    
    string inputFile1 = INPUT_FILE;
    cout << "Scan from a file: " << inputFile1 << endl;
    cout << "Ignore whitespace." << endl;
    cout << "-------------------------------" << endl;
    ifstream infile1;
    infile1.open(inputFile1.c_str());
    
    EzTokenScanner scanner1(infile1);
    scanner1.ignoreWhitespace();
    while (scanner1.hasMoreTokens()) {
        string token = scanner1.nextToken();
        cout << "[" + token + "]" ;
    }
    cout << endl << endl;
    
    // Test token scanner with file stream input; token whitespace.
    
    string inputFile2 = INPUT_FILE;
    cout << "Scan from a file: " << inputFile2 << endl;
    cout << "Tokenize whitespace." << endl;
    cout << "-------------------------------" << endl;
    ifstream infile2;
    infile2.open(inputFile2.c_str());
    
    EzTokenScanner scanner2(infile2);
    while (scanner2.hasMoreTokens()) {
        string token = scanner2.nextToken();
        cout << "[" + token + "]" ;
    }
    cout << endl << endl;

    // Regression test with string buffer input; ignore whitespace.
    
    cout << "Scan from a string buffer: " << endl;
    cout << "Ignore whitespace." << endl;
    cout << "-------------------------------" << endl;
    string str1 = "This is an input buffer of tokens\nand     blank spaces.";
    
    EzTokenScanner scanner3(str1);
    scanner3.ignoreWhitespace();
    while (scanner3.hasMoreTokens()) {
        string token = scanner3.nextToken();
        cout << "[" + token + "]" ;
    }
    cout << endl << endl;
    
    // Regression test with string buffer input; tokenize whitespace.
    
    cout << "Scan from a string buffer: " << endl;
    cout << "Tokenize whitespace." << endl;
    cout << "-------------------------------" << endl;
    string str2 = "This is an input buffer of tokens\nand     blank spaces.";
    
    EzTokenScanner scanner4(str2);
    while (scanner4.hasMoreTokens()) {
        string token = scanner4.nextToken();
        cout << "[" + token + "]" ;
    }

    return 0;
}