//
// main.cpp
//
// This program solves the English Peg Solitaire puzzle.
//
// Reference
//
//    https://en.wikipedia.org/wiki/Peg_solitaire
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/2/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "grid.h"
#include "vector.h"
#include "error.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.08\n";
const std::string DETAIL = "Peg Solitaire";
const std::string BANNER = HEADER + DETAIL;
const int BOARD_DIMENSION = 7;
const int CENTER = 3;

// Types

enum HoleT {
    MISSING,            // Hole is missing from the non-square gameboard.
    EMPTY,              // Hole is empty.
    HAS_PROTECTED_PEG,  // Hole is filled with a peg that can't be jumped.
    HAS_THREATENED_PEG  // Hole is filled with a peg that /can/ be jumped.
};

struct PositionT {
    int row;
    int col;
};

typedef Grid<HoleT> BoardT;

struct GameT {
    BoardT board;
    int numPegs;
};

struct MoveT {
    PositionT start;    // Attacking peg's start position.
    PositionT victim;   // Position of peg getting jumped.
    PositionT stop;     // Attacking peg's landing position.
};

// Prototypes

//
// TODO: Figure out how to make 'BoardT & b' --> 'const BoardT & b'
//       without incurring this compile error:
//
//       "No matching constructor for initialization of 'Grid<HoleT>::GridRow'"
//
//       For now, I'm passing the game board by-reference for performance
//       reasons, but I'd like the added safety of designating that
//       parameter as 'const' where mutations of state are not expected
//       (nor desired) by the caller.
//

void init(GameT & g);
void initDebug(GameT & g);
ostream & operator<<(ostream & os, BoardT b);
ostream & operator<<(ostream & os, HoleT h);
ostream & operator<<(ostream & os, MoveT m);
ostream & operator<<(ostream & os, Vector<MoveT> moves);
ostream & operator<<(ostream & os, PositionT p);
bool inBounds(const PositionT & p);
bool isEmpty(HoleT hole);
bool hasPeg(const HoleT & hole);
bool hasPeg(BoardT & b, const PositionT & p);
Vector<PositionT> getOrthogPositions(const PositionT & p);
bool isThreatened(BoardT & b, const PositionT & p);
Vector<PositionT> getAttackingPositions(BoardT & b, const PositionT & p);
Vector<PositionT> getThreatenedPositions(BoardT & b);
void updateThreatenedPositions(BoardT & b);
bool solve(GameT & game, Vector<MoveT> & moves);
Vector<MoveT> getPossibleMoves(BoardT & b);
void makeMove(GameT & g, const MoveT & m);
void unmakeMove(GameT & g, const MoveT & m, const HoleT & prevAttackerState);

// Main program

int main(int argc, char * argv[]) {
    
    cout << BANNER << endl << endl;
    GameT game;
    Vector<MoveT> winningMoves;
    
    init(game); // Use initDebug(game) to test with a simple configuration
                // that takes seconds and not hours to solve. :-)
    
    cout << game.board << endl;

    if (solve(game, winningMoves)) {
        cout << game.board << endl;
        cout << "Here's my solution to Peg Solitaire:" << endl;
        cout << winningMoves << endl;
    } else {
        cout << "Unable to solve Peg Solitare :-/" << endl;
        cout << "Here's how far I got:" << endl << game.board << endl;
    }
    return 0;
}

//
// Function: solve
// Usage: if (solve(game, moves)) { . . . }
// ----------------------------------------
// Returns true if a given configuration of English Peg Solitaire is solvable.
// The sequence of required moves is returned to the caller in a
// pass-by-reference variable.
//
// Uses recursion with backtracking to return when the first valid solution
// sequence is discovered.
//
// The elementary, or base case, is an otherwise empty board with a single
// peg in the middle.
//
// The more interesting recursive case is to survey the set of possible moves,
// choose one, then recursively discern if that choice leads to a solution.
//
// Generally it will not and you'll often iterate across all the possible
// moves for a given state of the board.
//
// If no joy comes of the exhaustive iteration of possible moves, then
// you need to backtrack and undo an earlier move so an alternate move from the 
// set of possible moves can be tried and it's consequences explored.
//
// If you run out of moves to backtrack without a solve, then return false,
// indicating no solution found for the starting configuration provided.
//
// TODO: Improve performance.
//
//       The problem is inherently complex.
//       See: Uehara, R.; Iwata, S. (1990), "Generalized Hi-Q is NP-complete",
//            Trans. IEICE 73: 270–273.
//
//       However, there are some opportunities to improve performance.
//       At the more exotic level, leveraging multi-core to explore solution
//       space in parallel would be interesting.  
//
//       More simply, the updateThreatenedPositions() function (as a 
//       prereq for discerning all the valid moves for a given configuration) is
//       probably over-kill and could be optimized in favor of proximate
//       state management for a given move.
//
//       I had to leave this running overnight before it rewarded me with a
//       solution.  Still, that's ... pretty cool (Miley Cyrus voice).
//

bool solve(GameT & g, Vector<MoveT> & moves){

    // Base case
    
    if (g.numPegs == 1 && hasPeg(g.board[CENTER][CENTER])) {
        return true;
    }
    
    // Recursive case
    
    updateThreatenedPositions(g.board);
    for (MoveT nextMove : getPossibleMoves(g.board)) {
        HoleT attackerState = g.board[nextMove.start.row][nextMove.start.col];
        
        makeMove(g, nextMove);
        moves += nextMove;
        if (solve(g, moves)) {
            // Solution found.  Unwind recursion and return.
            return true;
        }
        // 
        // Backtrack by undoing that move since it didn't lead to a solve.
        // Loop around for a different move or, failing that, even more 
        // backtracking and reconsideration of earlier (sketchy) moves.
        //
        moves.remove(moves.size() - 1);
        unmakeMove(g, nextMove, attackerState);
        
    }
    return false; // Trigger upstream backtrack (or fail if no solution.)
}

//
// Function: makeMove
// Usage: makeMove(game, move);
// ----------------------------
// Updates the state of the game board effected by a given move.
//
// TODO: The state HAS_PROTECTED_PEG is not definitive.  The most
//       we can really say is HAS_PEG.  This turns out to be a don't
//       care because updateThreatenedPositions() sweeps through and
//       resets state, though at a significant performance penalty.
//       So if you wanted to improve performance, start here with better
//       proximate fix-ups of state near the move so updateThreatened
//       could be scrubbed entirely.
//

void makeMove(GameT & g, const MoveT & m) {
    g.board[m.start.row][m.start.col] = EMPTY;
    g.board[m.victim.row][m.victim.col] = EMPTY;
    g.board[m.stop.row][m.stop.col] = HAS_PROTECTED_PEG;
    g.numPegs--;
}

//
// Function: unmakeMove
// Usage: unmakeMove(game, move, previousState);
// ---------------------------------------------
// Reverses a move, typically the most recently made in the context of a
// an invocation of solve().
//

void unmakeMove(GameT & g, const MoveT & m, const HoleT & prevAttackerState) {
    g.board[m.start.row][m.start.col] = prevAttackerState;
    g.board[m.victim.row][m.victim.col] = HAS_THREATENED_PEG;
    g.board[m.stop.row][m.stop.col] = EMPTY;
    g.numPegs++;
}

//
// Function: init
// Usage: init(game);
// ------------------
// Initializes the game board with 32 pegs, leaving an open hole in the
// center.  This is the classic "English Peg Solitaire" setup.
//

void init(GameT & g) {
    BoardT b(BOARD_DIMENSION,BOARD_DIMENSION);
    int numPegs = 0;
    for (int r = 0; r < BOARD_DIMENSION; r++ ) {
        for (int c = 0; c < BOARD_DIMENSION; c++) {
            PositionT p;
            p.row = r; p.col = c;
            if (inBounds(p)) {
                b[r][c] = HAS_PROTECTED_PEG;
                numPegs++;
            } else {
                b[r][c] = MISSING;
            }
        }
    }
    b[CENTER][CENTER] = EMPTY;
    numPegs--;
    
    updateThreatenedPositions(b);
    
    g.board = b;
    g.numPegs = numPegs;
}

//
// Function: initDebug
// Usage: initDebug(game);
// -----------------------
// Initializes the board configuration with a simple, solvable state which
// also exercises backtracking.
//

void initDebug(GameT & g) {
    BoardT b(BOARD_DIMENSION,BOARD_DIMENSION);
    int numPegs = 0;
    for (int r = 0; r < BOARD_DIMENSION; r++ ) {
        for (int c = 0; c < BOARD_DIMENSION; c++) {
            PositionT p;
            p.row = r; p.col = c;
            if (inBounds(p)) {
                b[r][c] = EMPTY;
            } else {
                b[r][c] = MISSING;
            }
        }
    }
    b[CENTER-3][CENTER] = HAS_PROTECTED_PEG;
    b[CENTER-2][CENTER] = HAS_PROTECTED_PEG;
    b[CENTER][CENTER]   = HAS_PROTECTED_PEG;
    b[CENTER][CENTER-1] = HAS_PROTECTED_PEG;
    b[CENTER][CENTER+2] = HAS_PROTECTED_PEG;
    b[CENTER+2][CENTER] = HAS_PROTECTED_PEG;
    numPegs = 6;
    
    updateThreatenedPositions(b);
    
    g.board = b;
    g.numPegs = numPegs;
}

//
// Function: hasPeg
// Usage: if (hasPeg(hole)) { . . . }
//        if (hasPeg(board, position)) { . . . }
// ---------------------------------------------
// Returns true if a given hole or position on the gameboard has a peg.
//

bool hasPeg(const HoleT & hole) {
    return (hole == HAS_PROTECTED_PEG) || (hole == HAS_THREATENED_PEG);
}

bool hasPeg(BoardT & b, const PositionT & p) {
    return inBounds(p) && hasPeg(b[p.row][p.col]);
}

//
// Function: isEmpty
// Usage: if (isEmpty(hole)) { . . . }
//        if (isEmpty(board, position)) { . . . }
// ----------------------------------------------
// Returns true if a given hole or position on the gameboard is empty.
//

bool isEmpty(HoleT h) {
    return (h == EMPTY);
}

bool isEmpty(BoardT b, PositionT p) {
    return inBounds(p) && isEmpty(b[p.row][p.col]);
}

//
// Function: getOrthogPositions
// Usage: VectorT positions = getOrthogPositions(p);
// -------------------------------------------------
// Find positions that are orthogonal relative to a given position
// on the peg solitaire board.  This routine gracefully handles positions
// on the perimeter of the board.
//

Vector<PositionT> getOrthogPositions(const PositionT & p) {
    Vector<PositionT> result, raw;
    
    for (int r = p.row - 1; r <= (p.row + 1); r++) {
        for (int c = p.col - 1; c <= (p.col + 1); c++) {
            PositionT adjacent;
            adjacent.row = r;
            adjacent.col = c;
            if (inBounds(adjacent)) {
                if ((r + c) % 2 != (p.row + p.col) % 2) {
                    result += adjacent;
                }
            }
        }
    }
    return result;
}

//
// Function: isThreatened
// Usage: if (isThreatened(board, position)) { . . . }
// ---------------------------------------------------
// Returns true if a given position on the board is under threat of being
// jumped.
//

bool isThreatened(BoardT & b, const PositionT & p) {
    
    if (!inBounds(p)) return false;
    if (isEmpty(b[p.row][p.col])) return false;
    
    Vector<PositionT> attackingPositions = getAttackingPositions(b, p);
    return attackingPositions.size() > 0;
}

//
// Function: updateThreatenedPositions
// Usage: updateThreatenedPositions(gameBoard);
// --------------------------------------------
// Mark any position on the board that is currently under threat of
// direct attack.
//

void updateThreatenedPositions(BoardT & b) {
    for (PositionT p : getThreatenedPositions(b)) {
        b[p.row][p.col] = HAS_THREATENED_PEG;
    }
}

//
// Function: getThreatenedPositions
// Usage: Vector<PositionT> positions = getThreatenedPositions(board);
// -------------------------------------------------------------------
// Returns a collection of positions on the gameboard that are threatened
// (i.e., contains a peg that can be removed by a jump from an attacking peg).
//

Vector<PositionT> getThreatenedPositions(BoardT & b) {
    Vector<PositionT> results;
    
    for (int r = 0; r < BOARD_DIMENSION; r++ ) {
        for (int c = 0; c < BOARD_DIMENSION; c++) {
            PositionT p;
            p.row = r; p.col = c;
            if (isThreatened(b, p)) {
                results += p;
            }
        }
    }
    return results;
}

//
// Function: getAttackingPositions
// Usage: Vector<PositionT> positions = getAttackingPositions(board, position);
// ----------------------------------------------------------------------------
// Return a vector of positions on the board that threaten the peg at the 
// given position.
//

Vector<PositionT> getAttackingPositions(BoardT & b, const PositionT & p) {
    Vector<PositionT> results;
    
    Vector<PositionT> orthogP = getOrthogPositions(p);
    Vector<PositionT> orthogEmpty;
    for (PositionT p : orthogP) {
        if (isEmpty(b[p.row][p.col])) {
            orthogEmpty += p;
        }
    }
    for (PositionT e : orthogEmpty) {
        PositionT attackPos;
        if (p.row > e.row) {        // Empty on North; check South for attacker
            attackPos.row = p.row + 1;
            attackPos.col = p.col;
            if (hasPeg(b, attackPos)) {
                results += attackPos;
            }
        } else if (p.row < e.row) { // Empty on South; check North for attacker
            attackPos.row = p.row - 1;
            attackPos.col = p.col;
            if (hasPeg(b, attackPos)) {
                results += attackPos;
            }
        } else if (p.col > e.col) { // Empty on West; check East for attacker
            attackPos.row = p.row;
            attackPos.col = p.col + 1;
            if (hasPeg(b, attackPos)) {
                results += attackPos;
            }
        } else if (p.col < e.col) { // Empty on East; check West for attacker
            attackPos.row = p.row;
            attackPos.col = p.col - 1;
            if (hasPeg(b, attackPos)) {
                results += attackPos;
            }
        }
    }
    return results;
}

//
// Function: getPossibleMoves
// Usage: Vector<MoveT> moves = getPossibleMoves(gameBoard);
// ---------------------------------------------------------
// Returns a vector of possible moves that can be made given the current
// state of the board.
//
// TODO: Gather common code in this function and getAttackingPositions
//       into a single function.
//

Vector<MoveT> getPossibleMoves(BoardT & b) {
    Vector<MoveT> results;
    
    for (PositionT p : getThreatenedPositions(b)) {
        Vector<PositionT> orthogP = getOrthogPositions(p);
        Vector<PositionT> orthogEmpty;
        for (PositionT p : orthogP) {
            if (isEmpty(b[p.row][p.col])) {
                orthogEmpty += p;
            }
        }
        for (PositionT e : orthogEmpty) {
            MoveT move;
            PositionT attackPos;
            if (p.row > e.row) {        // Empty on North; check South for attacker
                attackPos.row = p.row + 1;
                attackPos.col = p.col;
                if (hasPeg(b, attackPos)) {
                    move.start = attackPos;
                    move.victim = p;
                    move.stop = e;
                    results += move;
                }
            } else if (p.row < e.row) { // Empty on South; check North for attacker
                attackPos.row = p.row - 1;
                attackPos.col = p.col;
                if (hasPeg(b, attackPos)) {
                    move.start = attackPos;
                    move.victim = p;
                    move.stop = e;
                    results += move;
                }
            } else if (p.col > e.col) { // Empty on West; check East for attacker
                attackPos.row = p.row;
                attackPos.col = p.col + 1;
                if (hasPeg(b, attackPos)) {
                    move.start = attackPos;
                    move.victim = p;
                    move.stop = e;
                    results += move;
                }
            } else if (p.col < e.col) { // Empty on East; check West for attacker
                attackPos.row = p.row;
                attackPos.col = p.col - 1;
                if (hasPeg(b, attackPos)) {
                    move.start = attackPos;
                    move.victim = p;
                    move.stop = e;
                    results += move;
                }
            }
        }
    }
    return results;
}

//
// Function: inBounds
// Usage: if (inBounds(position)) { . . . }
// ----------------------------------------
// Returns true if a given position corresponds to a valid position on the
// game board.  Row and column numbering is 0-based.
//

bool inBounds(const PositionT & p) {
    int r = p.row;
    int c = p.col;
    
    bool offBoard = (
            (((r < 2) || (r > 4)) && ((c < 2) || (c > 4))) || /* notched area */
            (r > (BOARD_DIMENSION - 1)) ||
            (c > (BOARD_DIMENSION - 1)) ||
            ((r < 0) || (c < 0))
             );

    return !offBoard;
}

//
// Operator: <<
// Usage: cout << gameBoard << endl;
// ---------------------------------
// Displays a simple character-graphics representation of the Peg Solitaire
// game board on an output stream.
//
// For example:
//
//         +---+
//         |***|    row = 0
//       +-+***+-+  row = 1
//       |*******|  row = 2
//       |*** ***|  row = 3
//       |*******|  row = 4
//       +-+***+-+  row = 5
//         |***|    row = 6
//         +---+
//
// col =  0123456
//
// Pegs positions under attack are represented with an '@' symbol:
//
//         +---+
//         |***|
//       +-+***+-+
//       |***@***|
//       |**@ @**|
//       |***@***|
//       +-+***+-+
//         |***|
//         +---+
//
//

ostream & operator<<(ostream & os, BoardT b) {
    os << "  +---+" << endl;
    for (int r = 0; r < BOARD_DIMENSION; r++ ) {
        for (int c = 0; c < BOARD_DIMENSION; c++) {
            if ((r == 0) || (r == 6)) {
                if ((c == 0) || (c == 6)) {
                    os << "  " ;
                    if (c == 6) {
                        os << endl;
                    }
                }
                if ((c == 1) || (c == 5)) {
                    os << "|" ;
                }
                os << b[r][c];
            }
            if ((r == 1) || (r == 5)) {
                if (c == 2) {
                    os << "+-+";
                    os << b[r][c];
                }
                if (c == 4) {
                    os << b[r][c];
                    os << "+-+" << endl;
                }
                if (c == 3) {
                    os << b[r][c];
                }
            }
            if ((r > 1) && (r < 5)) {
                if (c == 0) {
                    os << "|";
                }
                os << b[r][c];
                if (c == 6) {
                    os << "|" << endl;;
                }
            }
        }
    }
    os << "  +---+" << endl;
    return os;
}

//
// Operator: <<
// Usage: cout << hole ;
// ---------------------
// Displays the state of a hole on a peg solitaire playing board as a
// character on the output stream.
//

ostream & operator<<(ostream & os, HoleT h) {
    switch (h) {
        case EMPTY:
            os << " ";
            break;
            
        case HAS_PROTECTED_PEG:
            os << "*";
            break;
            
        case HAS_THREATENED_PEG:
            os << "@";
            break;
            
        case MISSING:
        default:
            break;
    }
    return os;
}

//
// Operator: <<
// Usage: cout << moves << endl;
// -----------------------------
// Displays a collection of moves on the output stream.
//

ostream & operator<<(ostream & os, Vector<MoveT> moves) {
    for (MoveT m : moves) {
        os << m;
    }
    return os;
}

//
// Operator: <<
// Usage: cout << move << endl;
// ----------------------------
// Displays a single move on the output stream.
//

ostream & operator<<(ostream & os, MoveT m) {
    os << "jump " << m.start << " to " << m.stop;
    os << " (remove peg: " << m.victim << ")." << endl;
    return os;
}

//
// Operator: <<
// Usage: cout << position << endl;
// --------------------------------
// Displays the coordinates of a board position on the output stream.
//

ostream & operator<<(ostream & os, PositionT p) {
    os << "[ " << p.row << ", " << p.col << " ]";
    return os;
}