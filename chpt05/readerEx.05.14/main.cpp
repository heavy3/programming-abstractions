//
// main.cpp
//
// This program implements the function
//
//    ParseResultT checkBracketOperators(string str, int & errPos);
//
// which parses code-like strings for syntax errors relating to
// bracket balance and nesting.
//
// For example:
//
//    This program checks that bracketing operators such as (), {}, []
//    in a string are properly matched.
//    --------------------------------------------------
//    valid: { s = 2 * (a[2] + 3); x = (1 + (2)); }
//
//    missing_close: (([])
//                        ^
//    improper_order: )(
//                    ^
//    improper_nesting: {(})
//                       ^
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 14
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/05/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "set.h"
#include "stack.h"
#include "map.h"
using namespace std;

// Types

enum ParseResultT {
    valid,
    missing_close,
    improper_order,
    improper_nesting,
    pending
};

// Function prototypes

void banner();
Set<char> stringToSet(string str);
ParseResultT checkBracketOperators(string str, int & errPos);
void reportResult(ParseResultT result, string parsed, int errPos);
void reportError(string resultStr, string parsed, int errPos = string::npos);
char getMatchingBracket(char bracket);
bool testCheckBracketOperators(string str, ParseResultT expectedResult);

// Constants

const string LINE = string(50, '-') + '\n';

// Define bracket pairs to validate.
//
//   !! Keep matching pairs of brackets in positional correspondence.
//

const string OPEN_BRACKETS  = "([{<";
const string CLOSE_BRACKETS = ")]}>";

Set<char> OpenBracketSet = stringToSet(OPEN_BRACKETS);
Set<char> CloseBracketSet = stringToSet(CLOSE_BRACKETS);
Set<char> BracketSet = OpenBracketSet + CloseBracketSet;

// Main program

int main(int argc, char * argv[]) {
    banner();
    cout << LINE;
    
    string parseMe = "{ s = 2 * (a[2] + 3); x = (1 + (2)); }";
    testCheckBracketOperators(parseMe, valid);

    parseMe = "(([])";
    testCheckBracketOperators(parseMe, missing_close);
    
    parseMe = ")(";
    testCheckBracketOperators(parseMe, improper_order);
    
    parseMe = "{(})";
    testCheckBracketOperators(parseMe, improper_nesting);
    
    return 0;
}

// Function definitions

//
// Function: banner
// Usage: banner(upperBound)
// -------------------------
// Displays the purpose of the program on the console.
//

void banner() {
    cout << "This program checks that bracketing operators such as (), {}, [] "
         << "in a string are properly matched."
         << endl;
}

//
// Function: testCheckBracketOperators
// Usage: testCheckBracketOperators("(um(this is[ok]until)this"; missing_close);
// -----------------------------------------------------------------------------
// Returns true if the actual validation result matches the expected result.
// Reports results to the console as well.
//

bool testCheckBracketOperators(string str, ParseResultT expectedResult) {
    int errPos;

    ParseResultT result = checkBracketOperators(str, errPos);
    reportResult(result, str, errPos);
    
    return (result == expectedResult);
}

//
// Function: checkBracketOperators
// Usage: ParseResultT result = checkBracketOperators("(my {string})", errPos);
// ----------------------------------------------------------------------------
// Returns an enumerated type reflecting the outcome of checking an input string
// for matching brackets (e.g., {}, (), []).
//
// If validation fails, a pass-by-reference position variable is updated
// with an index to the first problematic area of the input string.
//

ParseResultT checkBracketOperators(string str, int & errPos) {

    Stack<char> openBrackets;     // Push open brackets here if encountered.
    Map<string,int> openCount;    // Count of each bracket type on stack.
    
    ParseResultT result = pending;
    errPos = string::npos;        // String position index to flag errors.
    
    for (int i = 0; i < str.length(); i++) {
        if (BracketSet.contains(str[i])) {
           
            //
            // Push open brackets on the stack.
            // Pop them off when matching closed bracket found. 
            // Ignore all else.
            //
            // Stack should be empty after all bracket pairs have been matched.
            //
            
            if (OpenBracketSet.contains(str[i])) {
                
                // Hit an open bracket. Push it on the stack.
                char openB = str[i];
                openBrackets.push(openB);
                
                
                // Keep count to identify nesting issues versus order issues.
                string key = string(1, openB);
                openCount[key]++;
                
            } else {
                // Parsing a closing bracket.
                
                char closedB = str[i];
                if (openBrackets.isEmpty()) {
                    // 
                    // ERROR: Found closing bracket before open bracket.
                    //        Ex: ")("
                    //
                    result = improper_order;
                    errPos = i;
                    break;
                }
                
                char validOpenB = getMatchingBracket(closedB);
                string validBkey = string(1, validOpenB);
                
                // Check top of stack for expected matching open bracket.
                
                if (openBrackets.peek() != validOpenB) {
                    
                    // 
                    // Are there /any/ matching open brackets burried in
                    // the depths of the stack?  If so, implies nesting issue.
                    //
                    
                    if (openCount.containsKey(validBkey)) {
                        if (openCount[validBkey] > 0) {
                            // 
                            // ERROR: Found matching open bracket but burried 
                            //        in the stack apparently.
                            //        Ex: "{(})"
                            //
                            result = improper_nesting;
                            errPos = i;
                            break;
                        }
                    }
                    
                    // 
                    // ERROR: Closing bracket before open bracket.
                    //        Ex: ")("
                    //
                    result = improper_order;
                    errPos = i;
                    break;
                }
                
                // VALID :-)
                //
                // The closing bracket matches an open bracket on top of the 
                // stack.  Pop it off since validation is done for this pair.
                //
                
                openBrackets.pop();
                if (openCount.containsKey(validBkey)) {
                    openCount[validBkey]--;
                }
            }
        }
    }
    
    if (result == pending) {
        if (openBrackets.isEmpty()) {
            result = valid;
        } else {
            // 
            // ERROR: Non-empty stack implies no matching closing bracket.
            //        Ex: "(([])"
            //
            result = missing_close;
            errPos = str.length();
        }
    }
    
    return result;
}

//
// Function: getMatchingBracket
// Usage: char invBracket = getMatchingBracket('{'); // returns '}'
// -----------------------------------------------------------
// Returns the inverse bracket character of the passed-in character.
//
// Applies for all bracket characters specified in the LBracketSet or 
// RBracketSet.
//
// If a passed-in character does not belong to bracket sets, the character
// is returned unchanged.
//

char getMatchingBracket(char bracket) {
    char invBracket;
    int index;
    
    if (OpenBracketSet.contains(bracket)) {
        index = OPEN_BRACKETS.find(bracket);
        invBracket = CLOSE_BRACKETS[index];
    } else if (CloseBracketSet.contains(bracket)) {
        index = CLOSE_BRACKETS.find(bracket);
        invBracket = OPEN_BRACKETS[index];
    } else {
        invBracket = bracket;
    }
    return invBracket;
}

//
// Function: reportResult
// Usage: reportResult(result, parsedString, errPos);
// --------------------------------------------------
// Reports the results of parsing a string for valid matching bracket operators
// to the console.  In the event of a parsing error.  A carrot (^) symbol is
// displayed under the problematic location in the parsed string.
//

void reportResult(ParseResultT result, string parsed, int errPos) {
    switch (result) {
        case valid:
            reportError("valid: ", parsed);
            break;
            
        case missing_close:
            reportError("missing_close: ", parsed, errPos);
            break;
            
        case improper_order:
            reportError("improper_order: ", parsed, errPos);
            break;

        case improper_nesting:
            reportError("improper_nesting: ", parsed, errPos);
            break;

        case pending:
            reportError("pending: ", parsed, errPos);
            break;

        default:
            reportError("unknown_error: ", parsed, errPos);
            break;
    }
}

//
// Function: reportError
// Usage: reportError("missing_close: ", parsed, errPos);
// ------------------------------------------------------
// Formats low-level parsing result and displays to console.
//

void reportError(string resultStr, string parsed, int errPos) {
    cout << resultStr << parsed << endl;
    if (errPos != string::npos) {
        int padCount = (errPos == 0) ? 0 : errPos - 1;
        string padStr = string(resultStr.length() + padCount, ' ');
        cout << padStr << "^" << endl;
    } else {
        cout << endl;
    }
}

//
// Function: stringToSet
// Usage: Set<char> set = stringToSet("abcd");
// -------------------------------------------
// Returns a set of characters initialized from a string of characters.
//

Set<char> stringToSet(string str) {
    Set<char> chSet;
    for (char ch: str) {
        chSet.add(ch);
    }
    return chSet;
}
