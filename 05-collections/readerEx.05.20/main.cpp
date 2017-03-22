//
// main.cpp
//
// This program associates North American telphone area codes with
// geographic states, allowing bi-directional lookups of one given the other.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 20
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/18/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include "map.h"
#include "vector.h"
#include "simpio.h"
#include "strlib.h"
#include "tokenscanner.h"
#include "error.h"
using namespace std;

// Types and constants

const string LINE = string(60, '-') + '\n';
const string PROMPT = "Enter an area code or state name: ";
const string AREACODE_FILE = "AreaCodes.txt";
const int AREACODE_DIGITS = 3;

enum LookupResultT {
    MATCHED_STATE,
    MATCHED_AREACODE,
    UNRECOGNIZED_AREACODE,
    UNRECOGNIZED_STATE,
    NO_ASSOCIATED_AREACODE,
    NO_ASSOCIATED_STATE,
    NO_KEY,
    BAD_KEY
};

// Function prototypes

void banner();
string capitalize(string & inputStr);
void indexByAreaCode(Map<int,string> & areaCodes, string inputFile);
void indexByState(Map<string,Vector<int> > & states,
                  Map<int,string> & areaCodes);
void invertMap(Map<int,string> & acToState,
               Map<string,Vector<int> > & stateToAC);
bool isareacode(string str);
bool isstate(string str);
LookupResultT lookup(string key, Map<int,string> & areaCodes,
                     Map<string,Vector<int> > & states, string & matchingState,
                     Vector<int> & matchingAreaCodes);
void promptForLookups(string prompt, Map<int,string> & areaCodes,
                      Map<string,Vector<int> > & states);
void readAreaCodeFile(string inputFile, Map<int,string> & areaCodes);
void reportError(LookupResultT status, string key);
void reportMatch(string & matchingState);
void reportMatch(Vector<int> & matchingAreaCodes);

// Main program

int main(int argc, char * argv[]) {
    banner();
    
    Map<int,string> areaCodes;
    Map<string,Vector<int> > states;
    
    indexByAreaCode(areaCodes, AREACODE_FILE);
    indexByState(states, areaCodes);
    promptForLookups(PROMPT, areaCodes, states);

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
    cout << "This program lists the area codes for a given state"   << endl
         << "or identifies the state associated with an area code." << endl;
    cout << LINE;
}

//
// Function: indexByAreaCode
// Usage: indexByAreaCode(areaCodes, fileName);
// ---------------------------------------------
// Populates a mapping of area codes to states from a disk file.
// Area code serves as the primary key since they do not cross state
// boundaries.
//
// The input file has the following record structure:
//
//    areacode-state
//
// For example:
//
//    AreaCodes.txt
//   +------------------------------------
//   |201-New Jersey
//   |202-District of Columbia
//   |203-Connecticut
//   |...
//
// Error handling for bad input data is severe with no points for partial 
// credit.  An error message is reported to cerr and processing
// halts unceremoniously.
//

void indexByAreaCode(Map<int,string> & areaCodes, string fileName) {
    ifstream infile;
    TokenScanner scanner;
    
    infile.open(fileName.c_str());
    if (infile.fail()) error("Can't read the data file, " + fileName);
    
    scanner.ignoreComments();   // Allow C/C++ style comments in input file.
    scanner.addWordCharacters(" .");    // Don't delimit on spaces and dots,
                                        // common with two-word states and St.
    scanner.scanNumbers();              // Interpret the area code as a number
                                        // for better input validation.
    scanner.setInput(infile);           // Use scanner's built-in i/o on reads.
    
    while (scanner.hasMoreTokens()) {
        string acToken = scanner.nextToken();
        TokenType tokenType = scanner.getTokenType(acToken);
        if (tokenType != NUMBER) {
            error("Invalid data in input file: " + fileName +
                  "\nExpecting integer area code instead of: " + acToken);
        }
        
        string hyphenToken;
        if (scanner.hasMoreTokens()) {
            hyphenToken = scanner.nextToken();
            if (hyphenToken != "-") {
                error("Invalid data in input file: " + fileName +
                      "\nExpecting hyphen instead of: " + hyphenToken);
            }
        } else {
            error("Invalid data in input file: " + fileName +
                  "\nNo state associated with area code: " + acToken);
        }
        
        string stateToken;
        if (scanner.hasMoreTokens()) {
            stateToken = scanner.nextToken();
            tokenType = scanner.getTokenType(stateToken);
            if (tokenType != WORD) {
                error("Invalid data in input file: " + fileName +
                      "\nExpecting string for state instead of: " + stateToken);
            } else {
                /* Normalize case. */
                stateToken = capitalize(stateToken);
            }
        } else {
            error("Invalid data in input file: " + fileName +
                  "\nNo state associated with area code: " + acToken);
        }
        
        // Parsed a complete input record.  Add corresponding map entry.
        
        int areaCode = stringToInteger(acToken);
        areaCodes.put(areaCode, stateToken);
        
        string nlToken;
        if (scanner.hasMoreTokens()) {
            nlToken = scanner.nextToken();
            tokenType = scanner.getTokenType(nlToken);
            if (tokenType != SEPARATOR) {
                error("Invalid data in input file: " + fileName +
                      "\nExpecting newline instead of: " + nlToken);
            }
        }
    }
    infile.close();
}

//
// Function: indexByState
// Usage: indexByState(states, areaCodes);
// ---------------------------------------
// Populates a mapping of states to area codes with state serving as
// primary key to a vector of one or more associated area codes.
//

void indexByState(Map<string,Vector<int> > & states, Map<int,string> & areaCodes) {

    for (int ac: areaCodes) {
        string state = areaCodes[ac];
        Vector<int> acVector;
        if (states.containsKey(state)) {
            acVector = states.get(state);
        }
        acVector.add(ac);
        states.put(state, acVector);
    }
}

//
// Function: lookup
// Usage: LookupResultT status = lookup(key, areaCodes, states,
//                                      matchingState, matchingCodes, numMatch);
//        if (status == MATCHED_STATE) cout << matchingState << endl;
// -------------------------------------------------------------------------
// Returns an enumerated type reflecting the overall results of the lookup
// attempt.
//
// If the input key is recognized as a valid area code, the matchingState
// reference variable is updated accordingly.
//
// If the input key is a recognized state, the matchingCodes reference variable
// is updated accordingly with a vector of associated area codes for that state.
//
// Error codes are returned if the primary key is unrecognized or if the key
// is recognized, but no matching value is avaiable.
//
// This underscores the need to interpret the return value before displaying
// the contents of the matching reference variables.
//
// Some data validation is performed on the lookup key, though area codes
// are not exhaustively vetted.  A mix of numbers and letters for area code
// will result in a runtime error when converting the code to an integer.
//

LookupResultT lookup(string key, Map<int,string> & areaCodes,
                     Map<string,Vector<int> > & states,
                     string & matchingState, Vector<int> & matchingAreaCodes) {
    
    matchingState.erase();
    matchingAreaCodes.clear();
    
    string _key = trim(key); // Normalize key; delete leading/trailing spaces.
    
    if (_key.length() == 0) {
        return NO_KEY;
    }
    
    if (isnumber(_key[0])) {
        if (_key.length() == AREACODE_DIGITS) {
            // TODO: Still prone to runtime error if '30X' entered.
            int areaCode = stringToInteger(_key);
            if (areaCodes.containsKey(areaCode)) {
                matchingState = areaCodes[areaCode];
                if (matchingState.length()) {
                    return MATCHED_STATE;
                } else {
                    return NO_ASSOCIATED_STATE;
                }
            }
        } else {
            return BAD_KEY; // Incorrect number of digits in area code.
        }
    }
    
    if (isalpha(_key[0])) {
        string State = capitalize(_key);    // Normalize case so lookups are
                                            // case insensitive.
        if (states.containsKey(State)) {
            matchingAreaCodes = states[State];
            if (matchingAreaCodes.size()) {
                return MATCHED_AREACODE;
            } else {
                return NO_ASSOCIATED_AREACODE;
            }
        }
    }
    
    return BAD_KEY;
}

//
// Function: promptForLookups
// Usage: promptForLookups(prompt, areaCodes, states);
// ---------------------------------------------------
// Prompts user to enter an area code or North American state as a key for
// corresponding lookup of associated state or area codes, respectively.
//
// Results are reported to the console (or cerr in the event of a lookup error).
//
// Returns when the user enters a blank.
//

void promptForLookups(string prompt, Map<int,string> & areaCodes,
                      Map<string,Vector<int> > & states) {
    
    while (true) {
        string key = trim(getLine(prompt));
        if (key == "") break;
        
        string matchingState;
        Vector<int> matchingAreaCodes;
        
        LookupResultT status = lookup(key, areaCodes, states,
                                      matchingState, matchingAreaCodes);

        switch (status) {
            case MATCHED_STATE:
                reportMatch(matchingState);
                break;
                
            case MATCHED_AREACODE:
                reportMatch(matchingAreaCodes);
                break;
                
            default:
                reportError(status, key);
                break;
        }
    }
}

//
// Function: reportError
// Usage:    reportError(errorCode, failedKey);
// ----------------------------------------------
// Reports and ellaborated error message to cerr for various 
// lookup failure conditions.
//

void reportError(LookupResultT lookupError, string key) {
    switch (lookupError) {
        case UNRECOGNIZED_AREACODE:
            cerr << "Error: Unrecognized area code (" + key + ")" << endl;
            break;
            
        case UNRECOGNIZED_STATE:
            cerr << "Error: Unrecognized state (" + key + ")" << endl;
            break;
            
        case NO_ASSOCIATED_AREACODE:
            cerr << "Error: Missing area code for " + key << endl;
            break;
            
        case NO_ASSOCIATED_STATE:
            cerr << "Error: Missing state for area code (" + key + ")" << endl;
            break;
            
        case NO_KEY:
            cerr << "Error: No key or empty key." << endl;
            break;
            
        case BAD_KEY:
            cerr << "Error: Bad key." <<  endl
                 << "** Expecting North American area code or state." << endl;
            break;
            
        default:
            cerr << "Unrecognized error (" << lookupError << ")" << endl;
            break;
    }
    cerr << endl;
}

//
// Function: reportMatch
// Usage:    reportMatch(matchingState);
//           reportMatch(matchingAreaCodes);
// -----------------------------------------
// Reports to the console the results of a successful lookup against
// the mapping of area codes to states, or state to area codes.
//
// A matching state is passed in as a string reference variable.
// A matching set of area codes is passed in as a Vector of integers.
//

void reportMatch(string & matchingState) {
    cout << matchingState << endl << endl;
}

void reportMatch(Vector<int> & matchingAreaCodes) {
    for (int areaCode: matchingAreaCodes) {
        cout << areaCode << endl;
    }
    int nMatches = matchingAreaCodes.size();
    if (nMatches > 1) {
        cout << setw(3) << nMatches << " area codes found." << endl << endl;
    }
}

//
// Function: capitalize
// Usage: capitalized = capitalize(inputStr);
// ------------------------------------------
// Returns a string with each word from the input string capitalized.
//

string capitalize(string & str) {
    string result="";
    TokenScanner ts;
    
    ts.setInput(str);
    while (ts.hasMoreTokens()) {
        string token = ts.nextToken();
        token[0] = toupper(token[0]);
        result += token;
    }
    
    return result;
}
