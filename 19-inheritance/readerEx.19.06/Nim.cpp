//
// Nim.cpp
//
// This file implements the game-specific methods required to play a game
// of nim.
//
// Most of the code here amounts to elaborations of abstract methods in the
// game engine superclass.
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

#include "Nim.h"
using namespace std;

//
// Constructors: Nim
// Usage: Nim game;
//        Nim game(depth);  // specify resursive depth
// ---------------------------------------------------
// Instantiates the Nim game object.
//
// The second form allows you to override the default recusion
// depth limit when the algorithm is evaluating the game tree
// solution space relative to a given position.
//
// For a game as simple as nim, this is not a practical consideration.
// However if we subclassed the game engine for something like Chess,
// it might be a parameter we want to control.
//

Nim::Nim() {
    initGame();
}
Nim::Nim(int recursiveDepth): TwoPlayerGame<NimMove>(recursiveDepth) {
    initGame();
}

//
// Method: printInstructions
// Usage: game.printInstructions();
// --------------------------------
// This method explains the rules of the game to the user.
//

void Nim::printInstructions() {
    cout << "Welcome to the game of (better) Nim!" << endl << endl;
    cout << "In this game, we will start with a pile of" << endl;
    cout << N_COINS << " coins on the table.  On each turn, you" << endl;
    cout << "and I will alternately take between 1 and" << endl;
    cout << MAX_MOVE << " coins from the table.  The player who" << endl;
    cout << "takes the last coin loses." << endl << endl;
}

//
// Method: initGame
// Usage: game.initGame()
// ----------------------
// This is typically invoked by the game engine itself to restore the
// state of play to some initial condition.
//

void Nim::initGame() {
    nCoins = N_COINS;
    whoseTurn = STARTING_PLAYER;
}

//
// Method: displayGame
// Usage: displayGame();
// ---------------------
// Displays the current state of the game on the output stream.
//

void Nim::displayGame() const {
    cout << "There are " << nCoins << " coins in the pile." << endl;
}

//
// Method: displayMove
// Usage: displayMove(move);
// -------------------------
// Updates the display with the results of a player's move.
//

void Nim::displayMove(NimMove move) const {
    if (getCurrentPlayer() == COMPUTER) {
        cout << "I'll take " << move.nTaken << "." << endl;
    }
}

//
// Method: makeMove
// Usage: makeMove(move);
// ----------------------
// Updates the game state by making the given move.
//

void Nim::makeMove(NimMove move) {
    nCoins -= move.nTaken;
}

//
// Method: evaluateStaticPosition
// Usage:  int rating = evaluateStaticPosition();
// ----------------------------------------------
// Evaluates a particular state in the game without making any further
// recursive calls.
//
// The insight here is that you really don't need fancy recursion to
// discern if the current position is good or not.
//
// If it's your turn and the number of coins on the table is 1 plus some
// multiple of 4, you're pretty much hosed. :-/
//
// Your opponent just needs to keep removing enough coins:
//
//     4 - (# of coins you removed in your last turn)
//
// to maintain the the 1-plus-a-multiple-of-4 condition to win.
//

int Nim::evaluateStaticPosition() {
    int r; // rating

    r = (nCoins % (MAX_MOVE + 1) == 1) ? LOSING_POSITION : WINNING_POSITION;
    return r;
}
    
//
// Method: retractMove
// Usage: retractMove(move);
// -------------------------
// Retracts a move.
//

void Nim::retractMove(NimMove move) {
    nCoins += move.nTaken;
}

//
// Method: generateMoveList
// Usage: generateMoveList(moveList);
// ----------------------------------
// Fills the moveList vector with the legal moves available in the current
// state.

void Nim::generateMoveList(Vector<NimMove> & moveList) const {
    if (nCoins < 1) return;

    int limit = (nCoins < MAX_MOVE) ? nCoins : MAX_MOVE;
    for (int n = 1; n <= limit; n++) {
        NimMove move = { .nTaken = n };
        moveList += move;
    }
}

//
// Method: gameIsOver
// Usage: if (gameIsOver()) . . .
// ------------------------------
// Returns true if the game is over.

bool Nim::gameIsOver() {
    return (nCoins <= 1);
}

//
// Method: getUserMove
// Usage: NimMove move = getUserMove();
// ----------------------------------
// Asks the user to enter a move and returns the number of coins taken.
// If the move is not legal, the user is asked to reenter a valid move.

NimMove Nim::getUserMove() {
    NimMove move;
    while (true) {
        move.nTaken = getInteger("How many would you like? ");
        int limit = (nCoins < MAX_MOVE) ? nCoins : MAX_MOVE;
        if (move.nTaken > 0 && move.nTaken <= limit) return move;
        cout << "That's cheating!  Please choose a number";
        cout << " between 1 and " << limit << "." << endl;
        cout << "There are " << nCoins << " coins in the pile." << endl;
    }
}

//
// Method: announceResult
// Usage: announceResult();
// ------------------------
// This method announces the final result of the game.

void Nim::announceResult() const {
    if (nCoins == 0) {
        cout << "You took the last coin.  You lose." << endl;
    } else {
        cout << "There is only one coin left." << endl;
        if (whoseTurn == HUMAN) {
            cout << "I win." << endl;
        } else {
            cout << "I lose." << endl;
        }
    }
}
