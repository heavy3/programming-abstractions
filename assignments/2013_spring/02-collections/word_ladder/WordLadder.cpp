//
// WordLadder.cpp
//
// This program prompts for two words and then attempts to construct a word
// ladder that connects the two words via a sequence of intermediary words
// that differ only by one letter from the adjacent words.
//
// Given "code" and "data" returns: code → core → care → dare → date → data
//
// For example:
//
//     Enter start word (RETURN to quit): awake
//     Enter destination word: sleep
//     Found ladder: awake aware sware share sharn shawn shewn sheen sheep sleep
//     Enter start word (RETURN to quit): airplane
//     Enter destination word: tricycle
//     No ladder found.
//     Enter start word (RETURN to quit):
//
// --------------------------------------------------------------------------
// Assignment: 2, Part 1 Word Ladders
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/090%20Assignment%202.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/23/2015.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "simpio.h"
#include "strlib.h"
#include "error.h"
#include "tokenscanner.h"
#include "queue.h"
#include "vector.h"
#include "lexicon.h"

using namespace std;

const string PROMPT_FIRST = "Enter start word (RETURN to quit): ";
const string PROMPT_FINAL = "Enter destination word: ";
const string REPORT_FOUND = "Found ladder: ";
const string REPORT_NOT_FOUND = "No ladder found.";
const int ndiff = -1; // Use this when difference between two strings is undef.

/* Function prototypes */

void getNearbyWords(int numLetterMutations,
                    string refWord,
                    Lexicon & allWords,
                    Lexicon & usedWords,
                    Lexicon & nearbyWords);
void getNLengthWords(int length, Lexicon & allWords, Lexicon & lengthWords);
string getWord(string prompt);
bool getWords(string & start, string & dest);
Vector<string> getWordLadder(string firstWord,
                             string finalWord,
                             Lexicon & allWords);
void reportWordLadder(Vector<string> ladder);
int operator -(string s1, string s2);

int main() {
    string firstWord;
    string finalWord;
    Lexicon allWords("EnglishWords.dat");
    
    while (getWords(firstWord, finalWord)) {
        Vector<string> ladder;
        
        ladder = getWordLadder(firstWord, finalWord, allWords);
        reportWordLadder(ladder);
    }
    return 0;
}

//
// Function: reportWordLadder
// Usage: reportWordLadder(ladder);
// --------------------------------
// Displays the results of a word ladder search to the console.
//
// Assumes two string constants, REPORT_FOUND and REPORT_NOT_FOUND
// are defined with suitable text for giving context to the list of
// words presented.
//
// Example: Found ladder:  work fork form foam flam flay play
// Example: No ladder found.
//

void reportWordLadder(Vector<string> wordLadder) {
    if (wordLadder.size()) {
        cout << REPORT_FOUND;
        for (string word: wordLadder) {
            cout << " " << word;
        }
        cout << endl;
    } else {
        cout << REPORT_NOT_FOUND << endl;
    }
}

//
// Function: getWordLadder
// Usage: Vector<string> ladder = getWordLadder("work", "play", lexicon);
// ----------------------------------------------------------------------
// Returns a Vector of strings linking the first and final words
// through a sequence of words that differ from adjacent words
// by 1 letter position.
//
// If no such sequence can be devised, an empty Vector<string> is returned.
//
// Example:  firstWord = "work";
//           finalWord = "play";
//           ladder = {"work", "fork", "form", "foam", "flam", "flay", "play"}
//                              -          -      -      -         -    -
//

Vector<string> getWordLadder(string firstWord, string finalWord,
                             Lexicon & allWords) {
    //
    // Reduce the size of the lexicon to the subset of words with the same
    // length as the first word.
    //
    // This is more an intuitive stab at performance than a rigorus analysis
    // of lexicon lookups.  Still this removes 80 - 90 percent of the words
    // in the lexicon so there is probably some virtue here especially for
    // longer solution lists.
    //
    // Also, avoid reuse of words in downstream lists.
    //
    
    Lexicon nLengthWords;
    getNLengthWords(firstWord.length(), allWords, nLengthWords);
    
    Lexicon usedWords;
    usedWords.add(firstWord);
    
    Vector<string> noLadder;
    Vector<string> seed;
    seed.add(firstWord);
    
    if ((firstWord - finalWord) == 0) { // Overloading subtraction operator.
        
        // Short-circuit return if first and final are the same word.
        
        return seed;
    }
    
    if (firstWord.length() != finalWord.length()) {
        return noLadder;
    }
    
    Queue<Vector<string> > wordLadders;
    wordLadders.enqueue(seed);
    
    while (wordLadders.size()) {
        
        Vector<string> ladder = wordLadders.dequeue();
        
        //
        // For the word list currently dequeued, inspect the last word
        // and find the set of (unused) words that differ from it
        // in only one letter position.
        //
        
        string latestWord = ladder[ladder.size() - 1];
        int distance = 1;
        Lexicon nearbyWords;
        
        getNearbyWords(distance, latestWord, nLengthWords, usedWords,
                       nearbyWords);
        
        //
        // Create the next generation of word ladders from the current
        // ladder by replicating it and appending one new proximate word.
        //
        // Check if the latest proximate word matches the desired final
        // word.  In that case the candiate solution /is/ a solution.
        //
        // Eventually we'll either drain the queue dry because we run out of
        // unused proximate words with no solution or we'll find a solution.
        //
        
        for (string word : nearbyWords) {
            Vector<string> nextLadder = ladder;
            nextLadder.add(word);
            if ((word - finalWord) == 0) {
                return nextLadder;  // <<- Found solution
            }
            wordLadders.enqueue(nextLadder);
        }
    }
    return noLadder;
}

//
// Function: getWords
// Usage: getWords(first, final);
// ------------------------------
// Returns true if the user enters two words at the console.
// The words are returned as reference variables.
//
// Assumes two string constants, PROMPT_FIRST and PROMPT_FINAL
// are defined with the text for prompting input.
//

bool getWords(string & first, string & final) {
    first = getWord(PROMPT_FIRST);
    if (first == "") return false;
    
    final = getWord(PROMPT_FINAL);
    if (final == "") return false;

    return true;
}

//
// Function: getWord
// Usage: string word = getWord("Enter start word (RETURN to quit): ");
// --------------------------------------------------------------------
// Prompts the user to enter a word which is normalized to lowercase
// with any preceeding or trailing whitespace stripped away.
//

string getWord(string prompt) {
    cout << prompt;
    
    string line = toLowerCase(getLine());
    TokenScanner ts(line);
    ts.ignoreWhitespace();
    
    string token;
    string word = "";
    
    if (ts.hasMoreTokens()) {
        token = ts.nextToken();
        if (ts.getTokenType(token) == WORD) word = token;
    }
    return word;
}

//
// Function: operator-
// Usage: int numDifferentLetters = str1 - str2;
// ---------------------------------------------
// "Subtracts" two strings of the same length yielding the number of differing
// letters.  I know, I'm being a maverick with this overload.
//
// Example:
//
// "pie" - "pay" = 2
//
// Strings of differing length are not valid for comparison and return
// the constant ndiff.
//
// Identical strings return 0.
//

int operator-(string s1, string s2) {
    
    // Two words must have the same length to have a computable difference
    // in this universe. :-)
    
    if (s1.length() != s2.length()) {
        return ndiff;
    }
    
    // Start by assuming the strings differ in all letter positions.
    
    int difference = s1.length();
    
    for (int i = 0; i < s1.length(); i++) {
        if (s1[i] == s2[i]) {
            difference--;       // A difference of 0 means identical.
        }
    }
    return difference;
}

//
// Function: getNearbyWords
// Usage: getNearbyWords(1, "pie", lexicon, usedWords, nearbyWords);
// -----------------------------------------------------------------
// Returns a lexicon of words that are lexigraphically distant from
// a reference word by the specified number letter differences.
//
// Example: The word "pie" is lexigraphically distant from
//          the word "pin" by 1 unit since they differ in only one letter.
//
// Exclude from consideration any words already in the used-words
// lexicon.  Furthermore, and new proximate words discovered are also added
// to the used list to allow for blacklisting by the caller.
//

void getNearbyWords(int distance, string refWord, Lexicon & allWords,
                   Lexicon & usedWords, Lexicon & nearbyWords) {
    
    nearbyWords.clear();
    for (string word : allWords) {
        if (distance == (word - refWord)) {// Overload minus operator
            if (usedWords.contains(word)) continue;  // Avoid already used words
            
            nearbyWords.add(word);
            usedWords.add(word);    // Avoid reuse in future via blacklist.
        }
    }
}

//
// Function: getNLengthWords
// Usage: getNLengthWords(length, lexicon, nLengthWords);
// ------------------------------------------------------
// Returns a reference variable lexicon of all words of specified length
// found in another lexicon.
//

void getNLengthWords(int N, Lexicon & allWords, Lexicon & nLengthWords) {
    
    for (string word: allWords) {
        if (N == word.length()) {
            nLengthWords.add(word);
        }
    }
}
