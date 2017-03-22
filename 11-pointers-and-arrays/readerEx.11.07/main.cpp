//
// main.cpp
//
//
// This program reimplements the merge sort algorithm from Figure 10-3
// to use an array instead of a vector and with the following prototype:
//
//    void sort(int array[], int n);
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 11, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This work is largely derived from the merge sort in Figure 10-3.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 6/16/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include "error.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 11.07\n";
const std::string DETAIL = "Rewrite merge sort with arrays instead of vectors.";
const std::string BANNER = HEADER + DETAIL;
const int MAX_ITEMS = 6;

// Prototypes

void mergeSort(int array[], int n);
void merge(int array[], int a1[], int n1, int a2[], int n2);
void coutArray(int array[], int n);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    int array[MAX_ITEMS] = {3, 7, 5, 2, 9, 1};
    
    cout << "unsorted: "; coutArray(array, MAX_ITEMS);
    mergeSort(array, MAX_ITEMS);
    cout << "sorted:   "; coutArray(array, MAX_ITEMS);
    
    return 0;
}

// Function: sort
// Usage: sort(array, nItems);
// ---------------------------
// This function sorts the elements of the array into increasing order
// using the merge sort algorithm, which consists of the following steps:
//
// 1. Divide the array into two halves.
// 2. Sort each of these smaller arrays recursively.
// 3. Merge the two arrays back into the original one.

void mergeSort(int array[], int n) {
    if (n <= 1 ) return;
    
    int halfN = round(n/2.0);
    int a1[halfN];
    int a2[halfN];
    
    int n1 = 0;
    int n2 = 0;
    
    for (int i = 0; i < n; i++) {
        if (i < halfN) {
            a1[i] = array[i];
            ++n1;
        } else {
            a2[i - halfN] = array[i];
            ++n2;
        }
    }
    mergeSort(a1, n1);
    mergeSort(a2, n2);
    merge(array, a1, n1, a2, n2);
}

// Function: merge
// Usage: merge(array, a1, n1, a2, n2);
// ------------------------------------
// This function merges two sorted arrays, a1 and a2, into an array
// Because the input arrays are sorted, the implementation can always select
// the first unused element in one of the input arrays to fill the next
// position.

void merge(int array[], int a1[], int n1, int a2[], int n2) {
    int p1 = 0;
    int p2 = 0;
    int i = 0;
    while (p1 < n1 && p2 < n2) {
        if (a1[p1] < a2[p2]) {
            array[i++] = a1[p1++];
        } else {
            array[i++] = a2[p2++];
        }
    }
    while (p1 < n1) array[i++] = a1[p1++];
    while (p2 < n2) array[i++] = a2[p2++];
}

// Function: coutArray
// Usage: coutArray(array, nItems);
// --------------------------------
// Outputs the contents of an integer array to the console.

void coutArray(int array[], int n) {
    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}