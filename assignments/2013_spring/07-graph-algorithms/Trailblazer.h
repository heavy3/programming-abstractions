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

typedef Set<Loc> Cluster;

const int MIN_EDGE_WT = 1;
const int MAX_EDGE_WT = 8;

// Function prototypes

// Function: shortestPath
// Usage: shortestPath(start, end, world, costFn, hFn);
// ----------------------------------------------------
// Uses Dijkstra's algorithm (or the A* variant) to find the least-cost path
// between two locations.
//
// The cost of moving between adjacent locations is provided by costFn while
// a heuristic function may be passed in as a 5th parameter
// to effect the A* variant which focusses the search for optimal path
// to a more channel-like or elliptical region (depending upon hFn) versus
// the more radial pattern that would otherwise sweep out from the start
// location as Dijkstra's algorithm proceeds.  If hFn always returns 0, then
// shortestPath reduces to employing Dijkstra's algorithm without optimization.
//
// The resulting path is returned as a Vector<Loc> containing the sequence
// of locations to visit from start to end. If no path is found, this function
// reports and error.

Vector<Loc> shortestPath(Loc start,
                         Loc end,
                         Grid<double>& world,
                         double costFn(Loc from, Loc to, Grid<double>& world),
                         double hFn(Loc from, Loc to, Grid<double>& world));
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
// Usage: Set<Edge> maze = createMaze(numRows, numCols);
// -----------------------------------------------------
// Uses Kruskal's algorithm to generate a randomized rectangular maze
// of specified size.  The maze is provided in the form of a collection of
// graph edges that constitute a minimal spanning set and correspond
// to the passage ways through the maze.

Set<Edge> createMaze(int numRows, int numCols);

Loc getClusterKey(const Map<Loc, Set<Loc> >& clusters, Loc loc);
void mergeLocClusters(Map<Loc, Set<Loc> >& clusters, Loc key1, Loc key2);
void makeEdges(Set<Edge>& edges, int numRows, int numCols);
void makeClusters(Map<Loc, Set<Loc> >& clusters, int numRows, int numCols);
ostream& operator<<(ostream& os, Loc l);

#endif
