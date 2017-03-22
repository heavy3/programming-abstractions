//
// main.cpp
//
// The program implements the function
//
//     string acronym(str);
//
// which takes a string and returns the acronym formed from that string.
// Hyphenated words are also supported (e.g., 'self-contained' -> 'SC').
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 6
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/2/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
using namespace std;

// Function prototypes

string acronym(string str);
void prompt();
bool hasWords(string str);
string nextWord(string & str);
bool ishyphen(char ch);

int main(int argc, char * argv[]) {
    string line;
    string theAcronym;
    
    prompt();
    while (true) {
        cout << "Words: ";
        getline(cin, line);
        if (line.length() == 0) break;
        theAcronym = acronym(line);
        cout << setw(10) << theAcronym << " = " << line << endl;
    }
    return 0;
}

//
// Function: acronym
// Usage: string result = acronym("GNU's not Unix");
// -------------------------------------------------
// Transforms a string of space or hyphen delimited words into an ancronym.
// Result is retured as a new string to the caller.
//

string acronym(string line) {
    string result;
    
    while (hasWords(line)) {
        string word = nextWord(line);
        result += toupper(word[0]);
    }
    return result;
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
// Returns the next word in a string of input text.
//
// The input parameter is pass-by-reference with the effect of
// progressively popping a word off the head of the string
// with each call until the input is empty.
//

string nextWord(string & line) {
    string word;
    int cutIndex = 0;
    
    // Trim any whitespace or hyphens at the beginning of the line.
    
    for (int i = 0; i < line.length(); i++) {
        if (isspace(line[i])) cutIndex++;
        if (ishyphen(line[i])) cutIndex++;
        if (!isspace(line[i]) && !ishyphen(line[i])) break;
    }
    
    if (cutIndex) {
        // Had leading whitespace.  So erase that.
        line.erase(0, cutIndex);
    }
    
    //
    // Next occurence of whitespace, hypen, or end of string delimits the
    // end of the word.
    //
    
    cutIndex = 0;
    for (int i = 0; i < line.length(); i++) {
        if (isspace(line[i]) || ishyphen(line[i])) {
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

//
// Function: prompt
// Usage: prompt()
// ---------------------------------------------
// Humanely prompts user for input.
//

void prompt() {
    cout << "I turn space-delimited and hyphen-delimited words into acronyms."
    << endl << endl;
    cout << "Win big money (or at least temporary diversion) by playing along ."
    << endl << endl;
    cout << "If you want to quit, just hit [Enter] by itself." << endl << endl;
}

//
// Function: ishyphen
// Usage: if (ishypen(ch)) { ... }
// -------------------------------
// Returns true if the input character is a hyphen.
//

bool ishyphen(char ch) {
    return (ch == '-');
}
