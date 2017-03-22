//
// main.cpp
//
// This program implements the function:
//
//    int shortestPathLength(Maze & maze, Point start);
//
// which returns the length of the shortest path in the maze from the 
// specified position to any exit.  If there is no solution, 
// shortestPathLength returns –1.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// The code for adjacentPoint comes from Figure 9-3.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/17/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
#include "console.h"
#include "direction.h"
#include "gwindow.h"
#include "maze.h"
#include "point.h"
#include "vector.h"
#include "set.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.01\n";
const std::string DETAIL = "Shortest Path Through a Maze";
const std::string BANNER = HEADER + DETAIL;
const std::string MAZEFILE = "maze.txt";
const int NO_PATH = -1;     // No path through maze.

// Function prototypes

int shortestPathLength(Maze & maze, Point start);
Point adjacentPoint(Point start, Direction dir);
bool findSolutionLengths(Maze & maze, Point start, int & length, Set<int> & solveLengths);

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    GWindow gw;
    Maze maze(MAZEFILE, gw);
    maze.drawMaze();
    Vector<Point> path;
    
    int length = shortestPathLength(maze, maze.getStartPosition());
    if (length == NO_PATH) {
        cout << "No solution found. :-/" << endl;
    } else {
        cout << "Shortest past through maze = " << length << endl;
    }
    return 0;
}

//
// Function: shortestPathLength
// Usage: int shortestSolve = shortestPathLength(maze, start);
// -----------------------------------------------------------
// Returns length of shortest solve for a maze from a given start point.
// If no solve is possible, then NO_PATH is returned.
//

int shortestPathLength(Maze & maze, Point start) {
    int length = 0;
    Set<int> solveLengths;
    findSolutionLengths(maze, start, length, solveLengths);
    if (solveLengths.size()) {
        return solveLengths.first();    // shortest length is first entry
    } else {
        return NO_PATH;
    }
}

//
// Function: findSolutionLengths
// Usage: if (findSolutionLengths(maze, start, length, solveLengths)) ...
// ----------------------------------------------------------------------
// Finds the various solution paths through a maze and returns the corresponding
// solution lengths through a pass-by-reference set.
//

bool findSolutionLengths(Maze & maze, Point start, int & length,
                         Set<int> & 0lveLengths) {

    // Base case

    if (maze.isOutside(start)) return true;
    if (maze.isMarked(start)) return false;
    
    // Recursive case

    length++;
    maze.markSquare(start);
    for (Direction dir = NORTH; dir <= WEST; dir++) {
        if (!maze.wallExists(start, dir)) {
            if (findSolutionLengths(maze, adjacentPoint(start, dir), length,
                                    solveLengths)) {
                solveLengths.add(length);
                break;
            }
        }
    }

    // Backtracking
    
    maze.unmarkSquare(start);
    length--;
    return false;
}

//
// Function: adjacentPoint
// Usage: Point finish = adjacentPoint(start, dir);
// ------------------------------------------------
// Returns the Point that results from moving one square from start
// in the direction specified by dir.  For example, if pt is the
// point (1, 1), calling adjacentPoint(pt, EAST) returns the
// point (2, 1).  To maintain consistency with the graphics package,
// the y coordinates increase as you move downward on the screen.  Thus,
// moving NORTH decreases the y component, and moving SOUTH increases it.
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
