//
// main.cpp
//
// This program reads in an integer and display the number
// with the digits reversed.
//
// For example:
//
//    This program reverses the digits in an integer.
//    Enter a positive integer: 123456789
//    The reversed integer is 987654321
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 1, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/13/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
using namespace std;

int reverseDigits(int n);

int main(int argc, char * argv[]) {
    int posInt;
    
    cout << "This program reverses the digits in an interger." << endl;
    cout << "Enter a positive integer: ";
    cin >> posInt;
    
    cout << "The reversed integer is " << reverseDigits(posInt) << endl;
    return 0;
}

int reverseDigits(int n) {
    int newNum = 0;
    while (n > 0) {
        newNum = (newNum * 10) + (n % 10);
        n /= 10;
    }
    return newNum;
}