//
// main.cpp
//
// This program tests a two-player game engine class, customized for
// the game of Nim.
//
// This problem illustrates the use of inheritance and template base classes.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 06
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/25/17
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "Nim.h"
using namespace std;

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 19.06\n";
const std::string DETAIL = "Game of Nim (subclassed from 2-player game engine)";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main() {
    cout << BANNER << endl << endl;
    Nim game;
    game.printInstructions();
    game.play();
    return 0;
}
