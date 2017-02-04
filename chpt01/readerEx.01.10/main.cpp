//
// main.cpp
//
// Write a program that reads in a positive number and uses that to generate
// a so called 'hailstone sequence' according to the following rules:
// 
// • If n is equal to 1, you’ve reached the end of the sequence and can stop.
// • If n is even, divide it by two.
// • If n is odd, multiply it by three and add one.
//
// It's called the hailstone sequence because the values tend to go up and 
// and down before eventually converging to 1, similar to the dynamic behavior
// of hailstones in the clouds as they form.
// 
// For example:
//
//    Enter a number: 15
//    15 is odd, so I multiply by 3 and add 1 to get 46
//    46 is even, so I divide it by 2 and get 23
//    23 is odd, so I multiply by 3 and add 1 to get 70
//    70 is even, so I divide it by 2 and get 35
//    35 is odd, so I multiply by 3 and add 1 to get 106
//    ...
//    4 is even, so I divide it by 2 to get 2
//    2 is even, so I divide it by 2 to get 1
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 1, Exercise 10
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/14/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
using namespace std;

void hailstone(int & num);
void hailstoneEven(int & num);
void hailstoneOdd(int & num);

int main(int argc, char * argv[]) {
    int n = 0;
    
    while (n < 1) {
        cout << "I compute the hailstone sequence for a number (greater than 1)." << endl;
        cout << "Enter a number: ";
        cin >> n;
    }
    
    while (n > 1) {
        hailstone(n);
    }
    return 0;
}

void hailstone(int & num) {
    (num % 2) ? hailstoneOdd(num) : hailstoneEven(num);
}

void hailstoneEven(int & num) {
    cout << num << " is even, so I divide it by 2 to get ";
    num /= 2;
    cout << num << endl;
}

void hailstoneOdd(int & num) {
    cout << num << " is odd, so I multiply it by 3 and add 1 to get ";
    num = (num * 3) + 1;
    cout << num << endl;
}