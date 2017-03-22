//
// eztokenscanner.h
//
// This file exports an interface for tokenizing input from an input buffer
// of an input stream.
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

#ifndef eztokenscanner_h
#define eztokenscanner_h

#include <string>
#include <istream>
#include <iostream>

//
// Enumeration for identifying which input source is active.
//

enum InputSource {
    INPUT_BUFFER,
    INPUT_STREAM
};

//
// Class: EzTokenScanner
// ---------------------
// This class is used to represent a single instance of a scanner.
// In this simplified version of the class, tokens come in two forms.
//
// 1. Strings of consecutive letters and digits representing words.
// 2. One-character strings representing punctuation or separators.
//
// The use of the EzTokenScanner class is illustrated by the following code
// pattern, which reads the tokens in the string variable input:
//
//      EzTokenScanner scanner;
//      scanner.setInput(input);
//      while (scanner.hasMoreTokens()) {
//          string token = scanner.nextToken();
//          . . . process the token . . .
//      }
//
//  This version of the EzTokenScanner class includes the ignoreWhitespace
//  method.
//

class EzTokenScanner {
public:
    
//
// Constructor: EzTokenScanner
// Usage: EzTokenScanner scanner;
// Usage: EzTokenScanner scanner(str);
// ---------------------------------
// Initializes a scanner object.  The initial token stream comes from the
// string str, if it is specified.  The default constructor creates a scanner
// with an empty token stream.
//
    
    EzTokenScanner();
    EzTokenScanner(std::string str);
    EzTokenScanner(std::istream & infile);
    
//
// Method: setInput
// Usage: scanner.setInput(str);
// -----------------------------
// Sets the input for this scanner to the specified string.  Any previous
// input string is discarded.
//
    
    void setInput(std::string str);
    void setInput(std::istream & infile);
    
//
// Method: hasMoreTokens
// Usage: if (scanner.hasMoreTokens()) ...
// ---------------------------------------
// Returns true if there are additional tokens for this scanner to read.
//
    
    bool hasMoreTokens();
    
//
// Method: nextToken
// Usage: token = scanner.nextToken();
// -----------------------------------
// Returns the next token from this scanner.  If called when no tokens are
// available, nextToken returns the empty string.
//
    
    std::string nextToken();
    
//
// Method: ignoreWhitespace
// Usage: scanner.ignoreWhitespace();
// ----------------------------------
// Tells the scanner to ignore whitespace characters.  By default, the
// nextToken method treats whitespace characters (typically spaces and tabs)
// just like any other punctuation mark and returns them as single-character
// tokens.  Calling
//
//      scanner.ignoreWhiteSpace();
//
// changes this behavior so that the scanner ignores whitespace characters.
//
    
    void ignoreWhitespace();
    
private:

// Instance variables

    std::string buffer;         // The input string containing the tokens.
    int cp;                     // The current position in the buffer.
    bool ignoreWhitespaceFlag;  // Flag set by a call to ignoreWhitespace.
    std::istream *pinfile;      // Input stream containing tokens.
    InputSource inputSource;    // Specifies input from buffer or stream.
    
// Private methods
    
    void skipWhitespace();
    
};

#endif // eztokenscanner_h
