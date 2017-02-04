//
// eztokenscanner.h
//
// This interface exports a class for tokenizing input.  I've extended it
// to include these methods:
//
//      scanNumbers()   returns a number as a single token.
//      scanStrings()   returns quoted text as a single token
//      saveToken()     effectively pushes a scanned token back into
//                      the input buffer
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

#ifndef eztokenscanner_h
#define eztokenscanner_h

#include <string>
#include "Stack.h"

//
// Class: EzTokenScanner
// -------------------
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
    
//
// Method: setInput
// Usage: scanner.setInput(str);
// -----------------------------
// Sets the input for this scanner to the specified string.  Any previous
// input string is discarded.
//
    
    void setInput(std::string str);
    
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

//
// Method: saveToken
// Usage: scanner.saveToken(token);
// --------------------------------
// Pushes the specified token back into this scanner's input stream.
// On the next call to nextToken, the scanner will return
// the saved token without reading any additional characters from the
// token stream.
//

    void saveToken(std::string token);

//
// Method: scanNumbers
// Usage: scanner.scanNumbers();
// -----------------------------
// Controls how the scanner treats tokens that begin with a digit.  By
// default, the <code>nextToken</code> method treats numbers and letters
// identically and therefore does not provide any special processing for
// numbers.  Calling
//
// <pre>
//    scanner.scanNumbers();
// </pre>
//
// changes this behavior so that <code>nextToken</code> returns the
// longest substring that can be interpreted as a real number.
//

    void scanNumbers();
    
//
// Method: scanStrings
// Usage: scanner.scanStrings();
// -----------------------------
// Controls how the scanner treats tokens enclosed in quotation marks.  By
// default, quotation marks (either single or double) are treated just like
// any other punctuation character.  Calling
//
// <pre>
//    scanner.scanStrings();
// </pre>
//
// changes this assumption so that <code>nextToken</code> returns a single
// token consisting of all characters through the matching quotation mark.
// The quotation marks are returned as part of the scanned token so that
// clients can differentiate strings from other token types.
//
    
    void scanStrings();
    
private:

// Instance variables

    std::string buffer;         // The input string containing the tokens.
    int cp;                     // The current position in the buffer.
    bool ignoreWhitespaceFlag;  // Flag set by a call to ignoreWhitespace 
    bool scanNumbersFlag;       // Scanner parses numbers
    bool scanStringsFlag;       // Flag set by a call to scanStrings
    Stack<std::string> savedTokens; // Tokens pushed here by saveToken().
    
// Private methods
    
    void skipWhitespace();
    std::string scanNumber();
    bool isquote(char ch);
    
};

#endif // eztokenscanner_h
