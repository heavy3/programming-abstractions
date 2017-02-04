//
// main.cpp
//
// This program implements a cyclic cipher (or 'Caesar cipher')
// for ASCII-encoded text.
//
// For example:
//
// Enter a message: This is a secret message.
// Enter the number of character positions to shift: 13
// Encoded message: Guvf vf n frperg zrffntr.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 17
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/12/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
using namespace std;

// Constants

const int NUM_ALPHABET_CHARS = 26;

// Function prototypes

string encodeCaesarCipher(string str, int rotate);
char encodeChar(char ch, int rotate);
bool promptAndEncode();
bool testEncodeCaesarCipher(string plainText, int rotate, string expectedText);
int regressionTest();

// Main program

int main(int argc, const char * argv[]) {
    int count = 0;  // Failure count.

    // Interactive
    
    cout << "This program encodes a line of ASCII text using a cyclic cipher\n"
         << "and displays the result to the console." << endl;
    cout << "(To quit, hit 'Enter' with no message.)" << endl << endl;
    
    while (promptAndEncode());
    
    // Batch
    
    cout << endl << "And now for some regression testing ..." << endl << endl;

    count = regressionTest();
    string word = "failure";
    if ((count > 1) || (count == 0)) word += "s";
    cout << count << " " << word << "." << endl;
    
    return count;
}

// Function definitions

// Function: promptAndEncode
// Usage: while (promptAndEncode());
// ---------------------------------
// Prompts the user to enter text and a rotate value for encoding test.
// Returns false when 'Enter' is pressed by itself, causing control to return
// to caller.

bool promptAndEncode() {
    string plainText;
    int rotate;
    
    while (true) {
        cout << "Enter a message: ";
        getline(cin, plainText);
        if (plainText == "") break; // quit on empty message
        
        while (true) {
            string line;
            cout << "Enter the number of character positions to shift: ";
            getline(cin, line);
            stringstream numStream(line);
            if (numStream >> rotate) break; // break on valid integer conversion
            cout << "Invalid number, please try again." << endl;
        }
        
        string encodedText = encodeCaesarCipher(plainText, rotate);
        cout << "Encoded message: " << encodedText << endl;
    }
    return false;
}

// Function: encodeCaesarCipher
// Usage: string encoded = encodeCaesarCipher("Et tu, Brute?", 3);
// ------------------------------------------------------------------
// Returns a new string formed by rotating every letter in str forward the
// number of letters indicated by rotate, cycling back to the beginning of the
// alphabet if necessary.  If the rotate parameter is negative, then rotation
// is in the opposite direction.
//
// The transformation applies only to letters; any other characters are copied
// unchanged to the output. Moreover, the case of letters is unaffected:
// lowercase letters come out as lowercase, and uppercase letters come out as
// uppercase.

string encodeCaesarCipher(string str, int rotate) {
    string result;
    for (int i = 0; i < str.length(); i++) {
        result += encodeChar(str[i], rotate);
    }
    return result;
}

// Function: encodeChar
// Usage: char encodedCh = encodeChar('A', 3);   // returns 'D'
// --------------------------------------------------------------
// Returns encoded character based upon a cyclical ASCII cipher
// and a rotate parameter.  A letter is encoded by associating it
// with a letter rotateed n units to the right (or left, in the case
// of a negative rotate value).  Shifting wraps around to the beginning
// of the alphabet if the offset value is larger than the number of
// characters in the alphabet.

char encodeChar(char ch, int rotate) {
    if (!isalpha(ch)) {
        return ch;
    }
    
    char baseCh;
    (toupper(ch) == ch) ? baseCh = 'A' : baseCh = 'a';
    
    // TODO: Is % portable for negative values?
    //       Seems to work with xcode tool-chain.
    
    // Constrain offset to values between (-25, 25) inclusive.
    
    int offset = (ch - baseCh + rotate) % NUM_ALPHABET_CHARS;
    
    //  Bias offset in positive direction.
    
    if (offset < 0) offset += NUM_ALPHABET_CHARS;
    
    return char(baseCh + offset);
}

// Function: regressionTest
// Usage: int failures = regressionTest();
// ---------------------------------------
// Run regression test on hardcoded plain text input.

int regressionTest() {
    int failures = 0;
    
    if (!testEncodeCaesarCipher("encode me", 1, "fodpef nf")) ++failures;
    if (!testEncodeCaesarCipher("A1", -1, "Z1")) ++failures;
    if (!testEncodeCaesarCipher("A1", NUM_ALPHABET_CHARS, "A1")) ++failures;
    if (!testEncodeCaesarCipher("a1", 24, "y1")) ++failures;
    if (!testEncodeCaesarCipher("a1", 24 + NUM_ALPHABET_CHARS, "y1")) ++failures;
    if (!testEncodeCaesarCipher("0-rotation", 0, "0-rotation")) ++failures;
    if (!testEncodeCaesarCipher("expect failure", 0, "this failure :-)")) ++failures;
    
    return failures;
}

// Function: testEncodeCaesarCipher
// Usage: if (testEncodeCaesarCipher("encode me", 1, "fodpef nf")) { ... }
// -----------------------------------------------------------------------
// Returns true if the expected encoding matches the actual encoding for the
// provided message and rotate factor.

bool testEncodeCaesarCipher(string plainText, int rotate, string expectedText) {
    string p = "[PASS]: [";
    string f = "[FAIL]: [";
    string s = "         ";
    
    string encodedText = encodeCaesarCipher(plainText, rotate);
    
    bool result = (encodedText == expectedText);
    
    if (result) {
        cout << p << plainText << "] encoded to: [" << encodedText << "]\n";
    } else {
        cout << f << plainText << "] encoded to: [" << encodedText << "]\n";
        cout << s << "expected: [" << expectedText << "]" << endl;
    }
    return result;
}