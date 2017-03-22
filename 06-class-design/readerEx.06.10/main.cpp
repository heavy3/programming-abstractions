//
// main.cpp
//
// This program checks the spelling of words in a file.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 10
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/22/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cctype>
#include "tokenscanner.h"
#include "lexicon.h"
#include "filelib.h"
#include "error.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.10\n";
const std::string DETAIL = "SpellChecker";
const std::string BANNER = HEADER + DETAIL;
const std::string PROMPT = "Input file: ";
const std::string LEXFAIL = " is not in the dictionary";

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    ifstream infile;
    std::string filename;
    
    Lexicon english("EnglishWords.dat");
    filename = promptUserForFile(infile, PROMPT);
    TokenScanner scanner(infile);
    scanner.ignoreWhitespace();
    
    while (scanner.hasMoreTokens()) {
        std::string token = scanner.nextToken();
        if (isalpha(token[0])) {
            if (!english.contains(token)) {
                std::cout << '"' << token << '"' << LEXFAIL << std::endl;
            }
        }
    }
    
    return 0;
}