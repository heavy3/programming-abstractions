//
// main.cpp
//
// Write the prototype for and then implement the template function count_if
// from the <algorithm> library.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 09
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/4/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "error.h"
#include "vector.h"

using namespace std;

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 20.09\n";
const std::string DETAIL = "STL Programming: Implement <algorithm> count_if() ";
const std::string BANNER = HEADER + DETAIL;

typedef int elemType;
typedef Vector<elemType> CollType;
typedef CollType::iterator IterType;
typedef bool (*PredType)(elemType);

// Function prototypes

bool isEight(elemType elem);

int count_if(IterType itBegin, IterType itEnd, PredType fn);
template <typename PredFunctorType>
int count_if(IterType itBegin, IterType itEnd, PredFunctorType fn);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    CollType v;
    v += 8;
    v += 6;
    v += 7;
    v += 5;
    v += 3;
    v += 0;
    v += 9;
    v += 8;
    
    int count8s = count_if(v.begin(), v.end(), isEight);
    
    cout << "Number of 8's found in " << v << " = " << count8s << endl;
    return 0;
}

// Function: count_if
// Usage: int count = count_if(v.begin(), v.end(), isEight);
// ---------------------------------------------------------
// Returns a count of the number of elements in the collection between the
// begin and end iterators that satisfy the predicate function.
//
// We have two flavors here.  One that works with function pointers and
// one that works with function objects.  To avoid code duplication, the
// former calls the latter, casting the function pointer to a function
// object with ptr_fun.

int count_if(IterType itBegin, IterType itEnd, PredType fn) {
    return count_if(itBegin, itEnd, ptr_fun(fn));
}

template <typename PredFunctorType>
int count_if(IterType itBegin, IterType itEnd, PredFunctorType fn) {
    int result = 0;
    
    for (IterType it = itBegin; it != itEnd; it++) {
        if (fn(*it)) ++result;
    }
    
    return result;
}

// Function: isEight
// Usage if (isEight(elem)) { . . . }
// ----------------------------------
// Returns true if the input parameter is 8.

bool isEight(elemType elem) {
    return elem == 8;
}
