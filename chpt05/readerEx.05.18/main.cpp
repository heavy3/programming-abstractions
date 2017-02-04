//
// main.cpp
//
// This program simulates a simplified version of nuclear fission illustrated
// in a 1957 Disney film called "Our Friend the Atom".  An array of 625 mouse-
// traps, each loaded with two ping-pong balls, line the bottom of a square
// chamber.  A single ping-pong ball is tossed into the 25x25 grid of
// traps, triggering a cascade of flying balls which then set off other
// traps in a creshendo of action until the airborn balls eventually settle, 
// since no new traps are left to spring, or a minority of traps manage to avoid 
// being struck in the course of the simulation.
//
// Reported metrics for the simulation include:
//
//    elapsed time (in simulation cycles)
//    maximum balls in the air at one time
//    percentage of traps that are sprung.
//
// Typical output looks like this:
//
//    Time   Balls in Air (* = approx 10 balls)
//    -----------------------------------------
//    t=  10: *
//    t=  11: **
//    t=  12: **
//    t=  13: ****
//    t=  14: ******
//    t=  15: *******
//    t=  16: **********
//    t=  17: ***********
//    t=  18: ***************
//    t=  19: ******************
//    t=  20: ******************
//    t=  21: *******************
//    t=  22: *******************
//    t=  23: ******************
//    t=  24: *****************
//    t=  25: ***************
//    t=  26: *************
//    t=  27: **********
//    t=  28: ********
//    t=  29: ******
//    t=  30: ****
//    t=  31: ****
//    t=  32: ***
//    t=  33: **
//    t=  34: *
//    t=  35: *
//
//    Mousetrap-fission simulation results given the following conditions:
//    Balls Per Loaded Mousetrap:       2
//    Mousetrap rows:                  25
//    Mousetrap cols:                  25
//    Minimum airborn time (cycles):    1
//    Maximum airborn time (cycles):    4
//    Elapsed time (cycles):           43
//    Maximum Balls In Air:           201
//    Percent Traps Sprung:             0.81
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 18
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/13/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "grid.h"
#include "random.h"
#include "vector.h"
using namespace std;

//#define DETERMINISTIC
#define SHOW_FLYING_BALL_GRAPH          // Shows a rough visualization of the
                                        // number of balls currently airborne
                                        // during each cycle of the simulation
                                        // (e.g. normal shape turned on side).
#ifdef DETERMINISTIC
#define RANDOM_SEED (1)                 // Use this while debugging.
#else                                   //
#define RANDOM_SEED (int(time(NULL)))   // Use this for less deterministic
#endif                                  // initial conditions.

// Types and constants

const string LINE = string(60, '-') + '\n';

//
// Initial conditions taken from:
//
// Programming Abstractions in C++ by Eric Roberts
// Stanford University, Autumn Quarter 2012
// Page 257.
//
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//

const int BALLS_PER_TRAP = 2;
const int GRID_ROWS = 25;
const int GRID_COLS = 25;
const int MIN_FLY_TIME_CYCLES = 1;
const int MAX_FLY_TIME_CYCLES = 4;

struct BallT {
    int flying;             // Discrete (simulated) time until ball lands.
    int row;                // Target trap row coordinated.
    int col;                // Target trap col coordinated.
};

enum TrapState {            // States of a mousetrap.
    LOADED = 0,
    SPRUNG = 1
};

struct MouseTrapT {         // Attributes of a mousetrap in this simulation.
    TrapState state;
};

struct SimCondT {           // Simulation conditions.
    int ballsPerTrap;
    int gridRows;
    int gridCols;
    int minFlyTime;
    int maxFlyTime;
};

// Function prototypes

void banner();
void setTraps(Grid<MouseTrapT> & grid, TrapState state = LOADED);
BallT launch(int gridRows, int gridCols, int minFlyTime, int maxFlyTime);
void runSimulation(SimCondT conditions, int & elapsedTime, int & maxFlyingBalls,
                   double & percentSprung);
void reportResults(SimCondT conditions, int elapsedTime, int maxFlyingBalls,
                   double percentSprung);

// Main program

int main(int argc, char * argv[]) {
    banner();
    cout << LINE;
    
    // Metrics to compute in each simulation.
    
    int elapsedTime;
    int maxInAirBalls;
    double percentSprung;
    
    SimCondT simCond;
    simCond.ballsPerTrap = BALLS_PER_TRAP;
    simCond.gridRows = GRID_ROWS;
    simCond.gridCols = GRID_COLS;
    simCond.minFlyTime = MIN_FLY_TIME_CYCLES;
    simCond.maxFlyTime = MAX_FLY_TIME_CYCLES;
    
    runSimulation(simCond, elapsedTime, maxInAirBalls, percentSprung);
    reportResults(simCond, elapsedTime, maxInAirBalls, percentSprung);
    
    return 0;
}

// Function definitions

//
// Function: banner
// Usage: banner(upperBound)
// -------------------------
// Displays the purpose of the program on the console.
//

void banner() {
    cout << "This program simulates a Disney-fied version of nuclear fission"
         << endl
         << "by modeling atom splitting with neutrons via a grid of"
         << endl
         << "mousetraps loaded with two ping-pong balls each.  A chain-"
         << endl
         << "reaction is triggered by an incoming ball in the role of neutron."
         << endl;
}

//
// Function: runSimulation
// Usage: runSimulation(simCond, elapsedTime, maxFlyingBalls, percentSprung);
// --------------------------------------------------------------------------
// Models a nuclear fission chain-reaction by tossing a single ping-pong
// ball onto the floor of a square chamber covered with mousetraps loaded
// with 2 ping-pong balls each.  Here, the ping-pong ball represents a
// neutron and the mousetrap represents a radioactive material such as
// Uranium.
//
// Metrics for the simulation are returned by reference variables and include:
//
// 1. The elapsed time (in units of simulation cycles).
// 2. The maximum number of balls in air during any one cycle.
// 3. The percentage of mousetraps sprung during simulation.
//
// Assumptions: (Courtesy Eric Roberts, Programming Abstractions in C++ 2012)
//
// 1. Every ping-pong ball that falls always lands on a mousetrap, chosen 
//    randomly by selecting a random row and column in the grid. If the trap is 
//    loaded, its balls are released into the air. If the trap has already been 
//    sprung, having a ball fall on it has no effect.
//
// 2. Once a ball falls on a mousetrap—whether or not the trap is sprung—that 
//    ball stops and takes no further role in the simulation.
//
// 3. Balls launched from a mousetrap bounce around the room and land again 
//    after a random number of simulation cycles have gone by. That random 
//    interval is chosen independently for each ball and is always between one 
//    and four cycles.
//

void runSimulation(SimCondT sc, int & elapsedTime, int & maxBallsInAir,
                   double & percentSprung) {
    
    Grid<MouseTrapT> mouseTraps(sc.gridRows, sc.gridCols);
    Vector<BallT> flyingBalls;
    
    setTraps(mouseTraps);
    BallT triggerBall = launch(sc.gridRows, sc.gridCols, sc.minFlyTime,
                           sc.maxFlyTime);
    flyingBalls.add(triggerBall);
    
    maxBallsInAir = 1;
    percentSprung = 0;
    elapsedTime = 0;
    
    setRandomSeed(RANDOM_SEED);
    int trapsSprung = 0;
    while (flyingBalls.size()) {
        elapsedTime++;
        for (int b = 0; b < flyingBalls.size(); b++) {
            flyingBalls[b].flying--;
            if (!flyingBalls[b].flying) {
                BallT landedBall = flyingBalls.get(b);
                flyingBalls.remove(b);
                int lbRow = landedBall.row;
                int lbCol = landedBall.col;
                if (mouseTraps[lbRow][lbCol].state == LOADED) {
                    for (int i = 0; i < sc.ballsPerTrap; i++) {
                        flyingBalls.add(launch(sc.gridRows, sc.gridCols,
                                           sc.minFlyTime, sc.maxFlyTime));
                    }
                    mouseTraps[lbRow][lbCol].state = SPRUNG;
                    trapsSprung++;
                    maxBallsInAir = max(flyingBalls.size(), maxBallsInAir);
                }
            }
        }
        #ifdef SHOW_FLYING_BALL_GRAPH
        if (elapsedTime == 1) {
            cout << endl;
            cout << "Time   Balls in Air (* = approx 10 balls)" << endl;
            cout << "-----------------------------------------" << endl;
        }
        string visual = string(flyingBalls.size()/10,'*');
        if (visual.length()) {
            cout << "t="   << setw(4) << elapsedTime << ": "
                 << visual << endl;
        }
        #endif
    }
    percentSprung = (double)trapsSprung / (double)(sc.gridRows * sc.gridCols);
}

//
// Function: setTraps
// Usage: setTraps(mouseTraps, trapState);
// ---------------------------------------
// Set the initial state of all traps in the grid to a given state.
//

void setTraps(Grid<MouseTrapT> & grid, TrapState state) {
    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            grid[r][c].state = state;
        }
    }
}

//
// Function: launch
// Usage: launch(targetRow, targetColumn, minFlightTime, maxFlightTime);
// ---------------------------------------------------------------------
// Returns a flying ball that is headed toward a randomly selected
// mousetrap on the floor of the simulation chamber.  Discrete flight time
// is also randomized within the bounds of min and max values passed in.
//

BallT launch(int gridRows, int gridCols, int minFlyTime, int maxFlyTime) {
    BallT ball;
    
    ball.flying = randomInteger(minFlyTime, maxFlyTime);
    ball.row = randomInteger(0, gridRows - 1);
    ball.col = randomInteger(0, gridCols - 1);
    return ball;
}

//
// Function: reportResults
// Usage: reportResults(simCond, elapsedTime, maxInAirBalls, percentSprung);
// --------------------------------------------------------------------------
// Summarize the simulation results to the console.
//

void reportResults(SimCondT sc, int elapsedTime, int maxInAirBalls,
                   double percentSprung) {
    
    cout << "Mousetrap-fission simulation results given the following conditions:"
         << endl;
    cout << fixed << setprecision(2);
    cout << "Balls Per Loaded Mousetrap:    " << setw(4);
    cout << sc.ballsPerTrap                   << endl;
    cout << "Mousetrap rows:                " << setw(4);
    cout << sc.gridRows                       << endl;
    cout << "Mousetrap cols:                " << setw(4);
    cout << sc.gridCols                       << endl;
    cout << "Minimum airborn time (cycles): " << setw(4);
    cout << sc.minFlyTime                     << endl;
    cout << "Maximum airborn time (cycles): " << setw(4);
    cout << sc.maxFlyTime                     << endl;
    cout << "Elapsed time (cycles):         " << setw(4);
    cout << elapsedTime                       << endl;
    cout << "Maximum Balls In Air:          " << setw(4);
    cout << maxInAirBalls                     << endl;
    cout << "Percent Traps Sprung:          " << setw(7);
    cout << percentSprung                     << endl << endl;
}
