//
// TestCombinations.cpp
//
// This program provides a test driver for the Combinations.h interface.
//
// For example:
//
//     ----------------------------------------------
//     Regression testing module Combinations ...
//
//     pass c(5, 2) =  10  (using recursion)
//     pass c(0, 0) =   1  (using recursion)
//     pass c(0, 1) =   0  (using recursion)
//     pass c(7, 3) =  35  (using recursion)
//     pass c(5, 2) =  10  (using iteration)
//     pass c(0, 0) =   1  (using iteration)
//     pass c(0, 1) =   0  (using iteration)
//     pass c(7, 3) =  35  (using iteration)
//
//     :-) All tests passed for module Combinations
//
//                Pascal's  Triangle
//                ------------------
//                       1
//                     1    1
//                   1    2    1
//                 1    3    3    1
//               1    4    6    4    1
//             1    5    10   10   5    1
//           1    6    15   20   15   6    1
//         1    7    21   35   35   21   7    1
//       1    8    28   56   70   56   28   8    1
//     1    9    36   84   126  126  84   36   9    1
//     ----------------------------------------------
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

#include "Combinations.h"

// Types and constants

enum PassFail {
    fail = false,
    pass = true
};

const string MODULE = "Combinations";

const int FAILURE = -1;
const int SUCCESS = 0;
const unsigned ANSWER_WIDTH = 3;        // Pad the answer field with spaces for
                                        // improved alignment of test results.
const unsigned ROWS = 10;               // Number of rows of Pascals Triangle.

// Function prototypes

int regressionTestCombinations();
PassFail testCombinations(unsigned n,
                          unsigned k,
                          unsigned expected,
                          ImplementationT impl = recursion);

string testCombinationsToString(PassFail outcome,
                                unsigned n,
                                unsigned k,
                                unsigned actual,
                                unsigned expected,
                                ImplementationT impl = recursion,
                                unsigned widthModifier = ANSWER_WIDTH);

// Main program

int main(int argc, const char * argv[]) {
    int progResult;
    
    progResult = regressionTestCombinations();
    showPascalsTriangle(ROWS);
    return progResult;
}

// Function definitions

int regressionTestCombinations() {
    bool testResult = pass;
    int regressionResult;
    string module = MODULE;
    
    cout << endl;
    cout << "Regression testing module " << module << " ... " << endl << endl;
    
    unsigned n = 5;
    unsigned k = 2;
    unsigned expect = 10;
    
    testResult &= testCombinations(n, k, expect);
    testResult &= testCombinations(0, 0, 1);
    testResult &= testCombinations(0, 1, 0);
    testResult &= testCombinations(7, 3, 35, recursion);
    testResult &= testCombinations(5, 2, 10, iteration);
    testResult &= testCombinations(0, 0, 1, iteration);
    testResult &= testCombinations(0, 1, 0, iteration);
    testResult &= testCombinations(7, 3, 35, iteration);
    
    cout << endl;
    if (testResult == pass) {
        cout << ":-) All tests passed for module ";
        regressionResult = SUCCESS;
    } else {
        cout << ":-( One or more tests failed for module ";
        regressionResult = FAILURE;
    }
    cout << module << endl;
    return regressionResult;
}

//
// Function: testCombinations
// Usage: PassFail result = testCombinations(n, k, expectedAns);
//        PassFail result = testCombinations(n, k, expectedAns, iteration);
// ------------------------------------------------------------------------
// Returns pass if the expected answer equals the actual answer when
// computing c(n, k).  Otherwise returns fail.
//
// Additional text is written to the output console of this form:
//
//      pass c(5, 2) = 10  (using recursion)
//
// An optional 4th parameter is provided for specifying which underlying
// implementation to test (e.g., recursion versus iteration).
// The default is recursion.
//

PassFail testCombinations(unsigned n,
                          unsigned k,
                          unsigned expected,
                          ImplementationT impl) {
    PassFail outcome;
    
    unsigned actual = c(n, k, impl);
    outcome = (actual == expected) ? pass : fail;
    
    string testReport = testCombinationsToString(outcome, n, k, actual,
                                                 expected, impl);
    cout << testReport << endl;
    
    return outcome;
}

//
// Function: testCombinationsToString
// Usage: string testReport = testCombinationsToString(outcome, n, k, actual
//                              expected, impl);
// -------------------------------------------------------------------------
// Formats the results of a single test of c(n, k) as a one-line
// report of the form:
//
//      "pass c(5, 2) = 10  (using recursion)"
//  or
//      "fail c(7, 3) = 34  (using recursion) expected: c(7, 3) = 35"
//

string testCombinationsToString(PassFail outcome,
                                unsigned n,
                                unsigned k,
                                unsigned actual,
                                unsigned expected,
                                ImplementationT impl,
                                unsigned widthModifier) {
    ostringstream output;
    string outcomeStr;
    string methodStr;
    
    outcomeStr = (outcome == pass) ? "pass" : "fail";
    methodStr = (impl == recursion) ? "recursion" : "iteration";
    
    output << outcomeStr << " " << cnkToString(n, k, actual, widthModifier)
           << "  (using " << methodStr << ")";
    if (outcome == fail) {
        output << " expected: " << cnkToString(n, k, expected, widthModifier);
    }
    return output.str();
}
