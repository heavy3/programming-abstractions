//
// main.cpp
//
// This program solves the "N Queens" problem from the domain of chess,
// namely, decide if it's possible to place all N queens on an
// N x N chess board such than none is attacking another.
//
// For example,
//
//     CS106B Programming Abstractions in C++: Ex 9.05
//     Can you place N (non-attacking) Queens on an NxN chessboard?
//
//     Please choose a non-negative value for N: 4
//     Found this solution:
//
//       w  b  Q  b
//       Q  w  b  w
//       w  b  w  Q
//       b  Q  b  w
//
//     To play again, choose a non-negative value for N: -1
//     Goodbye.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/22/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "grid.h"
#include "simpio.h"
#include "error.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.05\n";
const std::string DETAIL = "Can you place N (non-attacking) Queens on an NxN chessboard?";
const std::string BANNER = HEADER + DETAIL;
const std::string PROMPT1 = "Please choose a non-negative value for N: ";
const std::string PROMPT2 = "To play again, choose a non-negative value for N: ";
const char W = 'w'; // Designates an empty white square on the game board.
const char B = 'b'; // Designates an empty black square on the game board.
const char Q = 'Q'; // Designates a queen occupying a square on the game board.

// Function prototypes

void initBoard(Grid<char> & gameBoard);
bool placeQueens(Grid<char> & gameBoard, int startCol);
bool isValidPosition(Grid<char> & gameBoard, int row, int col);
void placeQueen(Grid<char> & gameBoard, int row, int col);
void removeQueen(Grid<char> & gameBoard, int row, int col);
bool isWhiteSquare(int row, int col);
ostream & operator<<(ostream & os, Grid<char> g);

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    int N = getInteger(PROMPT1);
    while (N >= 0) {
        Grid<char> gameBoard(N, N);
        initBoard(gameBoard);
        if (placeQueens(gameBoard, 0)) {
            cout << "Found this solution:" << endl << endl;
            cout << gameBoard << endl;
        } else {
            cout << "Unable to place " << N << " queens on a " << N << "x" << N;
            cout << " gameBoard. :-/" << endl << endl;
        }
        N = getInteger(PROMPT2);
    }
    cout << "Goodbye." << endl;
    return 0;
}

//
// Function: initBoard
// Usage: initBoard(board);
// ---------------------------------
// Initializes a checkers/chess playing board with alternating white and
// black squares.
//
// The pass-by-reference board is modeled as a two-dimensional character grid
// and typically renders like this on the console with upper lefthand square
// corresponding to row = 0 and col = 0 of the grid.
//
//   w  b  w  b  w  b  w  b
//   b  w  b  w  b  w  b  w
//   w  b  w  b  w  b  w  b
//   b  w  b  w  b  w  b  w
//   w  b  w  b  w  b  w  b
//   b  w  b  w  b  w  b  w
//   w  b  w  b  w  b  w  b
//   b  w  b  w  b  w  b  w
//

void initBoard(Grid<char> & b) {
    for (int r = 0; r < b.numRows(); r++) {
        for (int c = 0; c < b.numCols(); c++) {
            if (isWhiteSquare(r, c)) {
                b[r][c] = W;
            } else {
                b[r][c] = B;
            }
        }
    }
}

//
// Function: placeQueens
// Usage: if (placeQueens(gameBoard, startCol)) { . . . }
// ------------------------------------------------------
// Attempts to place queens in adjacent columns on a gameBoard such that no 
// queen directly attacks any other.  If this is not possible, false is
// returned, otherwise the pass-by-reference grid is mutated to reflect the
// first solution found.
//
// The numbering for start column is 0-based so the first column on the
// board is col = 0.
//
// Recursion with backtracking is employed.
//

bool placeQueens(Grid<char> & gameBoard, int col) {
    
    // Base case
    //
    // Assume all queens have been placed if the specified column
    // exceeds the dimensions of the board.

    if (col >= gameBoard.numCols()) return true;
    
    // Recursvie case
    //
    // Attempt to place a queen somewhere in specified column.
    // If remaining n-1 queens can also be placed, return immediately with soln.

    for (int row = 0; row < gameBoard.numRows(); row++) {
        if (isValidPosition(gameBoard, row, col)) {
            placeQueen(gameBoard, row, col);
            if (placeQueens(gameBoard, ++col)) {
                return true;
            } else {
                // Land here when backtracking to undo bad earlier placement. 
                removeQueen(gameBoard, row, --col);
            }
        }
    }

    // Bactrack
    //
    // Unable to solve with the current placement choices so far.
    // Backtrack to prior stackframe to try different placement in prior column.

    return false;
}

//
// Function: removeQueen
// Usage: removeQueen(gameBoard, row, col);
// -----------------------------------------
// Removes a queen from the specified position.  The gameBoard
// is modeled as a character grid. 
//

void removeQueen(Grid<char> & gameBoard, int row, int col) {
    if (isWhiteSquare(row, col)) {
        gameBoard[row][col] = W;
    } else {
        gameBoard[row][col] = B;
    }
}

//
// Function: isWhiteSquare
// Usage: if (isWhiteSquare(row, col)) { . . . }
// ---------------------------------------------
// Predicate function returns true if the row and column coorespond to a white
// square on the gameBoard.  Upperlefthand square (r=0,col=0) is white by 
// convenction.
//
// If the sum of the coordinates is an even number, then consider it white.
//

bool isWhiteSquare(int row, int col) {
    return (row + col) % 2 == 0;
}

//
// Function: isValidPosition
// Usage: if (isValidPosition(gameBoard, row, col)) { ... }
// ---------------------------------------------------------
// Returns true if a queen can be placed on the game board at the row and
// column specified without falling under attack from another queen already
// on the board.
//

bool isValidPosition(Grid<char> & gameBoard, int row, int col) {
    
    // Check row for other queens.

    for (int c = 0; c < gameBoard.numCols(); c++) {
        if (c != col) {
            if (gameBoard[row][c] == Q) {
                return false;   /* Found another Q already in same row. */
            }
        }
    }
    
    // Check column for other queens.

    for (int r = 0; r < gameBoard.numRows(); r++) {
        if (r != row) {
            if (gameBoard[r][col] == Q) {
                return false;   /* Found another Q already in same col. */
            }
        }
    }

    // Check diagonals for other queens.  TODO: Could use some optimization.

    for (int r = 0; r < gameBoard.numRows(); r++) {
        for (int c = 0; c < gameBoard.numCols(); c++) {
            if ((r != row) && (c != col))  {

                // Squares in a NW/SE diagonal to our row and col will have
                // the same arithmetic difference as row - col.

                if ((r - c) == (row - col)) {
                    if (gameBoard[r][c] == Q) {
                        return false;           /* NW-SE diagonal has Q */
                    }
                }

                // Squares in a SW/NE diagonal to our row and col will have
                // the same arithmetic sum as row + col.

                if ((r + c) == (row + col)) {
                    if (gameBoard[r][c] == Q) {
                        return false;           /* SW-NE diagonal has Q */
                    }
                }
            }
        }
    }
    return true;
}

//
// Function: placeQueen
// Usage: placeQueen(gameBoard, row, col);
// ----------------------------------------
// Updates the pass-by-reference gameBoard by placing a queen at the
// position specified by row and col.
//
// Row 0 and column 0 reflect the upper lefthand corner of the board.
// Row dimensions increase toward the bottom of the screen.
// Column dimensions increase to the right.
//

void placeQueen(Grid<char> & gameBoard, int row, int col) {
    gameBoard[row][col] = Q;
}

//
// Operator: <<
// Usage: cout << gameBoard << endl;
// ----------------------------------
// Override the stream insertion operator to display a simple,
// character-based gameBoard.
//
// For example:
//
//   Q  b  w  b  w  b  w  b
//   b  w  b  w  b  w  Q  w
//   w  b  w  b  Q  b  w  b
//   b  w  b  w  b  w  b  Q
//   w  Q  w  b  w  b  w  b
//   b  w  b  Q  b  w  b  w
//   w  b  w  b  w  Q  w  b
//   b  w  Q  w  b  w  b  w
//

ostream & operator<<(ostream & os, Grid<char> gameBoard) {
    for (int r = 0; r < gameBoard.numRows(); r++) {
        for (int c = 0; c < gameBoard.numCols(); c++) {
            os << setw(3) << gameBoard[r][c];
        }
        os << endl;
    }
    return os;
}