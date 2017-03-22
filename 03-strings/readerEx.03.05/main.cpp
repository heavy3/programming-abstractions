//
// main.cpp
//
// This program that reads in words and prints out their Scrabble score
// according to the following point system:
//
// Points Letters
//   1      A, E, I, L, N, O, R, S, T, U
//   2      D, G
//   3      B, C, M, P
//   4      F, H, V, W, Y
//   5      K
//   8      J, X
//   10     Q, Z
//
// The problem spec says to ignore any lowercase letters.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/2/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>

using namespace std;

// Function prototypes

int getScore(char ch);
int getScore(string word);
void prompt();
bool hasWords(string str);
string nextWord(string & str);

//  Typical interaction:
//  --------------------------------------------------------
//  Enter some words and I'll compute their scrabble score.
// 
//  Sadly, you should use UPPERCASE as I'm not that cool and
//  don't know how to tally lowercase letters.
// 
//  If you want to quit, just hit [Enter] by itself.
// 
//  Words: THESE ARE SOME SWEET SKILZFUL WORDS
//  THESE score: 8
//  ARE score: 3
//  SOME score: 6
//  SWEET score: 8
//  SKILZFUL score: 24
//  WORDS score: 9

int main(int argc, char * argv[]) {
    string line;
    
    prompt();
    getline(cin, line);
    while (hasWords(line)) {
        string word = nextWord(line);
        int score = getScore(word);
        cout << word << " score: " << score << endl;
    }
}

//
// Function: getScore
// Usage: int letterScore = getScore("A");
// ---------------------------------------
// Returns the scrabble score for a given letter based upon it's usage
// frequency in the English language.
//

int getScore(char ch) {
    switch (ch) {
        case 'A': case 'E': case 'I': case 'L': case 'N':
        case 'O': case 'R': case 'S': case 'T': case 'U':
            return 1;

        case 'D': case 'G':
            return 2;
            
        case 'B': case 'C': case 'M': case 'P':
            return 3;
            
        case 'F': case 'H': case 'V': case 'W': case 'Y':
            return 4;
        
        case 'K':
            return 5;
            
        case 'J': case 'X':
            return 8;
        
        case 'Q': case 'Z':
            return 10;
            
        default:
            return 0;
    }
}

//
// Function: getScore
// Usage: int letterScore = getScore("meatball);
// ---------------------------------------------
// Returns the scrabble score for a given word based upon it's usage
// frequency in the English language.
//

int getScore(string word) {
    int score = 0;
    
    for (int i = 0; i < word.length(); i++) {
        score += getScore(word[i]);
    }
    return score;
}

//
// Function: prompt
// Usage: prompt()
// ---------------------------------------------
// Humanely prompts user for input.
//

void prompt() {
    cout << "Enter some words and I'll compute their scrabble score."
         << endl << endl;
    cout << "Sadly, you should use UPPERCASE as I'm not that cool and" << endl;
    cout << "don't know how to tally lowercase letters."
         << endl << endl;
    cout << "If you want to quit, just hit [Enter] by itself." << endl << endl;
    cout << "Words: ";
}

//
// Function: hasWords
// Usage: while (hasWords(str)) { ... }
// ------------------------------------
// Returns true if the string is comprised of any non-whitespace characters.
//

bool hasWords(string str) {
    bool has = false;
    
    for (int i = 0; i < str.length(); i++) {
        if (!isspace(str[i])) {
            has = true;
            break;
        }
    }
    return has;
}

//
// Function: nextWord
// Usage: string word = nextWord(line);
// ------------------------------------
// Returns the next space-delimited word in a string of input text which
// has been passed by reference such that the line is partially consumed
// as word tokens are returned from the head of the line.
//

string nextWord(string & line) {
    string word;
    int cutIndex = 0;
    
    // Trim any whitespace at the beginning of the line.
    
    for (int i = 0; i < line.length(); i++) {
        if (isspace(line[i])) cutIndex++;
        if (!isspace(line[i])) break;
    }
    
    if (cutIndex) {
        /* Had leading whitespace.  So erase that. */
        line.erase(0, cutIndex);
    }
    
    // 
    // Next occurence of whitespace or end of string delimits the
    // end of the word.
    //
    
    cutIndex = 0;
    for (int i = 0; i < line.length(); i++) {
        if (isspace(line[i])) {
            cutIndex = i;
            break;
        }
    }
    
    //
    // Snip the word out of the line string with the side-effect
    // of shortening the original string.
    //
    
    if (cutIndex) {
        word = line.substr(0, cutIndex);
        line.erase(0, cutIndex);
    } else {
        word = line;
        line.erase(0, line.length());
    }
    
    return word;
}
