//
// bufferUnitTest.cpp
//
// This program is a unit test driver for the EditorBuffer class that
// employs before-cursor and after-cursor stacks that occupy different
// ends of the same data structure.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 13, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/27/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <cctype>
#include <cassert>
#include <iostream>
#include "buffer.h"
#include "simpio.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 13.02\n";
const std::string DETAIL = "Memory-efficient stack-based EditorBuffer";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

void executeCommand(EditorBuffer & buffer, string line, bool disp=true);
void displayBuffer(EditorBuffer & buffer, bool disp=true);
void printHelpText();
void unitTestEditorBuffer(EditorBuffer & buffer);

// Main program

int main() {
    cout << BANNER << endl << endl;;
    EditorBuffer buffer;
    unitTestEditorBuffer(buffer);
    return 0;
}

// Function: unitTestEditorBuffer
// Usage:    unitTestEditorBuffer();
// ---------------------------------
// Uses the assert facility to exercise basic
// editing and cursor motion capabilities of the EditorBuffer class.
//
// Also covers various edge cases that may arise:
//
//      1. deleting from an empty buffer
//      2. deleting from the end of the buffer
//      3. moving backward when already at the beginning of the buffer
//      4. moving forward when already at the end of the buffer
//      5. jumping to beginning or end of an empty buffer
//
// Returns to caller if all unit tests passed.  Otherwise reports an assertion
// error to the console and halts.

void unitTestEditorBuffer(EditorBuffer & buffer) {
    
    string cmd;
    
    // Test initial state of empty buffer after constructor.
    
    assert(buffer.getText() == "");
    assert(buffer.getCursor() == 0);
    
    // Test inserting nothing (edge case).
    
    cmd = "I";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "");
    assert(buffer.getCursor() == 0);
    
    // Test initial load of buffer with simple input:
    //
    //  Iabc --> a b c
    //                ^
    
    cmd = "Iabc";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "abc");
    assert(buffer.getCursor() == 3);
    
    // Test deletion at end of buffer.
    //  D   --> a b c
    //               ^
    //  No change expected.
    
    cmd = "D";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "abc");
    assert(buffer.getCursor() == 3);
    
    // Test forward movement at end of buffer.
    // Expecting no change.
    
    cmd = "F";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "abc");
    assert(buffer.getCursor() == 3);
    
    // Test backward movement one character.
    // Test case-insensitivity of commands.
    //
    //  b   --> a b c
    //             ^
    
    cmd = "b";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "abc");
    assert(buffer.getCursor() == 2);
    
    // Test deletion of one character.
    //
    //  D   --> a b
    //             ^
    
    cmd = "D";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "ab");
    assert(buffer.getCursor() == 2);
    
    // Test backward movement to front of buffer
    //
    //  B   --> a b
    //           ^
    //  B   --> a b
    //         ^
    
    cmd = "B";
    executeCommand(buffer, cmd);
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "ab");
    assert(buffer.getCursor() == 0);
    
    // Test backward movement from front of buffer.
    // Should result in no change.
    
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "ab");
    assert(buffer.getCursor() == 0);
    
    // Test jump to end of buffer.
    //
    //  E   --> a b
    //             ^
    
    cmd = "E";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "ab");
    assert(buffer.getCursor() == 2);
    
    // Test jump to beginning of buffer.
    //
    //  J   --> a b
    //         ^
    
    cmd = "J";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "ab");
    assert(buffer.getCursor() == 0);
    
    // Test deletion of character from beginning of buffer.
    //
    //  D   --> b
    //         ^
    
    cmd = "D";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "b");
    assert(buffer.getCursor() == 0);
    
    // Test deletion of character from beginning of buffer.
    //
    //  D   -->
    //         ^
    
    cmd = "D";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "");
    assert(buffer.getCursor() == 0);
    
    
    // Test deletion of character from empty buffer.
    // No change expected.
    //
    //  D   -->
    //         ^
    
    cmd = "D";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "");
    assert(buffer.getCursor() == 0);
    
    // Test jumping to end of empty buffer.
    // No change expected.
    //
    //  E   -->
    //         ^
    
    cmd = "E";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "");
    assert(buffer.getCursor() == 0);
    
    // Test multiple insertions of text.
    //
    //  IUnit tests
    //
    
    cmd = "IUnit tests ";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "Unit tests ");
    assert(buffer.getCursor() == 11);
    
    cmd = "Ifor ";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "Unit tests for ");
    assert(buffer.getCursor() == 15);
    
    cmd = "IEditorBuffer";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "Unit tests for EditorBuffer");
    assert(buffer.getCursor() == 27);
    
    cmd = "B";
    for (int i = 0; i < 16; i++) {
        executeCommand(buffer, cmd, false);
    }
    
    cmd = "Iran ";
    executeCommand(buffer, cmd);
    assert(buffer.getText() == "Unit tests ran for EditorBuffer");
    assert(buffer.getCursor() == 15);
    
    cout << "Unit tests passed for EditorBuffer." << endl;
}

// Function: executeCommand
// Usage: executeCommand(buffer, line);
// Usage: executeCommand(buffer, line, displayResults);
// ----------------------------------------------------
// Executes the command specified by line on the editor buffer.
// Displays the result to the console if the optional display parameter is true.
// By default, the results of the command are displayed.

void executeCommand(EditorBuffer & buffer, string line, bool disp) {
    switch (toupper(line[0])) {
        case 'I': for (int i = 1; i < line.length(); i++) {
                buffer.insertCharacter(line[i]);
            }
            displayBuffer(buffer, disp);
            break;
            
        case 'D': buffer.deleteCharacter(); displayBuffer(buffer, disp); break;
            
        case 'F': buffer.moveCursorForward(); displayBuffer(buffer, disp); break;
            
        case 'B': buffer.moveCursorBackward(); displayBuffer(buffer, disp); break;

        case 'J': buffer.moveCursorToStart(); displayBuffer(buffer, disp); break;
            
        case 'E': buffer.moveCursorToEnd(); displayBuffer(buffer); break;
            
        case 'H': printHelpText(); break;
            
        case 'Q': exit(0);
            
        default: cout << "Illegal command" << endl; break;
    }
}

// Function: displayBuffer
// Usage: displayBuffer(buffer);
//        displayBuffer(buffer, false);
// ------------------------------------
// Displays the state of the buffer including the position of the cursor.
// Display is inhibited if the optional disp parameter is false.

void displayBuffer(EditorBuffer & buffer, bool disp) {
    if (!disp) return;
    
    string str = buffer.getText();
    for (int i = 0; i < str.length(); i++) {
        cout << " " << str[i];
    }
    cout << endl;
    cout << string(2 * buffer.getCursor(), ' ') << "^" << endl;
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
    cout << "  Q     Exits from the editor program" << endl;
}