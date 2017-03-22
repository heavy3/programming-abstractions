//
// main.cpp
//
// This program reimplements the generatePermutations() function from
// section 8-3 to use the following strategy:
//
// a) Remove the first character from the string and store it in the variable
//    ch.
//
// b) Generate the set containing all permutations of the remaining characters.
//
// c) Form a new set by inserting ch in every possible position in each of
//    those permutations.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/9/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "Set.h"
#include "simpio.h"
using namespace std;

// Prototypes

Set<string> generatePermutations(string str);
Set<string> generatePermutations2(string str);

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.08\n";
const std::string DETAIL = "Permutations Rewrite";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    string str = getLine("Enter a string: ");
    cout << "The permutations of \"" << str << "\" are:" << endl;
    for (string s : generatePermutations(str)) {
        cout << "  \"" << s << "\"" << endl;
    }
    cout << "The permutations of \"" << str << "\" (using method 2) are:" << endl;
    for (string s : generatePermutations2(str)) {
        cout << "  \"" << s << "\"" << endl;
    }
    return 0;
}

//
// Function: generatePermutations
// Usage: Set<string> permutations = generatePermutations(str);
// ------------------------------------------------------------
// Returns a set consisting of all permutations of the specified string.
// This implementation uses the recursive insight that you can generate
// all permutations of a string by selecting each character in turn,
// generating all permutations of the string without that character,
// and then concatenating the selected character on the front of each
// string generated.
//

Set<string> generatePermutations(string str) {
    Set<string> result;

    if (str == "") {
        result += "";
    } else {
        for (int i = 0; i < str.length(); i++) {
            char ch = str[i];
            string rest = str.substr(0, i) + str.substr(i + 1);
            for (string s : generatePermutations(rest)) {
                result += ch + s;
            }
        }
    }
    
    return result;
}

//
// Function: generatePermutations2
// Usage: Set<string> permutations = generatePermutations2(str);
// ------------------------------------------------------------
// Returns a set consisting of all permutations of the specified string.
// This implementation uses the recursive insight that you can generate
// all permutations of a string by
//
// 1 Remove the first character from the string and store it in the variable ch.
//
// 2 Generate the set containing all permutations of the remaining characters.
//
// 3 Form a new set by inserting ch in every possible position in each of
//   those permutations.
//

Set<string> generatePermutations2(string str) {
    Set<string> result;
    
    if (str == "") {
        result += "";
    } else {
        char ch = str[0];
        string rest = str.substr(1, str.length() - 1);
        Set<string> permuted = generatePermutations2(rest);
        
        // Re-insert first character into the permuted set of substrings.
        
        for (string s : permuted) {
            for (int i = 0; i <= s.length(); i++) {
                result += s.substr(0, i) + ch + s.substr(i);
            }
        }
    }
    return result;
}
