//
// SimpleTextEditor.cpp
//
// This variation of the simple editor program from the text adds support for:
//
//    T command - types contents of buffer to screen
//    A command - places editor in append mode for multi-line insertions
// #FBD command - add repetition count for F, B, and D commands
//                17F would move the cursor forward 17 character positions
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program extends the code from Figure 13-3.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/29/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <cctype>
#include <iostream>
#include <sstream>
#include "buffer.h"
#include "simpio.h"
#include "tokenscanner.h"
#include "error.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 13.05\n";
const std::string DETAIL = "Simple Text Editor with [#]{F|B|D} enhancement";
const std::string BANNER = HEADER + DETAIL;
const std::string END_OF_APPEND_MARKER = ".";
const char NEWLINE = '\n';
const std::string VALID_COMMANDS = "AIDFJEHQ";

enum ParseResultT {NO_ERROR, BAD_COMMAND};

// Prototypes

void executeCommand(EditorBuffer & buffer, const int repetition, const char cmd,
                    const std::string & content);
void initScanner(TokenScanner & scanner);
void displayBuffer(EditorBuffer & buffer);
ParseResultT parseLine(const string & line, TokenScanner & scanner, int & repetition,
                  char & cmd, string & content);
void typeBuffer(EditorBuffer & buffer);
void printHelpText();
int stringToInteger(string str);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    EditorBuffer buffer;
    TokenScanner scanner;
    initScanner(scanner);
    
    char cmd;
    string content;
    int rep;
    
    while (true) {
        string line = getLine("*");
        ParseResultT result = parseLine(line, scanner, rep, cmd, content);
        switch (result) {
            case NO_ERROR:
                executeCommand(buffer, rep, cmd, content);
                break;
            case BAD_COMMAND:
                cerr << "Illegal command.  H for help menu." << endl;
                break;
        }
    }
    return 0;
}

// Function: initScanner
// Usage: initScanner(scanner);
// ----------------------------
// Configures a token scanner to recognize the editor's single-letter commands
// and possible numeric repetition factors that may preceed some commands.

void initScanner(TokenScanner & scanner) {
    scanner.scanNumbers();
}

// Function: executeCommand
// Usage: executeCommand(buffer, line);
// ------------------------------------
// Executes the specified command, updating the editor buffer accordingly.
// Some commands may also have a repetition factor (forward, backward, delete).
// The append and insert commands may also have associated text content.

void executeCommand(EditorBuffer & buffer, const int repetition, const char cmd,
                    const std::string & content) {
    switch (cmd) {
        case 'A': {
                // Gather multiple lines of text to insert until a '.' is
                // typed as the first character on a line by itself.
            
                string multiLine = "";
                if (content.length() > 0) {
                    multiLine += content + NEWLINE;
                }
                string nextLine;
                while ((nextLine = getLine()) != END_OF_APPEND_MARKER) {
                    multiLine += nextLine + NEWLINE;
                }
                if (multiLine.length() > 0) {
                    multiLine = multiLine.substr(0, multiLine.length() - 1);
                }
                buffer.insertString(multiLine);
            }
            break;
    
        case 'I':
            buffer.insertString(content);
            break;
            
        case 'D':
            for (int i = 0; i < repetition; i++) {
                buffer.deleteCharacter();
            }
            break;
            
        case 'F':
            for (int i = 0; i < repetition; i++) {
                buffer.moveCursorForward();
            }
            break;
            
        case 'B':
            for (int i = 0; i < repetition; i++) {
                buffer.moveCursorBackward();
            }
            break;
        
        case 'J': buffer.moveCursorToStart(); break;
            
        case 'E': buffer.moveCursorToEnd(); break;
            
        case 'H': printHelpText(); break;
            
        case 'T': typeBuffer(buffer);break;
            
        case 'Q': exit(0);
            
        default: cerr << "Illegal command" << endl; break;
    }
}

// Function: parseLine
// Usage: result = parseLine(input, scanner, repetition, cmd, content);
// -----------------------------------------------------------------------
// Parses a line of raw input into several pass-by-reference parameters
// including:
//
// * an optional repetition factor for commands such as
//   delete, backward, forward
//
// * a single-character command
//
// * a string of associated content (such as text to insert)
//
// In the event of a parse failure, BAD_COMMAND is returned.
                      
ParseResultT parseLine(const string & line, TokenScanner & scanner, int & repetition,
                  char & cmd, string & content) {
    ParseResultT result = NO_ERROR;
    
    cmd = NULL;
    content = "";
    
    scanner.setInput(line);
    repetition = 1;
    
    // Valid commands have the following forms:
    //
    // 1. consist of a single character
    //
    //    J         // Jump to the beginning of the buffer
    //    E         // Jump to the end of the buffer
    //    Q         // Quit the editor
    //    H         // Display help text.
    //
    // 2. consist of a single character prefixed by an optional repetition
    //    factor
    //
    //    10D       // Delete 10 characters following the cursor.
    //    2B        // move backward 2 characters.
    //    F         // move forward one character.
    //
    // 3. consist of a single character followed by content to be inserted
    //    into the editor buffer:
    //
    //    Iinsert this
    //    Afirst line of multi-line input
    
    if (scanner.hasMoreTokens()) {
        string token = scanner.nextToken();
        TokenType tokenType = scanner.getTokenType(token);
        switch (tokenType) {
            case WORD: {
                    cmd = toupper(token[0]);
                    content = line.substr(1,line.length() - 1);
                }
                break;
                
            case NUMBER: {
                    repetition = stringToInteger(token);
                    int numDigits = token.length();
                    int dummyRep;
                    if (line.length() > numDigits) {
                        result = parseLine(line.substr(numDigits), scanner,
                                          dummyRep, cmd, content);
                    } else {
                        result = BAD_COMMAND;
                    }
                }
                break;
                
            default:
                result = BAD_COMMAND;
                break;
        }
    }
    return result;
}
                      
// Function: displayBuffer
// Usage: displayBuffer(buffer);
// -----------------------------
// Displays the state of the buffer including the position of the cursor.

void displayBuffer(EditorBuffer & buffer) {
    string str = buffer.getText();
    for (int i = 0; i < str.length(); i++) {
        cout << " " << str[i];
    }
    cout << endl;
    cout << string(2 * buffer.getCursor(), ' ') << "^" << endl;
}

// Function: typeBuffer
// Usage: typeBuffer(buffer);
// --------------------------
// Displays contents of buffer without cursor position and double spacing
// seen with displayBuffer().

void typeBuffer(EditorBuffer & buffer) {
    string str = buffer.getText();
    cout << str << endl;
}

// Function: printHelpText
// Usage: printHelpText();
// -----------------------
// Displays a message showing the legal commands.

void printHelpText() {
    cout << "Editor commands:" << endl;
    cout << "  A     Append text until '" + END_OF_APPEND_MARKER;
    cout << "' appears on a line by itself." << endl;
    cout << "  Iabc  Inserts the characters abc at the cursor position" << endl;
    cout << "  F     Moves the cursor forward one character" << endl;
    cout << "  B     Moves the cursor backward one character" << endl;
    cout << "  D     Deletes the character after the cursor" << endl;
    cout << "  #|{F|B|D}  Perform the command # times" << endl;
    cout << "  J     Jumps to the beginning of the buffer" << endl;
    cout << "  E     Jumps to the end of the buffer" << endl;
    cout << "  H     Prints this message" << endl;
    cout << "  T     Types the contents of the buffer to the console" << endl;
    cout << "  Q     Exits from the editor program" << endl;
}
