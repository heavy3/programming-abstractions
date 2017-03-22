/******************************************************************************
 * File: TrailblazerCosts.h
 *
 * Functions for computing the costs of navigating the terrain and maze worlds,
 * as well as heuristics for predicting costs.
 *
 * For extra credit, you may want to modify the heuristics contained in the
 * implementation file.	 Otherwise, you will not need to call these functions
 * directly.
 */
#ifndef TrailblazerCosts_Included
#define TrailblazerCosts_Included

#include "TrailblazerTypes.h"
#include "grid.h"

/* Function: terrainCost
 *
 * A function that given two adjacent locations in a terrain, returns the cost
 * associated with moving from the first location to the second.
 */
double terrainCost(Loc from, Loc to, Grid<double>& world);

/* Function: terrainHeuristic
 *
 * A function that, given two locations in a terrain, estimates the cost
 * of moving from the first location all the way to the second.
 */ 
double terrainHeuristic(Loc from, Loc to, Grid<double>& world);

/* Function: mazeCost
 *
 * A function that, given two adjacent locations in a maze, returns the cost
 * associated with moving from the first location to the second.
 */
double mazeCost(Loc from, Loc to, Grid<double>& world);

/* Function: mazeHeuristic
 *
 * A function that, given two locations in a maze, estimates the cost of
 * moving from the first location all the way to the second.
 */
double mazeHeuristic(Loc from, Loc to, Grid<double>& world);

/* Function: zeroHeuristic
 *
 * A heuristic function that always returns 0.	This function is used so that
 * the shortestPath function can be used to run Dijkstra's algorithm, which is
 * the same as running A* search with a zero heuristic function.
 */
double zeroHeuristic(Loc from, Loc to, Grid<double>& world);

#endif
