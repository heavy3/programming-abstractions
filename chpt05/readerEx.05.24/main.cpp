//
// main.cpp
//
// This program displays a table of word lengths and frequency.
//
// For example:
//
//     1       3
//     2      94
//     3     962
//     4    3862
//     5    8548
//     6   14383
//     7   21729
//     8   26448
//    ..      ..
//    24       1
//    28       1
//    29       1
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 24
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/19/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "lexicon.h"
using namespace std;

// Types and constants

const string LINE = string(60, '-') + '\n';

// Function prototypes

void banner();
void getWordLengthFreq(Lexicon english, Map<int,int> & wordLengthFreq);
void showWordLengthFreq(Map<int,int> & lengthFreq);

// Main program

int main(int argc, char * argv[]) {

    Map<int,int> map;
    
    Lexicon english("EnglishWords.dat");
    getWordLengthFreq(english, map);
    showWordLengthFreq(map);
    
    return 0;
}

// Function definitions

//
// Function: banner
// Usage: banner()
// -------------------------
// Displays the purpose of the program on the console.
//

void banner(int N) {
    cout << "This program ..." << endl;
    cout << LINE;
}

//
// Function: getWordLengthFreq
// Usage: getWordLengthFreq(lexicon, map);
// ---------------------------------------
// Populates a mapping of word lengths to frequency for a given lexicon.
//

void getWordLengthFreq(Lexicon english, Map<int,int> & map) {
    for (string word : english) {
        if (map.containsKey(word.size())) {
            map[word.size()]++;
        } else {
            map.put(word.size(), 1);
        }
    }
}

//
// Function: showWordLengthFreq
// Usage: showWordLengthFreq(map);
// -------------------------------
// Displays a two-column table of word lengths and their frequency of 
// occurrence in a lexicon.
//

void showWordLengthFreq(Map<int,int> & map) {
    for (int length : map) {
        cout << setw(2) << right << length
             << setw(8) << right << map[length] << endl;
    }
}