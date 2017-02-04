//
// main.cpp
//
// This program displays a list of words of length N that can 
// be formed by prepending or appending a letter to an 
// (N-1)-letter word.
//
// Typical output:
//
//   This program displays all 3-letter, 'Scrabble-friendly',
//   words in the Stanford English lexicon formed by prepending
//   or appending a letter to all 2-letter words.
//   ------------------------------------------------------------
//   aah
//   aal
//   aas
//   ...
//   zag
//   zax
//   zit
//   716  Scrabble-friendly words of length 3
//        created by prepending/appending a letter to another word.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 22
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/18/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "lexicon.h"
using namespace std;

// Types and constants

const string LINE = string(60, '-') + '\n';
const string ALPHABET = "abcdefghijklmnopqrstuvwxyz";

// Function prototypes

void banner(int wordLength);
void getScrabbleWords(Lexicon lex, int wordLength, string alphabet,
                        Set<string> & scrabbleWords);
void showScrabbleWords(Set<string> & scrabbleWords, int wordLength);

// Main program

int main(int argc, char * argv[]) {

    Set<string> scrabbleWords;
    Lexicon english("EnglishWords.dat");
    
    int wordLength = 3;
    banner(wordLength);
    getScrabbleWords(english, wordLength, ALPHABET, scrabbleWords);
    showScrabbleWords(scrabbleWords, wordLength);
    
    return 0;
}

// Function definitions

//
// Function: banner
// Usage: banner(wordLength)
// -------------------------
// Displays the purpose of the program on the console.
//

void banner(int N) {
    cout << "This program displays all " << N << "-letter, 'Scrabble-friendly',"
         << endl
         << "words in the Stanford English lexicon formed by prepending" << endl
         << "or appending a letter to all " << N-1 << "-letter words."
         << endl;
    cout << LINE;
}

//
// Function: getScrabbleWords
// Usage: getScrabbleWords(lex, N, alphabetString, ScrabbleWords);
// ------------------------------------------------------------------------
// Returns a set of N-length words that can be constructed by
// either prepending or appending a letter to an (N-1)-length word.
//

void getScrabbleWords(Lexicon lex, int N, string alphabet,
                        Set<string> & scrableWords) {
    
    // Gather all N-length words.
    
    Set<string> actualNLengthWords;
    for (string word : lex) {
        if (word.length() == N) {
            actualNLengthWords.add(word);
        }
    }
    
    // 
    // Build all N-length 'franken' words in the laboratory
    // by slapping a letter on the front or back of all 
    // (N-1)-length words.  Some of these will not be viable words.
    //
    
    Set<string> frankeNLengthWords;
    for (string word : lex) {
        if (word.length() == (N - 1)) {
            for (char ch : alphabet) {
                string prependW = ch + word;
                string appendW = word + ch;
                frankeNLengthWords.add(prependW);
                frankeNLengthWords.add(appendW);
            }
        }
    }
    
    //
    // Discern which of the 'franken' words are real by intersecting
    // with actual N-length words.  
    //
    // These are useful in Scrabble play since they build off of shorter
    // words already on the board.
    //

    scrableWords = actualNLengthWords * frankeNLengthWords;
}

//
// Function: showScrabbleWords
// Usage: showScrabbleWords(scrabbleWords, N);
// ----------------------------------------------------
// Dumps the list or N-length Scrabble Words to the console.
//

void showScrabbleWords(Set<string> & scrabbleWords, int N) {
    for (string word : scrabbleWords) {
        cout << word << endl;
    }
    cout << scrabbleWords.size()
         << "\tScrabble-friendly words of length " <<  N << endl
         << "\tcreated by prepending/appending a letter to another word."
         << endl;
}