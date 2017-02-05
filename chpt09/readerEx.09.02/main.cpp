//
// main.cpp
//
// This program evaluates the cost of maze traversal with and without
// marking visited positions.  To do this, the solveMaze program
// is extended to record the number of recursive calls made during
// traversal from start to exit for a cleverly formed maze.
//
// Q: How much effort is saved by not visiting already explored positions
//    with a recursive backtracking algorithm?
//
// A: For the maze supplied in the text, marking visited positions 
//    causes a ~100x runtime efficiency over leaving visited positions 
//    unmarked when backtracking.
//
//    Here's the data I gathered:
//
//    -------------------------------------------------------------------
//    Backtracking Algorithm         # recursive calls to solveMaze
//    -------------------------------------------------------------------
//    unmark visited positions                   6316
//    mark visited positions                      62
//
//    Traversing mazes with large open spaces especially benefit 
//    from this optimization, though it does come at the cost of visualizing
//    the final exit route.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// The code for solveMaze and adjacentPoint comes from Figure 9-3.
// The solveMaze function has been extended to support a 4th paramter.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/21/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <string>
#include "console.h"
#include "direction.h"
#include "gwindow.h"
#include "maze.h"
#include "point.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.02\n";
const std::string DETAIL = "Cost of Unmarking Visted Sqaures in a Maze";
const std::string BANNER = HEADER + DETAIL;
const std::string MAZEFILE = "maze.txt";

// Function prototypes

bool solveMaze(Maze & maze, Point start, int & numCalls, bool unmark = false);
Point adjacentPoint(Point start, Direction dir);

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    GWindow gw;
    Maze maze(MAZEFILE, gw);
    maze.drawMaze();
    int numCalls = 0;
    solveMaze(maze, maze.getStartPosition(), numCalls);
    cout << "Number of recursive calls: " << numCalls << endl;
    cout << "(without unmarking visited squares)" << endl;
    return 0;
}

//
// Function: solveMaze
// Usage: solveMaze(maze, start, numCalls);
//        solveMaze(maze, start, numCalls, unmarkVisited);
// ----------------------------------------------------------------
// Traverses a maze from a starting location to an exit by recursively 
// exploring the space along orthogonal lines. The efficiency of the 
// traversal is captured by the numCalls variable.  An optional 4th 
// parameter controls the backtracking behavior of the algorithm when 
// a given path that doesn't work out and we rollback to an earlier 
// state to try a different direction.
//

bool solveMaze(Maze & maze, Point start, int & numCalls, bool unmark) {
    
    // Base case

    if (maze.isOutside(start)) return true;
    if (maze.isMarked(start)) return false;
    
    // Recursive case

    maze.markSquare(start);
    for (Direction dir = NORTH; dir <= WEST; dir++) {
        if (!maze.wallExists(start, dir)) {
            if (solveMaze(maze, adjacentPoint(start, dir), ++numCalls)) {
                return true;
            }
        }
    }

    // Backtrack

    if (unmark) maze.unmarkSquare(start);
    return false;
}

//
// Function: adjacentPoint
// Usage: Point finish = adjacentPoint(start, dir);
// ------------------------------------------------
// Returns a point next to the current position along
// a given direction.
//

Point adjacentPoint(Point start, Direction dir) {
    switch (dir) {
        case NORTH: return Point(start.getX(), start.getY() - 1);
        case EAST:  return Point(start.getX() + 1, start.getY());
        case SOUTH: return Point(start.getX(), start.getY() + 1);
        case WEST:  return Point(start.getX() - 1, start.getY());
    }
    return start;
}