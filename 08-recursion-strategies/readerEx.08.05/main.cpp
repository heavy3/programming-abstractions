//
// main.cpp
//
// This program finds all the English words that can be formed from
// a subset of letters in order from a starting word.
//
// For example,
//
//    "happy" -> "a", "ha", "hap", "happy"
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/7/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "simpio.h"
#include "Set.h"
#include "Lexicon.h"
#include "error.h"

// Prototypes

Set<std::string> subStrings(std::string str);
Set<std::string> embeddedWords(std::string word, Lexicon & lex);
ostream & operator<<(ostream & os, Set<std::string> strset);

// 
// Um, so these routines are computationally expensive.  Even for a
// modest word like:
//
//      'establish'
//
// I see noticeable delay.  I rewrote the subStrings primitive with
// a pass-by-reference set in the hopes of nominal improvement, but no joy.
//
// Words like 'antidisestablishmentarianism' are hopeless.  Part of the
// message of section 8.2, though, is the notion of inherent intractability.
//

void subStrings2(std::string str, Set<std::string> & substr);
Set<std::string> embeddedWords2(std::string word, Lexicon & lex);

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.05\n";
const std::string DETAIL = "Embedded Words";
const std::string BANNER = HEADER + DETAIL;
const std::string PROMPT = "Enter starting word: ";
const std::string RESULT = "The embedded words are: ";
const std::string WORDS  = "EnglishWords.dat";

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    Lexicon lex(WORDS);
    
    std::string word = getLine(PROMPT);
    std::cout << RESULT << std::endl;
    std::cout << embeddedWords(word, lex);
    
    return 0;
}

//
// Function: subStrings
// Usage: Set<std::string> substrings = subStrings(str);
// -----------------------------------------------------
// Returns a set of all substrings that derive from the
// input string.
//
// NB: The substrings preserve relative order of
//     characters in the base string.
//

Set<std::string> subStrings(std::string str) {
    Set<std::string> result;
    
    result += str;  // Capture full-length string to the set.
    
    for (int i = 0; i < str.length(); i++) {
        
        // Create the next generation of substrings
        // by positionally snipping out one character at a time
        // from the base string.
        
        std::string rest = str.substr(0, i) + str.substr(i + 1);
        
        // Add that generation of substrings to the set.
        
        result += rest;
        
        // Recurse against the substrings.  Recursion bottoms out
        // when length of substring is 0 and for loop short-circuits.
        
        result += subStrings(rest);
    }
    return result;
}

//
// Function: embeddedWords
// Usage: Set<std::string> words = embeddedWords("happy", lexicon);
// ----------------------------------------------------------------
// Returns a lexicon-verified set of embedded words contained within
// an input word.
//
// The algorithm iterates across the set of all possible substrings
// implied by the word and determines which of these match entries
// in a lexicon of valid words.
//

Set<std::string> embeddedWords(std::string word, Lexicon & lex) {
    Set<std::string> words;
    
    for (std::string substr : subStrings(word)) {
        if (lex.contains(substr)) {
            words += substr;
        }
    }
    return words;
}

void subStrings2(std::string str, Set<std::string> & result) {
    
    result += str;  // Capture full-length string to the set.
    
    for (int i = 0; i < str.length(); i++) {
        
        // Create the next generation of substrings
        // by positionally snipping out one character at a time
        // from the base string.
        
        std::string rest = str.substr(0, i) + str.substr(i + 1);
        
        // Add that generation of substrings to the set.
        
        result += rest;
        
        // Recurse against the substrings.  Recursion bottoms out
        // when length of substring is 0 and for loop short-circuits.
        
        subStrings2(rest, result);
    }
}

//
// Nominal attempt at improving performance.
//

Set<std::string> embeddedWords2(std::string word, Lexicon & lex) {
    Set<std::string> words;
    Set<std::string> substrings;
    
    subStrings2(word, substrings);
    for (std::string s : substrings) {
        if (lex.contains(s)) {
            words += s;
        }
    }
    return words;
}

ostream & operator<<(ostream & os, Set<std::string> strset) {
    for (std::string s : strset) {
        os << "  " << s << std::endl;
    }
    return os;
}
