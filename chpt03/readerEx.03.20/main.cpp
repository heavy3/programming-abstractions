//
// main.cpp
//
// This program implements a function:
// 
//     int findDNAMatch(string s1, string s2, int start = 0);
// 
// that returns the first position at which the DNA strand s1 can attach to 
// the strand s2. As in the find method for the string class, the optional 
// start parameter indicates the index position at which the search should 
// start. If there is no match, findDNAMatch returns –1.
//
// I am adapting the character substitution cipher from the previous 
// problem to invert the short DNA strand.  The problem reduces to a 
// sub-string find of the inverted DNA snippet within the longer strand.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 20
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

const string PGM_BANNER = "Find DNA Snippet Binding Position";

const string MSG_PROMPT = "Enter a DNA snippet: ";
const string ENC_OUTPUT = "The inverted strand: ";
const string KEY_OUTPUT = "Inverted key: ";

const string ALPHABET = "ACTG";
const string alphabet = "actg";
const string DNA_INVERSION_KEY = "TGAC";

const string SNIPPET = "TGC";
const string LONG_STRAND = "TAACGGTACGTC";

const size_t KEY_LENGTH = ALPHABET.length();

const string MSG_ERROR_KEY_LENGTH = "Encryption key length is incorrect.";

// Function prototypes

string encode(string plainText, string letterSubstitutionKey);
char encodeChar(char ch, string letterSubstitutionKey);
int findDNAMatch(string s1, string s2, int start = 0);
void error(string msg);

int main(int argc, const char * argv[]) {
    string key;
    string decodeKey;
    string snippet;
    string invertedSnippet;
    string decodedMsg;
    
    cout << PGM_BANNER << endl << endl;
    cout << "Given the following DNA sequence: \n\t" << LONG_STRAND << endl << endl;
    cout << "I'll find valid binding positions for a DNA snippet you provide.";
    cout << endl << endl;
    
    key = DNA_INVERSION_KEY;
    
    cout << MSG_PROMPT;
    snippet = SNIPPET;
    cout << snippet << endl;
    
    invertedSnippet = encode(snippet, key);
    cout << ENC_OUTPUT << invertedSnippet << endl;
    
    int pos;
    pos = findDNAMatch(snippet, LONG_STRAND, pos);
    
    if (pos == -1) {
        cout << "No acceptable binding site was found for your snippet. :-(";
    } else {
        cout << "Your snippet can bind at (0-based) position: " << pos;
    }
    cout << endl;
    
    pos = 5;
    pos = findDNAMatch(snippet, LONG_STRAND, pos);
    
    if (pos == -1) {
        cout << "No acceptable binding site was found for your snippet. :-(";
    } else {
        cout << "Your snippet can bind at (0-based) position: " << pos;
    }
    cout << endl;
    
    return 0;
}

// Function definitions

//
// Function: findDNAMatch
// Usage: int pos = findDNAMatch("TGC", "TAACGGTACGTC", 0); // returns 2
// ---------------------------------------------------------------------
// Returns the first position at which a snippet of DNA can
// bind to a larger strand of DNA according to the constraint that A may
// connect to T and C may connect to G.
// Returns -1 otherwise.
//
// An optional search position start parameter may also be passed in
// if you want to begin the search from some location beyond the default of
// 0, the start of the long strand.
//

int findDNAMatch(string s1, string s2, int start) {
    string inverseS1 = encode(s1, DNA_INVERSION_KEY);
    
    int pos = int(s2.find(inverseS1, start));
    if (pos == string::npos) return -1;
    
    return pos;
}

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
// The offset of the charactert from the beginning of the alphabet is 
// used as an index into a 26-character long key string.  The looked 
// up value is returned as the encrypted value for the character.
//

char encodeChar(char ch, string letterSubstitutionKey) {
    size_t pos = string::npos;
    char ech;
    
    //
    // Find canonical position of unencoded character in alphabet.
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
