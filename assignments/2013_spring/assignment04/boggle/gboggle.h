/*
 * File: gboggle.h
 * ---------------
 * The gboggle.h file defines the interface for a set of
 * functions that
 *
 *   1. Draw the boggle board
 *   2. Manage the word lists
 *   3. Update the scoreboard
 */

#ifndef _gboggle_h
#define _gboggle_h

#include <string>
#include "gwindow.h"

//
// Type: Player
// ------------
// This type identifies valid players as human or computer.
//

enum Player { HUMAN, COMPUTER };

/*
 * Function: initGBoggle
 * Usage: initGBoggle(gw);
 * -----------------------
 * Initializes the GBoggle package and sets it to use the graphics window
 * provided by the client.
 */

void initGBoggle(GWindow & gw);

/*
 * Function: drawBoard
 * Usage: drawBoard(4, 4);
 * -----------------------
 * This function draws the empty layout of the board having the
 * specified dimensions.  It should be called once at the beginning
 * of each game.  It will draw the cubes, board, and scoreboard labels.
 * The scores and word lists are set to zero.  The boggle cubes are
 * drawn with blank faces, ready for letters to be set using the
 * labelCube function. If either dimension is <= 0 or > MAX_DIMENSION,
 * an error is raised.
 */

void drawBoard(int numRows, int numCols);

/*
 * Function: labelCube
 * Usage: labelCube(row, col, letter);
 * -----------------------------------
 * This function draws the specified letter on the face of the cube
 * at position (row, col).  The cubes are numbered from top to bottom
 * left to right starting  with zero. Therefore, the upper left corner is
 * is (0, 0); the lower right is (numRows-1, numCols-1).  Thus, the call
 *
 *      labelCube(0, 3, 'D');
 *
 * would put a D in the top right corner cube. An error is raised if
 * row or col is out of range for this boggle board.
 */

void labelCube(int row, int col, char letter);

/*
 * Function: highlightCube
 * Usage: highlightCube(row, col, flag);
 * -------------------------------------
 * This function highlights or unhighlights the specified cube
 * according to the setting of flag: if flag is true, the cube
 * is highlighted; if flag is false, the highlight is removed.
 * The highlight flag makes it possible for you to show which
 * cubes are using when forming a word on the board. An error is
 * raised if row or col is out of range for this boggle board.
 */

void highlightCube(int row, int col, bool flag);

/*
 * Function: recordWordForPlayer
 * Usage: recordWordForPlayer(word, player);
 * -----------------------------------------
 * This function records the specified word by adding it to
 * the screen display for the specified player and updating
 * the scoreboard accordingly.  Scoring is calculated as
 * follows:  a 4-letter word is worth 1 point, a 5-letter
 * is worth 2 points, and so on. An error is raised if player
 * is not a valid value for player (HUMAN or COMPUTER).
 */

void recordWordForPlayer(std::string word, Player player);

#endif
