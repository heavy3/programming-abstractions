//
// main.cpp
//
// This program uses recursion with backtracking to devise
// a 'knight's tour' wherein a knight traverse all 64 positions
// of a chessboard without revisiting previous positions.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 6
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/23/16.
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

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.06\n";
const std::string DETAIL = "A Knight's Tour on the Chessboard";
const std::string BANNER = HEADER + DETAIL;
const int N = 8;            // The length of a chessboard in squares.
const int MAXSQUARES = N*N; // Total number of squares on a chessboard.

// Types

struct SquareT {
    int seqNum;     // Sequence number reflects order in which this square was
                    // visited during the tour.  0-value means it is unvisited.
};

typedef Grid<SquareT> BoardT;

enum KnightMoveT {
    SE,
    ES,
    EN,
    NE,
    NW,
    WN,
    WS,
    SW,
    LAST
};

// Function prototypes

void initBoard(BoardT & gameBoard);
bool makeTour(BoardT & gameBoard);
bool visitSquares(BoardT & gameBoard, int row, int col, int nUnvisited, int & numCalls);
KnightMoveT operator++(KnightMoveT & move, int);
bool isValidPosition(BoardT & gameBoard, int row, int col);
void placePiece(BoardT & gameBoard, int row, int col, int seqNum);
void removePiece(BoardT & gameBoard, int row, int col);
int getRow(KnightMoveT move, int fromRow);
int getCol(KnightMoveT move, int fromCol);
ostream & operator<<(ostream & os, BoardT g);

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    BoardT gameBoard(N, N);
    initBoard(gameBoard);
    
    if (makeTour(gameBoard)) {
        cout << "Here ye find the wayward tour of our knight upon the field:";
    } else {
        cout << "Alas, our brave knight was confounded, only visiting thusly:";
    }
    cout << endl << endl;
    cout << gameBoard << endl;
    return 0;
}

//
// Function: initBoard
// Usage: initBoard(board);
// ---------------------------------
// Initializes a checkers/chess playing board with alternating white and
// black squares.
//
// The pass-by-reference board is modeled as a two-dimensional integer
// grid with zero values reflecting an unvisitied square.
//

void initBoard(BoardT & b) {
    for (int r = 0; r < b.numRows(); r++) {
        for (int c = 0; c < b.numCols(); c++) {
            b[r][c].seqNum = 0;
        }
    }
}

//
// Function: makeTour
// Usage: if (makeTour(gameBoard, startCol)) { . . . }
// ------------------------------------------------------
// Attempts to lead a knight on a tour of all positions on the game board
// without revisiting any square.
//
// Recursion with backtracking is employed.
//

bool makeTour(BoardT & gameBoard) {
    
    int nUnvisited = MAXSQUARES;
    int numCalls = 0;
    return visitSquares(gameBoard, 0, 0, nUnvisited, numCalls);
}

//
// Function: visitSquares
// Usage: if (visitSquares(board, startRow, startCol, squaresToVisit, numCalls))
// -----------------------------------------------------------------------------
// Returns true if the designated number of squares can be visited
// by a knight on the chessboard without revisiting the same square twice.
//

bool visitSquares(BoardT & gameBoard, int r, int c, int nUnvisited,
                  int & numCalls) {
    numCalls++;
    int seqNum = MAXSQUARES - nUnvisited + 1;

    // Base case
    //
    // Stop when all squares have been visited.

    if (nUnvisited <= 1) {
        placePiece(gameBoard, r, c, seqNum);
        return true;
    }

    //
    // Recursive case
    //
    // Iterate across directions a knight may move from the current square
    // looking for valid moves that don't visit already marked squares
    // (and that don't leap off the board :-).

    placePiece(gameBoard, r, c, seqNum);    // TODO: Hmm seqNum should be attr
                                            //       of the board itself.
    for (KnightMoveT move = SE; move < LAST; move++) {
        int nextRow = getRow(move, r);
        int nextCol = getCol(move, c);
        if (isValidPosition(gameBoard, nextRow, nextCol)) {
            if (visitSquares(gameBoard, nextRow, nextCol, nUnvisited - 1, numCalls)) {
                return true;
            }
        }
    }

    // Backtrack

    removePiece(gameBoard, r, c);
    return false;
}

//
// Operator: ++ (post-increment)
// Usage: for (KnightMoveT move = SE; move < LAST; move++) { . . . }
// -----------------------------------------------------------------
// Post-increment operator for variables of type KnightMoveT.
//
// This allows a knight chess piece to iterate across all move directions
// (expressed as a two-character cardinal value) on the chessboard.
//

KnightMoveT operator++(KnightMoveT & move, int) {
    KnightMoveT old = move;
    if (move < KnightMoveT(LAST)) {
        move = KnightMoveT(old + 1);
    }
    return old;
}

//
// Function: removePiece
// Usage: removePiece(gameBoard, row, col);
// -----------------------------------------
// Resets the sequence number of a square on the board indicating
// it has not been visited.  This is equivalent to removing a piece
// from that square.
//

void removePiece(BoardT & gameBoard, int row, int col) {
    gameBoard[row][col].seqNum = 0; // mark square as unvisited
}

//
// Function: isValidPosition
// Usage: if (isValidPosition(gameBoard, row, col)) { ... }
// ---------------------------------------------------------
// Returns true if a piece can be placed on the game board at the 
// unvisited row and column specified.
//

bool isValidPosition(BoardT & gameBoard, int row, int col) {
    
    // Is position on the board?
    if (!gameBoard.inBounds(row, col)) {
        return false;
    }
    
    // Is position unvisited?
    return (gameBoard[row][col].seqNum == 0);
}

//
// Function: getRow
// Usage: int newRow = getRow(knightMove, fromRow);
// ------------------------------------------------
// Returns the row value for a Knight movement from the
// given row position.
//
// This routine does not do bounds checking.
//

int getRow(KnightMoveT move, int fromRow) {
    int newRow = fromRow;;
    switch (move) {
        case NW:
        case NE:
            newRow -= 2;
            break;
            
        case SW:
        case SE:
            newRow += 2;
            break;
            
        case WN:
        case EN:
            newRow -= 1;
            break;
            
        case WS:
        case ES:
            newRow += 1;
            break;
            
        default:
            break;
    }
    return newRow;
}

//
// Function: getCol
// Usage: int newCol = getCol(knightMove, fromCol);
// ------------------------------------------------
// Returns the column value resulting from a Knight movement from the
// given column position.
//
// This routine does not do bounds checking.
//

int getCol(KnightMoveT move, int fromCol) {
    int newCol = fromCol;
    switch (move) {
        case NW:
        case SW:
            newCol -= 1;
            break;
            
        case NE:
        case SE:
            newCol += 1;
            break;
            
        case WN:
        case WS:
            newCol -= 2;
            break;
            
        case EN:
        case ES:
            newCol += 2;
            break;
            
        default:
            break;
    }
    return newCol;
}

//
// Function: placePiece
// Usage: placePiece(gameBoard, row, col, int seqNum);
// ----------------------------------------------------
// Updates the pass-by-reference gameBoard by marking the square at
// row and column with a sequence number indicating the order in which
// it was visited.
//
// Row 0 and column 0 reflect the upper lefthand corner of the board.
// Row dimensions increase toward the bottom of the screen.
// Column dimensions increase to the right.
//

void placePiece(BoardT & gameBoard, int row, int col, int seqNum) {
    gameBoard[row][col].seqNum = seqNum;
}

//
// Operator: <<
// Usage: cout << gameBoard << endl;
// ----------------------------------
// Override the stream insertion operator to display a simple,
// character-based gameBoard.
//

ostream & operator<<(ostream & os, BoardT gameBoard) {
    for (int r = 0; r < gameBoard.numRows(); r++) {
        for (int c = 0; c < gameBoard.numCols(); c++) {
            os << setw(3) << gameBoard[r][c].seqNum;
        }
        os << endl;
    }
    return os;
}