//
// main.cpp
//
// Augment the WordFrequency program from Chapter 15 (Figure 5-11) to use
// iterators instead of for-range loops.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 01
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// Most of this code comes from Figure 5-11.
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/30/17
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cctype>
#include "filelib.h"
#include "map.h"
#include "strlib.h"
#include "vector.h"
#include "error.h"
using namespace std;

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 20.01\n";
const std::string DETAIL = "Iterators: WordFrequency.cpp Fig 5-11 Refactor";
const std::string BANNER = HEADER + DETAIL;

// Function prototypes

void countWords(istream& stream, Map<string, int>& wordCounts);
void displayWordCounts(Map<string, int>& wordCounts);
void extractWords(string line, Vector<string>& words);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    
    ifstream infile;
    Map<string, int> wordCounts;
    promptUserForFile(infile, "Input file: ");
    countWords(infile, wordCounts);
    infile.close();
    displayWordCounts(wordCounts);
    return 0;
}

//
// Function: countWords
// Usage: countWords(stream wordCounts);
// -------------------------------------
// Count words in the input stream, storing the results in wordCounts.
//

void countWords(istream& stream, Map<string, int>& wordCounts) {
    Vector<string> lines, words;
    readEntireFile(stream, lines);
    for (Vector<string>::iterator it = lines.begin(); it != lines.end(); it++) {
        string line = *it;
        extractWords(line, words);
        for (Vector<string>::iterator it = words.begin();
             it != words.end();
             it++) {
            wordCounts[toLowerCase(*it)]++;
        }
    }
}

//
// Function: displayWordCounts
// Usage: displayWordCounts(wordCount);
// ------------------------------------
// Displays the count associated with each word in the wordCount map.
//

void displayWordCounts(Map<string, int>& wordCounts) {
    for (Map<string, int>::iterator it = wordCounts.begin();
         it != wordCounts.end();
         it++) {
        string word = *it;
        cout << left  << setw(20) << word
             << right << setw(20) << wordCounts[word] << endl;
    }
}

//
// Function: extractWords
// Usage: extractWords(line, words);
// ---------------------------------
// Extracts words from the line into the string vector words.
//

void extractWords(string line, Vector<string>& words) {
    words.clear();
    int start = -1;
    for (int i = 0; i < line.length(); i++) {
        if (isalpha(line[i])) {
            if (start == -1) start = i;
        } else {
            if (start >= 0) {
                words.add(line.substr(start, i - start));
                start = -1;
            }
        }
    }
    if (start >= 0) words.add(line.substr(start));
}
