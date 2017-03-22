//
// pair.h
//
// This file defines a template-based Pair class that associates two 
// potentially different types of data.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/5/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#ifndef _pair_h
#define _pair_h


// Class: Pair
// -----------
// This class defines a simple association between two data types
// with rudimentary getter methods exposed to the client.
//
// The spirit of the exercise is to explore multi-type templates.
// Although there are more exotic ways to implement this, but I'm
// opting for the least complexity that satisfies the requirements.
//

template <typename T1, typename T2>
class Pair {

public:
    
    // Constructor: Pair
    // Usage: Pair<std::string, int> keyValPair;
    //        Pair<std::string, int> keyValPair = new Pair("key", 42);
    // ---------------------------------------------------------------
    // The default constructor instantiates a Pair object with values
    // for the associated types initialized to default values.
    //
    // The parameterized constructor initializes the two instance variables
    // with specific values.
    
    Pair();
    Pair(T1 val1, T2 val2);

    
    // Method: first, second
    // Usage: T1 v1 = myPair.first();
    //        T2 v2 = myPair.second();
    // -------------------------------
    // Getter methods that return the first and second values that
    // comprise the pair.
    
    T1 first();
    T2 second();
    
private:
    
    T1 v1;
    T2 v2;
    
};

#include "pair.cpp"

#endif // _pair_h
