//
// main.cpp
//
// This program implements a maze-traversal function, findSolutionPath,
// which uses a vector to keep track of the current path as it goes 
// (to avoid the performance penalty in exercise 9.02).
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// The code for adjacentPoint comes from Figure 9-3.
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
#include "vector.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.03\n";
const std::string DETAIL = "A vector of cookie crumbs out of the maze.";
const std::string BANNER = HEADER + DETAIL;
const std::string MAZEFILE = "maze.txt";

// Function prototypes

bool findSolutionPath(Maze & maze, Point start, Vector<Point> & path);
Point adjacentPoint(Point start, Direction dir);

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    GWindow gw;
    Maze maze(MAZEFILE, gw);
    maze.drawMaze();
    Vector<Point> pathOutOfMaze;
    
    if (findSolutionPath(maze, maze.getStartPosition(), pathOutOfMaze)) {
        cout << "Here is a path out of the maze: " << endl;
        cout << pathOutOfMaze << endl;
    } else {
        cout << "No path found out of the maze." << endl;
    }
    return 0;
}

//
// Function: findSolutionPath
// Usage: findSolutionPath(maze, start, numCalls, pathOut);
// -------------------------------------------------
// Attempts to generate a solution to the current maze from the specified
// start point.  The findSolutionPath function returns true if the maze has a
// solution and false otherwise.  The implementation uses recursion
// to solve the submazes that result from marking the current square
// and moving one step along each open passage.
//
// A vector of points records the first path discovered out of the maze.
// This avoids the severe performance penalty of unmarking previously visited
// squares in order to capture the final path out.
//

bool findSolutionPath(Maze & maze, Point start, Vector<Point> & pathOut) {
    
    // Base case

    if (maze.isOutside(start)) {

        // Place final crumb just outside maze as we exit.

        pathOut += start;
        return true;
    }
    if (maze.isMarked(start)) return false;
    
    // Recursive case

    maze.markSquare(start);

    // Place a crumb before taking next step.

    pathOut += start;
    for (Direction dir = NORTH; dir <= WEST; dir++) {
        if (!maze.wallExists(start, dir)) {
            Point tryPos = adjacentPoint(start, dir);
            if (findSolutionPath(maze, tryPos, pathOut)) {
                return true;
            }
        }
    }

    // Backtrack from failed path to earlier decision point, however ...
    //
    // • don't unmark the failed path; that leads to inefficient retraversal
    // • do clean up any crumbs, though; crumbs should just mark the
    //   successful path out of the maze

    pathOut.remove(pathOut.size() - 1);
    return false;
}

//
// Function: adjacentPoint
// Usage: Point finish = adjacentPoint(start, dir);
// ------------------------------------------------
// Returns a point next to the current position along a given direction.
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