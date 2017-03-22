//
// main.cpp
//
// This program exercises a linked-list based string class.
//
// For example:
//
//    CS106B Programming Abstractions in C++: Ex 14.12
//    Linked-list based MyString Class
//    a
//    ab
//    ab
//    abab
//    ababab
//    Unit tests passed.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/4/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "error.h"
#include "MyString.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 14.12\n";
const std::string DETAIL = "Linked-list based MyString Class";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main() {
    cout << BANNER << endl;
    
    MyString myStr1("a");
    MyString myStr2("ab");
    assert(myStr1.length() == 1 && myStr2.length() == 2);
    
    cout << myStr1 << endl;
    cout << myStr2 << endl;
    
    myStr1 = myStr2;            // Exercise deep copy.
    assert(myStr1.length() == myStr2.length());
    cout << myStr1 << endl;
    assert(myStr1 == myStr2);
    assert(myStr1[1] == 'b');
    myStr1 += myStr2;
    cout << myStr1 << endl;
    assert(myStr1 == MyString("abab"));
    MyString myStr3 = myStr1 + myStr2;
    cout << myStr3 << endl;
    assert(myStr3 == MyString("ababab"));
    MyString myStr4 = myStr3.substr(1, 4);
    assert(myStr4 == MyString("baba"));
    MyString myStr5 = myStr3.substr(1);
    assert(myStr5 == MyString("babab"));
    assert(myStr4 < myStr5);
    cout << "Unit tests passed." << endl;
    return 0;
}
