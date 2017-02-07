//
// main.cpp
//
// This program helps illustrate the state of the heap & stack at the
// end of the second call to the Student constructor.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 12, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file uses the code from Figure 12-16.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/13/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

/*
 State of heap/stack at breakpoint in parameterized constructor.
-------------------------------------------------------------------------------
    Heap                             Stack
    ----                             -----

                              +---------------+
                              |      4.2      | gpa
                              +---------------+
                              |    3141592    | id
                              +---------------+
                              |      *-------------+ this
+---------------+             +===============+    |
|   ?   |   ?   | [1]         | [constructor] |    |
+-------+-------+             +===============+    |
|2718281|  3.61 | [0] <---+   |3141592|  4.2  | <--+ (rhs of advisees[1] = ...)
+-------+-------+         |   +---------------+
    id     gpa            +----------*        |  advisees
                              +===============+
                              |    [main]     |
                              +===============+
-------------------------------------------------------------------------------
*/

#include <iostream>
#include "error.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 12.08\n";
const std::string DETAIL = "Code for Heap/Stack Diagram";
const std::string BANNER = HEADER + DETAIL;

// Types

class Student {
public:
    
    Student() {
        id = 0;
        gpa = 4.0;
    }
    
    Student(int id, double gpa) {
        this->id = id;
        this->gpa = gpa;
        cout << "breakpoint" << endl; // Draw heap-stack diagram for
                                      // Student(3141592, 4.2);
    }
    
private:
    
    int id;
    double gpa;
    
};

// Main program

int main() {
    cout << BANNER << endl;
    
    Student *advisees = new Student[2];
    advisees[0] = Student(2718281, 3.61);
    advisees[1] = Student(3141592, 4.2);
    return 0;
}