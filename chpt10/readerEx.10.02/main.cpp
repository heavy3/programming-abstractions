//
// main.cpp
//
// This program implements an insertion sort algorithm.
//  Write an implementation of sort that uses the insertion sort algorithm.
//
//  Construct an informal argument to show that the worst-case behavior of
//  insertion sort is O(N^2).
//
//  The insertion sort algorithm is important in practice because it runs in
//  linear time if the vector is already more or less in the correct order.
//  It therefore makes sense to use insertion sort to restore order to a large
//  vector in which only a few elements are out of sequence.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 10, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/27/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "vector.h"
#include "error.h"
using namespace std;

#define HISTOGRAM   // Comment this out to disable histogram output.

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 10.02\n";
const std::string DETAIL = "Insertion Sort -- worst case O(N^2)";
const std::string BANNER = HEADER + DETAIL;

// Prototypes

void sort(Vector<int> & nums);  // Insertion sort
std::string toString(Vector<int> & nums, int s);

// Main program

int main() {
    cout << BANNER << endl << endl;
    Vector<Vector<int> > testNums;
    Vector<int> nums;
    
    // Example vector in book.

    nums += 56, 25, 37, 58, 95, 19, 73, 30;
    testNums.add(nums);
    
    // 
    // Worst case vector: Input data in reverse order. 
    // Complexity = O(n^2), where n = size of input vector.
    //

    nums.clear();
    nums += 7, 6, 5, 4, 3, 2, 1;
    testNums.add(nums);
    
    // Best case vector; already sorted. O(n) complexity. See histogram.

    nums.clear();
    nums += 1, 2, 3, 4, 5, 6, 7;
    testNums.add(nums);

    for (Vector<int> nums : testNums) {
        cout << "Unsorted numbers: " << nums << endl;
        sort(nums);
        cout << "Sorted numbers:   " << nums << endl << endl;
    }
  
    return 0;
}

//
// Function: sort
// Usage: sort(intNumVector);
// --------------------------
// Sorts a vector of integers that have been passed in by reference.
//
// Uses an Insertion Sort algorithm that iterates over the the vector
// gradually mutating the contents into ascending order as follows:
//
//
//  [ 56 | 25 | 37 | 58 | 95 | 19 | 73 | 30 ]
//    --
//      \_ in order
//
//  [ 25 | 56 | 37 | 58 | 95 | 19 | 73 | 30 ]
//    --   --
//           \_ in order
//
//  [ 25 | 37 | 56 | 58 | 95 | 19 | 73 | 30 ]
//    --   --   --
//                \_ in order
//
// Worst-case performance is O(N^2) when input is reversed sorted.
// Base-case performance is O(N) when input is already sorted.
//
// This algorithm is suited to input that is partially sorted.
//
// Complexity Analysis
// -------------------
// Broadly speaking, we have a double loop structure with variable cost
// contributed from inner loop comparisons plus a repositioning
// vector mutation.
//
// Roughly, that gives a runtime of:
//
//  t(n) ~= (n-1) * (t_innerloop + t_mutation)
//
// t_innerloop  effort varies from 1 to (n-1) iterations depending upon
//              structure of input data.
//
//              Worst case is repositioning final unsorted number from
//              one end of the vector to the other (especially when data
//              are in reversed order initially).
//
// t_mutation   effort to reposition a single number within the vector
//              depends upon the underlying implementation of the data type.
//
//              In the best case, this would be constant time.
//              In the worst case, it would be at most 2*(n-1) operations
//              to remove an unsorted number and re-insert it
//              in sorted order (especially at the beginning of the sorting
//              activity).
//
// Assuming worst case for both inner loop actions, we get:
//
//  t(n) ~= (n-1) * (c2(n-1) + 2(n-1))
//
// Simplifying:
//
//  t(n) ~= (n-1) * (3n -1) = 3n^2 - 4n + 1 + c0
//
// Dropping non-dominant lower-order terms and constants, we get:
//
//  O(n^2)
//
// Interestingly, the qualitative analysis yields the same result even if
// the vector mutation happens in constant time, since the inner loop
// still dominates with an O(n) term.
//

void sort(Vector<int> & items) {
    for (int u = 1; u < items.size(); u++) {
        int k = items[u];
        int s = u - 1;
        while (s >= 0 && items[s] > k) {
            items[s+1] = items[s];
            items[s--] = k;
        }
    }
}

// This was my ugly first attempt at insertion sort.

void sortUgly(Vector<int> & nums) {
    if (nums.size() < 2) return;
    
    for (int ns = 1; ns < nums.size(); ns++) {
        int insHere = ns;
        for (int s = ns - 1; s >= 0; s--) {
            #ifdef HISTOGRAM
            cout << "*";
            #endif
            if (nums[ns] >= nums[s]) break;
            insHere = s;
        }
        
        if (insHere < ns) {
            int tmp = nums[ns];
            nums.remove(ns);
            nums.insert(insHere, tmp);
            #ifdef HISTOGRAM
            cout << " " << tmp << " (repositioning)";
            #endif
        }
        #ifdef HISTOGRAM
        cout << endl;
        #endif
    }
}

//
// Function: toString
// Usage: cout << toString(numVector, partionIndex);
// -------------------------------------------------
// Display a vector of integers with a vertical parition separating the
// sorted portion of the vector from the unsorted.
//

std::string toString(Vector<int> & nums, int s) {
    ostringstream oss;
    
    oss << "[";
    for (int i = 0; i < nums.size(); i++) {
        oss << setw(3) << nums[i] << " ";
        if (i == s) oss << "|";
    }
    oss << "]";
    return oss.str();
}
