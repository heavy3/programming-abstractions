//
// main.cpp
//
// This program implements encryption using a letter-substitution cipher. 
// 
// For example:
//
//    Letter substitution cipher.
//    Enter a 26-letter key: QWERTYUIOPASDFGHJKLZXCVBNM
//    Enter a message: WORKERS OF THE WORLD UNITE!
//    Encoded message: VGKATKL GY ZIT VGKSR XFOZT!
// 
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 18
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/12/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

// Constants and types

const string PGM_BANNER = "Letter substitution cipher.";
const string KEY_PROMPT = "Enter a 26-letter key: ";
const string MSG_PROMPT = "Enter a message: ";
const string RPT_OUTPUT = "Encoded message: ";

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string alphabet = "abcdefghijklmnopqrstuvwxyz";
const string TEST_KEY = "QWERTYUIOPASDFGHJKLZXCVBNM";

const size_t KEY_LENGTH = ALPHABET.length();


const string MSG_ERROR_KEY_LENGTH = "Encryption key length is incorrect.";

// Function prototypes

string encode(string plainText, string letterSubstitutionKey);
char encodeChar(char ch, string letterSubstitutionKey);
void error(string msg);

int main(int argc, const char * argv[]) {
    string key;
    string msg;
    string encoded;
    
    cout << PGM_BANNER << endl;
    cout << KEY_PROMPT;
    getline(cin, key);
    cout << MSG_PROMPT;
    getline(cin, msg);
    
    cout << RPT_OUTPUT << encode(msg, key) << endl;
    return 0;
}

// Function definitions

//
// Function: encode
// Usage: string secret = encode("plain text", "QWERTYUIOPASDFGHJKLZXCVBNM");
// --------------------------------------------------------------------------
// Returns a string that encodes the input message with a 26-character
// letter-substitution cipher key.
//
// Calls error() if key length is inadequate.
//

string encode(string plainText, string key) {
    string result;
    
    if (key.length() != KEY_LENGTH) error(MSG_ERROR_KEY_LENGTH);
    
    for (int i = 0; i < plainText.length(); i++) {
        result += char(encodeChar(plainText[i], key));
    }
    return result;
}

//
// Function: encodeChar
// Usage: char secretCh = encodeChar('a', QWERTYUIOPASDFGHJKLZXCVBNM);
// -------------------------------------------------------------------
// Encodes a character using a simple letter substitution cipher key.
// The character's offset from the beginning of the alphabet is used as
// an index into a 26-character long key string.  The looked up value is 
// returned as the encrypted value for the character.
//

char encodeChar(char ch, string letterSubstitutionKey) {
    size_t pos = string::npos;
    char ech;
    
    //
    // Find canonical position of unencoded character in alphabet.
    //
    // (I know, this is O(n), which is lame.  But i'm playing with
    // string primitives and readability.  That's my story.)
    //
    
    if (toupper(ch) == ch) {
        pos = ALPHABET.find(ch);
    } else if (tolower(ch) == ch ) {
        pos = alphabet.find(ch);
    }
    
    if (pos == string::npos) {
        ech = ch;
    } else {
        ech = letterSubstitutionKey[pos];
    }
    return ech;
}

//
// Function: error
// Usage: error("shut 'er down, clancy. she's pumping mud!");
// ----------------------------------------------------------
// Writes message to error console and exits program with EXIT_FAILURE
// constant defined in <cstdlib>.
//

void error(string msg) {
    cerr << msg << endl;
    exit(EXIT_FAILURE);
}
