//
// twoplayergame.cpp
//
// This file implements a two-player, templatized, game-engine class suitable
// for subclassing.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 06
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file extends code from Figures 9-5 and 9-6.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/24/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include "twoplayergame.h"

// Constructors: TwoPlayerGame
// Usage: class MyGame: TwoPlayerGame<MyMove> { ... };
// ---------------------------------------------------
// Abstract class that implements a two-player game engine
// with recursive 'minimax' algorithm that seeks to minimize an opponent's
// maximum opportunity.  Allows for recursive depth to be limited for
// large solution spaces that would become intractable otherwise.

template <typename MoveType>
TwoPlayerGame<MoveType>::TwoPlayerGame() {
    recursiveDepth = DFLT_MAX_DEPTH;
}
template <typename MoveType>
TwoPlayerGame<MoveType>::TwoPlayerGame(int recursiveDepth) {
    this->recursiveDepth = recursiveDepth;
}

//
// Function: opponent
// Usage: Player other = opponent(player);
// ---------------------------------------
// Returns the opponent of the player.  The opponent of the computer
// is the human player and vice versa.
//

template <typename MoveType>
Player TwoPlayerGame<MoveType>::opponent(Player player) const {
    return (player == HUMAN) ? COMPUTER : HUMAN;
}

//
// Method: play
// Usage: game.play();
// -------------------
// Plays a game between a human and a computer.
//

template <typename MoveType>
void TwoPlayerGame<MoveType>::play() {
    initGame();
    while (!gameIsOver()) {
        displayGame();
        MoveType move;
        if (getCurrentPlayer() == HUMAN) {
            move = getUserMove();
        } else {
            move = getComputerMove();
        }
        makeMove(move);
        displayMove(move);
        switchTurn();
    }
    announceResult();
}

//
// Method: getRecursiveDepth
// Usage: int depth = game.getRecursiveDepth();
// ---------------------------------------------
// Returns the player currently selected to take a turn.
//

template <typename MoveType>
int TwoPlayerGame<MoveType>::getRecursiveDepth() const {
    return this->recursiveDepth;
}

//
// Method: setRecursiveDepth
// Usage: game.setRecursiveDepth(3);
// ---------------------------------
// Sets the recursive depth limit (beyond which static position evaluation
// is used to assess an optimal move).  This limits backtracking in more
// complicated games (exploring the solution space becomes intractable).
//

template <typename MoveType>
void TwoPlayerGame<MoveType>::setRecursiveDepth(int recursiveDepth) {
    this->recursiveDepth = recursiveDepth;
}

//
// Method: getCurrentPlayer
// Usage: if (game.getCurrentPlayer() == HUMAN) . . .
// --------------------------------------------------
// Returns the player currently selected to take a turn.
//

template <typename MoveType>
Player TwoPlayerGame<MoveType>::getCurrentPlayer() const {
    return whoseTurn;
}

//
// Method: switchTurn
// Usage: game.switchTurn();
// -------------------------
// Selects the next player to take a turn.
//

template <typename MoveType>
void TwoPlayerGame<MoveType>::switchTurn() {
    whoseTurn = opponent(whoseTurn);
}
    
//
// Method: getComputerMove
// Usage: int nTaken = game.getComputerMove();
// -------------------------------------------
// Figures out what move is best for the computer player and returns that.
//

template <typename MoveType>
MoveType TwoPlayerGame<MoveType>::getComputerMove() {
    return findBestMove();
}
    
//
// Method: findBestMove
// Usage: MoveType move = game.findBestMove();
//        MoveType move = game.findBestMove(depth, rating);
// --------------------------------------------------------
// Finds the best move for the current player and returns that move as the
// value of the function.  The second form is used for later recursive calls
// and includes two parameters.  The depth parameter is used to limit the
// depth of the search for games that are too difficult to analyze.  The
// reference parameter rating is used to store the rating of the best move.
//

template <typename MoveType>
MoveType TwoPlayerGame<MoveType>::findBestMove() {
    int rating;
    return findBestMove(0, rating);
}

template <typename MoveType>
MoveType TwoPlayerGame<MoveType>::findBestMove(int depth, int & rating) {
    Vector<MoveType> moveList;
    generateMoveList(moveList);
    if (moveList.isEmpty()) error("No moves available");
    
    MoveType bestMove;
    int minRating = WINNING_POSITION + 1;
    for (MoveType move : moveList) {
        makeMove(move);
        int moveRating = evaluatePosition(depth + 1);
        if (moveRating < minRating) {
            bestMove = move;
            minRating = moveRating;
        }
        retractMove(move);
    }
    rating = -minRating;
    return bestMove;
}
    
//
// Method: evaluatePosition
// Usage: int rating = game.evaluatePosition(depth);
// -------------------------------------------------
// Evaluates a position by finding the rating of the best move starting at
// that point.  The depth parameter is used to limit the search depth.
//

template <typename MoveType>
int TwoPlayerGame<MoveType>::evaluatePosition(int depth) {
    if (gameIsOver() || depth >= this->recursiveDepth) {
        return evaluateStaticPosition();
    }
    int rating;
    findBestMove(depth, rating);
    return rating;
}
