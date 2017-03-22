//
// main.cpp
//
// This program implements encryption using a letter-substitution cipher.
// It then computes an inverted key and decodes the encoded message to
// reveal the original, plain text message.
//  
// For example:
//
//    Letter substitution cipher.
//    Enter a 26-letter key:  QWERTYUIOPASDFGHJKLZXCVBNM
//    Enter a message: WORKERS OF THE WORLD UNITE
//    Encoded message: VGKATKL GY ZIT VGKSR XFOZT
//    Inverted 26-letter key: KXVMCNOPHQRSZYIJADLEGWBUFT
//    Decoded message: WORKERS OF THE WORLD UNITE
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 19
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
const string KEY_PROMPT = "Enter a 26-letter key:  ";
const string MSG_PROMPT = "Enter a message: ";
const string ENC_OUTPUT = "Encoded message: ";
const string DEC_OUTPUT = "Decoded message: ";
const string KEY_OUTPUT = "Inverted 26-letter key: ";

const string alphabet = "abcdefghijklmnopqrstuvwxyz";
const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string TEST_KEY = "QWERTYUIOPASDFGHJKLZXCVBNM";
const string TEST_MSG = "WORKERS OF THE WORLD UNITE";

const size_t KEY_LENGTH = ALPHABET.length();

const string MSG_ERROR_KEY_LENGTH = "Encryption key length is incorrect.";

// Function prototypes

string encode(string plainText, string letterSubstitutionKey);
char encodeChar(char ch, string letterSubstitutionKey);
string invertKey(string key);
void error(string msg);

int main(int argc, const char * argv[]) {
    string key;
    string decodeKey;
    string msg;
    string encodedMsg;
    string decodedMsg;
    
    cout << PGM_BANNER << endl;
    
    cout << KEY_PROMPT;
    key = TEST_KEY;
    cout << key << endl;
    
    cout << MSG_PROMPT;
    msg = TEST_MSG;
    cout << msg << endl;
    
    encodedMsg = encode(msg, key);
    cout << ENC_OUTPUT << encodedMsg << endl;
    
    decodeKey = invertKey(key);
    cout << KEY_OUTPUT << decodeKey << endl;
    decodedMsg = encode(encodedMsg, decodeKey);
    cout << DEC_OUTPUT << decodedMsg << endl;
    
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
    // TODO: It's O(n) which is a bit lame but very readable. 

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

//
// Function: invertKey
// Usage: string decryptionKey = invertKey(encryptionKey);
// -------------------------------------------------------
// Returns an inverted key, suitable for decrypting whatever was encrypted
// using the passed-in letter-substitution key.
//
// The algorithm works by:
//
//  1. iterating across the length of the encryption key
//
//  2. noting the encrypted character associated with the i-th position in
//     the encrypted key string
//
//  3. saving off the canonical position of the encrypted character in the
//     alphabet so we know where to save the unencrypted character in the
//     inverted key
//
//  4. looking up the unencrypted character associated with the i-th
//     character of the alphabet.
//
//  5. populating the inverted key with the unencrypted character at the
//     position from step 3.
//

string invertKey(string key) {
    string invKey = string(key.length(), ' ');
    
    for (int i = 0; i < key.length(); i++ ) {
        char ch, ech;
        size_t pos;
        if (toupper(key[i]) == key[i]) {
            
            ech = key[i];
            pos = ALPHABET.find(ech);
            ch = ALPHABET[i];
            
        } else {

            ech = key[i];
            pos = alphabet.find(ech);
            ch = alphabet[i];
            
        }
        invKey[pos] = ch;
    }
    
    return invKey;
}
