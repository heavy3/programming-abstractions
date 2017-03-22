/******************************************************************************
 * File: WorldGenerator.h
 *
 * Code for generating the terrains and mazes in which paths will be computed.
 */

#include "WorldGenerator.h"
#include "Trailblazer.h"
#include "TrailblazerConstants.h"
#include "vector.h"
#include "set.h"
#include "random.h"
#include <cmath>
#include <algorithm>
#include <limits>
using namespace std;

/*** Internal constants ***/

/* Constant determining the roughness introduced in the middle point of a step
 * in the diamond-square algorithm.
 */
const double kTerrainShrinkFactor = 0.7;

/* Variance used by the Gaussian blur step. */
const double kSigma = 1.0;

/* Pi, more or less. */
const double kPi = 3.1415926535897932384626433;

/* The size of the window in which our Gaussian kernel will be applied.
 * Wikipedia suggest uses a window whose size is six times the variance, since
 * that amount should capture most of the distribution.
 */
const int kWindowSize = int(kSigma * 6.0 + 0.5);


/*** Internal function prototypes ***/

static void diamondStep(Grid<double>& heights, int size, double variation);
static double diamondStepAverage(Grid<double>& heights, int size,
                                 int row, int col);
static void squareStep(Grid<double>& heights, int size, double variation);
static double squareStepAverage(Grid<double>& heights, int size, 
                                int row, int col);

static Grid<double> wallsToGrid(Set<Edge>& walls, int numRows, int numCols);
static void smoothTerrain(Grid<double>& toSmooth);
static void normalizeTerrain(Grid<double>& heights);
static void flattenValleys(Grid<double>& heights);
static void diamondSquareGenerate(Grid<double>& heights);

/*** Function implementations ***/

/* Generates a random terrain of the specified size. */
Grid<double> generateRandomTerrain(int numRows, int numCols) {
  Grid<double> heights(numRows, numCols);

  diamondSquareGenerate(heights);
  smoothTerrain(heights);
  normalizeTerrain(heights);
  flattenValleys(heights);
  return heights;
}

/* Uses the user-provided maze creation routine to generate a maze and then
 * render it into a grid.
 */
Grid<double> generateRandomMaze(int numRows, int numCols) {
  Set<Edge> maze = createMaze(numRows, numCols);
  return wallsToGrid(maze, numRows, numCols);
}

/*** Internal function implementations ***/

/* Using the diamond-square algorithm (also called the plasma fractal), generate
 * an initial terrain.
 */
static void diamondSquareGenerate(Grid<double>& heights) {
  /* The size of each of the squares in the diamond-square algorithm. */
  int size = (min(heights.numRows(), heights.numCols()) - 1) / 2;
  double variation = 1.0;

  /* Continue running iterations until the square size shrinks down to 0. */
  while (size > 0) {
    diamondStep(heights, size, variation);
    squareStep(heights, size, variation);
    size /= 2;
    variation *= kTerrainShrinkFactor;
  }
}

/* Given a terrain whose heights are variable, normalize the terrain heights
 * to the range [0, 1].
 */
static void normalizeTerrain(Grid<double>& heights) {
  /* Compute the maximum and minimum heights. */
  double maxHeight = -numeric_limits<double>::infinity();
  double minHeight = numeric_limits<double>::infinity();
  for (int row = 0; row < heights.numRows(); row++) {
    for (int col = 0; col < heights.numCols(); col++) {
      maxHeight = max(maxHeight, heights[row][col]);
      minHeight = min(minHeight, heights[row][col]);
    }
  }
  
  /* Remap everything from the range [minHeight, maxHeight] to [0, 1]. */
  double range = maxHeight - minHeight;
  for (int row = 0; row < heights.numRows(); row++) {
    for (int col = 0; col < heights.numCols(); col++) {
      heights[row][col] -= minHeight;
      heights[row][col] /= range;
    }
  }
}

/* To make the terrain more interesting, square eight height.  This has the
 * effect of depressing valleys further and sharpening peaks.
 */
static void flattenValleys(Grid<double>& heights) {
  for (int row = 0; row < heights.numRows(); row++) {
    for (int col = 0; col < heights.numCols(); col++) {
      heights[row][col] *= heights[row][col];
    }
  }
}

/* Performs a diamond step in the diamond-square algorithm, averaging out the
 * value of each cell based on the cells in the same diamond as it.
 */
static void diamondStep(Grid<double>& heights, int size, double variation) {
  int stride = size * 2;
  for (int row = size; row < heights.numRows(); row += stride) {
    for (int col = size; col < heights.numCols(); col += stride) {
      heights[row][col] = diamondStepAverage(heights, size, row, col) +
                          randomReal(-variation, variation);
    }
  }
}

/* Computes the average of four grid points in a diamond around the specified
 * location.
 */
static double diamondStepAverage(Grid<double>& heights, int size, 
                                 int row, int col) {
  double sum = heights[row - size][col - size] +
               heights[row - size][col + size] +
               heights[row + size][col - size] +
               heights[row + size][col + size];
  return sum / 4.0;
}

/* Performs a square step in the diamond-square algorithm, averaging each
 * cell with the values of neighbors in a square pattern near it.
 */
static void squareStep(Grid<double>& heights, int size, double variation) {
  int stride = size * 2;
  for (int row = size; row < heights.numRows(); row += stride) {
    for (int col = 0; col < heights.numCols(); col += stride) {
      heights[row][col] = squareStepAverage(heights, size, row, col) +
                          randomReal(-variation, variation);
    }
  }
  
  for (int row = 0; row < heights.numRows(); row += stride) {
    for (int col = size; col < heights.numCols(); col += stride) {
      heights[row][col] = squareStepAverage(heights, size, row, col) +
                          randomReal(-variation, variation);
    }
  }
}

/* Computes the average of all cells in the same square as the given cell. */
static double squareStepAverage(Grid<double>& heights, int size, 
                                int row, int col) {
  double sum = 0.0;
  int count = 0;
	
  if (row - size >= 0) {
    sum += heights[row - size][col];
    count++;
  }
  if (row + size < heights.numRows()) {
    sum += heights[row + size][col];
    count++;
  }
  if (col - size >= 0) {
    sum += heights[row][col - size];
    count++;
  }
  if (col + size < heights.numCols()) {
    sum += heights[row][col + size];
    count++;
  }
  
  return sum / count;
}

/* Creates a kernel matrix that can be used for a Gaussian blur. */
static Grid<double> createGaussianKernel() {
	Grid<double> result(kWindowSize, kWindowSize);
	for (int i = 0; i < kWindowSize; i++) {
		for (int j = 0; j < kWindowSize; j++) {
      /* We want to compute
       *
       * (1 / sqrt(2 pi sigma) * e^{(x^2 + y^2) / 2(sigma^2)}
       *
       * Sorry it's messy!
       */
			result[i][j] = exp(-(pow(i - kWindowSize / 2.0, 2.0) +
													 pow(j - kWindowSize / 2.0, 2.0)) / 
                         (2 * kSigma * kSigma)) /
                    sqrt(2 * kPi * kSigma);
		}
	}
	return result;
}

/* Applies a Gaussian blur to the maze. */
static void smoothTerrain(Grid<double>& terrain) {
	Grid<double> kernel = createGaussianKernel();
	Grid<double> result(terrain.numRows(), terrain.numCols());
	
  /* Convolve the two matrices together.  For each point in the original grid,
   * compute the weighted sum of the Gaussian kernel applied at that point, then
   * write the result back to the resulting image.
   */
	for (int i = 0; i < terrain.numRows(); i++) {
		for (int j = 0; j < terrain.numCols(); j++) {
      /* We might not apply the entire kernel if we're on a border, so track
       * how much total weight was used.
       */
			double totalWeightUsed = 0.0;
			for (int a = 0; a < kernel.numRows(); a++) {
				for (int b = 0; b < kernel.numCols(); b++) {
          /* Determine the sample point from the original image.  If it's not
           * in-bounds, skip it.
           */
					int sampleRow = i + a - kernel.numRows() / 2;
					int sampleCol = j + b - kernel.numRows() / 2;
					if (!terrain.inBounds(sampleRow, sampleCol)) continue;
					
					totalWeightUsed += kernel[a][b];
					result[i][j] += kernel[a][b] * terrain[sampleRow][sampleCol];
				}
			}
			result[i][j] /= totalWeightUsed;
		}
	}
	
	terrain = result;
}

/* Given a set of walls, converts those walls into a 2D grid. */
static Grid<double> wallsToGrid(Set<Edge>& walls, int numRows, int numCols) {
  /* We need numRows - 1 interstitial blocks and numRows - 1 interstitial
   * blocks.
   */
  Grid<double> result(2 * numRows - 1, 2 * numCols - 1);
	
  /* Fill everything in. */
  for (int i = 0; i < result.numRows(); i++) {
    for (int j = 0; j < result.numCols(); j++) {
      result[i][j] = kMazeWall;
    }
  }
	
  /* Clear all cells corresponding to grid points. */
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      result[2 * i][2 * j] = kMazeFloor;
    }
  }
	
  /* Clear all interstitial cells corresponding to connections. */
  foreach (Edge wall in walls) {
    /* Confirm the endpoints are in-bounds. */
    if (!result.inBounds(wall.start.row, wall.start.col) ||
        !result.inBounds(wall.end.row, wall.end.col)) {
      error("Edge endpoints are out of range.");
    }

    int row = 2 * wall.start.row + (wall.end.row - wall.start.row);
    int col = 2 * wall.start.col + (wall.end.col - wall.start.col);
    result[row][col] = kMazeFloor;
  }
	
  return result;
}
