//
// main.cpp
//
// This program augments the "Beacons of Gondor" program from the text
// to read in the list of towers from a file with a call to:
//
//    Tower *readBeaconsFromFile(string filename);
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 12, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program extends the code in Figure 12-3.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/11/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <istream>
#include "simpio.h"
#include "filelib.h"
#include "error.h"

using namespace std;

// Types

struct Tower {
    string name;
    Tower * link;
};

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 12.03\n";
const std::string DETAIL = "Beacons of Gondor Linked Lists + File I/O";
const std::string BANNER = HEADER + DETAIL;
const std::string INPUTFILE = "BeaconsOfGondor.txt";

// Prototypes

ifstream & getInputStream(string fname);
Tower * readBeaconsFromFile(string fname);
Tower * createTower(string name, Tower * link);
void signal(Tower * start);

// Main program

int main() {
    cout << BANNER << endl << endl;;
    
    Tower * list = readBeaconsFromFile(INPUTFILE);
    signal(list);
    return 0;
}

// Function: readBeaconsFromFile
// Usage: Tower * list = readBeaconsFromFile("BeaconsOfGondor.txt");
// -----------------------------------------------------------------
// Returns a linked list of towers as read from an input file.
// The list is built up in reverse order, though, for ease of implementation.

Tower * readBeaconsFromFile(string fname) {
    Tower * tp = NULL;
    
    ifstream & input = getInputStream(INPUTFILE);
    string towerName;
    
    while (getline(input, towerName)) {
        tp = createTower(towerName, tp);
    }
    return tp;
}

// Function: createTower
// Usage: Tower * pTower = createTower(towerName, pPrevTower);
// Attribution: Eric Roberts
// -----------------------------------------------------------
// Returns a pointer to a heap-resident Tower structure, populated with
// data from the input parameters.

Tower * createTower(string name, Tower * link) {
    Tower * tp = new Tower;
    tp->name = name;
    tp->link = link;
    return tp;
}

// Function: signal
// Usage: signal(towerList);
// -------------------------
// Walks a linked list of towers, displaying the tower names to the console
// in reverse list order.
//
// Uses recursive formulation to walk the list in reverse order,
// recapturing the original file ordering for presentation.

void signal(Tower * start) {
    if (start == NULL) {
        return;
    }
    signal(start->link);
    cout << "Lighting " << start->name << endl;
}

// Function: getInputStream
// Usage: ifstream & input = getInputStream(fnameStr);
// ---------------------------------------------------
// Returns a reference to an input stream associated with the filename
// passed in.  If the file is not found and cannot be opened, an error
// is displayed and execution halts.

ifstream & getInputStream(string fname) {
    static ifstream input;
    
    if (!openFile(input, INPUTFILE)) {
        error("Unable to open input file: " + INPUTFILE);
    }
    return input;
}