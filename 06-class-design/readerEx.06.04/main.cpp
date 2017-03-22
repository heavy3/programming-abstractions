//
// main.cpp
//
// This program models a "yarn-art peg board" that allows yarn to be
// strung between a sequence of peg-pairs offset from each other by a fixed
// number of pegs.  Depending upon the interval chosen, you can end up with
// interesting curved effects, some that even take on a 3-dimensional
// appearance.
//
// The following parameters may be specified programmatically:
//
// * Number of pegs along the horizontal and vertical dimensions.
// * The peg delta (between pegs that should be connected by yarn).
// * Yarn color.
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

#include <iostream>
#include "PegBoard.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.4";
const std::string DETAIL = "Create yarn-art pattern on a virtual peg board.";
const std::string BANNER = HEADER + DETAIL;

const int N_ACROSS = 50;    // Number of pegs across the top and bottom.
const int N_DOWN = 30;      // Number of pegs down the left and right sides.
const int DELTA = 67;       // Connect first peg to 67th, second peg to 68th

int main(int argc, char * argv[]) {
    
    std::cout << BANNER << std::endl << std::endl;
    
    PegBoard pb(N_ACROSS, N_DOWN);
    
    // Slide show of 'yarn-art' at different peg deltas.
    
    for (int pegDelta = 1; pegDelta < (2 * N_ACROSS + N_DOWN - 2);
         pegDelta += 2)
    {
        pb.draw(pegDelta, RANDOM);
        sleep(1);
        pb.clear();
    }
    
    // Conclude with same image from Figure 6-13 of the text.
    
    pb.draw(DELTA, BLUE);
    waitForClick();  // Prevent auto-teardown of graphic window.
    
    return 0;
}
