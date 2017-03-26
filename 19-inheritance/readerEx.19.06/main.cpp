//
// main.cpp
//
// This program tests a templated, two-player game engine class:
//
//      TwoPlayerGame<MoveType>
//
// The engine provides a game model that implements a 'minimax' recusive
// backtracking algorithm for assessing optimal play (by the computer).
//
// Subclasses are responsible for implementing methods that embody the notion
// of 'move' which is highly unique to each game.  The engine implements
// game-invariant behavior such as turn-taking and position evalution.
//
// I've adapted earlier implementations of Nim and Tic-Tac-Toe from Chapter 9
// to leverage the game engine.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 06  (See also Chapter 9, Exercises 12 & 15)
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/26/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "Nim.h"
#include "TicTacToe.h"
using namespace std;

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 19.06\n";
const std::string DETAIL = "Inheritance: Nim/TicTacToe w game-engine superclass";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    Nim nGame;
    nGame.printInstructions();
    nGame.play();
    
    TicTacToe tGame(HUMAN, TicTacToe::XSYM);
    tGame.printInstructions();
    tGame.play();
    
    return 0;
}
