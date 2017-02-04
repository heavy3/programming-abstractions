//
// PegBoard.h
//
// This file exports a Peg Board interface which models an art board
// strung with yarn between offset pairs of pegs a fixed distance apart.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 12/8/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#ifndef PegBoard_h
#define PegBoard_h

#include "GWindow.h"
#include "error.h"

// 
// Predefined yarn colors.
//
// NB: Updates to this list require corresponding updates to colorToString()
//     and must be supported by the underlying GWindow.h graphics interface
//     provided through the Stanford C++ Library (libStanfordCPPLib).
//

enum ColorT {
    BLACK,
    BLUE,       // Default color.
    CYAN,
    DARK_GRAY,
    GRAY,
    GREEN,
    LIGHT_GRAY,
    MAGENTA,
    ORANGE,
    PINK,
    RED,
    WHITE,
    YELLOW,
    RANDOM      // Each strand of yarn will take on a random color.
};

//
// Class: PegBoard
// ---------------
// This class is used to represent a single instance of a 'yarn-art' peg
// board that ornaments a graphics window with not-so-yarn-like colored lines
// connecting offset sequences of pegs arrayed along the perimeter at 
// fixed intervals.
//
// The following attributes are programmatically available to the client
// of this class:
//
//    * Number of pegs across the horizontal dimension.
//    * Number of pegs down the vertical dimension.
//    * Color of lines connecting offset pegs.
//    * The size of the peg offset.
//
// The typical usage pattern would be:
//
//    #include "PegBoard.h"
//
//    int main(int argc, char * argv[]) {
//        int pegsAcross = 50;
//        int pegsDown = 30;
//        int pegDelta = 67;
//
//        PegBoard pb(pegsAcross, pegsDown);
//        pb.draw(pegDelta, RED);
//    }
//
// Limitations
//
//    * The graphics window does not currently respond to resize events.
//
//    * Your runtime environment must include a JRE since the application is
//      dependent upon a portable graphics interface that leverages java.
//

class PegBoard {

public:
    
//
// Constructor: PegBoard
// Usage: PegBoard pb;
//        PegBoard pb(numPegsAcross, numPegsDown);
// -----------------------------------------------
// Initializes a peg board object of specified dimension.
//
// The default constructor creates an uninitialized peg board suitable for
// the left hand side of an assignment of a fully initialized board.
//
    
    PegBoard();
    PegBoard(int pegsAcross, int pegsDown);
    
//
// Destructor: ~PegBoard
// ---------------------
// Reliquishes object and graphics associated resources.
//
    
    ~PegBoard();
    
//
// Method: clear
// Usage: pegboard.clear();
// ------------------------
// Clears the image displayed in the graphics window.  
//
// The count and position of pegs is unaffected.
//
    
    void clear();
    
//
// Method: draw
// Usage: pegboard.draw(pegDelta);
//        pegboard.draw(pegDelta, RED);
// ------------------------------------
// Iterates across all the pegs associated with the board starting in the
// upper lefthand corner and proceeding clockwise until arriving back
// at the first peg.
//
// For each peg visited, a corresponding distant peg (delta units away), 
// is connected with a line of specified color.  If no color is provided,
// BLUE is used.  If RANDOM is specified, each color is randomly picked
// from the enumeration of predefined colors exported by the interface.
//
    
    void draw(int pegDelta, ColorT color = BLUE);
    
private:
    
// Instance variables
    
    GWindow gw;     // Graphics window in which peg board art is rendered.
    int pegsA;      // Number of pegs across the horizontal dimension.
    int pegsD;      // Number of pegs down the vertical dimension.
    Vector<GPoint> pegs;    // Vector of peg positions.
    

// Private methods
    
    std::string colorToString(ColorT color);
    void positionPegs();
    
}; // end PegBoard

#endif // PegBoard_h