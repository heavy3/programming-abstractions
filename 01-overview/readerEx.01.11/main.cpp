//
// main.cpp
//
// This program calculates an approximation of pi using the first 10,000
// terms of the infinite series (courtesy of the mathematician, Leibniz):
//
//    pi/4 = 1 - 1/3 + 1/5 - 1/7 + 1/9 ...
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 1, Exercise 11
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/14/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
using namespace std;

const int seriesTerms = 10000;

int main(int argc, char * argv[]) {
    double piOver4 = 0.0;
    string termString = (seriesTerms == 1) ? "term" : "terms";
    
    for (int i = 0; i < seriesTerms; i++) {
        int divisor = (i * 2) + 1;
        int sign = (i % 2) ? -1 : 1;
        
        piOver4 += (sign) * (1/double(divisor));
    }
    
    cout << "When using " << seriesTerms << " " << termString << " in the Leibniz series," << endl;
    cout << "pi ~= " << piOver4 * 4.0 << endl;
    return 0;
}