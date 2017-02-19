//
// main.cpp
//
// This program implements stringToReal and realToString by leveraging
// the input and output string stream to perform the underlying conversions
// from one format to another.
//
// For example:
//
//    CS106B Programming Abstractions in C++: Ex 4.11
//    Implement stringToReal & realToString.
//
//    cmath pi: 3.1415926535897931
//       strPi: 3.1415926535897931
//        myPi: 3.1415926535897931
//
//    Unit tests passed.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 4, Exercise 11
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program uses stringToInteger and integerToString in the reader
// as a model.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/19/17. (derivative work)
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cmath>
#include <cassert>
#include "error.h"

using namespace std;

// Constants and Types

const string HEADER = "CS106B Programming Abstractions in C++: Ex 4.11\n";
const string DETAIL = "Implement stringToReal & realToString.";
const string BANNER = HEADER + DETAIL;
const int PRECISION = 16;  // Max double precision.  Portable?

// Function prototypes

double stringToReal(const string & str);
string realToString(const double value);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    
    double pi = M_PI;
    cout << setw(10) << right
         << fixed
         << setprecision(PRECISION)
         << "cmath pi: " << pi << endl;
    
    string strPi = realToString(pi);
    cout << setw(10) << right
         << "strPi: " << strPi << endl;

    double myPi = stringToReal(strPi);
    cout << setw(10) << right
         << fixed
         << setprecision(PRECISION)
         << "myPi: " << myPi << endl;
    
    assert(myPi == pi);
    cout << endl << "Unit tests passed." << endl;

    return 0;
}

// Function definitions

double stringToReal(const string & str) {        
    istringstream istream(str);
    double value;
    istream >> value >> ws;
    if (istream.fail() || !istream.eof()) {
        error("stringToReal: Illegal real format");
    }
    return value;
}

string realToString(const double value) {
    ostringstream ostream;
    ostream << fixed << setprecision(PRECISION) << value;
    if (ostream.fail()) {
        error ("realToString: Unable to convert real to string");
    }
    return ostream.str();
}
