//
// Subsequences.cpp
//
// Given two strings, returns whether the second string is a subsequence of
// the first string.
//
// Example output:
//
//     CS106B Programming Abstractions: Assignment 3.1
//     Subsequences
//
//     Determines if one string is a subsequence of another.
//
//     Is pin a subsequence of springtime?  true
//     Is nip a subsequence of springtime?  false
//     Is steal a subsequence of least?     false
//     Is springtime a subsequence of pin?  false
//
// Analysis
//
// This problem decomposes nicely into a recursion-with-backtracking pattern
// since we're only looking for the first instance of subsequence
// (as opposed to exhaustively enumerating all valid subsequences).
//
// --------------------------------------------------------------------------
// Assignment: 3, Part 1 Subsequences
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/110%20Assignment%203.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/6/2017.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
using namespace std;

// Constants and Types

const string HEADER = "CS106B Programming Abstractions: Assignment 3.1\n";
const string DETAIL = "Subsequences";
const string BANNER = HEADER + DETAIL;
const int FMT = 37;

// Prototypes

bool isSubsequence(string text, string subseq);
void testSubsequence();

int main() {
    cout << BANNER << endl << endl;
    cout << "Determines if one string is a subsequence of another." << endl;
    cout << endl;
    
    testSubsequence();
    return 0;
}

// Function: isSubsequence
// Usage: if (isSubsequence("springtime", "pin")) . . .
// ----------------------------------------------------
// Returns true if the second string is a subsequence of the first,
// meaning the character sequence of the second string can be found
// in the same relative order (though possibly with intervening characters)
// in the first.
//
// For example, "pin is a subsequence of "springtime".
//                                         - --
// The function uses a recursive strategy with backtracking to match up
// letters in the second string against letters in the first.
//
// We iterate across the letters of the text string, looking for a match
// against the first character of the subseq candidate string.  On match,
// we recursively test for subsequence in the substrings that follow the
// matched character.  This diminishes the input and bounds the recursion.
//
// An empty subseq string signifies a subsequence has been detected, since
// the subseq string is only diminished as letters are matched against the
// text string.
//
// Otherwise, if we fail to match for a given value of text[i], we backtrack
// to an earlier invocation and attempt to re-match against later characters
// in the text string, hoping -this- time, we'll find matches for all subsequent
// characters as well.  Failing that, we drop through to the end of the
// function and simply return false.

bool isSubsequence(string text, string subseq) {
    if (subseq == "") return true;
    for (int i = 0; i < text.length(); i++) {
        if (subseq[0] == text[i]) {
            return isSubsequence(text.substr(i+1), subseq.substr(1));
        }
    }
    return false;  // Trigger backtracking to upstack invocation.
}

void testSubsequence() {
    cout << setw(FMT) << left << "Is pin a subsequence of springtime? ";
    cout << boolalpha << isSubsequence("springtime", "pin") << endl;

    cout << setw(FMT) << left << "Is nip a subsequence of springtime? ";
    cout << boolalpha << isSubsequence("springtime", "nip") << endl;

    cout << setw(FMT) << left << "Is steal a subsequence of least? ";
    cout << boolalpha << isSubsequence("steal", "least") << endl;

    cout << setw(FMT) << left << "Is springtime a subsequence of pin? ";
    cout << boolalpha << isSubsequence("pin", "springtime") << endl;
}
