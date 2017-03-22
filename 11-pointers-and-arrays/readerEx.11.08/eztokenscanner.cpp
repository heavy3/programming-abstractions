//
//  eztokenscanner.cpp
//
//  This file implements the EzTokenScanner class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 11, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code is largely derived from Figure 6-10.  It has been extended
// to support input from file streams.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 6/19/16.
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

EzTokenScanner::EzTokenScanner(istream & infile) {
    setInput(infile);
}

void EzTokenScanner::setInput(string str) {
    buffer = str;
    cp = 0;
    inputSource = INPUT_BUFFER;
}

void EzTokenScanner::setInput(istream & infile) {
    pinfile = &infile;
    inputSource = INPUT_STREAM;
}

bool EzTokenScanner::hasMoreTokens() {
    if (ignoreWhitespaceFlag) {
        skipWhitespace();
    }
    
    if (inputSource == INPUT_BUFFER) {
        return cp < buffer.length();
    } else if (inputSource == INPUT_STREAM) {
        if (pinfile->fail()) {
            if (pinfile->eof()) {
                return false;
            }
            pinfile->clear();
            return true;
        }
        return true;
    } else {
        return false;
    }
}

//
// Implementation notes: nextToken
// -------------------------------
// Input is pulled from a string buffer or from a file stream depending
// upon the scanner configuration.
//
// The method starts by looking at single characters either at the current
// string buffer index, cp, or at the current position within the stream.
// If the index is out of range relative to the buffer or at the end of file
// within the stream, nextToken returns an empty string.
//
// Otherwise, if the current character is alphanumeric, nextToken scans ahead
// until it finds the end of the word and returning that word as the token; 
// if not, nextToken returns that character as a one-character token string.
//

string EzTokenScanner::nextToken() {
    string token = "";
    if (ignoreWhitespaceFlag) skipWhitespace();
    if (inputSource == INPUT_BUFFER) {
        if (cp >= buffer.length()) {
            return "";
        } else {
            if (isalnum(buffer[cp])) {
                int start = cp;
                while (cp < buffer.length() && isalnum(buffer[cp])) {
                    cp++;
                }
                return buffer.substr(start, cp - start);
            } else {
                return string(1, buffer[cp++]);
            }
        }
    } else if (inputSource == INPUT_STREAM) {
        if (!pinfile->fail()) {
            char ch;
            while (pinfile->get(ch) && isalnum(ch)) {
                token += string(1, ch);
            }
            if (token.size() > 0) {
                pinfile->unget();   // Unget the non-alnum ch.
                return token;
            } else {
                if (ignoreWhitespaceFlag && isspace(ch)) {
                    return "";
                } else {
                    token = string(1, ch);
                    return token;
                }
            }
        }
    }
    return token;
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
    if (inputSource == INPUT_BUFFER) {
        while (cp < buffer.length() && isspace(buffer[cp])) {
            cp++;
        }
    } else if (inputSource == INPUT_STREAM) {
        char ch;
        while (pinfile->get(ch)) {
            if (isspace(ch)) {
                continue;
            }
            pinfile->unget(); // Return the non-space char back to stream.
            break;
        }
    }
}
