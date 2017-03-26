//
// TicTacToe.h
//
// This program defines the interface for a tic-tac-toe class reimplemented
// to use our newly hatched two-player game engine class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 06  (See also Chapter 9, Exercise 15)
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/14/16 and 3/26/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#ifndef TicTacToe_h
#define TicTacToe_h

#include "twoplayergame.h"
#include "set.h"
#include "grid.h"
using namespace std;

//
// Type: TicTacToeMove
// -------------------
// Defines a structure for tracking state changes with each move.

struct TicTacToeMove {
    char mark;      // 'X' or 'O'
    int square;     // Describe position linearly from 1 to MAX_SQUARE
                    // for user interface.
};

// Class: TicTacToe
// ----------------
// Implements the classic tic-tac-toe game.

class TicTacToe: public TwoPlayerGame<TicTacToeMove> {
public:
    
    static const int LENGTH = 3;                  // Grid dimension; side length
    static const int MAX_SQUARE = LENGTH * LENGTH;// Sane limit on game play
    static const char XSYM = 'X';                 // Marker for X
    static const char OSYM = 'O';                 // Marker for O
    static const char BLANK = ' ';                // Unmarked square
    static const char STARTING_SYMBOL = XSYM;     // Starting player's symbol
    
    TicTacToe( Player firstPlayer, char symbol);
    ~TicTacToe();
    void printInstructions();
    
private:
    
    // Instance variables
    
    char startMark;        // Identifies marking symbol for first player.
    Set<int> emptySquares; // This set determines list of possible moves.
    Grid<char> *gp;        // Pointer to the grid field of play.
    
    // Prototypes
    
    // Abstract methods from the superclass we're overriding here.
    
    void initGame();
    void displayGame() const;
    void displayMove(TicTacToeMove move) const;
    void makeMove(TicTacToeMove move);
    int evaluateStaticPosition();
    void retractMove(TicTacToeMove move);
    void generateMoveList(Vector<TicTacToeMove> & moveList) const;
    bool gameIsOver();
    TicTacToeMove getUserMove();
    void announceResult() const;
    
    // More game-specific methods.
    
    int getSq(int row, int col) const;
    void getRC(int squareNum, int& row, int& col) const;
    int rateEmptySpace() const;
    bool isFull() const;
    bool hasWon(char mark) const;
    bool hasWon(Player player) const;
    bool hasRowWin(int row, char mark) const;
    bool hasColWin(int col, char mark) const;
    bool hasDiagWin(char mark) const;
    int threatRating(char mark) const;
    int threatRating(Player player) const;
    bool isThreateningWin(char mark) const;
    bool isThreateningWin(Player player) const;
    bool hasRowColThreat(char mark) const;
    bool hasRowThreat(int row, char mark) const;
    bool hasColThreat(int col, char mark) const;
    bool hasDiagThreat(char mark) const;
    char opponentFromChar(char s) const;
    char getMark(Player p) const;
    Player getPlayer(char mark) const;
};

// Free functions

//
// Operator: <<
// Usage: cout << game << endl;
// ----------------------------
// Displays the current state of a tic-tac-toe game to the output stream.
//
// NB: Attempted to use 'const Grid<char>& grid' for second parameter and
//     suffered all manner of grid constructor wrath b/c of how grid.h, a
//     dependency outside my control, is coded. Looks like this version of
//     grid.h is not entirely const-friendly.

ostream & operator<<(ostream & os, Grid<char> grid);

#endif // TicTacToe_h
