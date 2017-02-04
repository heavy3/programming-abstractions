//
// main.cpp
//
// This program implements the predicate function, isSentencePalindrome(str).
//
// For example:
//
//    isSentencePalindrome("Madam, I’m Adam.") would return true.
//
// Notice that punctuation and whitespace are factored out.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 11
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/5/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
using namespace std;

// Function prototypes

bool testIsSentencePalindrome(string str, bool expectedAnswer);
bool isSentencePalindrome(string str);

// Main program

int main(int argc, const char * argv[]) {
    string pass = "[PASS] :-)";
    string fail = "[FAIL] :-(";
    
    string str = "Madam, I’m Adam.";
    bool expect = true;
    (testIsSentencePalindrome(str, expect)) ? cout << pass : cout << fail;
    cout << endl << endl;
    
    str = "A man, a plan, a canal -- Panama!";
    expect = true;
    (testIsSentencePalindrome(str, expect)) ? cout << pass : cout << fail;
    cout << endl << endl;
    
    str = "This is not my beautiful wife.";
    expect = false;
    (testIsSentencePalindrome(str, expect)) ? cout << pass : cout << fail;
    cout << endl << endl;

    return 0;
}

//
// Function: testIsSentencePalindrome
// Usage: (testIsSentencePalindrome(str, true)) ? cout<< "pass" :cout << "fail";
// -----------------------------------------------------------------------------
// Returns true if expected answer matches actual answer when testing for
// a so-called sentence palindrome.  Otherwise returns false.
//

bool testIsSentencePalindrome(string str, bool expect) {
    string is_isnot;
    bool actual = isSentencePalindrome(str);
    
    is_isnot = (actual) ? "is" : "is not";
    
    string strResult = "That sentence " + string(is_isnot) + " a palindrome.";
    //
    // Pretty print in left aligned space so subsequent pass/fail test
    // results can be reported by the caller easily.
    //
    cout << str << endl;
    cout << setw(42) << left << strResult;
    return actual == expect;
}

//
// Function: isSentencePalindrome
// Usage: if (isSentencePalindrome(str)) cout << ":-)" << endl;
// ------------------------------------------------------------
// Returns boolean true if the provided string adheres to the reversability
// requirements of a string palindrome; meaning, if you filter out non-
// alphanumeric characters, you get a result which is the same when read
// forwards or backwards.
//

bool isSentencePalindrome(string str) {
    string filtered;
    string reversed;
    
    for (int i = 0; i < str.length(); i++) {
        if (isalnum(str[i])) {
            char ch = tolower(str[i]);
            filtered += ch;
            reversed  = ch + reversed;
        }
    }
    return filtered == reversed;
}
