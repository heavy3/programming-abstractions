//
// main.cpp
//
// This program displays a list of all palindromes in an English lexicon
// followed by some summarizing details.
//
// For example:
//
//    This program identifies all the palindromes in a lexicon.
//    ------------------------------------------------------------
//    ...
//    civic
//    dad
//    deed
//    ...
//
//    Summary
//    -------
//    Words examined: 127145
//    Palindromes found: 103
//    Longest palindrome(s) found: 7 letters
//    deified
//    halalah
//    ...
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 21
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
#include <cmath>
#include "lexicon.h"
using namespace std;

// Types and constants

const string LINE = string(60, '-') + '\n';

// Function prototypes

void banner();
bool isPalindrome(string str);
Vector<string> getPalindromes(Lexicon & lexicon);
void showPalindromes(Vector<string> & palindromes);
void showSummary(Vector<string> & palindromes, int nLexWords);

// Main program

int main(int argc, char * argv[]) {
    banner();
    
    Lexicon english("EnglishWords.dat");
    
    Vector<string> palindromes = getPalindromes(english);
    showPalindromes(palindromes);
    showSummary(palindromes, english.size());

    return 0;
}

// Function definitions

//
// Function: banner
// Usage: banner(upperBound)
// -------------------------
// Displays the purpose of the program on the console.
//

void banner() {
    cout << "This program identifies all the palindromes in a lexicon." << endl;
    cout << LINE;
}

//
// Function: getPalindromes
// Usage: Vector<string> p = getPalindromes(lexicon);
// --------------------------------------------------
// Returns a vector of palindromes found in a lexicon.
//

Vector<string> getPalindromes(Lexicon & lexicon) {
    Vector<string> palindromes;
    
    for (string word: lexicon) {
        if (isPalindrome(word)) palindromes.add(word);
    }
    return palindromes;
}

//
// Function: isPralindrome
// Usage: if (isPalindrome(str)) { ... };
// --------------------------------------
// Returns true is an input string is a palindrome (i.e., same string when
// read forward or backward).
//

bool isPalindrome(string str) {
    int n = str.length();
    for (int i = 0; i < n / 2; i++) {
        if (str[i] != str[n - i - 1]) return false;
    }
    return true;
}

//
// Function: showPalindromes
// Usage: showPalindromes(Vector<string> palindromes);
// ------------------------------------------------------------
// Dumps the vector of palindromes to the console.
//

void showPalindromes(Vector<string> & palindromes) {
    for (string p : palindromes) {
        cout << p << endl;
    }
}

//
// Function: showSummary
// Usage: showSummary(palindromes, nLexWords);
// -------------------------------------------
// Summarizes the palindrome search results with stats on number of
// words searched, number of palindromes found, longest palindrome size,
// and the list of longest palindromes displayed to the console.
//

void showSummary(Vector<string> & palindromes, int nLexWords) {
    int numP = palindromes.size();
    Vector<string> longestP;
    
    int longest = 0;
    for (string p : palindromes) {
        longest = (longest > p.length()) ? longest : p.length();
    }
    
    for (string p : palindromes) {
        if (p.length() == longest) {
            longestP.add(p);
        }
    }
    
    cout << endl << "Summary" << endl << "-------" << endl;
    cout << "Words examined: " << nLexWords << endl;
    cout << "Palindromes found: " << numP << endl;
    cout << "Longest palindrome(s) found: " << longest << " letters" << endl;
    for (string p : longestP) {
        cout << p << endl;
    }
}
