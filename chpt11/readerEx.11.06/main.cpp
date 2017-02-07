//
// main.cpp
//
// This program uses 'Olympic scoring' to average a set of numbers
// where the highest and lowest scores are tossed out.
//
// Notes
//
// Arrays feel clunky after getting used to vectors.  But vectors are built
// from arrays and I'll probably abuse vectors if I don't retain an
// appreciation for the humble array.
//
// Also, I'm playing with passing parameters using pointers instead
// of references as a warm-up to some pointerly stuff on the horizon.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 11, Exercise 6
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 6/16/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include "simpio.h"
#include "strlib.h"
#include "error.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::setprecision;
using std::fixed;
using std::ostringstream;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 11.06\n";
const std::string DETAIL = "Gymanstic Scoring Calculation (Trimmed Mean)";
const std::string BANNER = HEADER + DETAIL;

const double MAX_SCORE = 10.0;  // Maximum valid score.
const double MIN_SCORE = 0.0;   // Minimum valid score.
const int MAX_JUDGES = 100;     // Maximum number of judges allowed.
const int NUM_OUTLIERS = 2;     // Remove highest and lowest scores from calc.
const int MIN_NUM_SCORES = NUM_OUTLIERS + 1;
                                // Trimmed mean tosses out two scores so need
                                // at least 3 for a meaningful result.

// Function Prototypes

void getScores(double scores[], int * pNumScores,
               double * pMinScore, double * pMaxScore);
double trimmedAverage(double scores[], int nScores,
                      double minScore, double maxScore);
void reportAverage(double trimmedAvg, double minScore, double maxScore);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    
    cout << "Enter score for each judge in the range ";
    cout << fixed << setprecision(1) << MIN_SCORE << " to " << MAX_SCORE;
    cout << "." << endl;
    cout << "Enter a blank line to signal the end of the list." << endl << endl;
    cout << "The maximum number of scores you may enter is " << MAX_JUDGES;
    cout << "." << endl;
    cout << "-------------------------------------------------" << endl;
    
    double scores[MAX_JUDGES];
    int nScores;
    double minScore;
    double maxScore;
    
    getScores(scores, &nScores, &minScore, &maxScore);
    double trimmedAvg = trimmedAverage(scores, nScores, minScore, maxScore);
    reportAverage(trimmedAvg, minScore, maxScore);
    
    return 0;
}

// Function: getScores
// Usage: getScores(scores, &nScores, &minScore, &maxScore);
// ---------------------------------------------------------
// Populates an intrinsically pass-by-reference array with individual
// judge's scores by polling the user at the console until a blank
// line is entered or the maximum number of judges is reached.
//
// The number of scores, the minimum score, and the maximum score
// are also tracked and returned through parameter pointers to variables
// defined by the caller.
//
// Input scores are checked to be within the range of MIN_SCORE to MAX_SCORE
// and require re-entry if an out-of-range score is entered.

void getScores(double scores[], int * pNumScores,
               double * pMinScore, double * pMaxScore) {
    
    if (MAX_JUDGES < MIN_NUM_SCORES) {
        error("Code bug: MAX_JUDGES < MIN_NUM_SCORES.");
    }
    
    *pNumScores = 0;
    *pMinScore = MAX_SCORE;
    *pMaxScore = MIN_SCORE;
    
    while (true) {
        ++(*pNumScores);
        if (*pNumScores > MAX_JUDGES) {
            --(*pNumScores);
            break;
        }
        string prompt = "Judge #" + integerToString(*pNumScores) + ": ";
        string strScore = getLine(prompt);
        if (strScore.length() == 0) {
            --(*pNumScores);
            break;
        }
        double score = stringToReal(strScore);
        if (score < MIN_SCORE || score > MAX_SCORE) {
            --(*pNumScores);
            cerr << "Score must be between " << MIN_SCORE;
            cerr << " and " << MAX_SCORE << ".  Please re-enter." << endl;
            continue;
        }
        *pMaxScore = fmax(*pMaxScore, score);
        *pMinScore = fmin(*pMinScore, score);
        scores[*pNumScores - 1] = score;
    }
}

// Function: trimmedAverage
// Usage: double avg = trimmedAverage(scores, nScores, minScore, maxScore);
// ------------------------------------------------------------------------
// Returns the trimmed average of an array of scores by tossing out the
// highest and lowest scores and computing the subsequent average.

double trimmedAverage(double scores[], int nScores,
                      double minScore, double maxScore) {
    
    if (nScores < MIN_NUM_SCORES) {
        string errMsg = "Trimmed average calculation requires at least ";
        errMsg += integerToString(MIN_NUM_SCORES);
        errMsg += " scores.  Found only ";
        errMsg += integerToString(nScores);
        errMsg += ".";
        error(errMsg);
    }
    
    if (NUM_OUTLIERS == nScores) {
        error("Code bug: NUM_OUTLIERS and MIN_NUM_SCORES cause div by 0.");
    }
    
    double total = 0;
    for (int i = 0; i < nScores; i++) {
        total += scores[i];
    }
    
    // Remove min and max outliers from total before computing trimmed avg.
    
    total -= maxScore;
    total -= minScore;
    return total / double(nScores - NUM_OUTLIERS);
}

// Function: reportAverage
// Usage: reportAverage(trimmedAvg, minScore, maxScore);
// -----------------------------------------------------
// Displays a message to the console summarizing the results of the
// trimmed average calculation:
//
// "The average after eliminating 8.80 and 9.30 is 9.03."

void reportAverage(double trimmedAvg, double minScore, double maxScore) {
    cout << "The average after eliminating ";
    cout << fixed << setprecision(2) << minScore << " and " << maxScore;
    cout << fixed << setprecision(2) << " is " << trimmedAvg << "." << endl;
}