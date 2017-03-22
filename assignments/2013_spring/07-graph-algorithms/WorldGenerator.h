/******************************************************************************
 * File: WorldGenerator.h
 *
 * Code for generating the terrains and mazes in which paths will be computed.
 */

#ifndef WorldGenerator_Included
#define WorldGenerator_Included

#include "grid.h"

/* Function: generateRandomTerrain
 *
 * Generates a random terrain to navigate which has the specified number of
 * rows and columns.
 */
Grid<double> generateRandomTerrain(int numRows, int numCols);

/* Function: generateRandomMaze
 *
 * Generates a random maze to navigate.  This code will internally call your
 * maze generation function to obtain the set of edges in the maze, then will
 * convert that set of edges into a Grid.
 */
Grid<double> generateRandomMaze(int numRows, int numCols);

#endif
