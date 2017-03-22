//
// File: main.cpp
//
// This program displays a list of English words that can 
// form other words by prepending or appending another 
// string to a base word.
//
// Typical output:
//
//   This program displays all base words that can be prepended
//   or appended with 's' to form another word in the lexicon.
//   ------------------------------------------------------------
//   able
//   ad
//   adder
//   ..
//   wot
//   wound
//   680 s-words
//
//   This program displays all base words that can be prepended
//   or appended with 'se' to form another word in the lexicon.
//   ------------------------------------------------------------
//   ar
//   el
//   men
//   mi
//   rin
//   5 se-words
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 23
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

void banner(string chopStr);
void getBaseWords(Lexicon lex, string chopStr, Set<string> & baseWords);
void showBaseWords(Set<string> & words, string chopStr);

// Main program

int main(int argc, char * argv[]) {

    Set<string> baseWords;
    Lexicon english("EnglishWords.dat");
    
    string chopStr = "s";
    banner(chopStr);
    getBaseWords(english, chopStr, baseWords);
    showBaseWords(baseWords, chopStr);
    
    chopStr = "se";
    banner(chopStr);
    getBaseWords(english, chopStr, baseWords);
    showBaseWords(baseWords, chopStr);
    
    return 0;
}

// Function definitions

//
// Function: banner
// Usage: banner(chopStr)
// -------------------------
// Displays the purpose of the program on the console.
//

void banner(string chopStr) {
    cout << "This program displays all base words that can be prepended"
         << endl
         << "or appended with '" << chopStr << "' to form another word in "
         << "the lexicon." << endl;
    cout << LINE;
}

//
// Function: getBaseWords
// Usage: getBaseWords(lexicon, chopStr, baseWords);
// -------------------------------------------------
// Returns a set of words that can be prepended or appended with a chop string
// to form another lexicon word.
//
// For example:
//
// base word == 'cold' since s + cold = scold
//                       and cold + s = colds
//

void getBaseWords(Lexicon lex, string chopStr, Set<string> & baseWords) {
    Set<string> strChopped;
    Set<string> choppedStr;
    
    for (string word: lex) {
        string wordSansStr;
        int chopLength = chopStr.length();
        if (chopLength < word.size()) {
            if (startsWith(word, chopStr)) {
                wordSansStr = word.substr(chopLength, word.length()-chopLength);
                if (lex.contains(wordSansStr)) {
                    strChopped.add(wordSansStr);
                }
            }
            if (endsWith(word, chopStr)) {
                wordSansStr = word.substr(0, word.length()-chopLength);
                if (lex.contains(wordSansStr)) {
                    choppedStr.add(wordSansStr);
                }
            }
        }
    }
    baseWords = strChopped * choppedStr;
}

//
// Function: showBaseWords
// Usage: showBaseWords(baseWords, chopStr);
// -----------------------------------------
// Displays a set of words to the console, followed by a count
// of the words in the set.
//

void showBaseWords(Set<string> & baseWords, string chopStr) {
    for (string word : baseWords) {
        cout << word << endl;
    }
    cout << baseWords.size() << " " << chopStr << "-words" << endl << endl;
}