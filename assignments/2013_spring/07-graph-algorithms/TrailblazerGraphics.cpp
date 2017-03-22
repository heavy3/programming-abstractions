#include "TrailblazerGraphics.h"
#include "TrailblazerConstants.h"
#include "WorldGenerator.h"
#include "TrailblazerCosts.h"
#include "Trailblazer.h"
#include "vector.h"
#include "gwindow.h"
#include "gobjects.h"
#include "gevents.h"
#include "ginteractors.h"
#include "filelib.h"
#include "strlib.h"
#include "error.h"
#include <string>
#include <iomanip>
#include <iostream>
#include <limits>
using namespace std;

/*** Module Types ***/

/* Type: WorldType
 *
 * An enumerated type tracking what type of world is currently selected so that
 * we can determine which clicked locations are legal.
 */
enum WorldType {
  TERRAIN_WORLD,
  MAZE_WORLD
};

/* Type: WorldSize
 *
 * An enumerated type representing how large the world is, categorized as one
 * of three different size classes.
 */
enum WorldSize {
  SMALL_WORLD, MEDIUM_WORLD, LARGE_WORLD, HUGE_WORLD
};

/* Type: AlgorithmType
 *
 * An enumerated type representing one of Dijkstra's algorithm or A* search.
 */
enum AlgorithmType {
  DIJKSTRA, A_STAR
};

/* Type: UIState
 *
 * The UI is a state machine that can be in one of four states:
 *
 *  1. Fresh:  Nothing has been done yet.
 *  2. Marked: One location has been selected.
 *  3. Drawn:  Both locations have been selected, and a path is drawn.
 *
 * This enumerated type stores this information.
 */
enum UIState {
  FRESH, MARKED, DRAWN
};

/* Type: State
 *
 * A utility struct that bundles together the state of the world.
 */
struct State {
  Grid<double> world;  // The world.
  WorldType worldType; // The type of world.
  UIState uiState;       // Which state we're in.
}; 

/*** Internal Constants ***/

/* Maximum number of rows or columns we allow in a world.	 This is mostly a
 * safety feature to prevent an OOM on a malformed input file.
 */
const int kMaxRows = 400;
const int kMaxCols = 400;

/* Various UI strings. */
const string kNewWorldLabel("New World");
const string kRerunLabel("Rerun");
const string kClearLabel("Clear");
const string kQuitLabel("Quit");
const string kRandomTerrainLabel("Random Terrain		");
const string kRandomMazeLabel("Random Maze		 ");
const string kLoadWorldLabel("Load World");
const string kSmallWorldLabel("Small World		 ");
const string kMediumWorldLabel("Medium World		");
const string kLargeWorldLabel("Large World		 ");
const string kHugeWorldLabel("Huge World       ");
const string kDijkstraLabel("Dijkstra's Algorithm			");
const string kAStarLabel("A* Search	 ");
const string kSelectedLocationColor("RED");
const string kPathColor("RED");
const string kBackgroundColor("Black");

/* The size, in pixels, of the displayed world. */
const int kDisplayWidth = 600;
const int kDisplayHeight = 600;

/* Constants controlling the number of rows/cols in the different sizes of
 * worlds.	The indices into these arrays are controlled by the enumerated
 * type WorldSize.
 */
const int kTerrainNumRows[] = { 33, 65, 129, 257 };
const int kTerrainNumCols[] = { 33, 65, 129, 257 };
const int kMazeNumRows[] = { 10, 30, 80, 160 };
const int kMazeNumCols[] = { 10, 30, 80, 160 };

/* Number of padding pixels between the border of the window and the the
 * start of the content.
 */
const int kMargin = 5;

/* Constants controlling the amount we should adjust the size of the window
 * to reflect the extra space used up by the border and controls.
 */
const int kWidthAdjustment = 0;
const int kHeightAdjustment = 75;

/* Constants reflecting the colors that are used to highlight cells different
 * colors.	This is an array of arrays, where each array contains RGB triplets
 * for the collors.	 The ordering here should be consistent with the ordering
 * of the Color type.
 */
const int kColorMultipliers[3][3] = {
		{ 255, 255, 255 },
		{ 255, 255, 0 },
		{ 0, 255, 0 }
};

/*** Internal function prototypes ***/

static void removeOverlays();
static void removeMarkedSquares();
static void locToCoord(Loc loc, double& x, double& y);
static string valueToColor(double value, Color locColor);
static void colorLocation(Loc loc, double value, Color locColor);
static void removeAndDelete(GObject* object);
static Loc coordToLoc(double x, double y);
static GRect* makeSelectionRectangle(Loc loc, string color);
static void findMidpoint(Loc loc, double& xc, double& yc);
static WorldSize getWorldSize(string sizeLabel);
static double runShortestPath(Grid<double>& world, 
                              WorldType worldType,
                              Loc start, Loc end);
static
Vector<Loc> invoke(Vector<Loc> pathFn(Loc start,
                                      Loc end,
                                      Grid<double>& world,
                                      double costFn(Loc, Loc, Grid<double>&)),
                   Loc start,
                   Loc end,
                   Grid<double>& world,
                   double costFn(Loc from, Loc to, Grid<double>& world),
                   double heuristicFn(Loc from, Loc to, Grid<double>& world));

static
Vector<Loc> invoke(Vector<Loc> pathFn(Loc start,
                                      Loc end,
                                      Grid<double>& world,
                                      double costFn(Loc, Loc, Grid<double>&),
                                      double heuristicFn(Loc, Loc, Grid<double>&)),
                   Loc start,
                   Loc end,
                   Grid<double>& world,
                   double costFn(Loc from, Loc to, Grid<double>& world),
                   double heuristicFn(Loc from, Loc to, Grid<double>& world));

/* Internal global variables */

static GWindow* gWindow = NULL;
static GChooser* gTypeList = NULL;
static GChooser* gSizeList = NULL;
static GChooser* gAlgorithmList = NULL;

/* The width and height of each cell. */
static double gPixelsPerWidth;
static double gPixelsPerHeight;

/* Which cells were seletected so far, and their corresponding locations. */
static GRect* gFirstSelected = NULL;
static GRect* gSecondSelected = NULL;
static Loc gStartLocation;
static Loc gEndLocation;

/* All highlighting lines. */
static Vector<GLine*> gHighlightedPath;

/* Which cells have been colored by the user. */
static Grid<bool> gMarked;

/* When they're colored, the values we've marked them with. */
static Grid<double> gMarkedValues;

/*** Function implementations ***/

static void fillRect(int x, int y, int width, int height, string color) {
	gWindow->setColor(color);
	gWindow->fillRect(x, y, width, height);
}

/* Draws the specified grid in the world. */
static void drawWorld(Grid<double>& world) {
	if (gWindow == NULL) error("Cannot draw world before window is set up.");

  /* This should act as if we are repainting the top-level display, so we
   * will remove all overlays.
   */
	removeMarkedSquares();
  removeOverlays();

  /* Recompute the size of a cell in the display. */
	gPixelsPerWidth = (double) kDisplayWidth / world.numCols();
	gPixelsPerHeight = (double) kDisplayHeight / world.numRows();

	/* Draw the background. */
	fillRect(0, 0, kDisplayWidth + 2 * kMargin, kDisplayHeight + 2 * kMargin,
	         kBackgroundColor);

  /* With the redraw, no locations are marked anymore. */
	gMarked.resize(world.numRows(), world.numCols());
	gMarkedValues = world;

  /* Draw each cell. */
	for (int row = 0; row < world.numRows(); row++) {
		for (int col = 0; col < world.numCols(); col++) {
			Loc loc = { row, col };
			double x, y;
			locToCoord(loc, x, y);
			
			fillRect(x, y, gPixelsPerWidth, gPixelsPerHeight,
			         valueToColor(world[row][col], GRAY));
		}
	}

  /* Issue a repaint so that we see the changes. */
	gWindow->repaint();
}

/* Restores all squares to their original colors. */
static void uncolorSquares() {
  /* Restore all colored cells to their original condition. */
  for (int row = 0; row < gMarked.numRows(); row++) {
    for (int col = 0; col < gMarked.numCols(); col++) {
      if (gMarked[row][col]) {
        Loc loc = { row, col };
        colorLocation(loc, gMarkedValues[row][col], GRAY);

        /* Unmark this cell; it's no longer colored. */
        gMarked[row][col] = false;
      }
    }
  }
}

/* Removes the highlighted squares from the display. */
static void removeMarkedSquares() {
  removeAndDelete(gFirstSelected);
  removeAndDelete(gSecondSelected);
  gFirstSelected = gSecondSelected = NULL;
}

/* Removes the line overlays. */
static void removeOverlays() {  
  /* Clear the lines drawn from the path. */
  for (int i = 0; i < gHighlightedPath.size(); i++) {
    removeAndDelete(gHighlightedPath[i]);
  }
  gHighlightedPath.clear();
}

/* Clears all cells that are currently selected and deselects any currently-
 * selected squares.
 */
static void restoreWorldDisplay() {
  if (gWindow == NULL)
    error("Window has not yet been initialized.");
 
  uncolorSquares();
  removeMarkedSquares();
  removeOverlays();
  
  /* Repaint the window so the changes show. */
  gWindow->repaint();
}

/* Registers that a click occurred, storing the location appropriately and
 * updating the graphics.  If the click was on an invalid location, returns
 * false.  If the click was at a valid location, returns true.
 */
static bool registerClick(Grid<double>& world, double x, double y,
                          WorldType worldType) {
  if (gWindow == NULL)
    error("Window has not yet been initialized.");
  if (gFirstSelected != NULL && gSecondSelected != NULL)
    error("Two tiles have already been selected.");

  /* Determine where we clicked and make sure it's in-bounds. */
  Loc loc = coordToLoc(x, y);
  if (!world.inBounds(loc.row, loc.col) || 
      (worldType == MAZE_WORLD && world[loc.row][loc.col] == kMazeWall)) {
    return false;
  }

  /* Add the selection to the display. */  
  GRect* selection = makeSelectionRectangle(loc, kSelectedLocationColor);
  gWindow->add(selection);

  /* Store the selection appropriately based on whether this is the first or
   * second click.
   */
  if (gFirstSelected == NULL) {
    gFirstSelected = selection;
    gStartLocation = loc;
  } else {
    gSecondSelected = selection;
    gEndLocation = loc;
  }
  
  return true;
}

/* (Public function)
 *
 * Colors the specified cell in the world the indicated color.
 */
void colorCell(Grid<double>& world, Loc loc, Color locColor) {
  colorLocation(loc, world[loc.row][loc.col], locColor);
  gMarked[loc.row][loc.col] = true;
}

/* Draws a line connecting the two specified locations. */
static void highlightConnection(Loc from, Loc to) {
  double srcX, srcY, dstX, dstY;
  findMidpoint(from, srcX, srcY);
  findMidpoint(to, dstX, dstY);
  
  GLine* connection = new GLine(srcX, srcY, dstX, dstY);
  connection->setColor(kPathColor);
  connection->setLineWidth(3.0);
  gWindow->add(connection);
  gHighlightedPath.add(connection);
}

/* Internal function implementations */
static void initializeWindow() {
  /* Calculate the intended width and height of the window based on the content
   * area size, the margin size, and the adjustment amount.
   */
  int windowWidth = kDisplayWidth + 2 * kMargin + kWidthAdjustment;
  int windowHeight = kDisplayHeight + 2 * kMargin + kHeightAdjustment;

  gWindow = new GWindow(windowWidth, windowHeight);
  gWindow->setWindowTitle("Trailblazer");

  /* This silly pause is required due to an as-of-yet-unresolved issue with
   * the Java Back End.  If this issue is fixed, please feel free to remove
   * this pause.
   */
  pause(500);
  
  /* Add in the list of terrain types. */
  gTypeList = new GChooser();
  gTypeList->addItem(kRandomTerrainLabel);
  gTypeList->addItem(kRandomMazeLabel);
  gTypeList->setSelectedItem(kRandomTerrainLabel);
  gWindow->addToRegion(gTypeList, "SOUTH");
  
  /* Add in the list of sizes. */
  gSizeList = new GChooser();
  gSizeList->addItem(kSmallWorldLabel);
  gSizeList->addItem(kMediumWorldLabel);
  gSizeList->addItem(kLargeWorldLabel);
  gSizeList->addItem(kHugeWorldLabel);
  gSizeList->setSelectedItem(kMediumWorldLabel);
  gWindow->addToRegion(gSizeList, "SOUTH");
  
  /* Add the algorithms list. */
  gAlgorithmList = new GChooser();
  gAlgorithmList->addItem(kDijkstraLabel);
  gAlgorithmList->addItem(kAStarLabel);
  gWindow->addToRegion(gAlgorithmList, "NORTH");

  /* Add the buttons. */
  gWindow->addToRegion(new GButton(kNewWorldLabel), "SOUTH");
  gWindow->addToRegion(new GButton(kLoadWorldLabel), "SOUTH");
  gWindow->addToRegion(new GButton(kRerunLabel), "NORTH");
  gWindow->addToRegion(new GButton(kClearLabel), "NORTH");
  gWindow->addToRegion(new GButton(kQuitLabel), "NORTH");
	
  /* As before, this is required due to an unknown race condition. */
  pause(500);
}

/* Generates a new world based on the user's perferences. */
static bool regenerateWorld(Grid<double>& world, WorldType& worldType) {
  string typeLabel = gTypeList->getSelectedItem();
  WorldSize worldSize = getWorldSize(gSizeList->getSelectedItem());

  /* To make this gracefully handle exceptions, we'll make a dummy world and
   * world type that we'll set up out here.
   */
  Grid<double> newWorld;
  WorldType newType;
	
  /* Based on the type of world, generate a different world of an appropriate
   * size.
   */
  if (typeLabel == kRandomTerrainLabel) {
    int numRows = kTerrainNumRows[worldSize];
    int numCols = kTerrainNumCols[worldSize];
    newWorld = generateRandomTerrain(numRows, numCols);
    newType = TERRAIN_WORLD;
  } else if (typeLabel == kRandomMazeLabel) {
    int numRows = kMazeNumRows[worldSize];
    int numCols = kMazeNumCols[worldSize];

    /* The default student code throws an exception, which we must handle. */
    try {
      /* The maze's logical number of rows and columns is not the same as its
       * physical number of rows and columns in the grid.  In particular, an
       * m x n maze has size (2m - 1) x (2n - 1), so we rescale the size of the
       * maze based on the number of logical rows and columns we want.
       */
      newWorld = generateRandomMaze(numRows / 2 + 1, numCols / 2 + 1);
      newType = MAZE_WORLD;
    } catch (const ErrorException& e) {
      cout << e.getMessage() << endl;
      return false;
    }
  } else {
    error("Invalid world type provided.");
  }

  world = newWorld;
  worldType = newType;
  return true;
}

/* Given the contents of the world size selector, returns a WorldSize encoding
 * the desired world size.
 */
static WorldSize getWorldSize(string sizeLabel) {
  if (sizeLabel == kSmallWorldLabel) {
    return SMALL_WORLD;
  } else if (sizeLabel == kMediumWorldLabel) {
    return MEDIUM_WORLD;
  } else if (sizeLabel == kLargeWorldLabel) {
    return LARGE_WORLD;
  } else if (sizeLabel == kHugeWorldLabel) {
    return HUGE_WORLD;
  } else {
    error("Invalid world size provided.");
  }
}

/* Returns which type of algorithm is currently selected in the drop-down
 * menu.
 */
static AlgorithmType getAlgorithmType() {
  if (gWindow == NULL) error("Window has not yet been initialized.");
  string algorithmLabel = gAlgorithmList->getSelectedItem();

  if (algorithmLabel == kDijkstraLabel) {
    return DIJKSTRA;
  } else if (algorithmLabel == kAStarLabel) {
    return A_STAR;
  } else {
    error("Invalid algorithm provided.");
  }
}

/* Given an elevation and a color, determines what #rrggbb format color to use
 * to draw it.
 */
static string valueToColor(double value, Color locColor) {
  /* To make non-gray colors show up better, we artificially cap the lowest
   * possible intensity at 0.2, rather than 0.0.  This next step is a linear
   * remapping of the range [0, 1] to [0.2, 1].
   */
  if (locColor != GRAY) {
    value = 0.8 * value + 0.2;
  }

  stringstream hexValue;
  hexValue << "#" << hex << setfill('0');
  for (int i = 0; i < 3; i++) {
    int intensity = int(value * kColorMultipliers[locColor][i]);
    hexValue << setw(2) << intensity;
  }
  
  return hexValue.str();
}

/* Utility function to remove a GObject from the display window and deallocate
 * it.
 */
static void removeAndDelete(GObject* object) {
  if (object != NULL) {
    gWindow->remove(object);
    delete object;
  }
}

/* Given a physical screen location, maps it to a logical grid location. */
static Loc coordToLoc(double x, double y) {
  Loc loc;
  loc.row = (int) ((y - kMargin) / gPixelsPerHeight);
  loc.col = (int) ((x - kMargin) / gPixelsPerWidth);
  return loc;
}

/* Given a logical grid location, maps it to a physical screen location. */
static void locToCoord(Loc loc, double& x, double& y) {
  x = loc.col * gPixelsPerWidth + kMargin;
  y = loc.row * gPixelsPerHeight + kMargin;
}

/* Given a logical grid location, creates a rectangle outlining that location
 * and returns a pointer to it.
 */
static GRect* makeSelectionRectangle(Loc loc, string color) {
  double x, y;
  locToCoord(loc, x, y);
  GRect* selection = new GRect(x, y, gPixelsPerWidth, gPixelsPerHeight);
  selection->setLineWidth(2.0);
  selection->setColor(color);
  return selection;
}

/* Colors the given location, which has the specified intensity, the indicated
 * color.
 */
static void colorLocation(Loc loc, double value, Color locColor) {
  double x, y;
  locToCoord(loc, x, y);
  
  fillRect(x, y, gPixelsPerWidth, gPixelsPerHeight, 
           valueToColor(value, locColor));
}

/* Given a logical grid location, returns the physical screen coordinates of
 * its midpoint.
 */
static void findMidpoint(Loc loc, double& xc, double& yc) {
  locToCoord(loc, xc, yc);
  xc += gPixelsPerWidth / 2;
  yc += gPixelsPerHeight / 2;
}

/* Tries to read a world file from the specified stream.	On success, returns
 * true and updates the input parameters to mark the type of the world and
 * the world contents.	On failure, returns false, but may still modify the
 * input parameters.
 */
static bool readWorldFile(ifstream& input, Grid<double>& world,
                          WorldType& worldType) try {
	/* Enable exceptions on the stream so that we can handle errors using try-
	 * catch rather than continuously testing everything.
	 */
	input.exceptions(ios::failbit | ios::badbit);

	/* The file line of the file identifies the type, which should be either
	 * "terrain" or "maze."
	 */
	string type;
	input >> type;
	
	if (type == "terrain") {
		worldType = TERRAIN_WORLD;
	} else if (type == "maze") {
		worldType = MAZE_WORLD;
	} else return false;
	
	/* Read the size of the world. */
	int numRows, numCols;
	input >> numRows >> numCols;
	
	if (numRows <= 0 || numCols <= 0 ||
			numRows >= kMaxRows || numRows >= kMaxCols) {
		return false;
	}
	
	world.resize(numRows, numCols);
	
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols; col++) {
			double value;
			input >> value;

			/* Validate the input based on the type of world. */			
			if (worldType == MAZE_WORLD) {
				if (value != kMazeWall && value != kMazeFloor) {
					return false;
				}
			} else /* worldType == TERRAIN_WORLD */ {
				if (value < 0.0 || value > 1.0) {
					return false;
				}
			}
			world[row][col] = value;
		}
	}

		return true;
} catch (...) {
	/* Something went wrong, so report an error. */
	return false;
}

/* Prompts the user for a file and tries to load a world from it, returning true
 * on success and false otherwise.
 */
static bool tryLoadWorld(Grid<double>& world, WorldType& worldType) {
	/* Open a file for reading. */
	ifstream input;
	string filename = promptUserForFile(input, "Choose world file: ");
	
	/* Otherwise, we now have an open file.	 Attempt to read from it. */
	Grid<double> newWorld;
	WorldType newWorldType;

  /* Try reading in the world file.  If we can't, report an error. */
	if (!readWorldFile(input, newWorld, newWorldType)) {
		cout << filename << " is not a world file." << endl;
    return false;
  }

  world = newWorld;
  worldType = newWorldType;
  return true;
}

/* Given a State object representing the state of the world, initializes it to
 * hold a default set of values.
 */
static void initializeState(State& state) {
  if (!regenerateWorld(state.world, state.worldType)) {
    error("Cannot set up initial world properly!");
  }
  state.uiState = FRESH;
}

static void runSearch(State& state) {
	try {
		double pathCost = runShortestPath(state.world, 
	                                    state.worldType,
	                                    gStartLocation,
	                                    gEndLocation);
	  cout << "Path cost: " << pathCost << endl;
	} catch (const ErrorException& e) {
	  cout << e.getMessage() << endl;
	}
}

/* Reacts to a mouse event in the window. */
static void processMouseEvent(State& state, GMouseEvent e) {
  switch (state.uiState) {
    /* Already have something drawn?  Clear it and pretend we're fresh. */
    case DRAWN:
      restoreWorldDisplay();
      state.uiState = FRESH;
      /* deliberate: don't break. */

    /* In a fresh state?  Try to select what the user clicked on. */
    case FRESH:
      if (registerClick(state.world, e.getX(), e.getY(),
                        state.worldType)) {
        state.uiState = MARKED;
      }
      break;

      /* Already marked?  Try to select what the user clicked on, then
       * try to find a path if it worked.
       */
    case MARKED:
      if (registerClick(state.world, e.getX(), e.getY(),
                        state.worldType)) {
				runSearch(state);
        state.uiState = DRAWN;
      }
  }
}

/* Reacts to an action event in the window. */
static void processActionEvent(State& state, GActionEvent e) {
  string cmd = e.getActionCommand();
  /* Need to make a new world?  Deselect everything and return to the fresh
   * state.
   */
  if (cmd == kNewWorldLabel) {
    /* This might fail, in which case we do nothing. */
    if (regenerateWorld(state.world, state.worldType)) {
      drawWorld(state.world);
      state.uiState = FRESH;
    }
  }
  /* Want to load a new world?  Try to do so and update the UI accordingly. */
  else if (cmd == kLoadWorldLabel) {
    if (tryLoadWorld(state.world, state.worldType)) {
      drawWorld(state.world);
      state.uiState = FRESH;
    }
  }
  /* Rerunning the search is only possible if we already did a search. */
  else if (cmd == kRerunLabel) {
  	if (state.uiState == DRAWN) {
	  	uncolorSquares();
	  	removeOverlays();
  		runSearch(state);
  	} else {
  		cout << "Cannot rerun a search; no search has been done." << endl;
  	}
  }
  /* Clearing the display just sets us back to the fresh state. */
  else if (cmd == kClearLabel) {
    restoreWorldDisplay();
    state.uiState = FRESH;
  }
  /* Trying to quit exits the entire program. */
  else if (cmd == kQuitLabel) {
    exitGraphics();
  }
}

/* Given a path, draws that path on the screen. */
static void drawPath(Vector<Loc>& path) {
  for (int i = 1; i < path.size(); i++) {
    highlightConnection(path[i - 1], path[i]);
  }
}

/* Given a path, returns the cost of that path. */
static double costOf(Vector<Loc>& path,
                     Grid<double>& world,
                     double costFn(Loc, Loc, Grid<double>&)) {
  double result = 0.0;
  for (int i = 1; i < path.size(); i++) {
    result += costFn(path[i - 1], path[i], world);
  }
  return result;
}

/* Computes the shortest path between the start and end locations, displaying
 * it on the screen and returning its length.
 */
static double runShortestPath(Grid<double>& world, 
                              WorldType worldType,
                              Loc start, Loc end) {
  AlgorithmType algType = getAlgorithmType();
  Vector<Loc> path;

  /* Determine which cost/heuristic functions to use. */
  double (*costFn)(Loc, Loc, Grid<double>&);
  double (*hFn)(Loc, Loc, Grid<double>&);

  if (worldType == TERRAIN_WORLD) {
    costFn = terrainCost;
    hFn = terrainHeuristic;
  } else if (worldType == MAZE_WORLD) {
    costFn = mazeCost;
    hFn = mazeHeuristic;
  } else error("Unknown world type.");

  /* Invoke the student's shortestPath function to find out the cost of the path.
   * This uses the indirect calling function invoke, which is described later
   * on.  Note that if we're using A* search, we disable the heuristic.
   */
	path = invoke(shortestPath, start, end, world, costFn,
                algType == A_STAR ? hFn : zeroHeuristic);

	if (path.isEmpty()) {
		cout << "Warning: Returned path is empty." << endl;
	} else if (path[0] != start) {
		cout << "Warning: Start of path is not the start location." << endl;
	} else if (path[path.size() - 1] != end) {
		cout << "Warning: End of path is not the end location." << endl;
	}
                
  drawPath(path);
  return costOf(path, world, costFn);
}

/* This code requires explanation.
 *
 * Our goal is to make it so that if the student uses the default signature for
 * the shortestPath function, we will call it and pass in the zero heuristic.
 * Otherwise, we will call it and pass in the specified heuristic.
 *
 * To do this, we overload a function called invoke that takes in a function
 * to call (this should be the student's shortestPath function), then all the
 * arguments we would ever want to pass in.  One version of the overload uses
 * the normal, four-argument version of shortestPath and ignores its final
 * argument.  The other version will match the five-argument version of
 * shortestPath and does indeed use the final argument.
 *
 * This extra level of indirection makes it possible for us to not have to
 * change the driver code and just have everything "work."  We do need to
 * rebuild the driver code object file when the header changes, though.
 */
static 
Vector<Loc> invoke(Vector<Loc> pathFn(Loc start,
                                      Loc end,
                                      Grid<double>& world,
                                      double costFn(Loc, Loc, Grid<double>&)),
                   Loc start,
                   Loc end,
                   Grid<double>& world,
                   double costFn(Loc from, Loc to, Grid<double>& world),
                   double heuristicFn(Loc from, Loc to, Grid<double>& world)) {
  return pathFn(start, end, world, costFn);
}

static 
Vector<Loc> invoke(Vector<Loc> pathFn(Loc start,
                                      Loc end,
                                      Grid<double>& world,
                                      double costFn(Loc, Loc, Grid<double>&),
                                      double heuristicFn(Loc, Loc, Grid<double>&)),
                   Loc start,
                   Loc end,
                   Grid<double>& world,
                   double costFn(Loc from, Loc to, Grid<double>& world),
                   double heuristicFn(Loc from, Loc to, Grid<double>& world)) {
  return pathFn(start, end, world, costFn, heuristicFn);
}

/* Main program. */
int main() {
  State state;
  initializeWindow();
  initializeState(state);
  drawWorld(state.world);
	
  /* Process events as they happen. */
  while (true) {
    GEvent e = waitForEvent(ACTION_EVENT | MOUSE_EVENT);
    if (e.getEventType() == MOUSE_CLICKED) {
      processMouseEvent(state, GMouseEvent(e));
    } else if (e.getEventClass() == ACTION_EVENT) {
      processActionEvent(state, GActionEvent(e));
    }
  }
}
