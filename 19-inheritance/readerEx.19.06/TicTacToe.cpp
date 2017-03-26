//
// TicTacToe.cpp
//
// This file implements the game-specific methods required to play a game
// of tic-tac-toe.
//
// Most of the code here amounts to elaborations of abstract methods in the
// game engine superclass.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 15  (See also Chapter 9, Exercise 15)
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/14/16 and 3/26/17
// Copyright © 2016,2017 Glenn Streiff. All rights reserved.
//

#include <iomanip>
#include "TicTacToe.h"

//
// Constructor: TicTacToe
// Usage: TicTacToe game(HUMAN, 'X');
// ----------------------------------
// Instantiates a game object to play tic-tac-toe with your computer :-)
// Then that computer will kill you, because skynet ... /jk, skynet, lol.
//

TicTacToe::TicTacToe(const Player firstPlayer, const char symbol) {
    startPlayer = firstPlayer;
    startMark = symbol;
    this->gp = new Grid<char>();
    gp->resize(LENGTH, LENGTH);;
    initGame();
}

//
// Destructor: ~TicTacToe
// Usage: (Usually implicit.)
// --------------------------
// Reclaims heap memory allocated for 3x3 game grid when game goes out of scope.
//

TicTacToe::~TicTacToe() {
    delete this->gp;
}
    
//
// Method: printInstructions
// Usage: game.printInstructions();
// --------------------------------
// This method explains the rules of the game to the user.
//
    
void TicTacToe::printInstructions() {
    cout << endl << "Welcome to Tic-Tac-Toe" << endl << endl;
    cout << "The objective is to get three of your marks in a row." << endl;
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

//
// Method: initGame
// Usage: initGame();
// ------------------
// Sets a game to an initial state of play.
//
    
void TicTacToe::initGame() {

    whoseTurn = startPlayer;
    emptySquares.clear();
    for (int i = 1; i <= MAX_SQUARE; i++) {
        emptySquares.add(i);
    }
    
    for (int r = 0; r < LENGTH; r++) {
        for (int c = 0; c < LENGTH; c++) {
            gp->set(r, c, BLANK);
        }
    }
}
    
//
// Method: displayGame
// Usage: displayGame();
// ---------------------
// Displays the current state of the game on the output stream.
//
    
void TicTacToe::displayGame() const {
    static bool firstTime = true;
    
    if (firstTime) {
        firstTime = false;
    } else {
        cout << "The game now looks like this:" << endl << endl;
        cout << *gp << endl;
    }
}
    
//
// Method: displayMove
// Usage: displayMove(move);
// -------------------------
// Updates the display with the results of a player's move.
//
    
void TicTacToe::displayMove(TicTacToeMove move) const {
    if (getCurrentPlayer() == COMPUTER) {
        cout << "I'll move to " << move.square << endl;
    }
}

//
// Method: makeMove
// Usage: makeMove(move);
// ----------------------
// Updates the game state by making the given move.
//

void TicTacToe::makeMove(TicTacToeMove move) {
    int row, col;
    
    getRC(move.square, row, col);
    gp->set(row, col, move.mark);
    emptySquares.remove(move.square);
}

//
// Method: getUserMove
// Usage: Move move = getUserMove();
// ----------------------------------
// Asks the user to select a row and column in which to place their 'X' or 'O'.
//
    
TicTacToeMove TicTacToe::getUserMove() {
    TicTacToeMove move;
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
    
void TicTacToe::announceResult() const {
    displayGame();
    switch (outcome) {
        case HUMAN_WINS:
            cout << "You win." << endl;
            break;
            
        case COMPUTER_WINS:
            cout << "I win." << endl;
            break;
            
        case TIE_GAME:
        default:
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
    
int TicTacToe::getSq(int row, int col) const {
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
    
void TicTacToe::getRC(int squareNum, int & row, int & col) const {
    if (squareNum > MAX_SQUARE || squareNum < 1) {
        error("Internal error: getRC: Grid boundary exeeded.");
    }
    row = (squareNum - 1) / LENGTH;
    col = (squareNum - 1) % LENGTH;
}
    
//
// Method: retractMove
// Usage: retractMove(move);
// -------------------------
// Retracts a move.
//
    
void TicTacToe::retractMove(TicTacToeMove move) {
    int row, col;
    
    getRC(move.square, row, col);
    gp->set(row, col, BLANK);
    emptySquares.add(move.square);
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
    
int TicTacToe::evaluateStaticPosition() {
    
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
    
int TicTacToe::rateEmptySpace() const {
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
    
void TicTacToe::generateMoveList(Vector<TicTacToeMove> & moveList) const {
    if (isFull()) return;

    for (int square : emptySquares) {
        TicTacToeMove move;
        
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
    
bool TicTacToe::gameIsOver() {
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
        outcome = TIE_GAME;
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
    
bool TicTacToe::isFull() const {
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
    
bool TicTacToe::hasWon(char mark) const {
    for (int i = 0; i < LENGTH; i++) {
        if (hasRowWin(i, mark)) return true;
        if (hasColWin(i, mark)) return true;
    }
    return hasDiagWin(mark);
}

bool TicTacToe::hasWon(Player player) const {
    return hasWon(getMark(player));
}
    
//
// Method: hasRowWin
// Usage: if hasRowWin(row, 'X');
// ------------------------------
// Returns true if a given row is filled with the same mark.
//
    
bool TicTacToe::hasRowWin(int row, char mark) const {
    int markCount = 0;
    for (int c = 0; c < LENGTH; c++) {
        if ((*gp)[row][c] == mark) {
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

bool TicTacToe::hasColWin(int col, char mark) const {
    int markCount = 0;
    for (int r = 0; r < LENGTH; r++) {
        if ((*gp)[r][col] == mark) markCount++;
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
    
bool TicTacToe::hasDiagWin(char mark) const {
    int markCount = 0;
    for (int r = 0; r < LENGTH; r++) {
        int c = r;
        if ((*gp)[r][c] == mark) markCount++;
        if (markCount <= r) break;
    }
    if (markCount == LENGTH) return true;
    
    markCount = 0;
    for (int r = 0; r < LENGTH; r++) {
        int c = LENGTH - (r + 1);
        if ((*gp)[r][c] == mark) markCount++;
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
    
int TicTacToe::threatRating(char mark) const {
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

int TicTacToe::threatRating(Player player) const {
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
    
bool TicTacToe::isThreateningWin(char mark) const {
    return hasDiagThreat(mark) || hasRowColThreat(mark);
}

bool TicTacToe::isThreateningWin(Player player) const {
    return isThreateningWin(getMark(player));
}
    
//
// Method: hasRowColThreat
// Usage: if (hasRowColThreat('X')) { . . . }
// ------------------------------------------
// Returns true if the given symbol is threatening a win along a row or column.
//
    
bool TicTacToe::hasRowColThreat(char mark) const {
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
    
bool TicTacToe::hasRowThreat(int row, char mark) const {
    int markCount = 0;
    int oppMarkCount = 0;
    for (int c = 0; c < LENGTH; c++) {
        if ((*gp)[row][c] == mark) {
            markCount++;
        } else if ((*gp)[row][c] == opponentFromChar(mark)) {
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
    
bool TicTacToe::hasColThreat(int col, char mark) const {
    int markCount = 0;
    int oppMarkCount = 0;
    for (int r = 0; r < LENGTH; r++) {
        if ((*gp)[r][col] == mark) {
            markCount++;
        } else if ((*gp)[r][col] == opponentFromChar(mark)) {
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
    
bool TicTacToe::hasDiagThreat(char mark) const {
    int markCount = 0;
    int oppMarkCount = 0;
    for (int r = 0; r < LENGTH; r++) {
        int c = r;
        if ((*gp)[r][c] == mark) {
            markCount++;
        } else if ((*gp)[r][c] == opponentFromChar(mark)) {
            oppMarkCount++;
        }
    }
    if ((markCount == (LENGTH - 1)) && (oppMarkCount == 0)) return true;
    
    markCount = 0;
    oppMarkCount = 0;
    for (int r = 0; r < LENGTH; r++) {
        int c = LENGTH - (r + 1);
        if ((*gp)[r][c] == mark) {
            markCount++;
        } else if ((*gp)[r][c] == opponentFromChar(mark)) {
            oppMarkCount++;
        }
    }
    return markCount == (LENGTH - 1) && oppMarkCount == 0;
}
    
//
// Method: opponent
// Usage: char mark = opponent('X');
// ---------------------------------------
// Returns the opponent of the player. 
// Returns the mark for the opponent (e.g., 'X' or 'O').
//
// The opponent of the computer is the human player and vice versa.
//
    
char TicTacToe::opponentFromChar(char s) const {
    return (s == XSYM) ? OSYM : XSYM;
}
    
//
// Method: getMark
// Usage: char mark = getMark(COMPUTER);
// -------------------------------------
// Returns the mark for a given player.
//
    
char TicTacToe::getMark(Player p) const {
    char mark = (startPlayer == p) ? startMark : opponentFromChar(startMark);
    return mark;
}
    
//
// Method: getPlayer
// Usage: Player p = getPlayer('X');
// ---------------------------------
// Returns the player who associated with a given mark.
//
    
Player TicTacToe::getPlayer(char mark) const {
    Player p = (startMark == mark)? startPlayer : opponent(startPlayer);
    return p;
}

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
//

ostream & operator<<(ostream & os, Grid<char> grid) {
    for (int r = 0; r < grid.nRows; r++) {
        for (int c = 0; c < grid.nCols; c++) {
            os << setw(2) << grid[r][c] << " ";
            if ((c + 1) % grid.nCols == 0) {
                if ((r + 1) < grid.nRows) {
                    os << endl << "---" ;
                    for (int i = 1; i < TicTacToe::LENGTH; i++) {
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
