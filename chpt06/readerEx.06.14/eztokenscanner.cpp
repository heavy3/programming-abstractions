//
// eztokenscanner.cpp
//
// This file implements the EzTokenScanner class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 14
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This exercise extends the token scanner source in Figure 6-10 of the
// course reader to add a scanStrings() & saveToken() methods.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/25/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <cctype>
#include <string>
#include "eztokenscanner.h"
using namespace std;

EzTokenScanner::EzTokenScanner() {
    // Empty
}

EzTokenScanner::EzTokenScanner(string str) {
    setInput(str);
}

void EzTokenScanner::setInput(string str) {
    buffer = str;
    cp = 0;
    savedTokens.clear();
}

bool EzTokenScanner::hasMoreTokens() {
    if (ignoreWhitespaceFlag) {
        skipWhitespace();
    }
    return cp < buffer.length() || !savedTokens.isEmpty();
}

//
// Implementation notes: nextToken
// -------------------------------
// This method draws the next token from the savedTokens stack if it isn't
// empty.  Otherwise it begins by looking at the character indicated by the 
// index, cp.
//
// If that index is past the end of the string, nextToken returns the
// empty string.  If that character is alphanumeric, nextToken scans ahead
// until it finds the end of the word.
//
// If that character is a single or double quote and scanStrings is set,
// nextToken scans ahead for a matching closing quote as the end of the
// token.  If no closing quote is found, the one-character quote is returned.
//
// Otherwise, nextToken returns the character at the cp index as a
// one-character string.
//
// Limitations
// -----------
// May get confused by a mixture of contractions and singly quoted input
// text if scanStrings is asserted.
//
//    Example: Don't scan 'me'
//    Returns:    't scan ' 
//
//    as the quoted token instead of 'me'. :-/
//

string EzTokenScanner::nextToken() {
    if (savedTokens.size()) {
        return savedTokens.pop();
    }
    if (ignoreWhitespaceFlag) skipWhitespace();
    if (cp >= buffer.length()) {
        return "";
    } else if (isalnum(buffer[cp])) {
        int start = cp;
        while (cp < buffer.length() && isalnum(buffer[cp])) {
            cp++;
        }
        return buffer.substr(start, cp - start);
    } else if (scanStringsFlag && isquote(buffer[cp])) {
        char matchingQuote = buffer[cp];
        int pos = buffer.find(matchingQuote, cp + 1);
        if (pos != string::npos) {
            int start = cp;
            cp = pos + 1;
            return buffer.substr(start, cp - start);
        } else {
            return string(1, buffer[cp++]);
        }
    } else {
        return string(1, buffer[cp++]);
    }
}

void EzTokenScanner::saveToken(std::string token) {
    savedTokens.push(token);
}

//
// Implementation notes: scanStrings
// ---------------------------------
// Sets a flag that causes the private method, scanString, to return
// quoted text as a single token.
//

void EzTokenScanner::scanStrings() {
    scanStringsFlag = true;
}

//
// Implementation notes: ignoreWhitespace and skipWhitespace
// ---------------------------------------------------------
// This ignoreWhitespace method simply sets a flag.  The private method
// skipWhitespace is called only if that flag is true.
//

void EzTokenScanner::ignoreWhitespace() {
    ignoreWhitespaceFlag = true;
}

void EzTokenScanner::skipWhitespace() {
    while (cp < buffer.length() && isspace(buffer[cp])) {
        cp++;
    }
}

bool EzTokenScanner::isquote(char ch) {
    return ch == '\'' || ch == '"';
}
