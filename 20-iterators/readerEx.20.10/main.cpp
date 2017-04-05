//
// main.cpp
//
// Customize the STL's sort algorithm to perform a case-insensitive
// sort of strings by implementing our own comparision callback predicate,
// lessIgnoringCase.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 10
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/5/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <algorithm>
#include <functional>
#include <iostream>
#include <cassert>
#include "error.h"
#include "vector.h"
using namespace std;

// Constants and types

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 20.10\n";
const std::string DETAIL = "STL sort: w case-insensitive comparison predicate.";
const std::string BANNER = HEADER + DETAIL;

typedef std::string::iterator StrIter;

// Function prototypes

bool lessIgnoringCase(std::string str1, std::string str2);
bool testVectorSort(Vector<std::string>& v, const Vector<std::string>& vExp);
void testDriver();

// Main program

int main() {
    cout << BANNER << endl;
    testDriver();
    return 0;
}

void testDriver() {
    Vector<std::string> v1;
    v1 += "Eight";
    v1 += "six";
    v1 += "seven";
    v1 += "Five";
    v1 += "three";
    v1 += "oh";
    v1 += "Nine";
    
    Vector<std::string> v1Expect;
    v1Expect += "Eight";
    v1Expect += "Five";
    v1Expect += "Nine";
    v1Expect += "oh";
    v1Expect += "seven";
    v1Expect += "six";
    v1Expect += "three";
    
    // Sort the input vector and compare it to expected result.
    assert(testVectorSort(v1, v1Expect));
    
    // Sadly, something like this didn't work :-/
    //
    // sortVector(v1);
    // int diff = count_if(v1.begin(), v1Expect.begin(), equal_to<std::string>());
    // assert(diff == 0);
    
    Vector<std::string> v2;
    v2 += "efg";
    v2 += "abc";
    v2 += "EFG";
    v2 += "ABC";
    
    Vector<std::string> v2Expect;
    v2Expect += "abc";
    v2Expect += "ABC";
    v2Expect += "efg";
    v2Expect += "EFG";
    
    // Sort the input vector and compare it to expected result.
    assert(testVectorSort(v2, v2Expect));
    
    cout << endl << "Passed all unit tests." << endl;
}

bool testVectorSort(Vector<std::string>& v, const Vector<std::string>& vExp) {
    if (v.size() != vExp.size()) return false;
    
    cout << endl << "before sort: " << v << endl;
    
    sort(v.begin(), v.end(), lessIgnoringCase); // STL sort with my predicate.
    
    cout << "after sort: " << v << endl;
    
    bool result = true;
    for (int i = 0; i < v.size(); i++) {
        if (v[i] != vExp[i]) return false;
    }
    return result;
}

// Function: lessIgnoringCase
// Usage: if (lessIgnoringCase("ABC", "efg")) { . . . }
// ----------------------------------------------------
// Returns true if the first string is lexigraphically less than the
// second string, ignoring case sensitivity.
//
// This provides the callback required by one form of the STL's sort
// function.

bool lessIgnoringCase(std::string str1, std::string str2) {
    bool result = false;
    for (StrIter it1 = str1.begin(), it2 = str2.begin();
         it1 != str1.end();
         it1++, it2++) {
        
        // Reached end of str2 before end of str1, so str1 > str2.
        // Essentially performaing (*it1 < '') which is always false.
        
        if (it2 == str2.end()) break;
        
        char lc1 = tolower(*it1);
        char lc2 = tolower(*it2);
        if (lc1 < lc2) {
            result = true;
            break;
        } else if (lc1 > lc2) {
            break;
        }
    }
    return result;
}
