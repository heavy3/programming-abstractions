//
// main.cpp
// --------------
// This program implements a function, reshape:
//
//     void reshape(Grid<int> & grid, int nRows, int nCols);
//
// which resizes a grid but fills in the data from the original grid by
// copying elements in the standard row-major order 
// (left-to-right/top-to-bottom).
//
// For example, the 3 x 4 grid could be reshaped from:
//
//     1   2   3   4
//     5   6   7   8
//     9  10  11  12
//
// to a 4 x 3 grid:
//
//     1   2   3
//     4   5   6
//     7   8   9
//    10  11  12
//
// The function truncates values if the target grid is too small.
// It does nothing special (other than leave cells with their default initial
// values) if the target grid is too large.
//
// Typical output:
//
// This program implements a function for resizing
// a grid without destroying its contents.
// ----------------------------------------
// 1   2   3
// 4   5   6
// 7   8   9 Original
//
// 1   2   3   4   5
// 6   7   8   9   0 Reshaped
//
// 1   2   3   4   5
// 6   7   8   9   0 Expected (pass)
//
// Notice the last value is intrinsically initialized to 0 based upon an 
// integer data type for the grid.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 11
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/04/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "error.h"
#include "grid.h"
#include "vector.h"
#include "queue.h"

// Constants and types

const string LINE   = '\n' + string(40, '-');

struct DimensionT { /* Type for specifying the desired size of a grid. */
    int row;
    int col;
};

// Function prototypes

void banner();
bool equals(Grid<int> g1, Grid<int> g2);
void reshape(Grid<int> & grid, int nRows, int nCols);
bool sequenceFill(Grid<int> & grid, DimensionT dimension);
bool sequenceFill(Grid<int> & grid, DimensionT dimension, int capacity);
void showGrid(Grid<int> & grid, int fieldWidth = 4);
bool testReshapeGrid(DimensionT beforeDim, DimensionT afterDim);

// Main program

int main(int argc, char * argv[]) {
    banner();
    cout << LINE;

    // 
    // Test case 1:
    // Capacity stays constant at 12. 
    //
    
    DimensionT before = {3, 4};
    DimensionT after  = {4, 3};
    testReshapeGrid(before, after);

    // 
    // Test case 2: 
    // Reshaped grid capacity increases from 9 to 10,
    // leaving default initialization for 10th cell.
    //
    
    before.row = 3;
    before.col = 3;
    after.row  = 2;
    after.col  = 5;
    testReshapeGrid(before, after);

    // 
    // Target case 3:
    // Capacity decreases from 9 to 6, truncating a row of data. 
    //
    
    before.row = 3;
    before.col = 3;
    after.row  = 2;
    after.col  = 3;
    testReshapeGrid(before, after);
    
    // 
    // Test case 4:
    // Force error condition with invalid number of rows = 0 in reshaped grid.
    //
    // Should exit with error message like:
    //     Error: reshape: row or col in reshaped grid < 1.
    //
    
    before.row = 2;
    before.col = 2;
    after.row = 0;
    after.col = 1;
    testReshapeGrid(before, after);

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
    cout << "This program implements a function for resizing"
    << endl << "a grid without destroying its contents.";
}

//
// Function: equals
// Usage: if (equals(grid, expectedGrid)) { ... }
// ----------------------------------------------
// Returns true if two grids are the same.
//

bool equals(Grid<int> g1, Grid<int> g2) {
    return (g1.toString() == g2.toString());
}

//
// Function: reshape
// Usage: reshape(grid, nRows, nCols);
// -----------------------------------
// Returns a mutated grid such that the dimensions are altered
// but contents preserved to the extent allowed by the capacity of the
// new grid.  
//
// Smaller target grids result in data truncation.  Larger target
// grids preserve original content but also include additional
// elements intialized to default values for the grid's data type.
//

void reshape(Grid<int> & grid, int nRows, int nCols) {
    
    // Bounds check target dimensions.
    if ((nRows < 1) || (nCols < 1)) {
        error("reshape: row or col in reshaped grid < 1.");
    }
        
    // Short-circuit return if proposed grid dimensions match existing.
    if ((grid.numRows() == nRows) &&
        (grid.numCols() == nCols)) return;
    
    // Save grid contents to a queue.
    Queue<int> fifo;
    for (int value: grid) {
        fifo.enqueue(value);
    }
    
    // Mutate grid dimensions, destroying contents.
    grid.resize(nRows, nCols);
    
    // Populate reshaped grid from queue.
    int i = 0;
    while (fifo.size()) {
        int c = i % grid.numCols();
        int r = i / grid.numCols();
        
        // Is reshaped grid smaller than original?
        if (!grid.inBounds(r, c)) break;
        
        grid[r][c] = fifo.dequeue();
        i++;
    }
}

//
// Function: sequenceFill
// Usage: if (sequenceFill(grid, dimension) { ... }
//        if (sequenceFill(grid, dimension, fillLimit)) { ... }
// ----------------------------------------------------------------
// Fills a grid of a given dimension with a sequence of integers
// up to a capacity limit which  may be smaller than the inherent
// capacity of the grid itself.
//
// If fillLimit is not provided, it is calculated from the
// dimension parameter.
//

bool sequenceFill(Grid<int> & grid, DimensionT dim) {
    int fillLimit = dim.row * dim.col;
    return (sequenceFill(grid, dim, fillLimit));
}

bool sequenceFill(Grid<int> & grid, DimensionT dim, int fillLimit) {
    if ((dim.row < 1) || (dim.col < 1)) return false;
    
    grid.resize(dim.row, dim.col);
    int i = 1;
    for (int r = 0; r < dim.row; r++) {
        for (int c = 0; c < dim.col; c++) {
            if (i > fillLimit) {
                break;
            }
            grid[r][c] = i++;
        }
    }
    return true;
}

//
// Function: showGrid
// Usage: showGrid(grid, fieldWidth);
// ----------------------------------
// Displays a simply formatted grid of integers to the console.
//

void showGrid(Grid<int> & grid, int fieldWidth) {
    for (int r = 0; r < grid.numRows(); r++) {
        cout << endl;
        for (int c = 0; c < grid.numCols(); c++) {
            cout << setw(fieldWidth) << grid[r][c];
        }
    }
}

//
// Function: testReshapeGrid
// Usage: if (testReshapeGrid(beforeDimension, afterDimension)) { ... }
// --------------------------------------------------------------------
// Returns true if the reshaped grid matches a computed expected grid.
//
// Displays before, after, and expected grids to the console along with
// a pass/fail indicator.
//
// Grids are filled with simple, integer sequences from 1 to the
// capacity of the grid prior to reshaping.
//

bool testReshapeGrid(DimensionT beforeDim, DimensionT afterDim) {
    Grid<int> grid;
    Grid<int> gridExpected;
    
    // Populate original grid.
    
    sequenceFill(grid, beforeDim);
    showGrid(grid); cout << " Original" << endl;
    
    // Mutate the dimensions, preserving contents as best as possible.
    
    reshape(grid, afterDim.row, afterDim.col);
    showGrid(grid); cout << " Reshaped" << endl;
    
    // Populate expected grid.
    //
    // Fill with integer sequence but limited to
    // size of initial grid (when reshaped grid capacity exceeds original).
    //
    // This allows for the desired intrinsic initialization in the expected grid
    // of values beyond the original grid's capacity.

    int fillLimit = beforeDim.row * beforeDim.col;
    sequenceFill(gridExpected, afterDim, fillLimit);
    
    // Report results
    
    showGrid(gridExpected); cout << " Expected";
    bool result = equals(grid, gridExpected);
    (result) ? cout << " (pass)" : cout << " (fail)";
    cout << endl;
    
    return result;
}
