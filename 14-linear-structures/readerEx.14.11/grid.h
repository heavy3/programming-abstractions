//
// grid.h
//
// This files exports an interface for a Grid class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 11
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/3/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#ifndef grid_h
#define grid_h

#include "vector.h"
#include <iostream>
#include <sstream>
#include <iomanip>

template <typename ValueType>
class Grid {
public:
    Grid();
    Grid(int rows, int cols);
    ~Grid();
    Grid(const Grid & src);
    Grid & operator=(const Grid & src);
    
    int numRows() const;
    int numCols() const;
    bool inBounds(int row, int col) const;
    ValueType get(int row, int col) const;
    void set(int row, int col, ValueType val);
    void resize(int rows, int cols);
    std::string toString() const;
    
private:
    Vector<ValueType> *pv;
    int rows;
    int cols;
    
    int getIndex(int row, int col) const;
    void deepCopy(const Grid & g);
};

#include "grid.cpp"

#endif // grid_h
