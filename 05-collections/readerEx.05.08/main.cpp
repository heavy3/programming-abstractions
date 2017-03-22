//
// main.cpp
//
// This program implements a predicate function to determine if a square grid
// of integers is a 'magic square' wherein all rows, columns, and diagonals
// sum to the same value.
//
// Typical output:
//
//    This program tests if a grid of integers is a magic square.
//
//      8   1   6
//      3   5   7
//      4   9   2   is magic. :-)
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/01/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "error.h"
#include "grid.h"

// Constants and types

const string E_SIZE = "fillGrid: Vector size doesn't match grid capacity.";
const string E_NOT_SQUARE = "Grid is not square.";
const string LINE = '\n' + string(40, '-');
const string MAGIC    = "   is magic. :-)";
const string NOTMAGIC = "   is not magic. :-(";

enum DiagonalT {        /* Specify which diagonal in the square to sum. */
    NW_SE,
    NE_SW,
};

// Function prototypes

void banner();
void fillGrid(Grid<int> & grid, Vector<int> & values);
bool isMagicSquare(Grid<int> & grid);
int colSum(Grid<int> & grid, int col);
int diagSum(Grid<int> & grid, DiagonalT dt);
int rowSum(Grid<int> & grid, int row);
void showGrid(Grid<int> & grid, int fieldWidth = 4);

// Main program

int main(int argc, char * argv[]) {
    banner();
    
    Grid<int> matrix(3, 3);
    Vector<int> values;
    
    // Start with magic square with sum of 15.
    
    values += 8, 1, 6;
    values += 3, 5, 7;
    values += 4, 9, 2;
    
    fillGrid(matrix, values);
    string result = (isMagicSquare(matrix)) ? MAGIC : NOTMAGIC ;
    showGrid(matrix);
    cout << result << endl;

    cout << LINE;
    
    matrix[0][0]++;
    result = (isMagicSquare(matrix)) ? MAGIC : NOTMAGIC ;
    showGrid(matrix);
    cout << result << endl;

    cout << LINE;
    
    // 
    // Return to grace, though the angles are not impressed.
    // Use the magic square in Albrecht Durer's painting, Melencolia I (1514)
    //
    
    matrix.resize(4, 4);
    values.clear();
    values += 16, 3, 2, 13;
    values += 5, 10, 11, 8;
    values += 9, 6, 7, 12;
    values += 4, 15, 14, 1;
    
    fillGrid(matrix, values);
    result = (isMagicSquare(matrix)) ? MAGIC : NOTMAGIC ;
    showGrid(matrix);
    cout << result << endl;
    
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
    cout << "This program tests if a grid of integers is a magic square.";
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
            int row = (i - col) / grid.numRows();
            grid[row][col] = values[i];
        }
    }
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

void showGrid(Grid<int> & grid, int fieldWidth) {
    for (int r = 0; r < grid.numRows(); r++) {
        cout << endl << endl;
        for (int c = 0; c < grid.numCols(); c++) {
            cout << setw(fieldWidth) << grid[r][c];
        }
    }
}

//
// Function: isMagicSquare
// Usage: if (isMagicSquare(grid)) cout << "yay, magic";
// -----------------------------------------------------
// Returns true if a grid of integers is a magic square such that all rows,
// columns, and diagonals sum to the same value.
//

bool isMagicSquare(Grid<int> & grid) {
    
    // Grid must be square.
    
    if (grid.numRows() != grid.numCols()) return false;
    
    // 1 x 1 squares are always magic.
    
    if (grid.numRows() == 1) return true;
    
    // All rows add up to same sum?
    
    int refSum = rowSum(grid, 0);
    for (int r = 1; r < grid.numRows(); r++) {  // no need to sum row 0 again
        if (refSum != rowSum(grid, r)) {
            return false;
        }
    }
    
    // All columns add up to same sum?
    
    for (int c = 0; c < grid.numCols(); c++) {
        if (refSum != colSum(grid, c)) {
            return false;
        }
    }
    
    // Both diagonals add up to same sum?
    
    if (refSum != diagSum(grid, NE_SW)) return false;
    if (refSum != diagSum(grid, NW_SE)) return false;
    
    // Square is magic.
    
    return true;
}

//
// Function: colSum
// Usage: int sum = colSum(intGrid, col);
// -----------------------------------
// Returns the summation of numbers in a given column of a grid.
//

int colSum(Grid<int> & grid, int col) {
    if (col >= grid.numCols()) {
        error("colSum: Requested col exceeds cols in Grid.");
    }
    
    int sum = 0;
    for (int row = 0; row < grid.numRows(); row++) {
        sum += grid[row][col];
    }
    return sum;
}

//
// Function: diagSum
// Usage: int sum = diagSum(intGrid, diagonal);
// --------------------------------------------
// Returns the summation of numbers along a given diagonal of a square grid.
//

int diagSum(Grid<int> & grid, DiagonalT diagonal) {
    int maxRow = grid.numRows() - 1;
    int maxCol = grid.numCols() - 1;
    
    if (maxRow != maxCol) {
        ostringstream os;
        os << "diagSum: " << E_NOT_SQUARE;
        error(os.str());
    }
    
    int sum = 0;
    switch (diagonal) {
        case NE_SW:
            for (int d = 0; d <= maxRow; d++) {
                sum += grid[d][d];
            }
            break;
    
        case NW_SE:
            for (int r = 0; r <= maxRow; r++) {
                int c = maxCol - r;
                sum += grid[r][c];
            }
            break;
    
        default:
            error("diagSum: Unknown diagonal type specified.");
            break;
    }
    return sum;
}

//
// Function: rowSum
// Usage: int sum = rowSum(intGrid, col);
// -----------------------------------
// Returns the summation of numbers in a given row of a grid.
//

int rowSum(Grid<int> & grid, int row) {
    if (row >= grid.numRows()) {
        error("rowSum: Requested row exceeds rows in Grid.");
    }
    
    int sum = 0;
    for (int col = 0; col < grid.numCols(); col++) {
        sum += grid[row][col];
    }
    return sum;
}
