//
// KarelGoesHome.cpp
//
// Given a street number and avenue number, which are 1-indexed, this program
// returns the number of paths Karel-the-Robot can take back home that only
// consist of left and downward moves.
//
//                  __ 5th avenue
//                 /
//        1 2 3 4 5
//      +-----------+
//     5| . . . . . |
//     4| . . . . . |
//     3| . . K . . |
//     2| . . . . . |
//     1| H . . . . | -- 1st street
//      +-----------+
//
// where H = home and K = Karel-the-Robot
//
// Example output:
//
//     CS106B Programming Abstractions: Assignment 3.0A
//     Karel Goes Home
//
//     Find the number of paths home that Karel-the-Robot can
//     take home that only involve left or downward moves.
//
//     Starting  at street: 3 and avenue: 3
//     Returning to street: 1 and avenue: 1
//
//     Number of move-constrained paths home: 6
//
//     Paths home:
//     {"DDLL", "DLDL", "DLLD", "LDDL", "LDLD", "LLDD"}
//
// Analysis
//
// If you encode the solution paths as a string of D's and L's, you realize
// this can be thought of as a permutation problem (as opposed to combination).
//
// For example, these two paths:
//
//    D D D L L  and  L D D D L
//
// may both be valid and unique paths home and yet they are comprised of the
// same letters which would be seen as identical combinations.  So permutaion
// seems the better strategy.
//
// The wrinkle here is some permutations will not be unique since we
// generally have duplicate symbols on the input (selection with
// replacement) for non-trivial distances from home.
//
// We need to avoid, for example, treating the following two permutes as
// different paths:
//
//    D1 D2 D3 L L  ==  D2 D1 D3 L L
//    -- --             -- --
//
// Employing a set could de-duplicate the collection of paths home.
//
// Thankfully, the constraint of movement to leftward and downward directions 
// avoids the complication of cyclic paths, so we need not track
// where Karel has been over the journey home.
//
// The solution space of possible paths home can be thought of as a tree of
// decision points.  The depth of that tree is equal to the linearized 
// distance between the start and home locations; we'll have to make that 
// number of decisions to get home.
//
// The width of the tree at a given point will be two, since
// we have at most two choices to make, head left or head downward.
//
// --------------------------------------------------------------------------
// Assignment: 3, Part 0A Karel Goes Home
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/110%20Assignment%203.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/6/2017.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <ostream>
#include "set.h"
using namespace std;

// Constants & Types

const int START_STREET = 3;
const int START_AVENUE = 3;

const int HOME_STREET = 1;
const int HOME_AVENUE = 1;


const string ALREADY_HOME = "Karel is already home. :-)";

const string HEADER = "CS106B Programming Abstractions: Assignment 3.0A\n";
const string DETAIL = "Karel Goes Home";
const string BANNER = HEADER + DETAIL;

struct LocationT {
    int street;
    int avenue;
};

// Prototypes

bool hasSolution(string & refSoln,
                 const LocationT & start,
                 const LocationT & home);

int numPathsHome(Set<string> & pathsHome,
                 LocationT start,
                 const LocationT & home);

int uniquePermutes(Set<string> & pathsHome,
                   string thisPath,
                   string wayHome);

ostream & operator<<(ostream & os, const LocationT & loc);

// Main

int main() {
    cout << BANNER << endl << endl;;
    
    LocationT start, home;
    
    start.street = START_STREET;
    start.avenue = START_AVENUE;
    home.street = HOME_STREET;
    home.avenue = HOME_AVENUE;
    
    cout << "Find the number of paths home that Karel-the-Robot can" << endl;
    cout << "take home that only involve left or downward moves." << endl;
    cout << endl;
    
    cout << "Starting  at " << start << endl;
    cout << "Returning to " << home << endl << endl;
    
    Set<string> pathsHome;
    
    cout << "Number of move-constrained paths home: ";
    cout << numPathsHome(pathsHome, start, home) << endl << endl;
    
    cout << "Paths home:" << endl;
    cout << pathsHome << endl;

    return 0;
}

// Functions

// Function: hasSolution
// Usage: if (hasSolution(strSolution, loc1, loc2)) ...
// ----------------------------------------------------
// Returns true if at least one path can be found from location 1
// to location 2 taking only left and downward movements.
//
// In that event, a reference solution is passed back as a string
// of D's and L's.  Otherwise false is returned and the solution
// parameter will be an empty string.

bool hasSolution(string & refSoln,
                 const LocationT & start,
                 const LocationT & home) {
    refSoln = "";
    
    // How far north is start location from home?
    int northDelta = start.street - home.street;
    
    // How far east is start location from home?
    int eastDelta = start.avenue - home.avenue;
    
    // Can't go west to get east in Karel's limited world.
    if (northDelta < 0 || eastDelta < 0) return false;
    
    // Construct a reference solution that can be permuted
    // to find all unique paths home.
    
    refSoln = string(northDelta, 'D') + string(eastDelta, 'L');
    
    return true;
}

// Function: numPathsHome
// Usage: int num = numPathsHome(pathsHome, startLoc, homeLoc);
// ------------------------------------------------------------
// Returns the number of unique paths home from a starting location.
//
// Serves as a wrapper function for a recursive function, uniquePermutes,
// which permutes a reference solution into a set of possible solutions.

int numPathsHome(Set<string> & pathsHome,
                 LocationT start,
                 const LocationT & home) {
    string refSoln;
    int result = 0;
    
    if (hasSolution(refSoln, start, home)) {
        if (refSoln == "") {
            pathsHome.add(ALREADY_HOME);
        }
        else {
            result = uniquePermutes(pathsHome, "", refSoln);
        }
    }
    return result;
}

// Function: uniquePermutes
// Usage: numPaths = uniquePermutes(uniquePaths, somePath, refPathHome);
// ---------------------------------------------------------------------
// Uses a recursive strategy to find the unique permutes of a reference
// path home, called wayHome.  Unique paths are collected in a set over
// successive calls.
//
// The paths are represented as a string of D's and L's to indicate downward
// or leftward steps.
//
// The number of paths collected is returned back to the caller along with
// the actual set of unique paths, through a pass-by-reference parameter.

int uniquePermutes(Set<string> & pathsHome, string thisPath, string wayHome) {
    
    // base case: no more letters in wayHome to permute
    
    if (wayHome == "") {
        bool isNewPath = !pathsHome.contains(thisPath);
        if (isNewPath) {
            pathsHome.add(thisPath);
        }
        return pathsHome.size();
    }
    
    // recursive case: select a direction letter and permute the remaining.
    //                 remove the letter from the wayHome path so we make
    //                 progress against a diminishing reference path.
    
    for (int i = 0; i < wayHome.length(); i++) {
        string next = thisPath + wayHome[i];
        string remaining = wayHome.substr(0, i) + wayHome.substr(i + 1);
        uniquePermutes(pathsHome, next, remaining);
    }
    
    return pathsHome.size();
}

ostream & operator<<(ostream & os, const LocationT & loc) {
    os << "street: " << loc.street << " and avenue: " << loc.avenue;
    return os;
}
