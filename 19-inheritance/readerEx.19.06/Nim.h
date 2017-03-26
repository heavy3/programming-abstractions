//
// Nim.h
//
// This program defines the interface for a Nim class that relies upon
// a two-player game engine class called, TwoPlayerGame.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 06  (See also Chapter 9, Exercise 12)
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code relies significantly upon code from Figures 9-5 and 9-6.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/7/16 and 3/25/17
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//

#ifndef Nim_h
#define Nim_h
#include "twoplayergame.h"

//
// Type: NimMove
// -------------
// Moves in Nim are the number of coins to remove during a turn.
// This can me modified for games other than Nim.
//

struct NimMove {
    int nTaken;
};

// Class: Nim
// ----------
// Implements the coin-game, Nim

class Nim: public TwoPlayerGame<NimMove> {
    
public:
    
    Nim();
    Nim(int recursiveDepth);
    void printInstructions();
    
private:
    
    // Constants
    
    static const int N_COINS = 13;         // Initial number of coins
    static const int MAX_MOVE = 3;         // Number of coins a player may take
    
    // Instance variables
    
    int nCoins;                            // Number of coins left on the table
    
    void initGame();
    void displayGame() const;
    void displayMove(NimMove move) const;
    void makeMove(NimMove move);
    int evaluateStaticPosition();
    void retractMove(NimMove move);
    void generateMoveList(Vector<NimMove> & moveList) const;
    bool gameIsOver();
    NimMove getUserMove();
    void announceResult() const;
};

#endif // Nim_h
