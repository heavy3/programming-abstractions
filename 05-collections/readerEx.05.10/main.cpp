//
// main.cpp
//
// This program relates to the game, Minesweeper, in which players search for
// hidden mines on a grid.  
//
// The program initializes a playing field with an initial mine layout
// then calculates a second grid of integers corresponding to the number of 
// mines neighboring each location of the mine field.
//
// The neighborhood of a location includes the location itself and the eight 
// adjacent locations, but only if they are inside the boundaries of the grid.
//
// I think the point of this exercise is to play with grids and leverage
// the inBounds(r,c) method, in particular, when locating the subset of
// adjacent cells for perimeter locations.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 10
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

// Constants and types

const string E_SIZE = "fillGrid: Vector size doesn't match grid capacity.";
const string LINE = '\n' + string(40, '-');
const int BOARD_SIZE = 6;   // Square dimension of playing field.
const bool T = true;        // Represents presence of a mine.
const bool F = false;

struct Cell {
    int row;
    int col;
    bool hasMine;
};

// Function prototypes

void banner();
void fillGrid(Grid<bool> & grid, Vector<bool> & values);
void fixCounts(Grid<bool> & mineField, Grid<int> & counts);
int getMineCounts(Grid<bool> & mineField, int row, int col);
Vector<Cell> getNearbyCells(Grid<bool> & mineField, int row, int col);
void showGrid(Grid<bool> & grid, int fieldWidth = 4);
void showGrid(Grid<int> & grid, int fieldWidth = 4);

// Main program

int main(int argc, char * argv[]) {
    banner();
    cout << LINE << endl << endl;
    
    // Initialize the field of play using vector syntax for convenience.
    
    Vector<bool> values;
    values += T, F, F, F, F, T;
    values += F, F, F, F, F, T;
    values += T, T, F, T, F, T;
    values += T, F, F, F, F, F;
    values += F, F, T, F, F, F;
    values += F, F, F, F, F, F;
    
    // Initialize mine field with vector values.
    
    Grid<bool> mineField(BOARD_SIZE, BOARD_SIZE);
    fillGrid(mineField, values);
    
    // Calculate corresponding grid of neighboring mine counts.
    
    Grid<int> mineCounts;
    mineCounts.resize(BOARD_SIZE, BOARD_SIZE);
    fixCounts(mineField, mineCounts);
    
    // Display the result.

    cout << "Mine Field";
    showGrid(mineField);
    
    cout << endl << endl
         << "Mine Field Counts (# of proximate mines)";
    showGrid(mineCounts);
    
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
    cout << "This program calculates the number of mines"
    << endl << "in the neighborhood of each cell on a two dimensional"
    << endl << "field of play for a simplified version of the game,"
    << endl << "Minesweeper.";
}

//
// Function: fillGrid
// Usage: fillGrid(grid, vector);
// ------------------------------
// Initializes a grid container with the contents of a vector.
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
            int row = (i - col) / grid.numRows();
            grid[row][col] = values[i];
        }
    }
}

//
// Function: fixCounts
// Usage: fixCounts(mineField, mineCounts);
// ------------------------------------
// Populates a pass-by-reference grid of mine counts that reflects the
// number of adjacent mines for each location in a corresonding mine
// field.
//

void fixCounts(Grid<bool> & mineField, Grid<int> & mineCounts) {
    if ((mineField.numRows() != BOARD_SIZE) ||
        (mineField.numCols() != BOARD_SIZE)) {
        ostringstream os;
        os << "mines must be on a " << BOARD_SIZE << "x" << BOARD_SIZE
           << " grid.";
        error("fixCounts:" + os.str());
    } else {
        for (int r = 0; r < mineField.numRows(); r++) {
            for (int c = 0; c < mineField.numCols(); c++) {
                mineCounts[r][c] = getMineCounts(mineField, r, c);
            }
        }
    }
}

//
// Function: getMineCounts
// Usage: int nearbyMines = getMineCounts(minefield, row, col);
// ------------------------------------------------------------
// Returns a count of mines adjacent to (and including) a given location in 
// a mine field.
//

int getMineCounts(Grid<bool> & mineField, int row, int col) {
    int count = 0;
    
    // Get all the cells nearby a given location in the mine field.
    
    Vector<Cell> Cells = getNearbyCells(mineField, row, col);
    
    for (Cell nearby: Cells) {
        if (nearby.hasMine) count++;
    }
    return count;
}

//
// Function: getNearbyCells
// Usage: Vector<Cell> nearbyCells = getNearbyCells(minefield, row, col);
// ----------------------------------------------------------------------
// Returns a vector of cells adjacent to (and including) a specified
// location in the mine field.  For interior locations, this could be up
// to 9 cells in the vector.  Locations on the perimeter will have fewer
// than 9 adjacent cells since perimeter locations are not surrounded
// by other cells on all sides by definition.
//

Vector<Cell> getNearbyCells(Grid<bool> & mineField, int row, int col) {
    
    Vector<Cell> cells;
    for (int nearbyRow = row - 1; nearbyRow <= (row + 1); nearbyRow++) {
        for (int nearbyCol = col - 1; nearbyCol <= (col + 1); nearbyCol++) {
            if (mineField.inBounds(nearbyRow, nearbyCol)) {
                Cell nearbyCell;
                
                nearbyCell.row = nearbyRow;
                nearbyCell.col = nearbyCol;
                nearbyCell.hasMine = mineField[nearbyRow][nearbyCol];
                
                cells.add(nearbyCell);
            }
        }
    }
    return cells;
}

//
// Function: showGrid
// Usage: showGrid(grid);
//        showGrid(grid, fieldWidth);
// ----------------------------------
// Displays a grid on the console as a 2D matrix of numbers.
// An optional fieldWidth specifier may be passed in to improve alignment
// for grids with numbers of large magnitude.
//

// Overloaded to display a grid of boolean values (as 0s & 1s).

void showGrid(Grid<bool> & grid, int fieldWidth) {
    for (int r = 0; r < grid.numRows(); r++) {
        cout << endl << endl;
        for (int c = 0; c < grid.numCols(); c++) {
            cout << setw(fieldWidth) << grid[r][c];
        }
    }
}

// Overloaded to display a grid of integers.

void showGrid(Grid<int> & grid, int fieldWidth) {
    for (int r = 0; r < grid.numRows(); r++) {
        cout << endl << endl;
        for (int c = 0; c < grid.numCols(); c++) {
            cout << setw(fieldWidth) << grid[r][c];
        }
    }
}