//
// main.cpp
//
// Customize the STL's sort algorithm to perform a case-insensitive,
// 'title sort' of two string where the following initial articles are
// ignored: a, an, or the.
//
// TODO: Spidey sense says I should be leveraging STL more.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 11
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
#include <iomanip>
#include <cassert>
#include "strlib.h"
#include "error.h"
#include "set.h"
using namespace std;

// Constants and types

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 20.11\n";
const std::string DETAIL = "Title Comparison Function";
const std::string BANNER = HEADER + DETAIL;

// Function prototypes

bool titleComesBefore(const std::string& t1, const std::string& t2);
void testDriver();
int findPrefix(const std::string& str, const std::string& prefix);
std::string normalizeTitle(const std::string& title);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    testDriver();
    return 0;
}

void testDriver() {
    std::string t1 = "The Great Gatsby";
    std::string t2 = "Frankenstein";
    
    assert(titleComesBefore(t2, t1));
    
    t1 = "A Miracle on 34th Street";
    t2 = "It's A Wonderful Life";
    assert(!titleComesBefore(t1, t2));

    cout << "Passed all unit tests." << endl;
}

// Function: titleComesBefore
// Usage: if (titleComesBefore("Some Title", "Another Title")) { . . . }
// ---------------------------------------------------------------------
// Returns true if the first title is lexigraphically less than the
// second title, ignoring case an preceding articles.
//
// This provides the callback required by one form of the STL's sort
// function.

bool titleComesBefore(const std::string& t1, const std::string& t2) {
    
    bool result = false;
    cout << "Raw Titles" << endl;
    cout << "------------------------------------------" << endl;
    cout << t1 << endl;
    cout << t2 << endl << endl;
    
    std::string n1 = normalizeTitle(t1);
    std::string n2 = normalizeTitle(t2);
    cout << "Normalized Titles" << endl;
    cout << "------------------------------------------" << endl;
    cout << n1 << endl;
    cout << n2 << endl << endl;
    
    for (std::string::iterator it1 = n1.begin(), it2 = n2.begin();
         it1 != n1.end();
         it1++, it2++) {
        
        // Reached end of t2 before end of t1, so t1 > t2.
        // Essentially performaing (*it1 < '') which is always false.
        
        if (it2 == n2.end()) break;
        if (*it1 < *it2) {
            result = true;
            break;
        } else if (*it1 > *it2) {
            break;
        }
    }
    cout << "First title lexigraphically comes before second: ";
    cout << boolalpha << result << endl << endl;
    
    return result;
}

// Function: normalizeTitle
// Usage: std::string normTitle = normalizeTitle(titleStr);
// --------------------------------------------------------
std::string normalizeTitle(const std::string& title) {
    
    Set<std::string> prefixSet;
    prefixSet.add("a");
    prefixSet.add("an");
    prefixSet.add("the");
    
    // Strip leading articles that are safe to ignore.
    std::string result = toLowerCase(title);
    int pos = string::npos;
    for (std::string prefix: prefixSet) {
        if ((pos = findPrefix(title, prefix)) != string::npos) {
            result = result.substr(pos);
            break;
        }
    }
    
    // Leading and trailing whitespace.
    result = trim(result);
    
    // Strip punctuation
    std::string normTitle;
    for (int i = 0; i < result.size(); i++) {
        if (ispunct(result[i])) continue;
        normTitle += result[i];
    }
    
    return normTitle;
}

// Function: findPrefix
// Usage: int pos = findPrefix("A Walk in the Park", prefixSet);
// -------------------------------------------------------------
// Searches a string against a set of prefix words (e.g., a, an, the).
// If the prefix is found, a position index pointing to the first
// character -after- the prefix is returned.  Otherwise string::npos
// is returned.

int findPrefix(const std::string& str, const std::string& prefix) {
    if (str.length() < prefix.length()) return string::npos;
    for (int i = 0; i < prefix.length(); i++) {
        if (tolower(str[i]) != tolower(prefix[i])) return string::npos;
    }
    return prefix.size();
}
