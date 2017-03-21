//
// Trailblazer.cpp
//
// Implementation of the graph algorithms that comprise the Trailblazer
// assignment.
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
// Created by Glenn Streiff on 3/16/2017 - 3/21/2017.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include "Trailblazer.h"
#include "TrailblazerGraphics.h"
#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
#include "TrailblazerCosts.h"
#include "random.h"

using namespace std;

// Function: shortestPath
// Usage: shortestPath(start, end, world, costFn, hFn);
// ----------------------------------------------------
// If a path from start to end exists, this function returns a sequence
// vector of locations that describe that path.  Otherwise an error is thrown.
//
// Approach
//
// Employs Dijkstra's algorithm (as well as the heuristic-optimized variant
// known as A* Search) to find the least-cost path between two locations.
//
// The big idea is minimizing the cost between two locations by considering a
// (possibly cheaper) route through an intermediary location proximate to both,
// a kind of transitive adjacency.  For example, it may be cheaper to fly from
// Austin to Seattle by way of Denver.
//
// Algorithmically, we start with an initial position and only consider the
// cost of travelling to directly adjacent locations, easily calculated with the
// costFn function.  This puts us in a position to evaluate -transitive-
// adjacencies on subsequent iterations.  A priority queue is used to carefully
// manage the order in which we evaluate proximate locations for least cost
// from the path's origin.
//
// This ensures we've minimized those costs before evaluating paths that radiate
// out from the intermediary positions in an expanding region that will grow
// to encompass the destination.  Once the search region and destination
// intersect, we backtrack from end to start, picking up 'cookie crumbs'
// we've left prior that describe how we arrived at a given location (described
// as the parent location for a given position).
//
// Dijkstra versus A* Search
//
// The cost of moving from one location to the next is specified by the given
// cost function, costFn.  The optional heuristic function, hFn, can improve
// performance by giving preference in the priority queue to intermediate
// locations which are more proximate to the end location.

Vector<Loc> shortestPath(Loc start,
                         Loc end,
                         Grid<double>& world,
                         double costFn(Loc from, Loc to, Grid<double>& world),
                         double hFn(Loc from, Loc to, Grid<double>& world)) {
    
    Vector<Loc> path;           // Sequetial path from start to end.
    
    if (start == end) {
        path.add(start);
        return path;
    }
    
    // Track location attributes such as cost-from-start, parent location,
    // queue status, and blacklist status.  (Should probably use a set here
    // to avoid O(n²) memory usage :-| but set was fighting me :-/).
    
    Grid<LocStateT> state(world.nRows, world.nCols);
    double hCost = hFn(start, end, world);
    initLocState(state, start);
    
    TrailblazerPQueue<Loc> pq;  // Priority queue; cost from start is priority.
    pq.enqueue(start, hCost);   // Seed priority from heuristic function which
                                // favors more direct path from start to end.
    Loc n = start;              // Least cost loc so far from start.
    
    while (!pq.isEmpty()) {
        n = pq.dequeueMin();    // Dequeue least cost loc so far from start.
        
        setLocEnqueued(state, n, false);
        setLocBlackListed(state, n, true);
        colorCell(world, n, GREEN);

        if (n == end) break;    // Found least cost path to end location. Done!
        
        // Iterate over the locations adjacent to n.
        // Can we improve their 'cost-from-start' value if we pass through n?
        
        Vector<Loc> adjLocs = getAdjLocs(n, world, state);// non-BL'd adjacents
        for (int i = 0; i < adjLocs.size(); i++) {
            
            Loc adj = adjLocs[i];
            double nCost = getLocCostSoFar(state, n);   // node cost
            double eCost = costFn(n, adj, world);       // n-to-adj edge cost
            double hCost = hFn(adj, end, world);        // heuristic cost
            double costToAdjViaN = nCost + eCost;
            double costToAdjViaNwithHeuristic = costToAdjViaN + hCost;
            
            // Is this node already in the priority queue?
            // If so, it will have cost info we may want to update.
            
            if (isLocEnqueued(state, adj)){
                double currAdjCost = getLocCostSoFar(state, adj);
                if (costToAdjViaN < currAdjCost) {
                    
                    // It's cheaper to go from start to the adjacent location
                    // if we pass through n first.
                    
                    // Make it more likely we'll dequeue this node on a future
                    // iteration.
                    
                    pq.decreaseKey(adj, costToAdjViaNwithHeuristic);
                    setLocCostSoFar(state, adj, costToAdjViaN);
                    setLocParent(state, adj, n);    // Drop cookie crumb.
                }
            } else {
                
                // Wire up new adjacent location it into the graph, so to speak.
                //
                // Its associated cost from the start is just n's plus the edge
                // cost to get from n to the adjacent location.
                
                pq.enqueue(adj, costToAdjViaNwithHeuristic);
                setLocEnqueued(state, adj, true);
                setLocCostSoFar(state, adj, costToAdjViaN);
                setLocParent(state, adj, n);
                colorCell(world, adj, YELLOW);
            }
        }
    }
    
    if (n == end) {
        
        // Backtrack from end to start positions, following the parent
        // locations that delineate the path.
        
        for (Loc n = end; n != start; n = state[n.row][n.col].parent) {
            path.insert(0, n);
        }
        path.insert(0, start);
        
    } else {
        error("shortestPath: No path found from start to end.");
    }
    return path;
}

// Function: getAdjLocs
// Usage: Vector<Loc> v = getAdjLocs(loc, world, blacklist);
// ---------------------------------------------------------
// Returns a vector of locations adjacent to a given position in a 2D
// grid world.

Vector<Loc> getAdjLocs(const Loc& loc,
                       const Grid<double>& world,
                       Grid<LocStateT>& state) {
    
    Vector<Loc> adjLoc;
    
    for (int r = loc.row - 1; r <= loc.row + 1; r++) {
        for (int c = loc.col - 1; c <= loc.col + 1; c++) {
            if (world.inBounds(r, c) && !(r == loc.row && c == loc.col)) {
                Loc l = makeLoc(r, c);
                if (state[r][c].blackListed) continue;
                adjLoc.add(l);
            }
        }
    }
    return adjLoc;
}

// -------------------------------------
// Getters & Setters for Grid<LocStateT>
// -------------------------------------

double getLocCostSoFar(Grid<LocStateT>& state, const Loc& l) {
    return state[l.row][l.col].costSoFar;
}

void initLocState(Grid<LocStateT> state, const Loc& start) {
    for (int r = 0; r < state.nRows; r++) {
        for (int c = 0; c < state.nCols; c++) {
            state[r][c].enqueued = false;
            state[r][c].blackListed = false;
            state[r][c].costSoFar = LONG_MAX;   // Virtually ∞
            state[r][c].parent = makeLoc(start.row, start.col);
        }
    }
    state[start.row][start.col].enqueued = true;
    state[start.row][start.col].costSoFar = 0;
}

bool isLocEnqueued(Grid<LocStateT>& state, const Loc& l) {
    return state[l.row][l.col].enqueued;
}

void setLocEnqueued(Grid<LocStateT>& state, const Loc& l, bool b) {
    state[l.row][l.col].enqueued = b;
}

void setLocBlackListed(Grid<LocStateT>& state, const Loc& l, bool b) {
    state[l.row][l.col].blackListed = b;
}

void setLocCostSoFar(Grid<LocStateT>& state, const Loc& l, double cost) {
    state[l.row][l.col].costSoFar = cost;
}

void setLocParent(Grid<LocStateT>& state, const Loc& l, const Loc& parent) {
    state[l.row][l.col].parent = parent;
}


// Function: createMaze
// Usage: Set<Edge> maze = createMaze(numRows, numCols);
// -----------------------------------------------------
// Uses Kruskal's algorithm to generate a randomized rectangular maze
// of specified size.  The maze is provided in the form of a collection of
// graph edges that constitute a minimal spanning set and correspond
// to the passage ways through the maze.

Set<Edge> createMaze(int numRows, int numCols) {
    Set<Edge> minSpanTree;

    Set<Edge> edges;
    makeEdges(edges, numRows, numCols);
    
    // Create a collection of singleton node clusters where a
    // cluster is a set of joined locations/nodes in the graph.
    // Initially nothing is joined.  Every node in the graph is
    // an island.

    Map<Loc, Set<Loc> > clusters;
    makeClusters(clusters, numRows, numCols);
    
    // Associate random weights/costs to the arcs and add them to the
    // priority queue.
    
    TrailblazerPQueue<Edge> pq;
    for (Edge e: edges) {
        int randWt = randomInteger(MIN_EDGE_WT, MAX_EDGE_WT);
        double priority = randWt;
        pq.enqueue(e, priority);
    }
    
    // Build up the minimal spanning tree, considering
    // the least-cost edges first.
    
    while (!pq.isEmpty()) {
        Edge e = pq.dequeueMin();
        Loc n1 = e.start;
        Loc n2 = e.end;
        
        // Is this edge already part of a larger cluster?
        // If so, do nothing and proceed to next edge.
        
        Loc cl1Key = getClusterKey(clusters, n1);
        Loc cl2Key = getClusterKey(clusters, n2);
        if (cl1Key != cl2Key) {
            
            // Otherwise ...
            //
            // Endpoints are in different node clusters.
            // Merge those two clusters and add the edge to the
            // spanning tree.
            
            mergeLocClusters(clusters, cl1Key, cl2Key);
            minSpanTree.add(e);
        }
    }
    return minSpanTree;
}

// Function: getClusterKey
// Usage: Loc key = getClusterKey(clusters, loc)
// ---------------------------------------------
// Returns a key into the node-cluster which contains the specified location.
//
// Initially, each location is a key into a set with only that
// location as a member ... which probably seems a bit odd.
//
// However as the algorithm proceeds various endpoints will be clustered
// together into these location sets until only one cluster remains.
//
// So a given location key may be associated with a single member set
// that is either the location itself -or- the key to the larger
// encompassing cluster for that location.

Loc getClusterKey(const Map<Loc, Set<Loc> >& clusters, Loc loc) {
    if (!clusters.containsKey(loc)) {
        error("getClusterKey: corrupted cluster map");
    }
    
    // Singleton sets may be the location itself (typical at startup)
    // -or- the key to the cluster where that location now resides.
    
    if (clusters[loc].size() == 1) {
        if (clusters[loc].contains(loc)) {
            return loc;
        } else {
            return clusters[loc].first(); // Follow pointer to location cluster.
        }
    }
    return loc; // loc is key to a multi-node cluster
}

// Function: mergeLocClusters
// Usage: mergeLocClusters(clusters, locKey1, locKey2);
// ----------------------------------------------------
// Merge the set of locations indexed by key2 into the set of locations
// indexed by key1.
//
// Redirect all the keys for locations in the second set to the first set
// so we can easily find the new home of each location.

void mergeLocClusters(Map<Loc, Set<Loc> >& clusters, Loc key1, Loc key2) {
    clusters[key1] += clusters[key2];   // merge clusters
    
    // Redirect the keys associated with locations in the second cluster
    // to point to the first cluster.
    
    Set<Loc> updateKeys = clusters[key2] - key2;    // avoid self-modifying data
    for (Loc key: updateKeys) {
        clusters[key].clear();
        clusters[key].add(key1);
    }
    clusters[key2].clear();
    clusters[key2].add(key1);
}

// Function: makeClusters
// Usage: makeClusters(clusters, numRows, numCols);
// ------------------------------------------------
// Map each location in the graph/grid to a set containing just that location.
//
// (Over time, these sets will dwindle or grow depending upon how nodes are
// joined together.)

void makeClusters(Map<Loc, Set<Loc> >& clusters, int numRows, int numCols) {
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            Loc l = makeLoc(r, c);
            Set<Loc> locs;
            locs.add(l);
            clusters[l] = locs;
        }
    }
}

// Function: makeEdges
// Usage: makeEdges(edges, numRows, numCols);
// ------------------------------------------
// Build up a complete set of orthogonally connected edges for a given
// graph with the dimensions specified.

void makeEdges(Set<Edge>& edges, int numRows, int numCols) {
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            if (r + 1 < numRows) {
                Loc top = makeLoc(r, c);
                Loc bot = makeLoc(r+1, c);
                edges.add(makeEdge(top, bot));
            }
            if (c + 1 < numCols) {
                Loc left = makeLoc(r, c);
                Loc right = makeLoc(r, c+1);
                edges.add(makeEdge(left, right));
            }
        }
    }
}

ostream& operator<<(ostream& os, Loc l) {
    os << "[r=" << l.row << ", " << "c=" << l.col << "]";
    return os;
}
