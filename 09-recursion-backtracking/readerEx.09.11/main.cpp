//
// main.cpp
//
// This program implements the function:
//
//    bool wildcardMatch(string filename, string pattern);
//
// which simulates filename matching against wildcard patterns.
// 
// For example:
//
//    wildcardMatch("US.txt", "*.*")          returns true
//    wildcardMatch("test", "*.*")            returns false
//    wildcardMatch("test.h", "test.?")       returns true
//    wildcardMatch("test.cpp", "test.?")     returns false
//    wildcardMatch("x", "??*")               returns false
//    wildcardMatch("yy", "??*")              returns true
//    wildcardMatch("zzz", "??*")             returns true
//
// * matches 0 or more characters in a sequence
// ? matches 1 character
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 11
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/5/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "tokenscanner.h"
#include "vector.h"
#include "error.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.11\n";
const std::string DETAIL = "Filename Wildcarding";
const std::string BANNER = HEADER + DETAIL;
const std::string TESTDATA = "US.txt *.txt|US.txt *.*|test *.*|test.h test.?|"
                             "test.cpp test.?|x ??*|yy ??*|zzz ??*";
const std::string WORDCHARS = ".*?";

// Prototypes

bool wildcardMatch(std::string filename, std::string pattern);
bool matchNTokens(std::string filenameTail, Vector<std::string> & patternTokens);
void tokenizeTestData(std::string testData, Vector<std::string> & filenames,
                   Vector<std::string> & patterns);
Vector<std::string> subStringize(std::string str);
bool tokenMatches(std::string token, std::string str);
Vector<std::string> tokenizePattern(std::string pattern);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    Vector<std::string> filenames;
    Vector<std::string> patterns;
    
    tokenizeTestData(TESTDATA, filenames, patterns);
    
    for (int i = 0; i < filenames.size(); i++) {
        cout << "wildcardMatch(" << filenames[i] << ", " << patterns[i] << ");";
        cout << " " << boolalpha;
        
        cout << wildcardMatch(filenames[i], patterns[i]) << endl;
    }
    return 0;
}

//
// Function: wildcardMatch
// Usage: if (wildcardMatch(filename, pattern)) { . . . }
// ------------------------------------------------------
// Returns true if a wildcard pattern matches a filename string.
//

bool wildcardMatch(std::string filename, std::string pattern) {
    Vector<std::string> patternTokens = tokenizePattern(pattern);
    
    return matchNTokens(filename, patternTokens);
}

//
// Function: matchNTokens
// Usage: if (matchNTokens(filename, remainingPatternTokens)) { . . . }
// ---------------------------------------------------------------------
// Returns true if a vector of pattern-matching tokens match an input string.
//

bool matchNTokens(std::string filenameIsh,
                   Vector<std::string> & patternTokens) {
    
    // Base cases:
    //
    // 1. no more pattern-matching tokens left to map onto the filename /and/
    //    no more filname remnants to map.
    //
    // 2. only "*" pattern token remains.  will map to anything, including
    //    an empty string.
    //

    if (patternTokens.size() == 0) return filenameIsh.size() == 0;
    if ((patternTokens.size() == 1) && (patternTokens[0] == "*")) return true;
    
    // Recursive case
    
    std::string nthToken = patternTokens[0];
    patternTokens.remove(0);
    
    for (std::string subString : subStringize(filenameIsh)) {
        if (tokenMatches(nthToken, subString)) {
            filenameIsh.erase(0, subString.size());
            if (matchNTokens(filenameIsh, patternTokens)) {
                return true;
            }
            filenameIsh.insert(0, subString);
        }
    }

    // Trigger backtracking and undo earlier map of pattern token & substring.

    patternTokens.insert(0, nthToken);
    return false;
}

//
// Function: tokenizePattern
// Usage: Vector<std::string> patternTokens = tokenizePattern(pattern);
// --------------------------------------------------------------------
// Disassembles a wildcard pattern into a vector of individual tokens.
//

Vector<std::string> tokenizePattern(std::string pattern) {
    Vector<std::string> results;
    
    TokenScanner tscanner(pattern);
    tscanner.addWordCharacters(".");
    while (tscanner.hasMoreTokens()) {
        results += tscanner.nextToken();
    }
    return results;
}

//
// Function: tokenMatches
// Usage: if (tokenMatches(token, str)) { . . . }
// ----------------------------------------------
// Returns true if a pattern-matching token matches an input string.
//

bool tokenMatches(std::string token, std::string str) {
    if (token == "?") {
        return str.size() == 1;     // must match exactly 1 character
    } else if (token == "*") {
        return true;                // will match any input
    } else {
        return token == str;        // string-literal comparison
    }
}

//
// Function: subStringize
// Usage: Vector<std::string> subStrings = subStringize(str);
// ----------------------------------------------------------
// Returns all the substrings of a string that include the first character
// of the original string.
//
// For example: myname == { "m", "my", "myn", "myna", "mynam", "myname" }
//

Vector<std::string> subStringize(std::string str) {
    Vector<std::string> results;
    
    if (str.size() > 0) {
        for (int i = 1; i <= str.size(); i++) {
            results += str.substr(0, i);
        }
    }
    return results;
}

//
// Function: tokenizeTestData
// Usage: tokenizeTestData(testData, filenames, patterns);
// -------------------------------------------------------
// Parse test data into corresponding vectors filenames and patterns.
//

void tokenizeTestData(std::string testData, Vector<std::string> & filenames,
                   Vector<std::string> & patterns) {
    
    TokenScanner scanner(testData);
    scanner.addWordCharacters(WORDCHARS);
    
    while (scanner.hasMoreTokens()) {
        std::string token = scanner.nextToken();
        TokenType theType = scanner.getTokenType(token);
        switch (theType) {
            case WORD: {
                filenames += token;
            }   break;
                
            case SEPARATOR: { // Using space as field delimiter
                patterns += scanner.nextToken();
            }   break;
                
            default:
                break;
        }
    }
}