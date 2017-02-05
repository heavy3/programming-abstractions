//
// main.cpp
//
// This program implements the function, listCompletions, to ease texting 
// on an old-school touch-tone phone keyboard based upon a predictive 
// dictionary word lookup.
//
// For example,
//
//    If the sequence "72547" is entered on the standard
//    phone keyboard:
//
//       [  1  ] [  2  ] [  3  ]
//       [     ] [A B C] [D E F]
//
//       [  4  ] [  5  ] [  6  ]
//       [G H I] [J K L] [M N O]
//
//       [  7  ] [  8  ] [  9  ]
//       [PQRS ] [T U V] [WXYZ ]
//
//       [  *  ] [  0  ] [  #  ]
//       [     ] [  +  ] [     ]
//
//    then listCompletions("72547", english) should generated the following
//    strings:
//
//       palisade
//       palisaded
//       palisades
//       palisading
//       palish
//       rakis
//       rakish
//       rakishly
//       rakishness
//       sakis
//
//    since these strings match the prefixes associated with "72547"
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/11/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cctype>
#include "set.h"
#include "vector.h"
#include "simpio.h"
#include "lexicon.h"
using namespace std;

// Prototypes

void listCompletions(string digits, Lexicon & lex);
Set<string> getMnemonics(Vector<int> digits, Vector<string> & keypad);

Set<string> generatePermutations(string str);
Vector<int> normalizeNum(string phoneNum);

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.12\n";
const std::string DETAIL = "Predictive Text Completions";
const std::string BANNER = HEADER + DETAIL;
const std::string LEXICON = "EnglishWords.dat";

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    Lexicon english(LEXICON);
    
    string phoneStr = getLine("Start texting old-school with phone keypad: ");
    listCompletions(phoneStr, english);

    return 0;
}

//
// Function: listCompletions
// Usage: listCompletions(digits, lexicon)
// ---------------------------------------
// Enumerates all the words in a lexicon that map a numeric phone keypad
// prefix to words in a dictionary.
// 
// Output is sent to the console.
//
// This is a wrapper for the recusive function, getMnemonics.
//

void listCompletions(string digits, Lexicon & lex) {
    Vector<int> normalizedDigits = normalizeNum(digits);

    Vector<string> keypad;
    keypad +=    "+";                   // 0
    keypad +=    "1", "abc",   "def";   // 1   2   3
    keypad +=  "ghi", "jkl",   "mno";   // 4   5   6
    keypad += "pqrs", "tuv",  "wxyz";   // 7   8   9

    string digitChars;
    Set<string> mnemonics = getMnemonics(normalizedDigits, keypad);
    bool foundMatch = false;
    
    cout << "Possible words for: " << digits << endl;
    for (string prefix : mnemonics) {
        if (lex.containsPrefix(prefix)) {
            for (string word : lex) {
                string wordPrefix = word.substr(0, prefix.length());
                if (wordPrefix == prefix) {
                    foundMatch = true;
                    cout << word << endl;
                }
            }
        }
    }
    if (!foundMatch) {
        cout << "(No dictionary matches :-/)" << endl;
    }
}

//
// Function: getMnemonics
// Usage: Set<string> mnemonics = getMnemonics(digitsVector, keypad);
// ------------------------------------------------------------------
// Returns a set of strings representing all alpha-character mnemonics 
// associated with a vector of phone digits.
//
// Working from left-to-right on an n-length phone number, we iterate over 
// the list of characters associated with a given keypad digit, then 
// recursively build up all the unique mnemonic combinations of the remaining
// (n-1) digits to append to the higher-order iterated characters.
//

Set<string> getMnemonics(Vector<int> digits, Vector<string> & keypad) {
    Set<string> results;
    
    if (digits.size() > 0) {
        int digIndex = 0;   // Index of digit to process.
        
        // 
        // Reduce the size of the input phone number vector by one digit
        // in preperation for recursion.
        //
         
        Vector<int> fewerDigits(digits);
        fewerDigits.remove(digIndex);
        
        // Lookup the characters associated with this digit on the keypad.
        
        string digitChars = keypad[digits[digIndex]];

        //
        // For each character associated with the digit, build separate mnemonic 
        // strings that begin with that character.
        //
        
        for (char ch : digitChars) {
            
            if (fewerDigits.size() == 0) {
                
                //
                // Recursive Elementary Case
                //
                // We hit the last digit in the phone number, so add
                // the string form of the associated char for that digit
                // to the set of result strings.
                //
                // In the recursive unwinding, we'll prepend higher-order
                // mnemonic characters to this one.
                //
                
                results += string(1, ch);
                
            } else {
                
                //
                // General Case
                //
                // Recursively build up a set of smaller mnemonic strings 
                // to prepend with the just looked-up character value of the 
                // left-most digit in the input vector.
                //
                
                for (string rest : getMnemonics(fewerDigits, keypad)) {
                    results += string(1, ch) + rest;
                }
            }
        }
    }
    return results;
}

//
// Function: normalizeNum
// Usage: Vector<int> phnum = normalizeNum("867-5309");
// ----------------------------------------------------
// Converts a phone number string into a vector of int values.
// Filters out things like area code parenthesese, hypens, and spaces.
//

Vector<int> normalizeNum(string phoneNum) {
    Vector<int> normalized;
    
    for (char digit : phoneNum) {
        if (isnumber(digit)) {
            normalized.add(digittoint(digit));
        }
    }
    return normalized;
}