//
// Nim.cpp
//
// This program re-implements the game of Nim from the text to use the
// generalized minimax algroithm also from the text.  This boils down to
// implementing a suitable Move type and game-specific methods as follows:
//
//    Rewrite: 
//       getComputerMove
//
//    Replace:
//       findGoodMove  with findBestMove
//       isBadPosition with evaluatePosition
//
//    Implement:
//       announceResult
//       evaluateStaticPosition
//       displayGame
//       displayMove
//       gameIsOver
//       generateMoveList
//       getUserMove
//       initGame
//       makeMove
//       printInstructions
//       retractMove
//
//    One virtue of this exercise is I end up with some nicely abstracted code
//    suitable as an engine for other two-player games:
//
//       play
//       findBestMove
//       evaluatePosition
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code relies significantly upon code from Figures 9-5 and 9-6.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/7/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <string>
#include "error.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.12\n";
const std::string DETAIL = "The game of Nim (with min/max algorithm)";
const std::string BANNER = HEADER + DETAIL;
const int N_COINS = 13;         // Initial number of coins
const int MAX_MOVE = 3;         // Number of coins a player may take
const int MAX_DEPTH = 3;        // Max recursive move-evaluation depth
const int WINNING_POSITION = 1000; // Upper limit on position rating
const int LOSING_POSITION = -WINNING_POSITION; // Lower limit on rating

//
// Type: Player
// ------------
// This enumerated type differentiates the human and computer players.
//

enum Player { HUMAN, COMPUTER };

//
// Function: opponent
// Usage: Player other = opponent(player);
// ---------------------------------------
// Returns the opponent of the player.  The opponent of the computer
// is the human player and vice versa.
//

Player opponent(Player player) {
    return (player == HUMAN) ? COMPUTER : HUMAN;
}

//
// Constant: STARTING_PLAYER
// -------------------------
// Indicates which player should start the game.
//

const Player STARTING_PLAYER = HUMAN;

//
// Type: Move
// ----------
// Moves in Nim are the number of coins to remove during a turn.
// This can me modified for game other than Nim.
//

struct Move {
    int nTaken;
};

//
// Class: BetterNim
// ----------------
// The BetterNim class improves upon the simple version of Nim given in Fig 9-5
// of the course reader.
//

class BetterNim {

public:
    
//
// Method: play
// Usage: game.play();
// -------------------
// Plays a game between a human and a computer.
//
    
    void play() {
        initGame();
        while (!gameIsOver()) {
            displayGame();
            if (getCurrentPlayer() == HUMAN) {
                makeMove(getUserMove());
            } else {
                Move move = getComputerMove();
                displayMove(move);
                makeMove(move);
            }
            switchTurn();
        }
        announceResult();
    }
    
//
// Method: printInstructions
// Usage: game.printInstructions();
// --------------------------------
// This method explains the rules of the game to the user.
//
    
    void printInstructions() {
        cout << "Welcome to the game of (better) Nim!" << endl << endl;
        cout << "In this game, we will start with a pile of" << endl;
        cout << N_COINS << " coins on the table.  On each turn, you" << endl;
        cout << "and I will alternately take between 1 and" << endl;
        cout << MAX_MOVE << " coins from the table.  The player who" << endl;
        cout << "takes the last coin loses." << endl << endl;
    }
    
private:
    
//
// Method: initGame
// Usage: initGame();
// ------------------
// Sets a game to an initial state of play.
//
    
    void initGame() {
        nCoins = N_COINS;
        whoseTurn = STARTING_PLAYER;
    }
    
//
// Method: displayGame
// Usage: displayGame();
// ---------------------
// Displays the current state of the game on the output stream.
//
    
    void displayGame() {
        cout << "There are " << nCoins << " coins in the pile." << endl;
    }
    
//
// Method: displayMove
// Usage: displayMove(move);
// -------------------------
// Updates the display with the results of a player's move.
//
    
    void displayMove(Move move) {
         cout << "I'll take " << move.nTaken << "." << endl;
    }
    
//
// Method: getCurrentPlayer
// Usage: if (getCurrentPlayer() == HUMAN) . . .
// ---------------------------------------------
// Returns the player currently selected to take a turn.
//
    
    Player getCurrentPlayer() {
        return whoseTurn;
    }
    
//
// Method: switchTurn
// Usage: switchTurn();
// --------------------
// Selects the next player to take a turn.
//
    
    void switchTurn() {
        whoseTurn = opponent(whoseTurn);
    }
    
//
// Method: makeMove
// Usage: makeMove(move);
// ----------------------
// Updates the game state by making the given move.
//
    
    void makeMove(Move move) {
        nCoins -= move.nTaken;
    }
    
//
// Method: getComputerMove
// Usage: int nTaken = getComputerMove();
// --------------------------------------
// Figures out what move is best for the computer player and returns that.
//
    
    Move getComputerMove() {
        return findBestMove();
    }
    
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
    
    Move findBestMove() {
        int rating;
        return findBestMove(0, rating);
    }
    
    Move findBestMove(int depth, int & rating) {
        Vector<Move> moveList;
        generateMoveList(moveList);
        if (moveList.isEmpty()) error("No moves available");
        
        Move bestMove;
        int minRating = WINNING_POSITION + 1;
        for (Move move : moveList) {
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
// Usage: int rating = evaluatePosition(depth);
// --------------------------------------------
// Evaluates a position by finding the rating of the best move starting at
// that point.  The depth parameter is used to limit the search depth.
//
    
    int evaluatePosition(int depth) {
        if (gameIsOver() || depth >= MAX_DEPTH) {
            return evaluateStaticPosition();
        }
        int rating;
        findBestMove(depth, rating);
        return rating;
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
    
    int evaluateStaticPosition() {
        int r; /* rating */
        
        r = (nCoins % (MAX_MOVE + 1) == 1) ? LOSING_POSITION : WINNING_POSITION;
        return r;
    }
    
//
// Method: retractMove
// Usage: retractMove(move);
// -------------------------
// Retracts a move.
//
    
    void retractMove(Move move) {
        nCoins += move.nTaken;
    }
    
//
// Method: generateMoveList
// Usage: generateMoveList(moveList);
// ----------------------------------
// Fills the moveList vector with the legal moves available in the current
// state.
//
    
    void generateMoveList(Vector<Move> & moveList) {
        if (nCoins < 1) return;
        
        int limit = (nCoins < MAX_MOVE) ? nCoins : MAX_MOVE;
        for (int n = 1; n <= limit; n++) {
            Move move = { .nTaken = n };
            moveList += move;
        }
    }
    
//
// Method: gameIsOver
// Usage: if (gameIsOver()) . . .
// ------------------------------
// Returns true if the game is over.
//
    
    bool gameIsOver() {
        return (nCoins <= 1);
    }
    
//
// Method: getUserMove
// Usage: Move move = getUserMove();
// ----------------------------------
// Asks the user to enter a move and returns the number of coins taken.
// If the move is not legal, the user is asked to reenter a valid move.
//
    
    Move getUserMove() {
        Move move;
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
//
    
    void announceResult() {
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
    
// Instance variables
    
    int nCoins;         // Number of coins left on the table
    Player whoseTurn;   // Identifies which player moves next
};

// Main program

int main() {
    cout << BANNER << endl << endl;
    BetterNim game;
    game.printInstructions();
    game.play();
    return 0;
}