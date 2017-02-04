//
// eztokenscanner.cpp
//
// This file implements the EzTokenScanner class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 15
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This exercise extends the token scanner source in Figure 6-10 of the
// course reader to add scanNumbers(), scanStrings(), and saveToken() methods.
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

std::string EzTokenScanner::nextToken() {
    if (savedTokens.size()) {
        return savedTokens.pop();
    }
    
    if (ignoreWhitespaceFlag) skipWhitespace();
    if (cp >= buffer.length()) return "";
    
    int start = cp;
    if (isalnum(buffer[cp])) {
        if (scanNumbersFlag) {
            std::string n = scanNumber();
            if (!n.empty()) {
                return n;
            }
        }
        while (cp < buffer.length() && isalnum(buffer[cp])) {
            cp++;
        }
        return buffer.substr(start, cp - start);
    } else if (scanStringsFlag && isquote(buffer[cp])) {
        char matchingQuote = buffer[cp];
        int pos = buffer.find(matchingQuote, cp + 1);
        if (pos != string::npos) {
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
// Sets a flag that causes quoted text to be scanned as a single token.
//

void EzTokenScanner::scanStrings() {
    scanStringsFlag = true;
}

//
// Implementation notes: scanNumbers
// ---------------------------------
// Sets a flag that causes the private method, scanNumber, to return
// numbers as a single token
//

void EzTokenScanner::scanNumbers() {
    scanNumbersFlag = true;
}

//
// Implementation notes: scanNumber
// --------------------------------
// This method recognizes a sequence of characters as a number according to
// the following finite state machine:
//                                                   digit     digit
//              +--+         +--+          +--------------+    +--+
//              v  |digit    v  |digit     |              v    v  |
// s0-------->[s1]-+------>[s2]-+-------->s3------>s4------->[s5]-+
//    digit     |     .             E     ^   +,-     digit
//              +-------------------------+
//                    E
//
// Courtesy: Eric Roberts
//           Programming Abstractions in C++, Fig 6-15, 2012
//
// States delimited by square brakets (e.g., [s#]) are terminal
// states indicating a number has been detected.
//
// Strangely, this FSM parses the following into two tokens
// (if scanNumbers is asserted):
//
//    "... 3.14159a" --> [3.14159] [a]
//
// However,
//
//    "... 3.14159e" decomposes to --> [3] [.] [14159] [e]
//
// If and when I actually need this code, i'll tweak it to my needs.
//

enum IsNumFSM_T {
    s0,
    s1,
    s2,
    s3,
    s4,
    s5
};

std::string EzTokenScanner::scanNumber() {
    
    const std::string emptyString = "";
    int start = cp;
    
    IsNumFSM_T state = s0;
    while (cp < buffer.length()) {
        switch (state) {
            case s0: // Start of number?
                if (isnumber(buffer[cp])) {
                    state = s1;
                } else {
                    // Return immediately with null string.
                    //
                    //    Do /not/ advance cp since still needs to be
                    //    characterized as something other than a string.
                    return emptyString;
                }
                break;
                
            case s1: // Integers
                if (isnumber(buffer[cp])) {
                    state = s1;
                } else if (buffer[cp] == '.') {
                    state = s2;
                } else if (toupper(buffer[cp]) == 'E') {
                    state = s3;
                } else {
                    return buffer.substr(start, cp - start);
                }
                break;
                
            case s2: // Reals
                if (isnumber(buffer[cp])) {
                    state = s2;
                } else if (toupper(buffer[cp]) == 'E') {
                    state = s3;
                } else {
                    return buffer.substr(start, cp - start);
                }
                break;
                
            case s3: // Integer or Real with Exponent
                if (isnumber(buffer[cp])) {
                    state = s5;
                } else if (buffer[cp] == '+' || buffer[cp] == '-') {
                    state = s4;
                } else {
                    // Rollback  and return since this doesn't match a terminal
                    // state for detecting a number.
                    cp = start;
                    return emptyString;
                }
                break;
                
            case s4: // Signed exponent: ## E (+,-)
                if (isnumber(buffer[cp])) {
                    state = s5;
                } else {
                    // Rollback  and return since this doesn't match a terminal
                    // state for detecting a number.
                    cp = start;
                    return emptyString;
                }
                break;
                
            case s5: // Exponent value.
                if (isnumber(buffer[cp])) {
                    state = s5;
                } else {
                    return buffer.substr(start, cp - start);
                }
                break;
                
            default:
                break;
        }
        cp++;
    }
    
    switch (state) {
            
            // If we run out of buffer to parse while in an indeterminate
            // number-recognition state, rollback and return empty string.
            
        case s0:
        case s3:
        case s4: {
                cp = start;
                return emptyString;
            }
            break;
            
        default:
            
            // Otherwise return the entire substring from start to the end
            // of the input buffer.

            return buffer.substr(start, cp - start);
            break;
    }
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