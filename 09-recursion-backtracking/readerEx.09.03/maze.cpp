/*
 * File: maze.cpp
 * --------------
 * This file implements the Maze class.
 */

#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "direction.h"
#include "error.h"
#include "grid.h"
#include "gwindow.h"
#include "maze.h"
#include "point.h"
using namespace std;

/* Constants */

static const double SQUARE_SIZE = 25;
static const double MARK_SIZE = 7;

/* Public entries */

Maze::Maze(string filename) {
   gp = NULL;
   readMazeFile(filename);
}

Maze::Maze(string filename, GWindow & gw) {
   gp = &gw;
   readMazeFile(filename);
   x0 = (gw.getWidth() - cols * SQUARE_SIZE) / 2;
   y0 = (gw.getHeight() - rows * SQUARE_SIZE) / 2;
   drawMaze();
}

Point Maze::getStartPosition() {
   return startSquare;
}

bool Maze::isOutside(Point pt) {
   return !maze.inBounds(pt.getX(), pt.getY());
}

bool Maze::wallExists(Point pt, Direction dir) {
   if (pt.getX() == -1 && dir == EAST) {
      return wallExists(adjacentPoint(pt, EAST), WEST);
   }
   if (pt.getY() == -1 && dir == SOUTH) {
      return wallExists(adjacentPoint(pt, SOUTH), NORTH);
   }
   if (pt.getX() == cols && dir == WEST) {
      return wallExists(adjacentPoint(pt, WEST), EAST);
   }
   if (pt.getY() == rows && dir == NORTH) {
      return wallExists(adjacentPoint(pt, NORTH), SOUTH);
   }
   if (isOutside(pt)) error("Coordinates are out of range");
   return (maze[pt.getY()][pt.getX()].walls[dir]);
}

void Maze::markSquare(Point pt) {
   if (isOutside(pt)) error("Coordinates are out of range");
   maze[pt.getY()][pt.getX()].marked = true;
   if (gp != NULL) drawMark(pt);
}

void Maze::unmarkSquare(Point pt) {
   if (isOutside(pt)) error("Coordinates are out of range");
   maze[pt.getY()][pt.getX()].marked = false;
   if (gp != NULL) eraseMark(pt);
}

bool Maze::isMarked(Point pt) {
   if (isOutside(pt)) error("Coordinates are out of range");
   return maze[pt.getY()][pt.getX()].marked;
}

/* Private methods */

/*
 * Reads the data file and computes the dimensions
 * of the maze.  The file must then be read a second time to read
 * the actual data.  The values indicating the size of the maze
 * are stored in the private variables cols and rows.
 */

void Maze::readMazeFile(std::string filename) {
   ifstream infile;
   infile.open(filename.c_str());
   if (infile.fail()) error("Can't open " + filename);
   computeMazeSize(infile);
   infile.close();
   maze = Grid<Square>(rows, cols);
   for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
         for (int i = 0; i < 4; i++) {
            maze[row][col].walls[i] = false;
         }
         maze[row][col].marked = false;
      }
   }
   startSquare = Point(-1, -1);
   infile.clear();
   infile.open(filename.c_str());
   if (infile.fail()) error("Can't reopen maze file");
   processMazeFile(infile);
   infile.close();
   if (startSquare.getX() == -1) error("Maze contains no start square");
}

void Maze::computeMazeSize(ifstream & infile) {
   int nLines = 0;
   bool trailing = false;
   string line;
   while (getline(infile, line)) {
      int len = line.length();
      if (len == 0) {
         trailing = true;
      } else if (trailing) {
         error("Illegal blank lines in data file");
      } else if (nLines == 0) {
         if (len % 2 != 1) error("Illegal maze width");
         cols = (len - 1) / 2;
         nLines++;
      } else {
         nLines++;
      }
   }
   if (nLines % 2 != 1) error("Illegal maze height");
   rows = (nLines - 1) / 2;
}

/*
 * Method: processMazeFile
 * Usage: processMazeFile(infile);
 * -------------------------------
 * This method reads the actual maze data from the file.  The
 * methods ComputeMazeSize and InitMazeArray must be called
 * prior to calling ProcessMazeFile.  It is also the caller's
 * responsibility to make sure that infile has been set back
 * to the beginning of the file.
 */

void Maze::processMazeFile(ifstream & infile) {
   string line;
   for (int y = 0; y < rows; y++) {
      getline(infile, line);
      processDividerLine(line, y);
      getline(infile, line);
      processPassageLine(line, y);
   }
   getline(infile, line);
   processDividerLine(line, rows);
}

/*
 * Method: processDividerLine
 * Usage: processDividerLine(line, y);
 * -----------------------------------
 * This method reads the odd-numbered lines in the data file,
 * which specify the positions of the horizontal walls.  The
 * line have the form
 *
 *     +-+-+-+-+-+-+-+-+
 *
 * where the - symbols may be replaced by spaces to indicate a
 * corridor square.  The y value gives the index of the squares
 * immediately to the north of this line.
 */

void Maze::processDividerLine(string line, int y) {
   if (line.length() != 2 * cols + 1) {
      error("Divider line has incorrect width");
   }
   for (int x = 0; x < cols; x++) {
      if (line[2 * x] != '+') error("Missing corner symbol");
      switch (line[2 * x + 1]) {
       case ' ': break;
       case '-': setHorizontalWall(Point(x, y)); break;
       default:  error("Illegal character in maze file");
      }
   }
   if (line[2 * cols] != '+') error("Missing corner symbol");
}

/*
 * Method: processPassageLine
 * Usage: processPassageLine(line, y);
 * -----------------------------------
 * This method reads the even-numbered lines in the data file,
 * which specify the passageways and locations of the vertical
 * walls.  These lines have the form
 *
 *     | | | | | | | | |
 *
 * where the | symbols may be replaced by spaces to indicate a
 * corridor square.  One of the open passageway squares in the
 * file may also be marked with an 'S' to indicate the start
 * square.  The y argument gives the index of the squares on
 * this line.
 */

void Maze::processPassageLine(string line, int y) {
   int len = line.length();
   for (int x = 0; x < (len - 1) / 2; x++) {
      if (line[2 * x] == '|') {
         setVerticalWall(Point(x, y));
      }
      switch (line[2 * x + 1]) {
       case ' ': case '\0': break;
       case 'S': setStartSquare(Point(x, y)); break;
       default:  error("Illegal character in maze file");
      }
   }
   if (len % 2 == 1 && line[len - 1] == '|') {
      setVerticalWall(Point((len - 1) / 2, y));
   }
}

/*
 * Method: setHorizontalWall
 * Usage: setHorizontalWall(pt);
 * -----------------------------
 * This method sets a horizontal wall to the north of the square
 * at pt.  To maintain consistency in the data structure, it is
 * usually also necessary to create a wall to the south of the
 * square just north of this one.
 */

void Maze::setHorizontalWall(Point pt) {
   if (maze.inBounds(pt.getX(), pt.getY())) {
      maze[pt.getY()][pt.getX()].walls[NORTH] = true;
   }
   if (maze.inBounds(pt.getX(), pt.getY() - 1)) {
      maze[pt.getY() - 1][pt.getX()].walls[SOUTH] = true;
   }
}

/*
 * Method: setVerticalWall
 * Usage: setVerticalWall(pt);
 * ---------------------------
 * This method sets a vertical wall to the west of the square
 * at pt.  To maintain consistency in the data structure, it is
 * usually also necessary to create a wall to the east of the
 * square just west of this one.
 */

void Maze::setVerticalWall(Point pt) {
   if (maze.inBounds(pt.getX(), pt.getY())) {
      maze[pt.getY()][pt.getX()].walls[WEST] = true;
   }
   if (maze.inBounds(pt.getX() - 1, pt.getY())) {
      maze[pt.getY()][pt.getX() - 1].walls[EAST] = true;
   }
}

/*
 * Method: setStartSquare
 * Usage: setStartSquare(pt);
 * --------------------------
 * This method sets the start square to the indicated point and
 * draws a circle on the screen.
 */

void Maze::setStartSquare(Point pt) {
   if (startSquare.getX() != -1) {
      error("Multiple start squares specified");
   }
   startSquare = pt;
}

/*
 * Method: drawMaze
 * Usage: drawMaze();
 * ------------------
 * Draws the maze on the graphics window with its upper left corner
 * at the point (x0, y0).
 */

void Maze::drawMaze() {
   drawMazeGrid();
   drawWalls();
   drawMarks();
}

/*
 * Method: drawMazeGrid
 * Usage: drawMazeGrid();
 * ----------------------
 * This method clears the background to white and then draws a uniform
 * array of 2x2 squares to ensure that all of the corners are clean in
 * the complete display.
 */

void Maze::drawMazeGrid() {
   gp->setColor("WHITE");
   gp->fillRect(x0, y0, cols * SQUARE_SIZE + 1, rows * SQUARE_SIZE + 1);
   gp->setColor("BLACK");
   for (int x = 0; x <= cols; x++) {
      double xc = x0 + x * SQUARE_SIZE;
      for (int y = 0; y <= rows; y++) {
         double yc = y0 + y * SQUARE_SIZE;
         gp->fillRect(xc, yc, 2, 2);
      }
   }
}

void Maze::drawWalls() {
   for (int x = 0; x <= cols; x++) {
      double xc = x0 + x * SQUARE_SIZE;
      for (int y = 0; y <= rows; y++) {
         double yc = y0 + y * SQUARE_SIZE;
         Point pt(x, y);
         if (y < rows && wallExists(pt, WEST)) {
            gp->fillRect(xc, yc, 2, SQUARE_SIZE + 1);
         }
         if (x < cols && wallExists(pt, NORTH)) {
            gp->fillRect(xc, yc, SQUARE_SIZE + 1, 2);
         }
      }
   }
}

void Maze::drawMarks() {
   for (int x = 0; x < cols; x++) {
      for (int y = 0; y < rows; y++) {
         Point pt(x, y);
         if (isMarked(pt)) {
            drawMark(pt);
         }
      }
   }
}

void Maze::drawMark(Point pt) {
   double x = x0 + (pt.getX() + 0.5) * SQUARE_SIZE;
   double y = y0 + (pt.getY() + 0.5) * SQUARE_SIZE;
   double delta = MARK_SIZE / 2;
   gp->setColor("RED");
   gp->drawLine(x - delta, y - delta, x + delta, y + delta);
   gp->drawLine(x - delta, y + delta, x + delta, y - delta);
   gp->setColor("BLACK");
   pause(200);
}

void Maze::eraseMark(Point pt) {
   double x = x0 + (pt.getX() + 0.5) * SQUARE_SIZE;
   double y = y0 + (pt.getY() + 0.5) * SQUARE_SIZE;
   double delta = MARK_SIZE / 2;
   gp->setColor("WHITE");
   gp->fillRect(x - delta - 1, y - delta - 1, MARK_SIZE + 2, MARK_SIZE + 2);
   gp->setColor("BLACK");
   pause(200);
}

/*
 * Function: adjacentPoint
 * Usage: Point finish = adjacentPoint(start, dir);
 * ------------------------------------------------
 * Returns the Point that results from moving one square from start
 * in the direction specified by dir.  For example, if pt is the
 * point (1, 1), calling adjacentPoint(pt, EAST) returns the
 * point (2, 1).  To maintain consistency with the graphics package,
 * the y coordinates increase as you move downward on the screen.  Thus,
 * moving NORTH decreases the y component, and moving SOUTH increases it.
 */

Point Maze::adjacentPoint(Point start, Direction dir) {
   int x = start.getX();
   int y = start.getY();
   switch (dir) {
    case NORTH: return Point(x, y - 1);
    case EAST:  return Point(x + 1, y);
    case SOUTH: return Point(x, y + 1);
    case WEST:  return Point(x - 1, y);
   }
   return start;
}
