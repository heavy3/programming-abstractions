//
// main.cpp
//
// This program spins in a loop, prompting the user for input strings to 
// convert to or from Morse code.  Translated output is presented back 
// to the console.  The program supports plain and annotated modes.
//
// For example:
//
//    ---------------------------------------------------------------------
//    Enter text to translate to/from Morse code. (Blank line to quit.)
//    ? hello
//    .... . .-.. .-.. ---
//    ? .... . .-.. .-.. ---
//    HELLO
//    ?
//    Enter text to translate to/from Morse code. (Blank line to quit.)
//    ? .... . .-.. .-.. ---
//    ....(H) .(E) .-..(L) .-..(L) ---(O)
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 19
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// The the createMorseCodeMap function comes from:
// http://www.stanford.edu/class/archive/cs/cs106b/cs106b.1154/handouts/15-Section2.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/16/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include "map.h"
#include "simpio.h"
#include "strlib.h"
#include "tokenscanner.h"
using namespace std;

// Types and constants

const string LINE = string(60, '-') + '\n';
const char DOT = '.';
const char DASH = '-';
const string PROMPT = "Enter text to translate to/from Morse code. " +
                      string("(Blank line to quit.)");

// Formatting options

enum FormatT {
    PLAIN,      // Follows problem-spec guidelines.
    ANNOTATED   // Enhanced output for debugging and training modes.
};

// Function prototypes

void banner();
string alphaToMorse(string english, Map<char,string> alphaMap,
                    FormatT format = PLAIN);
Map<char,string> createMorseCodeMap();
Map<string,char> invertMap(Map<char,string> map);
bool isMorseCode(string str);
string morseToAlpha(string morseCode, Map<string,char> morseMap,
                    FormatT format = PLAIN);

void translateMorseAlpha(string prompt, FormatT format);

// Main program

int main(int argc, char * argv[]) {
    banner();
    cout << LINE;

    FormatT format = PLAIN;
    translateMorseAlpha(PROMPT, format);
    
    format = ANNOTATED;
    translateMorseAlpha(PROMPT, format);
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
    cout << "This program translates English to Morse code and vice versa."
         << endl;
}

//
// Function: translateMorseAlpha
// Usage: translateMorseAlpha("Enter text to translate:", PLAIN);
// --------------------------------------------------------------
// Spins in a loop taking input from the console and translating into
// or from Morse code, depending upon the detected input.
//
// Output is presented back to the console.
//
// Output may be ANNOTATED or PLAIN for more or less decoration.
//

void translateMorseAlpha(string prompt, FormatT format) {
    
    cout << prompt << endl;
    
    Map<char, string> alphaMap = createMorseCodeMap();
    Map<string, char> morseMap = invertMap(alphaMap);
    
    while (true) {
        string xlate = trim(getLine("? "));
        if (xlate == "") {
            break;
        }
        if (isMorseCode(xlate)) {
            string english = morseToAlpha(xlate, morseMap, format);
            cout << english << endl;
        } else {
            string morse = alphaToMorse(xlate, alphaMap, format);
            cout << morse << endl;
        }
    }
}

//
// Function: isMorseCode
// Usage: if (isMorseCode("... --- ...")) { ... }
// ----------------------------------------------
// Returns true if the input string is detected to be encoded in Morse code.
//
// If the first non-whitespace character in the string is a dot (.) or dash (-)
// then the input string is assumed to be Morse code.
//

bool isMorseCode(string str) {
    bool result = false;
    
    for (char ch: str) {
        if (isspace(ch)) continue;
        
        result = (ch == DASH || ch == DOT) ? true : false;
        break;
    }
    return result;
}

//
// Function: morseToAlpha
// Usage: string english = morseToAlpha("... --- ...", morseToAlphaMap);
//        string english = morseToAlpha(morseStr, morseToAlphaMap, ANNOTATED);
// ----------------------------------------------------------------------------
// Returns a string with the decoded text of a Morse code input message.
//
// The decoded text may be formatted as PLAIN or ANNOTATED, with the latter
// interleaving code and letter. 
//
// Invalidly encoded input results in a '.' for each unrecognized code with
// PLAIN formatting, whereas ANNOTATED formatting preserves the invalid input
// code to simplify debugging.
//
// NB: Morse code does not encode spaces, so messages will be lacking
//     space delimiters between decoded words.
//

string morseToAlpha(string morseCodes, Map<string, char> morseMap,
                    FormatT format) {
    
    TokenScanner scanner(morseCodes);
    scanner.ignoreWhitespace();
    
    string validMorseChars = string(1, DASH) + string(1, DOT);
    scanner.addWordCharacters(validMorseChars);
    
    string english;
    while (scanner.hasMoreTokens()) {
        string code = scanner.nextToken();
        if (morseMap.containsKey(code)) {
            char letter = morseMap[code];
            switch (format) {
                case ANNOTATED:
                    english += code + "(" + letter + ") ";
                    break;
                    
                case PLAIN:
                default:
                    english += letter;
                    break;
            }
        } else {
            // 
            // Error: Invalid morse code encountered.
            //
            // Provide hint of invalid input.
            // Translate remaining input on a best-effort basis.
            //
            switch (format) {
                case ANNOTATED:
                    english += code + "(?) ";
                    break;
                    
                case PLAIN:
                default:
                    english += ".";
                    break;
            }
        }
    }
    return english;
}

//
// Function: alphaToMorse
// Usage: string morse = alphaToMorse("... --- ...", alphaMap);
//        string morse = alphaToMorse(englishStr, alphaMap, ANNOTATED);
// ----------------------------------------------------------------------------
// Returns a string with the Morse encoded text of an input message.
//
// The encoded text may be formatted as PLAIN or ANNOTATED, with the latter
// interleaving code and letter.
//
// Input text with no Morse encoding is ignored under PLAIN formatting but
// passed through under ANNOTATED formatting to simplify debugging.
//

string alphaToMorse(string english, Map<char, string> alphaMap,
                      FormatT format) {
    
    TokenScanner scanner(english);
    scanner.ignoreWhitespace();
    
    string morse;
    while (scanner.hasMoreTokens()) {
        string str = toUpperCase(scanner.nextToken());
        for (char ch : str) {
            if (alphaMap.containsKey(ch)) {
                string code = alphaMap[ch];
                switch (format) {
                    case ANNOTATED:
                        morse += code + "(" + char(ch) + ") ";
                        break;
                        
                    case PLAIN:
                    default:
                        morse += code + " ";
                        break;
                }
            } else {
                //
                // No Morse code encoding for this character.
                //
                // Problem spec says to ignore ... which we do for plain
                // translations.
                //
                // For annotated translations, be more generous with
                // hints about encoding failure.
                //
                switch (format) {
                    case ANNOTATED:
                        morse += string(1, ch) + "(?) ";
                        break;
                        
                    case PLAIN:
                    default:
                        break;
                }
            }
        }
    }
    return morse;
}

//
// Function: createMorseCodeMap
// Usage: Map<char, string> alphaMap = createMorseCodeMap();
// ---------------------------------------------------------
// Returns a map between upper case alphabetic characters and their Morse
// code equivalent.
//

Map<char, string> createMorseCodeMap() {
    Map<char, string> map;
    
    map['A'] = ".-"   ;  map['J'] = ".---" ;  map['S'] = "..."  ;
    map['B'] = "-..." ;  map['K'] = "-.-"  ;  map['T'] = "-"    ;
    map['C'] = "-.-." ;  map['L'] = ".-.." ;  map['U'] = "..-"  ;
    map['D'] = "-.."  ;  map['M'] = "--"   ;  map['V'] = "...-" ;
    map['E'] = "."    ;  map['N'] = "-."   ;  map['W'] = ".--"  ;
    map['F'] = "..-." ;  map['O'] = "---"  ;  map['X'] = "-..-" ;
    map['G'] = "--."  ;  map['P'] = ".--." ;  map['Y'] = "-.--" ;
    map['H'] = "...." ;  map['Q'] = "--.-" ;  map['Z'] = "--.." ;
    map['I'] = ".."   ;  map['R'] = ".-."  ;
    
    return map;
}

//
// Function: invertMap
// Usage: Map<string, char> morseMap = invertMap(alphaToMorseMap);
// ---------------------------------------------------------------
// Returns a Map with key/value pairs inverted relative to an input map.
// Suited for deriving a Morse-to-alpha map from an alpha-to-Morse map.
//

Map<string, char> invertMap(Map<char, string> map) {
    Map<string, char> invMap;
    
    for (char key: map) {
        string value = map.get(key);
        invMap.put(value, key);
    }
    
    return invMap;
}
