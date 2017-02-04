//
// main.cpp
//
// This program implements a function, reverse(string str), which uses
// recursion to reverse a string.
//
// For example,
//
//    reverse("program") returns "margorp"
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 7, Exercise 10
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/29/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "simpio.h"
#include "error.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 7.10\n";
const std::string DETAIL = "Recursive String Reversal";
const std::string BANNER = HEADER + DETAIL;

std::string reverse(std::string str);

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    std::cout << "Recursive String Reversal\n";
    
    while (true) {
        std::string str = getLine("Enter string to reverse: ");
        if  (str == "") break;
        std::cout << reverse(str) << std::endl << std::endl;
    }
    return 0;
}

//
// Function: reverse
// Usage: str = reverse("reverse_me");
// -----------------------------------
// Returns an input string in reverse character order.
//
// The algorithm proceeds by transposing the last character to the
// front of a new string and then appending the recursively reversed
// remaining substring.
//

std::string reverse(std::string str) {
    int len = str.length();
    
    if (len < 2) {
        return str;
    } else {
        return str[len - 1] + reverse(str.substr(0, len - 1));
    }
}
