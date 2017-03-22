//
// SimpleTextEditor.cpp
//
// This variation of the simple editor program from the text implements a
// T command that types the contents of the buffer to the screen (as opposed
// to dumping the buffer contents to the screen after every command).
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program extends the code from Figure 13-3.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/28/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <cctype>
#include <iostream>
#include "buffer.h"
#include "simpio.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 13.03\n";
const std::string DETAIL = "Simple Text Editor w T(ype) command.";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

void executeCommand(EditorBuffer & buffer, string line);
void displayBuffer(EditorBuffer & buffer);
void typeBuffer(EditorBuffer & buffer);
void printHelpText();

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    EditorBuffer buffer;
    while (true) {
        string cmd = getLine("*");
        if (cmd != "") executeCommand(buffer, cmd);
    }
    return 0;
}

// Function: executeCommand
// Usage: executeCommand(buffer, line);
// ------------------------------------
// Executes the command specified by line on the editor buffer.

void executeCommand(EditorBuffer & buffer, string line) {
    switch (toupper(line[0])) {
        case 'I': for (int i = 1; i < line.length(); i++) {
                buffer.insertCharacter(line[i]);
            }
            break;
            
        case 'D': buffer.deleteCharacter(); break;
            
        case 'F': buffer.moveCursorForward(); break;
            
        case 'B': buffer.moveCursorBackward(); break;

        case 'J': buffer.moveCursorToStart(); break;
            
        case 'E': buffer.moveCursorToEnd(); break;
            
        case 'H': printHelpText(); break;
            
        case 'T': typeBuffer(buffer);break;
            
        case 'Q': exit(0);
            
        default: cout << "Illegal command" << endl; break;
    }
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
    cout << "  Iabc  Inserts the characters abc at the cursor position" << endl;
    cout << "  F     Moves the cursor forward one character" << endl;
    cout << "  B     Moves the cursor backward one character" << endl;
    cout << "  D     Deletes the character after the cursor" << endl;
    cout << "  J     Jumps to the beginning of the buffer" << endl;
    cout << "  E     Jumps to the end of the buffer" << endl;
    cout << "  H     Prints this message" << endl;
    cout << "  T     Types the contents of the buffer to the console" << endl;
    cout << "  Q     Exits from the editor program" << endl;
}
