//
// main.cpp
//
// This program exercises a polymorphic Quicksort that works with any
// base type that supports the standard relational operators.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/4/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "sort.h"

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 14.02\n";
const std::string DETAIL = "Templatized (Quick) sort() function.";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    Vector<int> vec;
    vec += 3, 7, 5, 2, 9, 1;
    
    std::cout << "unsorted: " << vec << std::endl;
    sort(vec);
    std::cout << "sorted:   " << vec << std::endl;
    
    std::cout << std::endl;
    
    Vector<char> lvec;
    lvec += 'c', 'g', 'e', 'b', 'i', 'a';
    
    std::cout << "unsorted: " << lvec << std::endl;
    sort(lvec);
    std::cout << "sorted:   " << lvec << std::endl;
    
    std::cout << std::endl;
    
    Vector<std::string> svec;
    svec += "cow", "goat", "eagle", "bison", "iguana", "antelope";
    
    std::cout << "unsorted: " << svec << std::endl;
    sort(svec);
    std::cout << "sorted:   " << svec << std::endl;
    
    return 0;
}
