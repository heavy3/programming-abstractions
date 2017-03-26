//
// twoplayergame.h
//
// This file defines the interface for a two-player game engine class called
// TwoPlayerGame.  Clients of this class provide their own 'move' type and
// add any game-specific methods in a subclass.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 06  (See also Chapter 9, Exercise 12)
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file extends code from Figures 9-5 and 9-6.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/25/17
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//

#ifndef twoplayergame_h
#define twoplayergame_h

#include <iostream>
#include <string>
#include "error.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
using namespace std;

//
// Type: Player
// ------------
// This enumerated type differentiates the human and computer players.
//

enum Player { HUMAN, COMPUTER };
enum Outcome { HUMAN_WINS, COMPUTER_WINS, TIE_GAME };

template <typename MoveType>
class TwoPlayerGame {
    
public:

    //
    // Method: play
    // Usage: game.play();
    // -------------------
    // Plays a game between a human and a computer.
    //
    
    void play();
    
    //
    // Method: printInstructions
    // Usage: game.printInstructions();
    // --------------------------------
    // This method explains the rules of the game to the user.
    //
    
    virtual void printInstructions() = 0;
    
    // Method: [get|set]RecursiveDepth
    // Usage: int depth = game.getRecursiveDepth();
    //        game.setRecursiveDepth(3);
    // ---------------------------------
    // Getter/setter for recursive depth limiting.
    //
    // Limits the recursive evaluation of the minimax algorithm at the heart
    // of the game engine.  This is a practical requirement given that some
    // games may quickly devolve into intractability otherwise.
    
    int getRecursiveDepth() const;
    void setRecursiveDepth(int recursiveDepth);
    
protected:
    
    // Protected instance data
    
    static const Player STARTING_PLAYER = HUMAN;
    static const int DFLT_MAX_DEPTH = 100;    // Max recursive move-evaluation
                                              // depth.
    static const int WINNING_POSITION = 1000; // Upper limit on position rating
    static const int NEUTRAL_POSITION = 0;    // For tie games.
    static const int LOSING_POSITION = -WINNING_POSITION; // Lower limit.
    
    Player startPlayer;             // Identifies player who starts the game.
    Player whoseTurn;               // Identifies which player moves next
    Outcome outcome;                // Results announced at end of game.
    int recursiveDepth;             // Limits recursive evaluation of game tree.
    
    // TwoPlayerGame is an abstract class.  Limiting the accessibility
    // of the constructor to subclasses protects against naive instantiation
    // by (hierarchically) unrelated clients.
    
    TwoPlayerGame();
    TwoPlayerGame(int recursiveDepth);
    
    // Specify destructor as virtual so subclass destructors may be invoked
    // ahead of the superclass destructor (especially when referenced by
    // superclass pointer).
    
    virtual ~TwoPlayerGame() {};
    
    //
    // Method: initGame
    // Usage: initGame();
    // ------------------
    // Sets a game to an initial state of play.
    //
    
    virtual void initGame() = 0;
    
    //
    // Method: displayGame
    // Usage: displayGame();
    // ---------------------
    // Displays the current state of the game on the output stream.
    //
    
    virtual void displayGame() const = 0;
    
    //
    // Method: displayMove
    // Usage: displayMove(move);
    // -------------------------
    // Updates the display with the results of a player's move.
    //
    
    virtual void displayMove(MoveType move) const = 0;
    
    //
    // Method: makeMove
    // Usage: makeMove(move);
    // ----------------------
    // Updates the game state by making the given move.
    //
    
    virtual void makeMove(MoveType move) = 0;
    
    //
    // Method: evaluateStaticPosition
    // Usage:  int rating = evaluateStaticPosition();
    // ----------------------------------------------
    // Evaluates a particular state in the game without making any further
    // recursive calls.
    //
    
    virtual int evaluateStaticPosition() = 0;
    
    //
    // Method: retractMove
    // Usage: retractMove(move);
    // -------------------------
    // Retracts a move.
    //
    
    virtual void retractMove(MoveType move) = 0;
    
    //
    // Method: generateMoveList
    // Usage: generateMoveList(moveList);
    // ----------------------------------
    // Fills the moveList vector with the legal moves available in the current
    // state.
    //
    
    virtual void generateMoveList(Vector<MoveType> & moveList) const = 0;
    
    //
    // Method: gameIsOver
    // Usage: if (gameIsOver()) . . .
    // ------------------------------
    // Returns true if the game is over.
    //
    
    virtual bool gameIsOver() = 0;
    
    //
    // Method: getUserMove
    // Usage: Move move = getUserMove();
    // ----------------------------------
    // Asks the user to enter a move and returns the number of coins taken.
    // If the move is not legal, the user is asked to reenter a valid move.
    //
    
    virtual MoveType getUserMove() = 0;
    
    //
    // Method: announceResult
    // Usage: announceResult();
    // ------------------------
    // This method announces the final result of the game.
    //
    
    virtual void announceResult() const = 0;
    
    //
    // Method: getCurrentPlayer
    // Usage: if (getCurrentPlayer() == HUMAN) . . .
    // ---------------------------------------------
    // Returns the player currently selected to take a turn.
    //
    
    Player getCurrentPlayer() const;
    
    //
    // Function: opponent
    // Usage: Player other = opponent(player);
    // ---------------------------------------
    // Returns the opponent of the player.  The opponent of the computer
    // is the human player and vice versa.
    //
    
    Player opponent(Player player) const;
    
private:
    
    //
    // Method: switchTurn
    // Usage: switchTurn();
    // --------------------
    // Selects the next player to take a turn.
    //
    
    void switchTurn();
    
    //
    // Method: getComputerMove
    // Usage: int nTaken = getComputerMove();
    // --------------------------------------
    // Figures out what move is best for the computer player and returns that.
    //
    
    MoveType getComputerMove();
    
    //
    // Method: findBestMove
    // Usage: Move move = findBestMove();
    //        Move move = findBestMove(depth, rating);
    // -----------------------------------------------
    // Finds the best move for the current player and returns that move as the
    // value of the function.  The second form is used for later recursive calls
    // and includes two parameters.  The depth parameter is used to limit the
    // depth of the search for games that are too difficult to analyze.  The
    // reference parameter rating is used to store the rating of the best move.
    //
    
    MoveType findBestMove();
    MoveType findBestMove(int depth, int & rating);
    
    //
    // Method: evaluatePosition
    // Usage: int rating = evaluatePosition(depth);
    // --------------------------------------------
    // Evaluates a position by finding the rating of the best move starting at
    // that point.  The depth parameter is used to limit the search depth.
    //
    
    int evaluatePosition(int depth);
};

#include "twoplayergame.cpp"

#endif // twoplayergame_h
