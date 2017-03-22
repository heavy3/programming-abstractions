//
// main.cpp
//
// This program exercises my reinvention of the string class, MyString.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 12, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/18/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "error.h"
#include "MyString.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 12.09\n";
const std::string DETAIL = "MyString Class";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main() {
    cout << BANNER << endl;
    
    MyString myStr1("a");
    MyString myStr2("aa");
    
    assert(myStr1 < myStr2);
    assert(myStr2 <= myStr2);
    assert(myStr2 > myStr1);
    assert(myStr2 >= myStr2);
    assert(myStr1 != myStr2);
    assert(myStr1 == myStr2.substr(1, 1));
    assert(myStr1[0] == 'a');
    
    MyString myStr2b = myStr2;
    assert(myStr2b == myStr2);
    assert(myStr1.length() == 1);
    assert(myStr2.length() == 2);
    
    MyString myStr3 = myStr1 + myStr2;
    MyString myStr3a("aaa");
    assert(myStr3 == myStr3a);
    
    myStr3 = myStr1;
    myStr3 += myStr2;
    assert(myStr3 == myStr3a);
    myStr1[0] = 'b';
    assert(myStr1 > myStr2);
    assert(myStr2 <= myStr1);
    cout << "Unit tests passed." << endl;
    return 0;
}