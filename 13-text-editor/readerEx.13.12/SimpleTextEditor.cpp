//
// SimpleTextEditor.cpp
//
// This variation of the simple editor program from the text adds support for:
//
//    T command - types contents of buffer to screen
//    A command - places editor in append mode for multi-line insertions
// #FBD command - add repetition count for F, B, and D commands
//                17F would move the cursor forward 17 character positions
// WFBD command - add word motion to F, B, and D commands
//    C command - copy chararacters to buffer
//    P command - paste chararacters from buffer
//    X command - cut chararacters from buffer
//    Stext     - searches for text in buffer; repositions cursor if found
//    Rthis/that- replaces 'this' text with 'that' text
//
// The underlying buffer implementation uses a doubly-linked list.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program extends the code from Figure 13-3.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/5/16.
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

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 13.12\n";
const std::string DETAIL = "Doubly-linked, list-based, editor. H for help."; 
const std::string BANNER = HEADER + DETAIL;

const bool DEBUGGING = true;    // Displays contents of buffer after each cmd.
const std::string END_OF_APPEND_MARKER = ".";
const char NEWLINE = '\n';

enum ParseResultT {NO_ERROR, BAD_COMMAND};
enum ScaleT {CHAR_SCALE, WORD_SCALE};

// Prototypes

void displayBuffer(EditorBuffer & buffer);
void executeCommand(EditorBuffer & buffer, const int repetition,
                    const ScaleT scale, const char cmd,
                    const std::string & content);
void initScanner(TokenScanner & scanner);
ParseResultT parseLine(const string & line, TokenScanner & scanner, int & repetition,
                  ScaleT & scale, char & cmd, string & content);
void printHelpText();
int stringToInteger(string str);
void typeBuffer(EditorBuffer & buffer);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    EditorBuffer buffer;
    TokenScanner scanner;
    initScanner(scanner);
    
    char cmd;
    string content;
    int rep;
    ScaleT scale;
    
    while (true) {
        string line = getLine("*");
        ParseResultT result = parseLine(line, scanner, rep, scale, cmd,
                                        content);
        switch (result) {
            case NO_ERROR:
                executeCommand(buffer, rep, scale, cmd, content);
                break;
                
            case BAD_COMMAND:
                cerr << "Illegal command.  H for help menu." << endl;
                break;

            default:
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

void executeCommand(EditorBuffer & buffer, const int repetition,
                    const ScaleT scale, const char cmd,
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
                if (DEBUGGING) displayBuffer(buffer);
            }
            break;
            
        case 'B':
            for (int i = 0; i < repetition; i++) {
                if (scale == WORD_SCALE) {
                    buffer.moveCursorBackwardWord();
                } else {
                    buffer.moveCursorBackward();
                }
            }
            if (DEBUGGING) displayBuffer(buffer);
            break;
            
        case 'C':
            if (scale == WORD_SCALE) {
                buffer.copyWords(repetition);
            } else {
                buffer.copy(repetition);
            }
            if (DEBUGGING) displayBuffer(buffer);
            break;
            
        case 'D':
            for (int i = 0; i < repetition; i++) {
                if (scale == WORD_SCALE) {
                    buffer.deleteWord();
                } else {
                    buffer.deleteCharacter();
                }
            }
            if (DEBUGGING) displayBuffer(buffer);
            break;
            
        case 'E': buffer.moveCursorToEnd();
            if (DEBUGGING) displayBuffer(buffer);
            break;
            
        case 'F':
            for (int i = 0; i < repetition; i++) {
                if (scale == WORD_SCALE) {
                    buffer.moveCursorForwardWord();
                } else {
                    buffer.moveCursorForward();
                }
            }
            if (DEBUGGING) displayBuffer(buffer);
            break;
            
        case 'H': printHelpText();
            break;
            
        case 'I':
            buffer.insertString(content);
            if (DEBUGGING) displayBuffer(buffer);
            break;
        
        case 'J': buffer.moveCursorToStart();
                if (DEBUGGING) displayBuffer(buffer);
                break;
            
        case 'P':
            buffer.paste();
            if (DEBUGGING) displayBuffer(buffer);
            break;
            
        case 'Q': exit(0);

        case 'T': typeBuffer(buffer);
                break;
            
        case 'S':
            if (!buffer.search(content)) {
                cout << "Search failed." << endl;
            }
            if (DEBUGGING) displayBuffer(buffer);
            break;
            
        case 'R':
            if (!buffer.replace(content)) {
                cout << "Replace failed." << endl;
            }
            if (DEBUGGING) displayBuffer(buffer);
            break;
            
        case 'X':
            if (scale == WORD_SCALE) {
                buffer.cutWords(repetition);
            } else {
                buffer.cut(repetition);
            }
            if (DEBUGGING) displayBuffer(buffer);
            break;
            
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
                      
ParseResultT parseLine(const string & line, TokenScanner & scanner,
                       int & repetition, ScaleT & scale, char & cmd,
                       string & content) {
    
    ParseResultT result = NO_ERROR;
    
    cmd = NULL;
    content = "";
    
    scanner.setInput(line);
    repetition = 1;
    scale = CHAR_SCALE;
    
    // Valid commands have the following forms:
    //
    // 1. consist of a single character
    //
    //    J         // Jump to the beginning of the buffer.
    //    E         // Jump to the end of the buffer.
    //    Q         // Quit the editor.
    //    H         // Display help text.
    //
    // 2. consist of a single character prefixed by an optional repetition
    //    factor
    //
    //    10D       // Delete 10 characters following the cursor.
    //    2B        // Move backward 2 characters.
    //    F         // Move forward one character.
    //    10C       // Copy 10 characters after the cursor to the clipboard.
    //    X         // Cuts a character to the clipboard and delete from editor
    //              // such that it could be pasted somewhere else.
    //
    // 3. consist of a single character prefixed by an optional word
    //    modifier:
    //
    //    WD        // Delete the word following the cursor.
    //    WB        // Move backward to the beginning of the word preceding
    //              // the cursor.
    //    WF        // Move forward one word (to the end of the word
    //              // following the cursor.
    //    WC        // Copies the next word following the cursor to the
    //              // clipboard.
    //    WX        // Cuts the next word to the clipboard and removes the
    //              // word from the editor buffer.
    //
    // 4. consist of a single character followed by content to be inserted
    //    into the editor buffer:
    //
    //    Iinsert this
    //    Afirst line of multi-line input
    //
    // 5. consists of a single character followed by content to be searched
    //    for after starting just after the cursor.
    //
    //    Sfind_me
    //
    
    if (scanner.hasMoreTokens()) {
        string token = scanner.nextToken();
        TokenType tokenType = scanner.getTokenType(token);
        switch (tokenType) {
            case WORD: {
                    if (token.length() > 1) {
                        if (toupper(token[0]) == 'W') {
                            switch(toupper(token[1])) {
                                case 'B':
                                case 'C':
                                case 'D':
                                case 'F':
                                case 'X':
                                    cmd = toupper(token[1]);
                                    scale = WORD_SCALE;
                                    content = line.substr(2, line.length() - 2);
                                    break;
                            }
                        } else {
                            cmd = toupper(token[0]);
                            content = line.substr(1,line.length() - 1);
                            switch (cmd) {
                                // Validate Rthis/that syntax
                                case 'R':
                                    int delimIndex = content.find("/");
                                    if (delimIndex == string::npos ||
                                        delimIndex == 0 ||
                                        delimIndex == content.size() - 1) {
                                        result = BAD_COMMAND;
                                    }
                                    break;
                            }
                        }
                    } else {
                        cmd = toupper(token[0]);
                        content = line.substr(1,line.length() - 1);
                    }
                }
                break;
                
            case NUMBER: {
                    repetition = stringToInteger(token);
                    int numDigits = token.length();
                    int dummyRep;
                    if (line.length() > numDigits) {
                        result = parseLine(line.substr(numDigits), scanner,
                                          dummyRep, scale, cmd, content);
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
    cout << "  A     Add multi-line text until '" + END_OF_APPEND_MARKER;
    cout << "' appears on a line by itself" << endl;
    cout << "  Iabc  Inserts subsequent text at cursor position" << endl;
    cout << "  F     Moves the cursor forward one character" << endl;
    cout << "  B     Moves the cursor backward one character" << endl;
    cout << "  D     Deletes the character after the cursor" << endl;
    cout << "  C     Copies character(s) following cursor to the clipboard" << endl;
    cout << "        Ex: 10C copies 10 chars, 2WC copies 2 words" << endl;
    cout << "  X     Cuts the character(s) following cursor to clipboard" << endl;
    cout << "  P     Pastes text from the clipboard" << endl;
    cout << "  #|{F|B|D|C|X}" << endl;
    cout << "        Perform the command # times" << endl;
    cout << "  #|W{F|B|D|C|X}" << endl;
    cout << "        Perform the command # times on W boundaries" << endl;
    cout << "  J     Jumps to the beginning of the buffer" << endl;
    cout << "  E     Jumps to the end of the buffer" << endl;
    cout << "  H     Prints this message" << endl;
    cout << "  Rthis/that" << endl;
    cout << "        Replaces the first instance of 'this' with 'that'" << endl;
    cout << "        after the cursor." << endl;
    cout << "  Stext Searches for 'text' after the cursor" << endl;
    cout << "  T     Types the contents of the buffer to the console" << endl;
    cout << "  Q     Exits from the editor program" << endl;
}