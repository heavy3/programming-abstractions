//
// main.cpp
//
// This program enhances a function that converts English sentences
// to Pig Latin to handle capitalized words sensibly.
//
// For example:
//
//    lineToPigLatin("Capital") returns "Apitalcay" (not "apitalClay")
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 15
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program refines the english-to-pig-latin function given in Figure 3-2
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/6/15.
// Copyright © 2015 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

// Function prototypes

string lineToPigLatin(string line);
string wordToPigLatin(string word);
int findFirstVowel(string word);
bool isVowel(char ch);
void swapCaps(string & head, string & tail);

// Main program

int main() {
    cout << "This program translate English to Pig Latin." << endl;
    string line;
    cout << "Enter English text: ";
    getline(cin, line);
    string translation = lineToPigLatin(line);
    cout << "Pig Latin output: " << translation << endl;
    return 0;
}

//
// Function: lineToPigLatin
// Usage: string translation = lineToPigLatin(line);
// -------------------------------------------------
// Translates each word in the line to Pig Latin, leaving all other
// characters unchanged.  The variable start keeps track of the index
// position at which the current word begins.  As a special case,
// the code sets start to -1 to indicate that the beginning of the
// current word has not yet been encountered.
//

string lineToPigLatin(string line) {
    string result;
    int start = -1;
    for (int i = 0; i < line.length(); i++) {
        char ch = line[i];
        if (isalpha(ch)) {
            if (start == -1) start = i;
        } else {
            if (start >= 0) {
                result += wordToPigLatin(line.substr(start, i - start));
                start = -1;
            }
            result += ch;
        }
    }
    if (start >= 0) result += wordToPigLatin(line.substr(start));
    return result;
}

//
// Function: wordToPigLatin
// Usage: string translation = wordToPigLatin(word);
// -------------------------------------------------
// This function translates a word from English to Pig Latin using the rules
// specified in the text.  The translated word is returned as the value of the
// function.
//

string wordToPigLatin(string word) {
    int vp = findFirstVowel(word);
    if (vp == -1) {
        return word;
    } else if (vp == 0) {
        return word + "way";
    } else {
        string head = word.substr(0, vp);
        string tail = word.substr(vp);
        swapCaps(head, tail);
        return tail + head + "ay";
    }
}

//
// Function: swapCaps
// Usage: swapCaps(head, tail);
// -------------------------------------
// Ensures that if a word is capitalized in English, it will be
// correspondingly capitalized in its pig-latin variant.
//
// For example, the word "Capital" splits on a vowel boundary into
//
//    head = C
//    tail = apital
//
// This function mutates head and tail to look like this:
//
//    head = c
//    tail = Apital
// 
// such that the calling routine can reassemble the parts into
// nicely capitalized pig-latin:
//
//    Capital --becomes--> Apitalcay (instead of apitalCay).
//

void swapCaps(string & head, string & tail) {
    
    if (head[0] == toupper(head[0])) {
        head[0] = tolower(head[0]);
        tail[0] = toupper(tail[0]);
    }
}

//
// Function: findFirstVowel
// Usage: int k = findFirstVowel(word);
// ------------------------------------
// Returns the index position of the first vowel in word.  If
// word does not contain a vowel, findFirstVowel returns -1.
//

int findFirstVowel(string word) {
    for (int i = 0; i < word.length(); i++) {
        if (isVowel(word[i])) return i;
    }
    return -1;
}

//
// Function: isVowel
// Usage: if (isVowel(ch)) ...
// ---------------------------
// Returns true if the character ch is a vowel.
//

bool isVowel(char ch) {
    const string vowels = "aeiouAEIOU";
    return (vowels.find(ch) != string::npos);
}