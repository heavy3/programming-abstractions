//
// Nim.cpp
//
// This program implements a Nim-game variant that starts with 17 coins
// and players alternate turns, removing up to 4 coins at a time to a
// pile the keep.  The winner is the player with an even number of coins
// in their pile after the common pile is empty.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 13
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This program relies significantly upon code from Figures 9-5 and 9-6.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/11/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <iostream>
#include <iomanip>
#include <string>
#include "error.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.13\n";
const std::string DETAIL = "Nim Variation 1: 17 coins, 1-4 draw, player piles";
const std::string BANNER = HEADER + DETAIL;
const int N_COINS = 17;         // Initial number of coins
const int MAX_MOVE = 4;         // Number of coins a player may take
                                // Naive changes to MAX_MOVE may render
                                // evaluateStaticPostion() suboptimal.
const int MAX_DEPTH = 10;       // Max recursive move-evaluation depth
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
// TODO: I dunno.  Something feels wrong about this old-school pointer lurking
//       in this structure.  My sense is I should have some player objects
//       running around that inherit or implement a Nim-specific move method.
//
//       My token stab at making playerCoins a reference instead of a
//       pointer yielded this compiler complaint:
//
//    >> Implicit default constructor for 'Move' must explicitly initialize
//       the reference member 'playerCoins'  :-/
//
//       Since the spirit of this problem is more about the min/max algorithm
//       I'm gonna move on.  I may loop back once I have more design fu and 
//       need a two-player game engine.
//

struct Move {
    int nTaken;        // Number of coins player is taking on this ply.
    int * playerCoins; // Point to the current player's coins for updating.
};

// Prototypes

Player opponent(Player player);
ostream & operator<<(ostream & os, Move move);
ostream & operator<<(ostream & os, Vector<Move> moves);

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
        cout << "Welcome to the game of Nim!" << endl << endl;
        cout << "In this game, we will start with a common pile of" << endl;
        cout << N_COINS << " coins on the table.  On each turn, you" << endl;
        cout << "and I will alternately take between 1 and" << endl;
        cout << MAX_MOVE << " coins from the table.  The player who" << endl;
        cout << "has an even number of coins when none are left" << endl;
        cout << "wins." << endl << endl;
    }
    
private:
    
//
// Method: initGame
// Usage: initGame();
// ------------------
// Sets a game to an initial state of play.
//
    
    void initGame() {
        if (isEven(N_COINS)) {
            error("Internal Error: N_COINS is an even number.  Expecting odd.");
        }
        nCoins = N_COINS;
        whoseTurn = STARTING_PLAYER;
        humanCoins = 0;
        computerCoins = 0;
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
            cout << "There are " << nCoins << " coins in the common pile." << endl;
            cout << "  (So far you have: " << humanCoins << " coins.)" << endl;
            cout << "  (So far I have:   " << computerCoins << " coins.)" << endl;
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
        if (move.playerCoins == NULL) {
            error("Encountered null 'move.playerCoins'.");
        } else {
            if (getCurrentPlayer() == COMPUTER) {
                cout << "I'll take " << move.nTaken << "." << endl << endl;
            }
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
        nCoins -= move.nTaken;
        if (move.playerCoins == NULL) {
            error("Internal Error: Encountered null 'move.playerCoins'.");
        } else {
            *(move.playerCoins) += move.nTaken;
        }
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
        int r; // rating from opponent's perspective
        int oppCoins = getOpponentCoins();
        
        switch (nCoins) {
            case 0:
                r = (isEven(oppCoins)) ? WINNING_POSITION : LOSING_POSITION;
                break;
                
            case 1:
                r = (isEven(oppCoins)) ? LOSING_POSITION : WINNING_POSITION;
                break;
                
                // case 2 through MAX_MOVE
                //
                // Opponent can always win in this state by either
                // taking all the coins, or taking all-but-1 coin
                // and forcing an opponent to lose on the next ply.

            case 2:
            case 3:
            case 4:
                r = WINNING_POSITION;
                break;

            default:

                // The following seems to be a pretty good heuristic I
                // uncovered while evaluating game trees that pass through
                // 5 and 6 coins remaining.
                //
                // It yields pretty good game play when recursion is turned
                // off, though a more generalized rule would factor in
                // N_COINS and MAX_MOVE.

                r = (isEven(oppCoins)) ? LOSING_POSITION : WINNING_POSITION;
        }
        return r;
    }
    
//
// Method: getOpponentCoins
// Usage: int count = getOponentCoins();
// -------------------------------------
// Returns the total number of coins taken so far by the opponent of the
// current (turn-taking) player.
//
    
    int getOpponentCoins() {
        return (getCurrentPlayer() == HUMAN) ? computerCoins: humanCoins;
    }
    
//
// Method: retractMove
// Usage: retractMove(move);
// -------------------------
// Retracts a move.
//
    
    void retractMove(Move move) {
        nCoins += move.nTaken;
        *(move.playerCoins) -= move.nTaken;
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
            Move move;
            move.nTaken = n;
            if (getCurrentPlayer() == COMPUTER) {
                move.playerCoins = &computerCoins;
            } else {
                move.playerCoins = &humanCoins;
            }
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
        move.playerCoins = &humanCoins;
        while (true) {
            move.nTaken = getInteger("How many would you like? ");
            int limit = (nCoins < MAX_MOVE) ? nCoins : MAX_MOVE;
            if (move.nTaken > 0 && move.nTaken <= limit) break;
            
            cout << "That's cheating!  Please choose a number";
            cout << " between 1 and " << limit << "." << endl;
            cout << "There are " << nCoins << " coins in the pile." << endl;
        }
        cout << endl;
        return move;
    }
    
//
// Method: announceResult
// Usage: announceResult();
// ------------------------
// This method announces the final result of the game.
//
    
    void announceResult() {
        if (nCoins == 0) {
            if (isEven(humanCoins)) {
                cout << "You have an even number of coins (" << humanCoins;
                cout << ").  You win.  Well played." << endl;
            } else {
                cout << "I have an even number of coins (" << computerCoins;
                cout << ").  I win." << endl;
            }
        } else {
            Move move;
            move.nTaken = 1;
            cout << "There is only one coin left." << endl;
            if (whoseTurn == HUMAN) {
                move.playerCoins = &(humanCoins);
                if (isOdd(humanCoins)) {
                    cout << "That will give you an even number of coins.  ";
                    cout << "You win.  Well played." << endl;
                } else {
                    cout << "That will give you an odd number of coins.  ";
                    cout << "You lose.  Better luck next time." << endl;
                }
            } else {
                move.playerCoins = &(computerCoins);
                if (isOdd(computerCoins)) {
                    cout << "That will give me an even number of coins.  ";
                    cout << "I win." << endl;
                } else {
                    cout << "That will give me an odd number of coins.  ";
                    cout << "I lose.  Well played." << endl;
                }
            }
            makeMove(move);
        }
        cout << "   (You ended up with: " << setw(2) << humanCoins;
        cout << " coins.)"<< endl;
        cout << "   (I ended up with:   " << setw(2) << computerCoins;
        cout << " coins.)"<< endl;
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
    
    int nCoins;         // Number of coins left on the table
    Player whoseTurn;   // Identifies which player moves next
    int humanCoins;     // Number of coins taken by the human player
    int computerCoins;  // Number of coins taken by the computer
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
    int yourCoins = *move.playerCoins;

    os << "m(t=" << move.nTaken << ",urC=" << yourCoins << ") ";
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

// Main program

int main() {
    cout << BANNER << endl << endl;
    BetterNim game;
    game.printInstructions();
    game.play();
    return 0;
}