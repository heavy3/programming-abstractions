//
// main.cpp
//
// Write a function:
//
//    void fillRegion(Grid<bool> & pixels, int row, int col)
//
// that simulates the operation of the paint-bucket tool by painting in
// black all white pixels reachable from the specified row and column without
// crossing an existing black pixel.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/21/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
#include "direction.h"
#include "point.h"
#include "grid.h"
#include "error.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.04\n";
const std::string DETAIL = "fillRegion() recursively";
const std::string BANNER = HEADER + DETAIL;
const std::string E_SIZE = "fillGrid: Vector size doesn't match grid capacity.";

// Types

enum pixelColorT {
    W = false,
    B = true    // outline color
};

// Function prototypes

void fillRegion(Grid<bool> & pixelGrid, int row, int col);
void fillGrid(Grid<bool> & grid, Vector<bool> & values);
int nextRow(Direction d, int row);
int nextCol(Direction d, int col);

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    Grid<bool> pixelGrid(23, 19);
    Vector<bool> pixelVector;
    
    // Simple house image.
    
    pixelVector += W, W, W, W, W, W, W, W, W, B, W, W, W, W, W, W, W, W, W;
    pixelVector += W, W, W, W, W, W, W, W, B, W, B, W, W, W, W, W, W, W, W;
    pixelVector += W, W, W, W, W, W, W, B, W, W, W, B, W, W, W, W, W, W, W;
    pixelVector += W, W, W, W, W, W, B, W, W, W, W, W, B, W, W, W, W, W, W;
    pixelVector += W, W, W, W, W, B, W, W, W, W, W, W, W, B, W, W, W, W, W;
    pixelVector += W, W, W, W, B, W, W, W, W, W, W, W, W, W, B, W, W, W, W;
    pixelVector += W, W, W, B, W, W, W, W, W, W, W, W, W, W, W, B, W, W, W;
    pixelVector += W, W, B, W, W, W, W, W, W, W, W, W, W, W, W, W, B, W, W;
    pixelVector += W, B, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, B, W;
    pixelVector += B, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, B;
    pixelVector += B, W, W, B, B, B, B, W, W, W, W, W, B, B, B, B, W, W, B;
    pixelVector += B, W, W, B, W, W, B, W, W, W, W, W, B, W, W, B, W, W, B;
    pixelVector += B, W, W, B, W, W, B, W, W, W, W, W, B, W, W, B, W, W, B;
    pixelVector += B, W, W, B, B, B, B, W, W, W, W, W, B, B, B, B, W, W, B;
    pixelVector += B, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, B;
    pixelVector += B, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, B;
    pixelVector += B, W, W, W, W, W, W, B, B, B, B, B, W, W, W, W, W, W, B;
    pixelVector += B, W, W, W, W, W, W, B, W, W, W, B, W, W, W, W, W, W, B;
    pixelVector += B, W, W, W, W, W, W, B, W, W, W, B, W, W, W, W, W, W, B;
    pixelVector += B, W, W, W, W, W, W, B, W, W, W, B, W, W, W, W, W, W, B;
    pixelVector += B, W, W, W, W, W, W, B, W, W, W, B, W, W, W, W, W, W, B;
    pixelVector += B, W, W, W, W, W, W, B, W, W, W, B, W, W, W, W, W, W, B;
    pixelVector += B, B, B, B, B, B, B, B, W, W, W, B, B, B, B, B, B, B, B;

    fillGrid(pixelGrid, pixelVector);
    cout << "Before fill: " << endl << pixelGrid << endl << endl;
    
    fillRegion(pixelGrid, 22, 8);   //  Fill the doorway.
    cout << "After fill: " << endl << pixelGrid << endl << endl;
    
    return 0;
}

//
// Function: fillRegion
// Usage: fillRegion(grid, row, col);
// ----------------------------------
// Fills in a region by painting black all white pixels reachable from the 
// specified row and column without crossing an existing black pixel.
//

void fillRegion(Grid<bool> & g, int row, int col) {
    // base case
    if (g[row][col] == B) return;   // Nothing to do; already black.
    
    // recursive case
    if  (g.inBounds(row, col)) g[row][col] = B;  // Paint white pixel black.
    for (Direction d = NORTH; d <= WEST; d++) {
        int nr = nextRow(d, row);
        int nc = nextCol(d, col);
        if (g.inBounds(nr, nc)) {
            fillRegion(g, nr, nc);
        }
    }
}

//
// Function: nextRow
// Usage: int newRow = nextRow(direction, currentRow);
// ---------------------------------------------------
// Returns the next row relative to a current row and cardinal direction.
// The origin of the coordinate system is the top left of the screen.
// Values of y increase downward.  Values of x increase to the right.
//

int nextRow(Direction d, int row) {
    switch (d) {
        case NORTH:
            return --row;
            break;

        case SOUTH:
            return ++row;
            break;
            
        case EAST:
        case WEST:
        default:
            return row;
            break;
    }
}

//
// Function: nextCol
// Usage: int newCol = nextCol(direction, currentCol);
// ---------------------------------------------------
// Returns the next column relative to a current col and cardinal direction.
// The origin of the coordinate system is the top left of the screen.
// Values of y increase downward.  Values of x increase to the right.
//

int nextCol(Direction d, int col) {
    switch (d) {
        case EAST:
        case WEST:
            return ++col;
            break;

        case NORTH:
        case SOUTH:
        default:
            return col;
            break;
    }
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

void fillGrid(Grid<bool> & grid, Vector<bool> & values) {
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
