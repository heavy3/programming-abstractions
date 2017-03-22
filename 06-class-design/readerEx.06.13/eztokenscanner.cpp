//
// eztokenscanner.cpp
//
// This file implements the EzTokenScanner class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 13
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file extends the token scanner source in Figure 6-10 of the
// course reader to add a saveToken() method.
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
// until it finds the end of the word; if not, nextToken returns that
// character as a one-character string.
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
    } else {
        return string(1, buffer[cp++]);
    }
}

void EzTokenScanner::saveToken(std::string token) {
    savedTokens.push(token);
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
