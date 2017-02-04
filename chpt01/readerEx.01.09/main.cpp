//
// main.cpp
//
// This program displays the prime factorizationn of a number, N.
//
// For example:
//
//    This program factors a number.
//    Enter number to be factored: 60
//    2 x 2 x 3 x 5
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 1, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/14/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
using namespace std;

int main(int argc, char * argv[]) {
    int n = -1;
    string factors = "";
    
    while (n < 0) {
        cout << "This program factors a positive number." << endl << "Enter number to be factored: ";
        cin >> n;
    }
    
    switch (n) {
        case 0:
        case 1:
            factors.append(to_string(n));
        default:
            
            // Algorithm: Search for prime factors by not knowing about primes! :-)
            //
            // Find first factor of n using divisors that range from 2 to n that yield 0 remainder.
            //
            // Save off the factor.
            //
            // Reduce n by diving out that factor and repeat process until n has dwindled
            // to 1, implying all factors have been harvested.

            while (n > 1) {
                for (int f = 2; f <= n; f++) {
                    int r = n % f;
                    if (r == 0) {
                        if (factors.length() > 0) {
                            factors.append(" x ");
                        }
                        factors.append(to_string(f));
                        n = n / f;
                        break;
                    }
                }
            }
    }
    
    cout << factors << endl;
    return 0;
}