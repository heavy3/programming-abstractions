//
// main.cpp
//
// This program implements the function:
//
//    bool checkSudokuSolution(Grid<int> & puzzle);
//
// that checks to see whether a proposed Sudoku solution follows the Sudoku 
// rules against duplicating values in a row, column, or 3x3 sub-square.
//
// It returns true if the puzzle is a valid solution. It tests for a 9x9 grid
// of integers and reports and error otherwise.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 9
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/02/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "error.h"
#include "grid.h"
#include "set.h"

// Constants and types

const string E_SIZE = "fillGrid: Vector size doesn't match grid capacity.";
const string LINE = '\n' + string(40, '-');
const int SUDOKU_SIZE = 9;

enum SudokuFeatureT {
    row,
    col,
    subsquare
};

// Function prototypes

void banner();
bool checkSudokuSolution(Grid<int> & puzzle);
void fillGrid(Grid<int> & grid, Vector<int> & values);
Set<int> getSubSquare(Grid<int> & puzzle, int majorRow, int majorCol);
bool isValidSudokuFeature(Grid<int> & puzzle, SudokuFeatureT feature, int index);
bool isValidSudokuSet(Set<int> & sudokuSet);
void showGrid(Grid<int> & grid, int fieldWidth = 4);

// Main program

int main(int argc, char * argv[]) {
    banner();
    
    Grid<int> puzzle(9, 9);
    Vector<int> values;
    
    values += 3, 9, 2,  4, 6, 5,  8, 1, 7;
    values += 7, 4, 1,  8, 9, 3,  6, 2, 5;
    values += 6, 8, 5,  2, 7, 1,  4, 3, 9;
    
    values += 2, 5, 4,  1, 3, 8,  7, 9, 6;
    values += 8, 3, 9,  6, 2, 7,  1, 5, 4;
    values += 1, 7, 6,  9, 5, 4,  2, 8, 3;
    
    values += 9, 6, 7,  5, 8, 2,  3, 4, 1;
    values += 4, 2, 3,  7, 1, 9,  5, 6, 8;
    values += 5, 1, 8,  3, 4, 6,  9, 7, 2;
    
    fillGrid(puzzle, values);
    
    cout << LINE;
    showGrid(puzzle);
    cout << LINE << endl;
    
    if (checkSudokuSolution(puzzle)) {
        cout << "This is a valid Sudoku solution. :-)";
    } else {
        cout << "This is not a valid Sudoku solution. :-(";
    }
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
    cout << "This program tests if a given Sudoku solution is valid.";
}

//
// Function: checkSudokuSolution
// Usage: if (checkSudokuSolution(puzzle)) { ... }
// -----------------------------------------------
// Returns true if a passed-in puzzle solution is valid.
//
// Solutions are valid if:
//
//    1. All rows consist of the numbers 1 - 9 with no duplicates.
//
//    2. All columns consist of the numbers 1 - 9 with no duplicates.
//
//    3. All 3x3 subsquares (within the larger 9x9 puzzle) consist of
//       the numbers 1 - 9 with no duplicates.
//

bool checkSudokuSolution(Grid<int> & puzzle) {
    
    if ((puzzle.numRows() != SUDOKU_SIZE) ||
        (puzzle.numCols() != SUDOKU_SIZE)) {
        ostringstream os;
        os << "Puzzle must be "
           << SUDOKU_SIZE << "x" << SUDOKU_SIZE << " grid.";
        error("checkSudokuSolution:" + os.str());
    }
    
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        if (!isValidSudokuFeature(puzzle, row, i)) return false;
        if (!isValidSudokuFeature(puzzle, col, i)) return false;
        if (!isValidSudokuFeature(puzzle, subsquare, i)) return false;
    }
    
    return true;
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
// Function: getSubSquare
// Usage: Set<int> square = getSubSquare(puzzle, majorRow, majorCol);
// ------------------------------------------------------------------
// Returns one of nine 3x3 sub-squares from the larger Sudoku puzzle.
// Squares are addressed by a row column pair with values between 0 and 2.
//
// Internally, the 3x3 addressing is converted to 9x9 addressing within
// the larger puzzle grid to extract out the 9 elements corresponding to the 
// subsquare of interest.
//

Set<int> getSubSquare(Grid<int> & puzzle, int majorRow, int majorCol) {
    Set<int> subSquare;
    
    for (int r = majorRow * 3; r < (majorRow * 3) + 3; r++) {
        for (int c = majorCol * 3; c < (majorCol * 3) + 3; c++) {
            #ifdef DEBUGGING
            cout << "row = " << r << "  col = " << c << endl;
            #endif
            subSquare.add(puzzle[r][c]);
        }
    }
    return subSquare;
}

//
// Function: isValidSudokuFeature
// Usage: if (isValidSudokuFeature(puzzle, row|col|subsquare, index)) { ... }
// --------------------------------------------------------------------------
// Validates a given row, column, or subsquare within a 9x9 Sudoku solution
// grid.  The feature to validate is indexed by an integer from 0 - 8.
//

bool isValidSudokuFeature(Grid<int> & puzzle, SudokuFeatureT feature,
                          int index) {
    
    if ((puzzle.numCols() != SUDOKU_SIZE) ||
        (puzzle.numRows() != SUDOKU_SIZE)) {
        return false;
    }
    
    Set<int> sudokuSet;
    switch (feature) {
        case row: {
                int row = index;
                for (int i = 0; i < SUDOKU_SIZE; i++) {
                        sudokuSet.add(puzzle[row][i]);
                }
            }
            break;
        
        case col: {
                int col = index;
                for (int i = 0; i < SUDOKU_SIZE; i++) {
                    sudokuSet.add(puzzle[i][col]);
                }
            }
            break;
            
        case subsquare: {
                // 
                // Convert 0 - 8 linear index to 0 - 2 (row, col) addressing
                // for selecting 3x3 subsquare of interest.
                //
                int majorCol = index % 3;
                int majorRow = (index - majorCol) / 3;
                sudokuSet = getSubSquare(puzzle, majorRow, majorCol);
            }
            break;
    }
    return isValidSudokuSet(sudokuSet);
}

//
// Function: isValidSudokuSet
// Usage: if (isValidSudokuSet(sudokuSet)) { ... }
// -----------------------------------------------
// Returns true if the passed-by-reference set is 9 elements large
// and contains the numbers 1 - 9.
//

bool isValidSudokuSet(Set<int> & sudokuSet) {
    if (sudokuSet.size() != SUDOKU_SIZE) return false;
    for (int value: sudokuSet) {
        if (value < 1) return false;
        if (value > SUDOKU_SIZE) return false;
    }
    return true;
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
