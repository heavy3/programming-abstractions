//
// main.cpp
//
// This program strips C & C++ style comments from an input stream.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 4, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/18/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include <fstream>
#include "error.h"

using namespace std;

// Constants and Types

const string HEADER = "CS106B Programming Abstractions in C++: Ex 4.08\n";
const string DETAIL = "Strip C/C++ comments from an input stream";
const string BANNER = HEADER + DETAIL;
const string PROMPT = "Input file: ";
const string E_NO_FILE = "File not found.  Please try again.";

// These are the states of the finite state machine for
// identifying C and C++ style comments in a stream of text.

enum CommentFsmT {SEEK_START, VERIFY_START, SEEK_EOL, SEEK_STAR, SEEK_CEND};
enum TriStateT {NO, YES, MAYBE};

// Function prototypes

string promptForFile(ifstream & stream, const string prompt);
TriStateT isOutsideComment(const char ch, CommentFsmT &state);
void removeComments(istream & is, ostream & os);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    ifstream is;
    string fname = promptForFile(is, PROMPT);
    removeComments(is, cout);
    return 0;
}

// Function definitions

// Function: removeComments
// Usage: removeComments(inputStream, outputStream);
// -------------------------------------------------
// This function strips out the characters associated with C or C++
// style comments encountered on the input stream.
//
// So anything from a '//' to the end of the line is not pushed to the output
// stream.  Likewise anything between /* .. */, possibly spanning multiple
// lines will also not be passed to the output stream.
//
// Limitation: It does not handle the case where comment delimiters are
//             parts of a string and should be treated as string text.

void removeComments(istream & is, ostream & os) {
    CommentFsmT state = SEEK_START;	// seeking comment start ('/*' or '//')
    
    int ch, ch2;
    while ((ch = is.get()) != EOF) {
        switch(isOutsideComment(ch, state)) {
            case YES: {
                os << char(ch);
            } break;
                
                // Encountered 1st / and we don't know yet if we'll get // or /*
                // or something else. So read another character from the stream.
                
            case MAYBE: {
                if ((ch2 = is.get()) == EOF) {
                    
                    // We're already in the comment 'blackout' zone.
                    // So just packup.  Don't echo the buffered ch.
                    if (state == SEEK_CEND ||
                        state == SEEK_STAR ||
                        state == SEEK_EOL) break;
                    
                    // It was looking like we had the start of a comment
                    // sequence but we hit the end of the file before that
                    // could be established. So push out the previously read
                    // character.
                    os << char(ch);
                    
                    is.unget(); // Let the parent 'while' catch this
                                // and bailout.
                } else {
                    switch(isOutsideComment(ch2, state)) {
                        case YES: {     // Ah, so maybe that '/' was a division
                                        // symbol and not the start of a
                                        // comment.
                            os << char(ch) << char(ch2);
                        } break;
                            
                        case MAYBE: {
                            cerr << "Uh, oh.  Shouldn't get here." << endl;
                        } break;
                            
                        default:
                            continue;
                    }
                }
            } break;
                
            default:
                continue;
        }
    }
}

// Function: isOutsideComment
// Usage: TriStateT triState = isOutsideComment(ch, state);
// --------------------------------------------------------
// Returns enumerated 'YES', 'NO' or 'MAYBE' if the input character
// lies within a comment section based upon the current state of the
// finite state machine.
//
// This can be use by a client to decide if that character should
// be passed through to an output stream, for example.

TriStateT isOutsideComment(const char ch, CommentFsmT & state) {
    TriStateT result = MAYBE;

    switch(state) {
        case SEEK_START: {      // No indication, yet that we hit the start of a 
                                // comment sequence.
            switch(ch) {
                case '/': {
                    state = VERIFY_START;
                    result = MAYBE; // We'll need to read another input char to
                                    // know for sure if we're inside or outside
                                    // a comment section.
                } break;

                default: {
                    result = YES; 
                } break;
            }
        } break;

        case VERIFY_START: {    // We already encountered an initial '/'.  Now
                                // we verify if it is, indeed, the start of a 
                                // comment sequence.
            switch(ch) {
                case '/': {     // Hit '//' so we're in a comment blackout
                                // zone until the end-of-line so don't echo out 
                                // chars from the input stream.
                    state = SEEK_EOL;
                    result = NO;
                } break;

                case '*': {     // Hit '/*' so we're in comment zone until '*/'
                    state = SEEK_STAR;
                    result = NO;
                } break;

                default: {      // False alarm.  Just hit single starting '/' and
                                // then some non-comment char.  Reset the state.
                    state = SEEK_START;
                    result = YES;
                } break;
            }
        } break;

        case SEEK_EOL: {        // We're in a C++ comment zone that ends when we
                                // detect and end-of-line character.
            switch(ch) {
                case '\n': {
                    state = SEEK_START;
                    result = YES;
                } break;

                default: {
                    result = NO;
                } break;
            }

        } break;

        case SEEK_STAR: {       // We're into a C-style comment zone.  We're
                                // looking for a '*/' sequence to end it.
            switch(ch) {
                case '*': {
                    state = SEEK_CEND;
                    result = NO;
                } break;

                default: {
                    result = NO;
                } break;
            }
        } break;

        case SEEK_CEND: {       // We're into a C-style comment zone.  We've
                                // found a '*' and we're looking for 
                                // subsequent '/' to make a '*/'.
            switch(ch) {
                case '/': {
                    state = SEEK_START;
                    result = NO;
                } break;

                default: {
                    state = SEEK_STAR;
                    result = NO;
                } break;
            }
        } break;

        default: {
            cerr << "Illegal state: " << state << endl;
        } break;
    }
    return result;
}

// Function: promptForFile
// Usage: string filename = promptForFile(inputFstream, "Input file: ");
// ---------------------------------------------------------------------
// Prompts the user for a filename to open and associate with a pass-by-
// reference input file stream.  Upon success, returns the name of the
// file.  Otherwise spins in a loop until a valid filename is provided
// and the stream is attached to that file.

string promptForFile(ifstream & stream, const string prompt) {
    while (true) {
        string fname;
        cout << prompt;
        getline(cin, fname);
        stream.open(fname.c_str());
        if (!stream.fail()) return fname;
        stream.clear();
        cerr << E_NO_FILE << endl;
    }
}

