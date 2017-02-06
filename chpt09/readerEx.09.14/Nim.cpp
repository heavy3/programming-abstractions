//
// Nim.cpp
//
// This program uses the minimax algorithm to play a game of 3-pile Nim.
//
// Here's a typical starting configuation:
//
//      Row  0: ooo
//      Row  1: oooo
//      Row  2: ooooo
//
// The program is general enough to change the number of rows or number of 
// coins in each row.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 14
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program relies significantly upon code from Figures 9-5 and 9-6.
//
// See also: http://www.cdf.toronto.edu/~ajr/270/probsess/03/strategy.html
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/12/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <iomanip>
#include <string>
#include "error.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "set.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.14\n";
const std::string DETAIL = "Multi-Pile Nim";
const std::string BANNER = HEADER + DETAIL;

// Initial configuration of coins.  Rows are delimited by comma.

const std::string NPILES = "ooo,oooo,ooooo";

const char NDELIM = ',';        // Character used to delimit rows in NPILES
const char COIN = 'o';          // Display coin as this character
const int MAX_DEPTH = 10;       // Max recursive position-evaluation depth
const int WINNING_POSITION = 1000; // Upper limit on position rating
const int LOSING_POSITION = -WINNING_POSITION; // Lower limit on rating

//
// Type: Player
// ------------
// This enumerated type differentiates the human and computer players.
//

enum Player { HUMAN, COMPUTER };

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
// In this variant, they also point to where those coins should be
// saved (i.e., atop the player's personal pile of coins).
//
//

struct Move {
    int nTaken;         // Number of coins player is taking on this ply.
    int row;            // Row (or pile) from which the coins are taken.
};

//
// Type: Piles
// -----------
// Piles of coins on the table are modeled as a vector of integers.
// Each entry represents the number of coins in a given row or pile.
//

typedef int PileT;
typedef Vector<PileT> Piles;

// Prototypes

Player opponent(Player player);
ostream & operator<<(ostream & os, Move move);
ostream & operator<<(ostream & os, Vector<Move> moves);
ostream & operator<<(ostream & os, Piles piles);

//
// Class: MultiPileNim
// -------------------
// The MultiPileNim class augments the simple version of Nim given in
// Fig 9-5 of the course reader by extending support for multiple piles of
// coins (typically arranged in rows).  Users can draw 1 or more coins
// from a single row during their turn.  In this variation, the loser
// is the player left with the last coin.
//

class MultiPileNim {

public:
    
//
// Constructor: MultiPileNim
// Usage: MultiPileNim game("ooo,oooo,ooooo");
// -------------------------------------------
// Initializes the game object to an initial state of play based upon
// the input string describing how coins are arranged upon the table.
//
    
    MultiPileNim(std::string coins, char delim = NDELIM) {

        initPiles = coins;
        pileDelim = delim;
    }
    
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
            Move move;
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
// Method: printInstructions
// Usage: game.printInstructions();
// --------------------------------
// This method explains the rules of the game to the user.
//
    
    void printInstructions() {
        cout << "Welcome to the game of Multi-Pile Nim!" << endl << endl;
        cout << "In this game, we will start with multiple rows of" << endl;
        cout << "coins arranged on the table.  On each turn, you" << endl;
        cout << "and I will alternately take between 1 and the" << endl;
        cout << "the maximum number of coins in a row, but you must" << endl;
        cout << "confine your move to a single row.  The player who" << endl;
        cout << "takes the last coin loses in this variation." << endl << endl;
    }
    
private:
    
//
// Method: initGame
// Usage: initGame();
// ------------------
// Sets a game to an initial state of play.
//
    
    void initGame() {

        whoseTurn = STARTING_PLAYER;
        
        //
        // Initialize game model as vector of integers.
        // Each entry represents the number of coins in a given pile.
        //
        // Delimiters in the input string mark the extent of each
        // pile or row of coins.
        //
        
        std::string coins = initPiles;
        while (!coins.empty()) {
            size_t pos = coins.find(pileDelim);
            if (pos == string::npos) {
                piles += coins.size();
                break;
            }
            piles += pos;
            coins.erase(0, pos+1);  // +1 to erase the delimiter too
        }
    }
    
//
// Method: displayGame
// Usage: displayGame();
// ---------------------
// Displays the current state of the game on the output stream.
//
    
    void displayGame() {
        static bool firstTime = true;
        
        if (getCurrentPlayer() == HUMAN || firstTime == true) {
            if (piles.isEmpty()) {
                cout << "No coins are on the table." << endl;
            } else {
                cout << "The table has these piles of coins: " << endl;
                cout << piles << endl;
            }
            firstTime = false;
        }
    }
    
//
// Method: displayMove
// Usage: displayMove(move);
// -------------------------
// Updates the display with the results of a player's move.
//
    
    void displayMove(Move move) {
        if (getCurrentPlayer() == COMPUTER) {
            cout << "From row " << move.row << ", I'll take " << move.nTaken;
            cout << endl << endl;
        }
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
        piles[move.row] -= move.nTaken;
    }
    
//
// Method: retractMove
// Usage: retractMove(move);
// -------------------------
// Retracts a move.
//
    
    void retractMove(Move move) {
        piles[move.row] += move.nTaken;
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
            
            // Rating is from perspective of opponent.
            // We desire least-best move in that case.
            
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
// Evaluates a particular state in the game from the perspective of
// the opponent (without making any further recursive calls).
//
// The caller is typically a computer player iterating over a list
// of possible moves and tentatively selecting one for evaluation.  
// If a resulting state is judged to be a winning position from the 
// opponent's perspective, the caller may want to discard that move 
// and try something less beneficial to the opponent.
//
// A player's best move is the one that results in the least favorable
// position for the opponent.
//
    
    int evaluateStaticPosition() {
        int r;          // IMPORTANT: Rating from opponent's perspective.

        switch (nCoins()) {
            case 0:
                r = WINNING_POSITION;   // Current player took last coin.
                break;                  // That's a win for the opponent.
                
            case 1:
                r = LOSING_POSITION;    // Opponent would take last coin.
                break;
                
            default: {

                //
                // Leverage some analysis from here:
                //
                // http://www.cdf.toronto.edu/~ajr/270/probsess/03/strategy.html
                //
                // however, account for the opposite interpretation of win ...
                //
                //      above asserts: win == you take the last coin
                //      but we desire: win == opponent takes the last coin
                //
                // Strategy:
                //
                // Use the notion of kernel as a broad guide to guide game-play
                // but adjust the endgame to reflect our requirements for a win.
                //
                
                int kernel = 0;     // Desire kernel == 0 until endgame.
                int nonEmpty = 0;   // Number of non-empty piles left.
                int singletons = 0; // How many of those piles have 1 coin?
                
                for (PileT p : piles) {
                    if (p > 0) {
                        nonEmpty++;
                        if (p == 1) singletons++;
                    }
                    kernel = p xor kernel;
                }
                if (nonEmpty == 2 && singletons >= 1) {
                    // endgame: Opponent wins by leaving you singleton.
                    //
                    //          Kernel logic could see this as a lose
                    //          especially with 2 piles of 1 coin each.
                    r = WINNING_POSITION;
                } else {
                    // stay within the game kernel for most of the play
                    r = (kernel == 0) ? LOSING_POSITION : WINNING_POSITION;
                }
            }
        }
        return r;
    }
    
//
// Method: generateMoveList
// Usage: generateMoveList(moveList);
// ----------------------------------
// Fills the moveList vector with the legal moves available in the current
// state.
//
    
    void generateMoveList(Vector<Move> & moveList) {
        if (piles.isEmpty()) return;
        
        for (int r = 0; r < piles.size(); r++) {
            int limit = piles[r];   // max # of coins we can remove in row
            for (int n = 1; n <= limit; n++) {
                Move move;
                move.nTaken = n;
                move.row = r;
                moveList += move;
            }
        }
    }
    
//
// Method: gameIsOver
// Usage: if (gameIsOver()) . . .
// ------------------------------
// Returns true if the game is over.
//
    
    bool gameIsOver() {
        return (nCoins() <= 1);
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
        Set<int> nonEmptyPiles;  // Indices to non-empty piles.
        
        if (nCoins() > 0) {
            for (int i = 0; i < piles.size(); i++) {
                if (piles[i] > 0) nonEmptyPiles += i;
            }

            while (true) {
                move.row = getInteger("Which row has your focus? ");
                if (nonEmptyPiles.contains(move.row)) break;
                cout << "Please select a (non-empty) row." << endl;
            }
        
            while (true) {
                move.nTaken = getInteger("How many coins would you like? ");
                int limit = piles[move.row];
                if (move.nTaken > 0 && move.nTaken <= limit) break;
                
                cout << "That's cheating!  Please choose a number";
                cout << " between 1 and " << limit << "." << endl;
                cout << "There are " << piles[move.row];
                cout << " coins in that pile." << endl;
            }
            cout << endl;
        }
        return move;
    }
    
//
// Method: announceResult
// Usage: announceResult();
// ------------------------
// This method announces the final result of the game.
//
    
    void announceResult() {
        displayGame();
        if (nCoins() == 0) {
            if (getCurrentPlayer() == HUMAN) {
                cout << "You win." << endl;
            } else {
                cout << "I win.  Better luck next time." << endl;
            }
        } else {
            cout << "There is only one coin left." << endl;
            if (getCurrentPlayer() == HUMAN) {
                cout << "That means you lose.  Better luck next time." << endl;
            } else {
                cout << "That means I lose.  Well played." << endl;
            }
        }
    }
    
//
// Method: nCoins
// Usage: int count = nCoins();
// -------------------------------------
// Returns the total number of coins still on the table.
//
    
    int nCoins() {
        int result = 0;
        for (PileT p : piles) {
            result += p;
        }
        return result;
    }
    
//
// Method: isEven
// Usage: if (isEven(number)) . . .
// --------------------------------
// Returns true if an integer is evenly divisible by two.
//
    
    bool isEven(int number) {
        return number % 2 == 0;
    }
    
//
// Method: isOdd
// Usage: if (isOdd(number)) . . .
// -------------------------------
// Returns true if an integer is not evenly divisible by two. 
//
    
    bool isOdd(int number) {
        return !(isEven(number));
    }
    
// Instance variables
    
    std::string initPiles; // Initial configuration of coin piles
    char pileDelim;        // Delimits piles in initPiles
    Piles piles;           // Number of coins in each pile on the table
    Player whoseTurn;      // Identifies which player moves next
};

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
// Operator: <<
// Usage: cout << move << endl;
// ----------------------------
// Displays a move to the output stream.
//

ostream & operator<<(ostream & os, Move move) {
    os << "(r:" << move.row << ", taken:" << move.nTaken << ") ";
    return os;
}

//
// Operator: <<
// Usage: cout << moves << endl;
// ----------------------------
// Displays a vector of moves to the output stream.
//

ostream & operator<<(ostream & os, Vector<Move> moves) {
    for (Move move : moves) {
        os << move;
    }
    os << endl;
    return os;
}

//
// Operator: <<
// Usage: cout << piles << endl;
// ----------------------------
// Displays piles of coins on the output stream.
//
// For example:
//
//      Row  0: ooo
//      Row  1: oooo
//      Row  2: ooooo
//

ostream & operator<<(ostream & os, Piles piles) {
    if (piles.isEmpty()) {
        return os << "<empty>" << endl;
    }
    
    os << endl;
    for (int i = 0; i < piles.size(); i++) {
        os << "Row " << setw(2) << i << ": " << string(piles[i], COIN) << endl;
    }
    return os;
}


// Main program

int main() {
    cout << BANNER << endl << endl;
    MultiPileNim game(NPILES);
    game.printInstructions();
    game.play();
    return 0;
}