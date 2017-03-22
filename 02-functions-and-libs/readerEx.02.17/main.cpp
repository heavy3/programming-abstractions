//
// main.cpp
//
// This program produces an 8 x 8 checker board with red and black
// pieces in starting position.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 17
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/25/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "gwindow.h"
using namespace std;

//#define TESTING           // Uncomment for debug text to the console.

// Constants and Types

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = WINDOW_WIDTH;
const int SQUARES_PER_SIDE = 8;
const int SQUARE_WIDTH = WINDOW_WIDTH / double(SQUARES_PER_SIDE);
const int CHECKER_DIAMETER = SQUARE_WIDTH * 0.80;

// Function prototypes

void moveTo(int row, int col, int& x0, int& y0);
void fillSquare(GWindow gw, int x0, int y0, string color);
void addChecker(GWindow gw, int x0, int y0, string color);
bool isColoredSquare(int row, int col);

// Main program

int main(int argc, char * argv[]) {
    GWindow gw(WINDOW_WIDTH, WINDOW_HEIGHT);
    gw.setWindowTitle("Checkerboard");
    
    for (int row = 0; row < SQUARES_PER_SIDE; row++) {
        for (int col = 0; col < SQUARES_PER_SIDE; col++) {
            if (isColoredSquare(row, col)) {
                int x0, y0;
                moveTo(row, col, x0, y0);
                fillSquare(gw, x0, y0, "GRAY");
                if (row < 3) {
                    addChecker(gw, x0, y0, "RED");
                } else if (row > 4){
                    addChecker(gw, x0, y0, "BLACK");
                }
            }
        }
    }
    return 0;
}

// Function definitions

//
// Function: moveTo
// Usage: moveTo(1, 1, x, y);
// --------------------------
// Returns the upper lefthand pixel coordinate of the specified
// row and column on the checkerboard.
// Assumes the 8 x 8 grid is numbered with 0-based row and column
// numbers (i.e., 0 - 7).
//

void moveTo(int row, int col, int& x0, int& y0) {
    x0 = col * SQUARE_WIDTH;
    y0 = row * SQUARE_WIDTH;
    #ifdef TESTING
    cout << "(" << col << ", " << row << ") = "
         << "(" << setw(3) << x0 << ", " << setw(3) << y0 << ")" << endl;
    #endif
}

//
// Function: isColoredSquare
// Usage: if (isColoredSquare(0, 1)) { ... }
// -----------------------------------------
// Predicate function discerns if a given row and column in an 8 x 8
// checkerboard is a white or a colored square.
// Assumes row and column are 0-based (i.e., 0 - 7).
//

bool isColoredSquare(int row, int col) {
    return (((row % 2 == 0) && (col % 2 != 0)) || ((row % 2 != 0) && (col % 2 == 0)));
}

//
// Function: fillSquare
// Usage: fillSquare(x, y, "GRAY");
// --------------------------------
// Fills a portion of the checkerboard with a darkened square.
// Assumes coordinates are given in pixels and represent the upper lefthand
// corner of the square to color.
// Can call moveTo() to convert logical row/coloumn position to pixel location.
//

void fillSquare(GWindow gw, int x0, int y0, string color) {
    
    gw.setColor(color);
    gw.fillRect(x0, y0, SQUARE_WIDTH, SQUARE_WIDTH);
    #ifdef TESTING
    cout << "(" << setw(3) << x0 << ", " << setw(3) << y0 << ") = " << color
         << " square" << endl;
    #endif
}

//
// Function: addChecker
// Usage: addChecker(x, y, "RED");
// -------------------------------
// Draws a checker piece within a given square.
//

void addChecker(GWindow gw, int x0, int y0, string color) {
    int checkerX0 = x0 + (SQUARE_WIDTH - CHECKER_DIAMETER)/2.0;
    int checkerY0 = y0 + (SQUARE_WIDTH - CHECKER_DIAMETER)/2.0;
    
    gw.setColor(color);
    gw.fillOval(checkerX0, checkerY0, CHECKER_DIAMETER, CHECKER_DIAMETER);
    #ifdef TESTING
    cout << "(" << setw(3) << x0 << ", " << setw(3) << y0 << ") = " << color
         << " checker" << endl;
    #endif
}