//
// TicTacToe.cpp
//
// This program uses the minimax algorithm to play a perfect game of 
// tic-tac-toe.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 15
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/14/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "error.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "set.h"
#include "grid.h"
using namespace std;

// Types

//
// Type: Player
// ------------
// This type identifies valid players as human or computer.
//

enum Player { HUMAN, COMPUTER };

//
// Type: GameState
// ---------------
// This type identifies the possible outcomes of a game.
//

enum Outcome {
    HUMAN_WINS,
    COMPUTER_WINS,
    CATS_GAME
};

//
// Type: Move
// ----------
// Moves are designated by a symbol and a position.
//

struct Move {
    char mark;      // 'X' or 'O'
    int square;     // Describe position linearly from 1 to MAX_SQUARE
                    // for user interface.
};

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.15\n";
const std::string DETAIL = "Tic-Tac-Toe";
const std::string BANNER = HEADER + DETAIL;
const string WELCOME = "Welcome to TicTacToe, the game of three in a row.";

const Player STARTING_PLAYER = COMPUTER;       // Player who starts the game
const char XSYM = 'X';                         // Marker for X
const char OSYM = 'O';                         // Marker for O
const char BLANK = ' ';                        // Unmarked square
const char STARTING_SYMBOL = XSYM;             // Starting player's symbol

const int LENGTH = 3;                          // Grid dimension; side length
const int MAX_SQUARE = LENGTH * LENGTH;        // Sane limit on game play
const int MAX_DEPTH  = MAX_SQUARE;             // Maximum recursion depth

const int WINNING_POSITION = 1000;             // Best rating for a position
const int NEUTRAL_POSITION = 0;                // For tie games, etc
const int LOSING_POSITION = -WINNING_POSITION; // Worst rating for a position
                                               // Changing requires refactor!

// Prototypes

ostream & operator<<(ostream & os, Grid<char> game);

//
// Class: TicTacToe
// ----------------
// The TicTactoe class implements a character-based tic-tac-toe game.
//

class TicTacToe {

public:
    
    TicTacToe(const Player firstPlayer, const char symbol) {
        startPlayer = firstPlayer;
        startMark = symbol;
        grid.resize(LENGTH, LENGTH);
        initGame();
    }
    
//
// Method: play
// Usage: game.play();
// -------------------
// Plays a game between a computer and a human.
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
// Method: explain
// Usage: game.explain();
// ----------------------
// This method explains the rules of the game to the user.
//
    
    void explain() {
        cout << WELCOME << endl;
        if (whoseTurn == COMPUTER) {
            cout << "I'll be " << getMark(whoseTurn) << ", and you'll be ";
        } else {
            cout << "You'll be " << getMark(whoseTurn) << ", and I'll be ";
        }
        cout << getMark(opponent(whoseTurn)) << "." << endl;
        cout << "The squares are numbered like this:" << endl << endl;
        Grid<char> demo(LENGTH, LENGTH);
        for (int r = 0; r < LENGTH; r++) {
            for (int c = 0; c < LENGTH; c++) {
                demo[r][c] = char('0' + getSq(r, c));
            }
        }
        cout << demo << endl;
    }
    
private:
    
//
// Method: initGame
// Usage: initGame();
// ------------------
// Sets a game to an initial state of play.
//
    
    void initGame() {

        whoseTurn = startPlayer;
        emptySquares.clear();
        for (int i = 1; i <= MAX_SQUARE; i++) {
            emptySquares.add(i);
        }
        
        for (int r = 0; r < LENGTH; r++) {
            for (int c = 0; c < LENGTH; c++) {
                grid.set(r, c, BLANK);
            }
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
        
        if (firstTime) {
            firstTime = false;
        } else {
            cout << "The game now looks like this:" << endl << endl;
            cout << grid << endl;
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
            cout << "I'll move to " << move.square << endl;
        }
    }
    
//
// Method: getUserMove
// Usage: Move move = getUserMove();
// ----------------------------------
// Asks the user to select a row and column in which to place their 'X' or 'O'.
//
    
    Move getUserMove() {
        Move move;
        cout << "Your move." << endl;
        while (true) {
            int square = getInteger("What square? ");
            if (square > MAX_SQUARE || square < 1) {
                cout << "Please enter a number between 1 and " << MAX_SQUARE;
                cout << endl;
            } else {
                if (emptySquares.contains(square)) {
                    emptySquares.remove(square);
                    move.square = square;
                    move.mark = getMark(whoseTurn);
                    break;
                } else {
                    cout << "That square is taken." << endl;
                }
            }
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
        switch (outcome) {
            case HUMAN_WINS:
                cout << "You win." << endl;
                break;
                
            case COMPUTER_WINS:
                cout << "I win." << endl;
                break;
                
            case CATS_GAME:
            default: /* Cat's Game */
                cout << "No winner.  It's a cat's game." << endl;
                break;
        }
    }
    
//
// Method: getSq
// Usage: int square = getSq(row, col);
// ------------------------------------
// Returns the 1-based number for a square within the 0-based row/column
// tic-tac-toe grid.
//
    
    int getSq(int row, int col) {
        int squareNum = (row * LENGTH) + (col + 1);
        if (squareNum > MAX_SQUARE || squareNum < 1) {
            error("Internal error: getSq: Grid boundary exceeded.");
        }
        return squareNum;
    }
    
//
// Method: getRC
// Usage: getRC(inSquareNum, outRow, outCol);
// ------------------------------------------
// Returns the 0-based row/column grid indices corresponding to a
// 1-based square number.
//
    
    void getRC(int squareNum, int & row, int & col) {
        if (squareNum > MAX_SQUARE || squareNum < 1) {
            error("Internal error: getRC: Grid boundary exeeded.");
        }
        row = (squareNum - 1) / LENGTH;
        col = (squareNum - 1) % LENGTH;
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
        int row, col;
        
        getRC(move.square, row, col);
        grid.set(row, col, move.mark);
        emptySquares.remove(move.square);
    }
    
//
// Method: retractMove
// Usage: retractMove(move);
// -------------------------
// Retracts a move.
//
    
    void retractMove(Move move) {
        int row, col;
        
        getRC(move.square, row, col);
        grid.set(row, col, BLANK);
        emptySquares.add(move.square);
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
    
//
// Method: findBestMove
// Usage: Move move = findBestMove(depth, rating);
// ------------------------------------------------------------
// Returns the best move for the current player to make given
// their position.
//
    
    Move findBestMove(int depth, int & rating) {
        Vector<Move> possibleMoves;
        generateMoveList(possibleMoves);
        if (possibleMoves.isEmpty()) error("No moves available");
        
        Move bestMoveSoFar;
        int minRatingSoFar = WINNING_POSITION + 1;
        
        // Iterate over a list of possible moves, looking for the best.
        
        for (Move tentativeMove : possibleMoves) {
            makeMove(tentativeMove);
            
            //
            // Use a recursive min/max algorithm to simulate
            // playing ahead to a terminal position.
            //
            // This discent down the game-tree yields a useful rating
            // of the resulting position from our from our opponent's view.
            //
            
            switchTurn(); // Switch to the opponent's perspective.
            int moveRating = evaluatePosition(depth + 1);
            if (moveRating < minRatingSoFar) {
                //
                // Capture the move with the lowest rating since that
                // minimizes subsequent damage by our opponent.
                //
                bestMoveSoFar = tentativeMove;
                minRatingSoFar = moveRating;
            }
            switchTurn();
            retractMove(tentativeMove);
        }
        rating = -minRatingSoFar; // Invert opponent's rating to get ours.
        return bestMoveSoFar;
    }
    
//
// Method: evaluatePostion
// Usage: int rating = evaluatePosition(depth);
// -----------------------------------------------
// Evaluates a position by finding the rating of the best move starting at
// that point.  The depth parameter is used to limit the recursive search depth.
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
// -------------------------------------------------------
// This method returns a numeric rating of a particular
// position as evaluated from the perspective of the current
// player, designated by the 'whoseTurn' instance variable.
//
// The evaluation is considered static because no further recursive
// calls are made.
//
// In practice, the caller is typically a computer player iterating 
// over a list of possible moves, tentatively making a move, and then
// seeking a rating of the resulting position.
//
// If the position is judged to be a winning position from the
// opponent's perspective, the caller may want to discard that move 
// and try something less beneficial to the opponent.
//
    
    int evaluateStaticPosition() {
        
        // Previous player just won, a loss for current player.
        
        Player otherPlayer = opponent(whoseTurn);
        if (hasWon(otherPlayer))
            return LOSING_POSITION;
        
        // The grid is full and previous move didn't result in a win. Tie.
        
        if (isFull())
            return NEUTRAL_POSITION;
        
        // Current player is threatening a win.
        
        if (isThreateningWin(whoseTurn))
            return WINNING_POSITION;
        
        // If previous player is threatening a win, rate that threat.
        
        int otherRating = threatRating(otherPlayer);
        int rating = -otherRating;
        if (otherRating > 0)
            return rating;
        
        // 
        // Neither player is threatening a win.  Evaluate position based
        // upon tactical value of available squares.
        //
        
        return rateEmptySpace();
    }
    
//
// Method: rateEmptySpace
// Usage: int rating = rateEmptySpace();
// -------------------------------------
// Numerically rate the empty squares.
//
// When evaluating available positions, this allows us to quantify 
// the value of certain squares with more tactical advantage than others.
//
// Intuitively, one would think the center square should be granted the
// highest value.  However, that yields a slightly weaker opening
// move (when recursion is limited).  Corner opening leaves the most
// opportunity for your opponent to make an error.
//
// See: http://www.pleacher.com/mp/puzzles/mgames/tictac.pdf
//      http://blog.ostermiller.org/tic-tac-toe-strategy
//
    
    int rateEmptySpace() {
        int rating = 0;
        if (LENGTH != 3) {
            error("rateEmptySpace(): Invalid LENGTH. Consider refactoring.");
        }
        for (int square : emptySquares) {
            switch (square) {
                case 2: case 4: case 6: case 8:
                    // Cross squares are involved in 2 solutions.
                    // They make for crummy opening moves.
                    rating -= 0.050 * double(WINNING_POSITION);
                    break;
                case 1: case 3: case 7: case 9:
                    // Corner squares are involved with 3 solutions.
                    // Really good for opening moves.
                    rating += 0.150 * double(WINNING_POSITION);
                    break;
                case 5:
                    // Middle square is involved with 4 solutions.
                    rating += 0.150 * double(WINNING_POSITION);
                    break;
                    
                default:
                    break;
            }
        }
        return rating;
    }
    
//
// Method: generateMoveList
// Usage: generateMoveList(moveList);
// ----------------------------------
// Fills the moveList vector with the legal moves available in the current
// state.
//
    
    void generateMoveList(Vector<Move> & moveList) {
        if (isFull()) return;

        for (int square : emptySquares) {
            Move move;
            
            move.square = square;
            move.mark = getMark(whoseTurn);
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
        Player p;
        if (hasWon(XSYM)) {
            p = getPlayer(XSYM);
            outcome = (p == COMPUTER) ? COMPUTER_WINS : HUMAN_WINS;
            return true;
        }
        if (hasWon(OSYM)) {
            p = getPlayer(OSYM);
            outcome = (p == COMPUTER) ? COMPUTER_WINS : HUMAN_WINS;
            return true;
        }
        if (isFull()) {
            outcome = CATS_GAME;
            return true;
        }
        return false;
    }
    
//
// Method: isFull
// Usage: if (isFull()) { . . . }
// ------------------------------
// Returns true if the tic-tac-toe grid is full and no more moves can be made.
//
    
    bool isFull() {
        return emptySquares.isEmpty();
    }
    
//
// Method: hasWon
// Usage: if (hasWon('X')) { . . . }
//        if (hasWon(COMPUTER)) { . . . }
// --------------------------------------
// Returns true if the passed in symbol or player has three in a row along
// the horizontal, vertical, or diagonal.
//
    
    bool hasWon(char mark) {
        for (int i = 0; i < LENGTH; i++) {
            if (hasRowWin(i, mark)) return true;
            if (hasColWin(i, mark)) return true;
        }
        return hasDiagWin(mark);
    }
    
    bool hasWon(Player player) {
        return hasWon(getMark(player));
    }
    
//
// Method: hasRowWin
// Usage: if hasRowWin(row, 'X');
// ------------------------------
// Returns true if a given row is filled with the same mark.
//
    
    bool hasRowWin(int row, char mark) {
        int markCount = 0;
        for (int c = 0; c < LENGTH; c++) {
            if (grid[row][c] == mark) {
                markCount++;
            }
            if (markCount <= c) return false;
        }
        return true;
    }
    
//
// Method: hasColWin
// Usage: if hasColWin(col, 'X');
// ------------------------------
// Returns true if a given col is filled with the same mark.
//

    bool hasColWin(int col, char mark) {
        int markCount = 0;
        for (int r = 0; r < LENGTH; r++) {
            if (grid[r][col] == mark) markCount++;
            if (markCount <= r) return false;
        }
        return true;
    }
    
//
// Method: hasDiaglWin
// Usage: if hasDiagWin('X');
// ------------------------------
// Returns true if a diagonal is filled with the same mark.
//
    
    bool hasDiagWin(char mark) {
        int markCount = 0;
        for (int r = 0; r < LENGTH; r++) {
            int c = r;
            if (grid[r][c] == mark) markCount++;
            if (markCount <= r) break;
        }
        if (markCount == LENGTH) return true;
        
        markCount = 0;
        for (int r = 0; r < LENGTH; r++) {
            int c = LENGTH - (r + 1);
            if (grid[r][c] == mark) markCount++;
            if (markCount <= r) return false;
        }
        return true;
    }

//
// Method: threatRating
// Usage: int rating = threatRating('X');
//        int rating = threatRating(COMPUTER);
// -------------------------------------------
// Returns a number between 0 and 90% of WINNING_POSITION if the given
// symbol or player is threatening a win (i.e., has two marks along an 
// orientation
// with no opposing marks).
//
    
    int threatRating(char mark) {
        int threatCount = 0;        // Track multiple simultaneous attacks.
        double threatStrength = 0.; // Not all threats are rated the same.
        
        if (hasDiagThreat(mark)) {
            // diagonal threats are rated the highest
            threatStrength = 1.0;
            threatCount++;
        }

        for (int i = 0; i < LENGTH; i++) {
            if (hasRowThreat(i, mark)) {
                if (threatStrength == 0) {
                    // Threats including the center square rate higher.
                    threatStrength = (i == 1) ? 0.9 : 0.8;
                }
                threatCount++;
                break;
            }
            if (hasColThreat(i, mark)) {
                if (threatStrength == 0) {
                    // Threats including the center square rate higher.
                    threatStrength = (i == 1) ? 0.9 : 0.8;
                }
                threatCount++;
                break;
            }
        }
        
        int rating = 0;
        switch (threatCount) {
            case 0:
                break;
                
            case 1: // threatening win in one orientation
                rating = double(WINNING_POSITION) * 0.8 * threatStrength;
                break;
                
            case 2: // threatening a win in two or more orientations
            default:
                rating = double(WINNING_POSITION) * 0.9 * threatStrength;
                break;
        }
        return rating;
    }
    
    int threatRating(Player player) {
        return threatRating(getMark(player));
    }
    
//
// Method: isThreateningWin
// Usage: if (isThreateningWin('X')) { . . . }
//        if (isTheateningWin(COMPUTER)) { . . . }
// -----------------------------------------------
// Returns true if the given symbol is threatening a win along the vertical,
// horizontal, or diagonal.
//
    
    bool isThreateningWin(char mark) {
        return hasDiagThreat(mark) || hasRowColThreat(mark);
    }
    
    bool isThreateningWin(Player player) {
        return isThreateningWin(getMark(player));
    }
    
//
// Method: hasRowColThreat
// Usage: if (hasRowColThreat('X')) { . . . }
// ------------------------------------------
// Returns true if the given symbol is threatening a win along a row or column.
//
    
    bool hasRowColThreat(char mark) {
        for (int i = 0; i < LENGTH; i++) {
            if (hasRowThreat(i, mark)) return true;
            if (hasColThreat(i, mark)) return true;
        }
        return false;
    }
    
//
// Function: hasRowThreat
// Usage: if (hasRowThreat(row, 'X')) { . . . }
// --------------------------------------------
// Returns true if a given row is filled with the same mark
// except for one empty square.
//
    
    bool hasRowThreat(int row, char mark) {
        int markCount = 0;
        int oppMarkCount = 0;
        for (int c = 0; c < LENGTH; c++) {
            if (grid[row][c] == mark) {
                markCount++;
            } else if (grid[row][c] == opponent(mark)) {
                oppMarkCount++;
            }
        }
        return markCount == (LENGTH - 1) && oppMarkCount == 0;
    }
    
//
// Function: hasColThreat
// Usage: if (hasColThreat(col, 'X')) { . . . }
// --------------------------------------------
// Returns true if a given col is filled with the same mark
// except for one empty square.
//
    
    bool hasColThreat(int col, char mark) {
        int markCount = 0;
        int oppMarkCount = 0;
        for (int r = 0; r < LENGTH; r++) {
            if (grid[r][col] == mark) {
                markCount++;
            } else if (grid[r][col] == opponent(mark)) {
                oppMarkCount++;
            }
        }
        return markCount == (LENGTH - 1) && oppMarkCount == 0;
    }
    
//
// Function: hasDiagThreat
// Usage: if (hasDiagThreat('X')) { . . . }
// --------------------------------------------
// Returns true if either diagonal is filled with the same mark
// except for one empty square.
//
    
    bool hasDiagThreat(char mark) {
        int markCount = 0;
        int oppMarkCount = 0;
        for (int r = 0; r < LENGTH; r++) {
            int c = r;
            if (grid[r][c] == mark) {
                markCount++;
            } else if (grid[r][c] == opponent(mark)) {
                oppMarkCount++;
            }
        }
        if ((markCount == (LENGTH - 1)) && (oppMarkCount == 0)) return true;
        
        markCount = 0;
        oppMarkCount = 0;
        for (int r = 0; r < LENGTH; r++) {
            int c = LENGTH - (r + 1);
            if (grid[r][c] == mark) {
                markCount++;
            } else if (grid[r][c] == opponent(mark)) {
                oppMarkCount++;
            }
        }
        return markCount == (LENGTH - 1) && oppMarkCount == 0;
    }
    
//
// Method: opponent
// Usage: Player other = opponent(player);
//        char mark = opponent('X');
// ---------------------------------------
// Returns the opponent of the player. 
// Returns the mark for the opponent (e.g., 'X' or 'O').
//
// The opponent of the computer is the human player and vice versa.
//
    
    Player opponent(Player player) {
        return (player == HUMAN) ? COMPUTER : HUMAN;
    }
    
    char opponent(char s) {
        return (s == XSYM) ? OSYM : XSYM;
    }
    
//
// Method: getMark
// Usage: char mark = getMark(COMPUTER);
// -------------------------------------
// Returns the mark for a given player.
//
    
    char getMark(Player p) {
        char mark = (startPlayer == p) ? startMark : opponent(startMark);
        return mark;
    }
    
//
// Method: getPlayer
// Usage: Player p = getPlayer('X');
// ---------------------------------
// Returns the player who associated with a given mark.
//
    
    Player getPlayer(char mark) {
        Player p = (startMark == mark)? startPlayer : opponent(startPlayer);
        return p;
    }
    
// Instance variables
    
    Player whoseTurn;      // Identifies which player moves next.
    Player startPlayer;    // Identifies player who starts the game.
    char startMark;        // Identifies marking symbol for first player.
    Set<int> emptySquares; // This set determines list of possible moves.
    Grid<char> grid;       // The field of play.
    Outcome outcome;       // Results announced at end of game.
};

// Free functions

//
// Operator: <<
// Usage: cout << game << endl;
// ----------------------------
// Displays the current state of a tic-tac-toe game to the output stream.
//

ostream & operator<<(ostream & os, Grid<char> grid) {
    for (int r = 0; r < grid.nRows; r++) {
        for (int c = 0; c < grid.nCols; c++) {
            os << setw(2) << grid[r][c] << " ";
            if ((c + 1) % grid.nCols == 0) {
                if ((r + 1) < grid.nRows) {
                    os << endl << "---" ;
                    for (int i = 1; i < LENGTH; i++) {
                        os << "+---";
                    }
                    os << endl;
                }
            } else {
                os << "|" ;
            }
        }
    }
    os << endl;
    return os;
}

// Main program

int main() {
    cout << BANNER << endl << endl;
    TicTacToe game(STARTING_PLAYER, STARTING_SYMBOL);
    
    game.explain();
    game.play();
    
    return 0;
}