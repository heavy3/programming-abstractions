//
// main.cpp
//
// This program generates a table comparing the performance of two
// algorithms, linear and binary search, when used to find a randomly chosen
// integer key in a sorted vector of integers.
//
// Notes
//
// Test at scale!  This seemed to be working fine for small N, using the
// data generation functions from 10.04.  However at N = 100000, I noticed
// linear performance sometimes was either significantly larger or smaller
// than the expected N/2 average.  Turns out I was only running 1 trial at
// large N because this was adequate (and even desired) in the earlier sorting
// problem where I grabbed the trial harness code for reuse.
//
// Here, though, I needed multiple trials at all N to get good average
// performance on the search of random keys.  Killing off the sliding scale
// and using a simple fixed number of trials (i.e., 100) resolved the issue.
//
// Also, I noticed a performance bias for ascending data versus descending
// data.  See the binarySearchR() description header for discussion.
//
// TODO Ideally we'd use the exact same (random) key against both search
//      functions for each trial.  For now, I'm relying upon a large-ish
//      number of trials to ensure the average key value is the same for
//      similar sized trials of the two search functions.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 10, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/29/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "vector.h"
#include "random.h"
#include "error.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 10.05\n";
const std::string DETAIL = "Performance of linear and binary search";
const std::string BANNER = HEADER + DETAIL;
const int NUM_TRIALS = 100; // Number of trials for each data set.
const int NO_MATCH = -1;    // Value returned if key is not found in input data.

// Types

typedef int (*searchFnT)(const int & key, const Vector<int>& data, int & index);

enum InputCondT {
    ASCENDING,              // Input data already sorted.
    DESCENDING,             // Input data reverse sorted.
    LAST_CONDITION,         // Terminate post increment iterator.
    SKIPPING,               // Unable generate data under current constraints.
};

struct ResultT {
    int N;                  // Size of input data.
    double linComp;         // Number of comparisons with linear search.
    double binComp;         // Number of comparisons with binary search.
    InputCondT cond;        // Is input data ascending or descending?
    int numTrials;          // Number of trials for each data set.
};

typedef Vector<ResultT> Report;

// Prototypes

int binarySearch(const int & key, const Vector<int> & data, int & compares);
int binarySearchR(const int & key, const Vector<int> & data, int begin,
     int end, int & compares);
void getData(Vector<int> & data, const int N, const InputCondT cond,
     ResultT & result);
double getSearchCompares(const Vector<int> & data, searchFnT fn, int numTrials);
int linearSearch(const int & key, const Vector<int> & data, int & compares);
void runSearch(searchFnT fn, const Vector<int> & data, ResultT & result);
ostream & operator<<(ostream & os, const InputCondT cond);
ostream & operator<<(ostream & os, searchFnT fn);
ostream & operator<<(ostream & os, const Report & report);
InputCondT operator++(InputCondT & cond, int);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    Vector<int> N;  // Predefined input data sizes.
    N += 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000;
    
    Vector<searchFnT> searchFns;
    searchFns += linearSearch, binarySearch;
    
    InputCondT cond = ASCENDING;
    Report report;
    for (int n: N) {
        ResultT result;
        Vector<int> data;
        
        getData(data, n, cond, result);
        for (searchFnT searchFn : searchFns) {
            runSearch(searchFn, data, result);
        }
        report.add(result);
    }
    cout << report << endl;
    return 0;
}

//
// Function: runSearch
// Usage: runSearch(searchFn, data, result);
// -----------------------------------------
// Runs a search simulation for a random key within a sorted vector of integers.
//
// Results are reported through a pass-by-reference result structure.
//
// The degree of randomness seen in the input data may be specified
// by an optional condition variable.
//

void runSearch(searchFnT searchFn, const Vector<int> & data, ResultT & result) {
    
    int numTrials = NUM_TRIALS;
    result.numTrials = numTrials;
    double compares = getSearchCompares(data, searchFn, numTrials);
    
    if (searchFn == linearSearch) {
        result.linComp = compares;
    } else if (searchFn == binarySearch) {
        result.binComp = compares;
    } else {
        error("runSearch(): Unknown search function.");
    }
}

//
// Function: getSearchCompares
// Usage: int compares = getSearchCompares(data, linearSearch);
// --------------------------------------------------------------
// Returns the elapsed time in seconds for sorting an input vector.
//
// Can be configured to return the overhead associated with with
// running the simulation over multiple repetitions.
//

double getSearchCompares(const Vector<int> & data, searchFnT searchFn,
                         int numTrials) {

    if (numTrials <= 0) {
        error("getSearchCompares(): numTrials not > 0");
    }
    
    double totCompares = 0;
    for (int i = 0; i < numTrials; i++) {
        int compares;
        int key = randomInteger(0, data.size() - 1);
        searchFn(key, data, compares);
        totCompares += compares;
    }
    return totCompares / double(numTrials);
}

//
// Function: linearSearch
// Usage: int index = linearSearch(key, data, numCompares);
// --------------------------------------------------------
// Searches an input vector of sorted integer data for the presence of a key.
// If the key is found, the corresponding vector index is returned.  Otherwise
// the sentinel value, NO_MATCH (= -1) is returned.
//
// In either event, a count of the number of comparisons taken during
// the search against the input vector is returned in a pass-by-reference
// variable.
//
// The algorithm is a simple linear traversal of the input, comparing the key
// to elements of the data vector until a match is found or the end of the
// vector is reached.
//
// Performance
//
// For random-generated keys over a uniformly distributed range of input
// the average number of compares before a match is found is approximately
// N/2, or O(N), where N is the number of elements in the input vector.
//
// Memory usage is O(1) since only a single local index variable is required.
// All the other parameters are pass-by-reference.
//

int linearSearch(const int & key, const Vector<int> & data, int & compares) {
    int index = NO_MATCH;
    compares = 0;
    
    for (int i = 0; i < data.size(); i++) {
        compares += 1;
        if (key == data[i]) {
            index = i;
            break;
        }
    }
    return index;
}

//
// Function: binarySearch
// Usage: int index = binarySearch(key, data, numCompares);
// --------------------------------------------------------
// Searches an input vector of sorted integer data for the presence of a key.
// If the key is found, the corresponding vector index is returned.  Otherwise
// the sentinel value, NO_MATCH (= -1) is returned.
//
// In either event, a count of the number of comparisons taken during
// the search against the input vector is returned in a pass-by-reference
// variable.
//
// This function is a wrapper script for a binarySearchR function that uses
// a divide and conquer strategy.
//
// Performance
//
// Runtime and memory performance are both O(logN).  See binarySearchR for
// more analysis.
//
// Limitations
//
// This algorithm assumes the input data are sorted.
//

int binarySearch(const int & key, const Vector<int> & data, int & compares) {
    int index = NO_MATCH;
    
    compares = 0;
    if (data.size() == 0) return index;
    
    return binarySearchR(key, data, 0, data.size() - 1, compares);
}

//
// Function: binarySearchR
// Usage: int index = binarySearchR(key, data, p1, p1, numCompares);
// -------------------------------------------------------------------------
// Looks for a matching key within the input data vector.  Returns the
// index if the value is found, otherwise returns -1.
//
// Also returns the number of compares, in a pass-by-reference parameter,
// required to find the match or conclude that no match is possible.
//
// Uses a RECURSIVE divide-and-conquer strategy that compares the key
// to the value at a synthetic midpoint of the vector relative to managed
// indices that biset the input vector (either from the left or the right)
// until a match is found or the left and right indices cross over.
//
// Runtime Performance
//
// The runtime performance of the algorithm is proportional to log2(N), or
// O(logN), since the size of the considered region of the vector is halved
// with each call.
//
// As implemented, there is a modest performance gain when input data are
// sorted in ascending order (versus descending order).
//
// The 10% - 15% penalty I'm seeing for descending order data arises from the
// mistaken assumption that
//
//          key < data[mid]
//
// implies priority be given to searching lower regions of input before upper
// regions.  The opposite is true when the input vector elements are in
// descending order.
//
// Memory Utilization
//
// The space used is O(logN) since the managed indices are pass-by-value
// and each level of recursion requires two new indices plus a local
// midpoint variable, or 3*log2(N).
//

int binarySearchR(const int & key, const Vector<int> & data,
                  int p1, int p2, int & compares) {
    
    if (p1 > p2) return NO_MATCH;
    int mid = (p1 + p2) / 2;
    
    compares += 1;
    if (key == data[mid]) return mid;
    
    if (key < data[mid]) // Don't count this compare if you want your
                         // results to match the results in the text. ;-)
        return binarySearchR(key, data, p1, mid - 1, compares);
    else
        return binarySearchR(key, data, mid + 1, p2, compares);
}

//
// Function: getData
// Usage: getData(data, N, ASCENDING, result);
// --------------------------------------------
// Populates a pass-by-reference data vector with N integers sorted
// in ascending or descending order.
//

void getData(Vector<int> & data, const int N, const InputCondT cond,
             ResultT & result){
    
    data.clear();
    switch (cond) {
        case DESCENDING:
            for (int n = N - 1; n >= 0; n--) data.add(n);
            break;
            
        case ASCENDING:
        default:
            for (int n = 0; n < N; n++) data.add(n);
            break;
    }
    result.cond = cond;
    result.N = data.size();
}

//
// Operator: <<
// Usage: cout << report << endl;
// ------------------------------
// Displays formatted results from multiple runs of a sort function under
// conditions of increasing scale.  Input data condition is also reflected.
//

ostream & operator<<(ostream & os, const Report & report) {
    os << "Input data are in " << report[0].cond << " order." << endl;
    os << "Random trials per N-sized dataset: " << report[0].numTrials << endl << endl;
    os << "          | Avg # of Compares" << endl;
    os << "----------+-------------------" << endl;
    os << "Data Size |  Linear  | Binary " << endl;
    os << "     N    |  Search  | Search " << endl;
    os << "----------+----------+--------" << endl;

    for (ResultT rec : report) {
        os << right;
        os << setw(8)  << rec.N << "  |";
        os << setw(8) << fixed << setprecision(1) << rec.linComp << "  |";
        os << setw(6) << fixed << setprecision(1) << rec.binComp << endl;
    }
    return os;
}

//
// Operator: <<
// Usage: cout << inputConditions << endl;
// ---------------------------------------
// Displays the string equivalents for the enumerated InputCondT type
// which characterizes the input data.
//

ostream & operator<<(ostream & os, const InputCondT cond) {
    switch (cond) {
        case DESCENDING:
            os << "descending";
            break;
            
        case SKIPPING:
            os << "skipping";
            break;
            
        case ASCENDING:
        default:
            os << "ascending";
            break;
    }
    return os;
}

//
// Operator: <<
// Usage: cout << sortfn << endl;
// ------------------------------
// Displays the string equivalents for the specified search function.
//

ostream & operator<<(ostream & os, searchFnT fn) {
    if (fn == linearSearch) {
        os << "Linear";
    } else if (fn == binarySearch) {
        os << "Binary";
    }else {
        os << "Unknown";
    }
    return os;
}

//
// Operator: ++ (InputCondT post-increment)
// Usage: for (InputCondT cond = ASCENDING; cond <= DESCENDING; cond++) {...}
// --------------------------------------------------------------------------
// Defines a post-increment operator for the input data conditioning type,
// InputCondT.
//

InputCondT operator++(InputCondT & cond, int) {
    InputCondT old = cond;
    if (cond < LAST_CONDITION) cond = InputCondT(cond + 1);
    return old;
}