//
// main.cpp
//
// This program illustrates the use of Vector syntax to initialize a Grid.
//
// Typical output:
//
//    This program leverages Vector syntax to initialize a Grid.
//
//    Vector: {1, 2, 3, 4, 5, 6, 7, 8, 9}
//    Grid:   {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/01/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "error.h"
#include "grid.h"

// Constants and types

const string E_SIZE = "fillGrid: Vector size doesn't match grid capacity.";

// Function prototypes

void banner();
void fillGrid(Grid<int> & grid, Vector<int> & values);

// Main program

int main(int argc, char * argv[]) {

    banner();
    
    Grid<int> matrix(4, 3);
    Vector<int> values;
    
    values += 1, 2, 3;
    values += 4, 5, 6;
    values += 7, 8, 9;
    values += 10, 11, 12;
    
    fillGrid(matrix, values);
    
    cout << setw(8) << left << "Vector: " << values << endl
         << setw(8) << left << "Grid: "   << matrix << endl;
    
    return 0;
}

// Function definitions

//
// Function: banner
// Usage: banner(upperBound)
// -------------------------
// Function displays the purpose of the program on the console.
//

void banner() {
    std::cout << "This program leverages Vector syntax to initialize a Grid."
              << std::endl << std::endl;
}

//
// Function: fillGrid
// Usage: fillGrid(grid, vector);
// ------------------------------
// Initializes a grid container with the contents of a vector.
//
// Both input parameters are pass-by-reference for efficiency.
//
// If the size of the input vector does not match the number of elements in
// the grid, an error is thrown and processing halts.
//

void fillGrid(Grid<int> & grid, Vector<int> & values) {
    int gridSize = grid.numRows() * grid.numCols();
    if (gridSize != values.size()) {
        ostringstream os;
        os << E_SIZE << " vector size = " << values.size()
                     << " grid capacity = " << gridSize;
        error(os.str());
    } else {
        for (int i = 0; i < values.size(); i++) {
            int col = i % grid.numCols();
            int row = i / grid.numCols();
            grid[row][col] = values[i];
        }
    }
}
