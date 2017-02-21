//
// Combinations.cpp
//
// This file implements recursive and iterative versions of the cominations
// function:
//
//     c(n, k) = n! / (k! (n-k)!)
//
// --------------------------------------------------------------------------
// Assignment: 1, Part 2 Combinations
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/050%20Assignment%201.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/15/2015.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

//#define SMOKE_TESTING     // Uncomment this for a quick smoke test.

#include "Combinations.h"

// Private function prototypes

unsigned combinationsR(unsigned nItems, unsigned chooseK);
unsigned combinationsI(unsigned nItems, unsigned chooseK);

// Main program

#ifdef SMOKE_TESTING
int main() {
    cout << cnkToString(6, 2, c(6, 2)) << endl;
    showPascalsTriangle(10);
    return 0;
}
#endif

//
// Implementation notes: c
// ----------------------------------
// Returns the number of combinations one can choose k things from a
// collection of n items without replacement.  The order in which the items are
// chosen is insignificant.
//
// In addition to the n and k parameters, an optional implementation parameter
// is provided to select whether a recursive or iterative approach is used.
//
// The default is recursion given the spirit of the specification which is to
// explore a purely recursive solution while leveraging an iterative
// implementation as cross-check in the test driver.
//

unsigned c(unsigned n, unsigned k, ImplementationT impl) {
    unsigned result;
    
    switch (impl) {
        case recursion:
            result = combinationsR(n, k);
            break;
            
        case iteration:
        default:
            result = combinationsI(n, k);
            break;
    }
    return result;
}

//
// Implementation notes: combinationsR
// -----------------------------------
// Implements the classic combinations function:
//
// c(n, k) = n! / (k! (n-k)!)
//
// using recursion and properties from Pascal's triangle, the entries of which
// represent combinations for given values of n and k arranged in a generational
// trianglular shape.
//
// From inspection, one can compute an entry from the triangle by taking the sum
// of the left and right ancestor entries in the row above.  The top-most and
// exterior-facing entries are always 1, providing the terminal case for which
// the recursion bottoms out before working its way back through the stack to a
// final answer from the original call.
//

unsigned combinationsR(unsigned n, unsigned k) {
    if (k > n) return 0;    // No way to choose k items when k > n without
                            // replacement.
    
    if (k == 0) return 1;   // Left side of Pascal's triangle.
    if (n == k) return 1;   // Right side of Pascal's triangle.
    
    // Interior entries of Pascal's triangle may be computed from the
    // left and right entries in the row above.
    
    unsigned lAncestor = combinationsR(n - 1, k - 1);
    unsigned rAncestor = combinationsR(n - 1, k);
    
    return lAncestor + rAncestor;
}

//
// Implementation notes: combinationsI
// -----------------------------------
// Implements the classic combinations function:
//
// c(n, k) = n! / ((n-k)! * k!)
//
// using iteration.
//
// Care is taken to factor out k! from numerator and denomenator before
// evaluating the overall expression to minimize overflow risk.
//

unsigned combinationsI(unsigned n, unsigned k) {
    if (k > n) return 0;    // No way to choose k items when k > n without
                            // replacement.
    
    unsigned numerator = 1;
    unsigned denomenator = 1;

    //
    // Factor out k! from numerator and denomenator to improve
    // performance and reduce risk of factorial overflow.
    //
    // e.g., c(n, k) = n! / ((n-k!) * k!)
    //
    //       c(5, 3) =  5 * 4 * (3 * 2 * 1)    =   5 * 4
    //                 ----------------------     --------
    //                 (5 - 3)! * (3 * 2 * 1)     (5 - 3)!
    //
    // NB: n - k = number of factors in numerator and denomenator
    //           = number of times to loop
    //
    
    for (unsigned nmk = (n - k); nmk > 0; nmk--) {
        numerator *= n--;
        denomenator *= nmk;
    }
    return numerator / denomenator;
}

//
// Implementation notes: showPascalsTriangle
// -----------------------------------------
// Displays the first n-rows of Pascal's Triangle to the console.
//
// Limitations:
//
// Formatting is approximate and suitable for n <= 15.  Beyond that,
// the triangle shape breaks down as I'm using a fixed width of 4
// digits maximum for each entry in the table for ease of implementation
// while still meeting the specification to support output for n = 10.
//

void showPascalsTriangle(unsigned rows) {

    string banner = "Pascal's  Triangle";
    string line = string(banner.length(), '-');
    int maxDigits = 4; // Number of digits to allow for largest entry.
    string leadingSpace = string((rows * maxDigits - banner.length()) / 2, ' ');
    
    cout << endl << leadingSpace << banner << endl;
    cout << leadingSpace << line << endl;

    for (unsigned n = 0; n < rows; n++) {
        leadingSpace = string(((rows - n) * maxDigits) / 2, ' ');
        cout << leadingSpace;
        for (unsigned k = 0; k <= n; k++) {
            cout << setw(maxDigits) << left << c(n, k);
            cout << " ";
        }
        cout << endl;
    }
}

//
// Implementation notes: cnkToString
// ---------------------------------
// Converts numeric values associated with c(n, k) to string form:
//
//      "c(n, k)"
//
// using ostringstream.
//

string cnkToString(unsigned n, unsigned k) {
    ostringstream output;
    string result;
    
    output << "c(" << n << ", " << k << ")";
    return output.str();
}

//
// Implementation notes: cnkToString
// ---------------------------------
// Converts numeric values associated with c(n, k) to string form:
//
//       "c(n, k) = answer"
//
// using ostringstream.
//

string cnkToString(unsigned n, unsigned k, unsigned answer, unsigned width) {
    ostringstream output;
    string result;
    
    output << cnkToString(n, k) << " = " << setw(width) << answer;
    return output.str();
}
