//
// Trailblazer.h
//
// Exports functions that use Dijkstra's algorithm, A* search, and Kruskal's
// algorithm as specified in the assignment handout.
//
// --------------------------------------------------------------------------
// Assignment: 7 Trailblazer
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/260%20Assignment%207.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/16/2017.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#ifndef Trailblazer_Included
#define Trailblazer_Included

#include "TrailblazerTypes.h"
#include "set.h"
#include "grid.h"

// Types

struct LocStateT {
    bool blackListed;
    bool enqueued;
    double costSoFar;
    Loc parent;
};

// Function prototypes

// Function: shortestPath
// Usage: shortestPath(start, end, world, costFn);
// -----------------------------------------------
// Uses Dijkstra's algorithm to find the least-cost path between two locations.
// The cost of moving from one location to the next is specified by the given
// cost function.
//
// The resulting path is returned as a Vector<Loc> containing the sequence
// of locations to visit from start to end.  If no path is found, this function
// reports and error.

// In Part Two of this assignment, you will need to add an additional parameter
// to this function that represents the heuristic to use while performing the
// search.  Make sure to update both this function prototype and the
// implementation inside of Trailblazer.cpp.

Vector<Loc> shortestPath(Loc start,
                         Loc end,
                         Grid<double>& world,
                         double costFn(Loc from, Loc to, Grid<double>& world));
Vector<Loc> getAdjLocs(const Loc& loc,
                       const Grid<double>& world,
                       Grid<LocStateT>& state);
double getLocCostSoFar(Grid<LocStateT>& state, const Loc& l);
void initLocState(Grid<LocStateT> state, const Loc& start);
bool isLocEnqueued(Grid<LocStateT>& state, const Loc& l);
void setLocEnqueued(Grid<LocStateT>& state, const Loc& l, bool b);
void setLocBlackListed(Grid<LocStateT>& state, const Loc& l, bool b);
void setLocCostSoFar(Grid<LocStateT>& state, const Loc& l, double c);
void setLocParent(Grid<LocStateT>& state, const Loc& l, const Loc& parent);

// Function: createMaze
//
// Creates a maze of the specified dimensions using a randomized version of
// Kruskal's algorithm, then returns a set of all of the edges in the maze.
//
// As specified in the assignment handout, the edges you should return here
// represent the connections between locations in the graph that are passable.
// Our provided starter code will then use these edges to build up a Grid
// representation of the maze.

Set<Edge> createMaze(int numRows, int numCols);
#endif
