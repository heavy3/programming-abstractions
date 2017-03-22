//
// main.cpp
//
// This program implements the function, createRegularPlural(word)
// which the plural of a word formed by following rules:
// 
// 1. If the word ends in s, x, z, ch, or sh, add es to the word.
// 2. If the word ends in a y preceded by a consonant, change the y to ies.
// 3. In all other cases, add just an s.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/5/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

// Types and constants

const string CONSONANTS = "bcdfghjklmnpqrstvwxyz";

enum PluralRule {
    add_es,
    add_ES,
    y_to_ies,
    Y_to_IES,
    add_s,
    add_S,
    no_change
};

// Function prototypes

bool endsWith(string str, string suffix);
bool endsWith(string str, char suffixCh);
bool testCreateRegularPlural(string word, string expectedPlural);
string createRegularPlural(string singular);
PluralRule getPluralRule(string word);
bool is_consonant(char ch);

// Main program

// Typical output
//
// [PASS] :-)  createRegularPlural("cat") = cats
// [PASS] :-)  createRegularPlural("Church") = Churches
// [PASS] :-)  createRegularPlural("CHURCH") = CHURCHES
// [PASS] :-)  createRegularPlural("candy") = candies
// [PASS] :-)  createRegularPlural("flakey") = flakey
// [PASS] :-)  createRegularPlural("ZOMBIE") = ZOMBIES
// [FAIL] :-(  createRegularPlural("dear") = dears
//

int main(int argc, const char * argv[]) {
    testCreateRegularPlural("cat", "cats");
    testCreateRegularPlural("Church", "Churches");
    testCreateRegularPlural("CHURCH", "CHURCHES");
    testCreateRegularPlural("candy", "candies");
    testCreateRegularPlural("flakey", "flakey");
    testCreateRegularPlural("ZOMBIE", "ZOMBIES");
    testCreateRegularPlural("dear", "dear");
}

// Function definitions

//
// Function: testCreateRegularPlural
// Usage: if (testCreateRegularPlural("church", "churches")) cout << ":-)";
// ------------------------------------------------------------------------
// Returns boolean true if the createRegularPlural primitive returns the
// expected plural version of a given input string.
//

bool testCreateRegularPlural(string word, string expectedPlural) {
    string p = "[PASS] :-)";
    string f = "[FAIL] :-(";
    
    string actual = createRegularPlural(word);
   
    (actual == expectedPlural) ? cout << p : cout << f;
    
    cout << "  createRegularPlural(\"" << word << "\") = "
         << setw(15) << left << actual << endl;
    
    return actual == expectedPlural;
}

//
// Function: createRegularPlural
// Usage: string plural = createRegularPlural("cat");
// Output: plural = "cats";
// --------------------------------------------------
// Returns the plural version of a given word according to the standard
// English rules:
//
//  a. If the word ends in s, x, z, ch, or sh, add es to the word.
//  b. If the word ends in a y preceded by a consonant, change the y to ies.
//  c. In all other cases, add just an s.
//
// Limitations: No provision is made for checking if the word is already
//              in plural form or both singular /and/ plural (e.g., dear).
//

string createRegularPlural(string word) {
    PluralRule rule;
    string plural;
    
    rule = getPluralRule(word);
    switch (rule) {
        
        case add_es:
            plural = word + "es";
            break;
            
        case add_ES:
            plural = word + "ES";
            break;
            
        case y_to_ies: {
            size_t last = word.length() - 1;
            plural = word.replace(last, 1, "ies");
            break;
        }

        case Y_to_IES: {
            size_t last = word.length() - 1;
            plural = word.replace(last, 1, "IES");
            break;
        }
            
        case add_s:
            plural = word + "s";
            break;
            
        case add_S:
            plural = word + "S";
            break;
            
        case no_change:
            plural = word;  // Some words are both plural and singular.
            break;
            
        default:
            char last = word[word.length() - 1];
            if (tolower(last) == last) {
                plural = word + "s";
            } else {
                plural = word + "S";
            }
    }
    return plural;
}

//
// Function: getPluralRule
// Usage: PluralRule rule = getPluralRule(church)
// Output: rule == add_es
// ----------------------------------------------
// Return the pluralization rule to apply to this word.
//

PluralRule getPluralRule(string word) {
    
    if (endsWith(word, 's')  || endsWith(word, 'x')  || endsWith(word, 'z') ||
        endsWith(word, "ch") || endsWith(word, "sh"))
        return add_es;
    
    if (endsWith(word, 'S')  || endsWith(word, 'X')  || endsWith(word, 'Z') ||
        endsWith(word, "CH") || endsWith(word, "SH"))
        return add_ES;

    if ((endsWith(word, 'y')) || endsWith(word, 'Y')) {
        if (word.length() > 1) {
            char neighbor = word[word.length() - 2];
            if (is_consonant(neighbor)) {
                if (endsWith(word, 'y'))
                    return y_to_ies;
                else
                    return Y_to_IES;
            } else {
                // Should "flakey" become "flakeys"? I think not.
                return no_change;
            }
        }
    }
    
    char last = word[word.length() - 1];
    if (tolower(last) == last) {
        return add_s;
    } else {
        return add_S;
    }
}

//
// Function: is_consonant
// Usage: if (is_consonant('b')) { ... }
// ------------------------------------
// Returns true if a given character is a consonant.
//
// Implementation involves normalizing the input to lowercase
// and searching a string that includes all the consonants.
//

bool is_consonant(char ch) {
    return (CONSONANTS.find(tolower(ch)) != string::npos);
}

//
// Function: endsWith
// Usage: if (endsWith(str, "sweet sorrow.")) cout << "word, yo" << endl;
// -----------------------------------------------------------------------------
// Returns true is a string ends with a provided suffix string.
// Overloaded to return true if a string ends with a given suffix character.
//
// Algorithm calculates the point in the full string where substring comparison
// should begin and returns results of a substring comparison from that point.
//
// For example:
//
// Given this input string:   "Parting is such sweet sorrow."    length = 29
//                             ----+----1----+-^--2----+----    compare @ 16
// and this suffix string:                    "sweet sorrow."    length = 13
//
// 0-based start index is:     str.length() - suffix.length() = 29 - 13 = 16
//

bool endsWith(string str, string suffix) {
    if (suffix.length() > str.length()) return false;
    
    size_t startAt = str.length() - suffix.length();
    return (str.substr(startAt) == suffix);
}

bool endsWith(string str, char suffixCh) {
    if (str.length() < 1) {
        // Generally returns false since str is trivially empty,
        // unless the suffixCh is also trivially null :-)
        return (suffixCh == char(NULL));
    }
    size_t last = str.length() - 1;  // Correct for 0-based index.
    return (str[last] == suffixCh);
}