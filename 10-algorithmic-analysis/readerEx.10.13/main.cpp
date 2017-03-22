//
// main.cpp
//
// TODO: Replace O(n^2) medianValue function with O(n) version to fully
//       meet the problem specifications.
//
// This program implements the function:
//
//    int findMajorityElement(Vector<int> & vec);
//
// which takes a vector of nonnegative integers and returns the majority
// element, which is the value that occurs in an absolute majority
// (at least 50 percent plus one) of the element positions. If no majority
// element exists, the function should return –1 to signal that fact.
//
// It attempts to meet the following constraints (with asterisk):
//
//  • It must run in O(N) time.
//
//  • It must use O(1) additional space. In other words, it may use individual
//    temporary variables but may not allocate any additional array or vector
//    storage. Moreover, this condition rules out recursive solutions, because
//    the space required to store the stack frames would grow with the depth of
//    the recursion.
//
//  • It may not change any of the values in the vector.
//
// Notes
//
// Ok, so I've cheated with this solution.  I've got a big,
// 'insert-special-code-here' sign in the middle of the solution.
//
// See: medianValue()
//
// Namely, I really still need an O(n) algorithm for finding the median of an
// unsorted vector of non-negative integers in O(1) space that doesn't
// involve mutating the input data.  Provided that exists, my algorithm
// meets the performance requirements.  For now, i'm getting by (for proof of
// concept) with an O(n^2) worst case runtime and O(n) space algorithm
// for the essential median part of the overall algorithm.
//
// Here's the clever idea:
//
// The median value of the list will be a majority element value
// (if one exists).  Provided there is a way to determine the median
// value from the unsorted list in O(n) time and O(1) space,
// it's just a matter of summing the number of instances of
// that value and comparing that to the overall size of the input to
// discern if it is a majority element.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 10, Exercise 13
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 5/22/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include "vector.h"
#include "error.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 10.13\n";
const std::string DETAIL = "Majority Element, O(n)";
const std::string BANNER = HEADER + DETAIL;

typedef int(*pPivotFn)(Vector<int>&, int, int);

// Prototypes

int findMajorityElement(Vector<int> & vec);
double medianValue(const Vector<int> d);
int fastPivot(Vector<int> & data, const int p1, const int p2);
double medianPivot(Vector<int> & data, const int p1, const int p2);
int partition(Vector<int> & data, const int p1, const int p2,
              const pPivotFn = fastPivot);
void quickSort(Vector<int> & data);
void quickSortR(Vector<int> & data, const int p1, const int p2,
                const pPivotFn = fastPivot);

// Main program

int main() {
    cout << BANNER << endl << endl;

    Vector<int> d1;
    d1 += 3, 5, 9, 5, 5, 2, 5;
    cout << "input data: " << d1 << endl;
    cout << "majority element: " << findMajorityElement(d1) << endl << endl;

    Vector<int> d2;
    d2 += 3, 560, 1, 99999, 2, 1;
    cout << "input data: " << d2 << endl;
    cout << "majority element: " << findMajorityElement(d2) << endl << endl;
    
    Vector<int> d3;
    d3 += 9, 3, 7, 4, 3, 3, 3;
    cout << "input data: " << d3 << endl;
    cout << "majority element: " << findMajorityElement(d3) << endl << endl;
    
    return 0;
}

// Function: findMajorityElement
// Usage: int majorityElement = findMajorityElement(&data);
// --------------------------------------------------------
// Returns the majority element, if it exists, of a vector of non-negative
// integers.

int findMajorityElement(Vector<int> & data) {
    int medval = medianValue(data);
    int count = 0;
    for (int i = 0; i < data.size(); i++) {
        if (medval == data[i]) {
            count++;
        }
        if (count > data.size() / 2) {
            return medval;
        }
    }
    return -1;
}

// ** THIS NEEDS TO BE REPLACED WITH A MYSTICAL O(n) R/T and O(1) SPACE
// ** IMPLEMENTATION TO FULLY SATISFY THE PROBLEM CONSTRAINTS.
//
// Function: medianValue
// Usage: double median = medianValue(data);
//        double median = medianValue(data, &medIndices);
// ------------------------------------------------------
// ** TEMPORARY IMPLEMENTATION (To be replaced by O(N) algroithm later.) **
//
// Returns the median value for a vector of unsorted integers.
//
// NB: This implementation doesn't meet the constraints of the problem.
//     It's here just to sketch out a solution based on the concept of
//     finding a median as a partition value for use in finding the
//     majority element.
//

double medianValue(const Vector<int> d) {
    Vector<int> dCopy(d);
    double medVal;
    
    if (dCopy.size() % 2 == 0) {
        dCopy.remove(dCopy.size() - 1); // make odd number list to get exact
                                        // median
    }
    quickSort(dCopy);
    int m = round(dCopy.size() / 2.0) - 1;
    medVal = dCopy[m];
    return medVal;
}

// Function: quickSort, quickSortMP
// Usage: quickSort(&data);
//        quickSortMP(&data);
//
// Attribution: Programming Abstractions in C++ by Eric Roberts
// ------------------------------------------------------------
// Returns a sorted collection of integers in a pass-by-reference vector.
// This is a wrapper for the recursive quickSortR function.
//
// quickSort   uses the first item at the low index as the pivot
// quickSortMP uses median of the first, middle, & high index values as a pivot
//

void quickSort(Vector<int> & data) {
    quickSortR(data, 0, data.size() - 1);
}

// Function: quickSortR
// Usage: quickSortR(&data, firstIndex, lastIndex);
//        quickSortR(&data, firstIndex, lastIndex, pivotFunction);
// Attribution: Programming Abstraction in C++ by Eric Roberts
// ----------------------------------------------------------------
// Returns a sorted collection of integers in a pass-by-reference vector.
//
// Employs Tony Hoare's classic algorithm for sorting elements by first
// partitioning the input into two rougly sorted groups of smaller and larger
// items separated by a single sorted value called the pivot.
//
// These groups are themselves subdivided into smaller groups with their own
// sorted pivot values until the entire collection is sorted.  Two indices
// mark the extent of the input vector under consideration the current level
// of recursion.
//
// If one had a priori knowledge of the input data, you would pick the
// median value as your pivot to get the best performance from this algorithm.
// As it is, the pivot selection defaults to the item pointed to
// by the first managed index.
//
// An optional pivot function parameter may be passed in for alternate pivot
// selection strategies.
//

void quickSortR(Vector<int> & data, const int p1, const int p2,
                const pPivotFn pivotFn) {
    
    if (p1 >= p2) return;
    int boundary = partition(data, p1, p2, pivotFn);
    quickSortR(data, p1, boundary - 1, pivotFn);
    quickSortR(data, boundary + 1, p2, pivotFn);
}

//
// Function: partition
// Usage: int boundary = partition(&data, p1, p2)
//        int boundary = partition(&data, p1, p2, pivotFn);
// Attribution: Programming Abstrations in C++ by Eric Roberts
// -----------------------------------------------------------
// This functions returns a partially sorted region of a vector
// of integers.  A pivot value is selected within the region bounded
// by two vector indices.  Then lefthand and righthand indices advance
// toward each other, swapping data items that are smaller or
// larger than the pivot value.  Once the two indices meet, the pivot
// value is positioned
//

int partition(Vector<int> & data, const int p1, const int p2,
              const pPivotFn pivotFn) {
    
    int pivot = pivotFn(data, p1, p2);
    int lh = p1 + 1;
    int rh = p2;
    while (true) {
        while (lh < rh && data[rh] >= pivot) rh--;
        while (lh < rh && data[lh] < pivot) lh++;
        if (lh == rh) break;
        int tmp = data[lh];     // NB: This section is heavily exercised when
        data[lh] = data[rh];    //     input data are presented in descending
        data[rh] = tmp;         //     order.
    }
    if (data[lh] >= pivot) return p1;
    data[p1] = data[lh];
    data[lh] = pivot;
    return lh;
}

// Function: pivot
// Usage: int pivot = fastPivot(&data, p1, p2);
//        int pivot = medianPivot(&data, p1, p2);
// -----------------------------------------------
// Mutates the data vector so the pivot value resides in data[p1]
// and returns that as the function value.
//
// fastPivot uses data[p1] as the pivot value.
//
// medianPivot takes the median of the fist, middle, and last
// values relative to the p1 and p2 indices.
//

int fastPivot(Vector<int> & data, const int p1, const int p2) {
    return data[p1];
}