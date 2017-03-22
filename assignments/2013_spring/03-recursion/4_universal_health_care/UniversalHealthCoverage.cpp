//
// UniversalHealthCare.cpp
//
// What combination of proposed hospital sites will provide coverage for
// a set of cities given the total number of hospitals actually built
// may be constrained due to budget limitations?
//
// Example output:
//
//     CS106B Programming Abstractions: Assignment 3.4
//     Universal Health Care
//
//     Can 2 hospitals provide health care for all
//     6 cities? false
//
//     Can 3 hospitals provide health care for all
//     6 cities? true
//
//     Here's one viable set of hospital locations:
//     {{"C", "E", "F"}, {"B", "F"}, {"A", "C", "D"}}
//
// --------------------------------------------------------------------------
// Assignment: 3, Part 4 Universal Health Care
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/110%20Assignment%203.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/7/2017.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "set.h"
#include "vector.h"
using namespace std;

// Constants

const string HEADER = "CS106B Programming Abstractions: Assignment 3.4\n";
const string DETAIL = "Universal Health Care";
const string BANNER = HEADER + DETAIL;

// Prototypes

bool allCitiesCovered(Set<string> & coveredCities,
                      Set<string> & uncoveredCities,
                      Vector<Set<string> > & pickedHospLocs,
                      Vector<Set<string> > & potentialHospLocs,
                      int maxNumHospitals);

bool canOfferUniversalCoverage(const Set<string> & cities,
                               const Vector<Set<string> > & potentialHospLocs,
                               int maxNumHospitals,
                               Vector<Set<string> > & result);

void initSets(Set<string> & cities, Vector<Set<string> > & potentialHospLocs);

void testCoverageAlgorithm();

void pickHospLoc(Set<string> & hospCities,
                 Vector<Set<string> > & potentialHospLocs,
                 Vector<Set<string> > & pickedHospLocs,
                 Set<string> & coveredCities,
                 Set<string> & uncoveredCities,
                 int choice);

void unpickHospLoc(const Set<string> & hospCities,
                   Vector<Set<string> > & potentialHospLocs,
                   Vector<Set<string> > & pickedHospLocs,
                   Set<string> & coveredCities,
                   Set<string> & uncoveredCities,
                   int choice);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    testCoverageAlgorithm();
    return 0;
}

// Function: testCoverageAlgorithm
// Usage: testCoverageAlgorithm();
// -------------------------------
// Runs some test data through the coverage engine with varying numbers
// of hospitals.
//
// Results are written to the console.

void testCoverageAlgorithm() {
    Set<string> cities;
    Vector<Set<string> > potentialHospLocs;
    
    initSets(cities, potentialHospLocs);
    for (int i = 3; i < 5; i++) {
        cout << "Can " << i << " hospitals provide health care for all" << endl;
        cout << cities.size() << " cities? " << boolalpha;
        
        Vector<Set<string> > oneSolution;
        bool result = canOfferUniversalCoverage(cities,
                                                potentialHospLocs,
                                                i,
                                                oneSolution);
        cout << result << endl << endl;
        if (result) {
            cout << "Here's one viable set of hospital locations:" << endl;
            cout << oneSolution << endl << endl;;
        }
    }
}

//
// Function: canOfferUniversalCoverage
// Usage: if (canOfferUniversalCoverage(cities, potentialHospLocs, 4, result);
// -------------------------------------------------------------------
// Given a set of cities, a list of what cities various hospitals can
// cover, and a number of hospitals, returns whether or not it's
// possible to provide coverage to all cities with the given number of
// hospitals.  If so, one specific way to do this is handed back in the
// result parameter.
//

bool canOfferUniversalCoverage(const Set<string> & cities,
                               const Vector<Set<string> > & potentialHospLocs,
                               int maxNumHospitals,
                               Vector<Set<string> > & soln) {
    
    Set<string> uncoveredCities = cities;
    Vector<Set<string> > mutableHospLocs = potentialHospLocs;
    Set<string> coveredCities;
    
    bool result = allCitiesCovered(coveredCities,
                                   uncoveredCities,
                                   soln,
                                   mutableHospLocs,
                                   maxNumHospitals);
    return result;
}

//
// Function: allCitiesCovered
// Usage: result = allCitiesCovered(coveredCities,
//                                  uncoveredCities,
//                                  soln,
//                                  potentialHospLocs,
//                                  maxNumHospitals);
// -----------------------------------------------
// Uses recursion with backtracking to discern what combination of
// hospital locations would provide coverage for a set of cities
// proximate to those locations.  The number of potential hospitals
// is constrained by a 'maxNumHospitals' parameter to reflects budgetary
// limits to the number of hospitals that may be built.
//
// If a solution is found, true is returned and the solution set
// is passed back to the caller in a 'pickedHospLocs' paramter.
//

bool allCitiesCovered(Set<string> & coveredCities,
                      Set<string> & uncoveredCities,
                      Vector<Set<string> > & pickedHospLocs,
                      Vector<Set<string> > & potentialHospLocs,
                      int maxNumHospitals) {
    // base case
    
    int k = maxNumHospitals - pickedHospLocs.size(); // num hosp still to pick
    
    // Have we picked the max number of hospitals (k == 0) allowed by our
    // budget? ... or have all the cities of interest already been covered
    // by a lesser number of hospitals?
    
    if (k == 0 || uncoveredCities.size() == 0) {
        return (uncoveredCities.size() == 0);
    }
    
    // recursive case (backtracking pattern)
    
    for (int choice = potentialHospLocs.size() - 1; choice >= 0; choice--) {
    
        // Select a hospital location and see if that (recursively)
        // converges to the goal state of hospital coverage for all cities.
        //
        // Selecting from the end of the list is designed to avoid confusing
        // the iterator if we regret the choice and want to unmake it by
        // adding it back to the collection.  The iterator should benignly
        // allow this but not revisit that location for the current level of
        // recursion.  (We may add a location back in situations where we want
        // deeper levels of recursion to consider it for inclusion in the
        // solution set.)
        
        Set<string> hospCities; // cities covered by this location
        pickHospLoc(hospCities,
                    potentialHospLocs,
                    pickedHospLocs,
                    coveredCities,
                    uncoveredCities,
                    choice);
        
        if (allCitiesCovered(coveredCities,
                             uncoveredCities,
                             pickedHospLocs,
                             potentialHospLocs,
                             maxNumHospitals)) {
            return true;
        }
        
        // Undo this choice of hospital locations since it didn't
        // result in coverage for all cities.
        
        unpickHospLoc(hospCities,
                      potentialHospLocs,
                      pickedHospLocs,
                      coveredCities,
                      uncoveredCities,
                      choice);
    }
    
    return false;   // Trigger recursive backtracking.  The current set of
                    // hospital location choices did not pan out.  So undo
                    // one or more of those choices and try again considering
                    // different locations.
}

//
// Function: pickHospLoc
// Usage: pickHospLoc(hospCities,
//                    potentialHospLocs,
//                    pickedHospLocs,
//                    coveredCities,
//                    uncoveredCities,
//                    locIndex);
// -------------------------------------------------------------------
// Updates the recursion configuration when tentatively selecting a given
// hospital location.  (This may get undone by a subsequent call to
// unpickHospLoc if recursive traversal of the solution space
// doesn't yield universal coverage for all cities.)
//

void pickHospLoc(Set<string> & hospCities,
                 Vector<Set<string> > & potentialHospLocs,
                 Vector<Set<string> > & pickedHospLocs,
                 Set<string> & coveredCities,
                 Set<string> & uncoveredCities,
                 int choice) {
    
    hospCities = potentialHospLocs[choice];
    
    pickedHospLocs.add(hospCities);
    potentialHospLocs.remove(choice);
    
    for (string city: hospCities) {
        coveredCities.add(city);
        uncoveredCities.remove(city);
    }
}

//
// Function: unpickHospLoc
// Usage: unpickHospLoc(hospCities,
//                      potentialHospLocs,
//                      pickedHospLocs,
//                      coveredCities,
//                      uncoveredCities,
//                      locIndex);
// -------------------------------------------------------------------
// Updates the recursion configuration when we're unhappy with the
// selection of a given hospital location and want to undo that.
//

void unpickHospLoc(const Set<string> & hospCities,
                   Vector<Set<string> > & potentialHospLocs,
                   Vector<Set<string> > & pickedHospLocs,
                   Set<string> & coveredCities,
                   Set<string> & uncoveredCities,
                   int choice) {
    
    potentialHospLocs.insert(choice, hospCities);
    pickedHospLocs.remove(pickedHospLocs.size() - 1);
    
    for (string city: hospCities) {
        uncoveredCities.add(city);
        coveredCities.remove(city);
    }
}

//
// Function: initSets
// Usage: initSets(cities, potentialHospLocs);
// -----------------------------------
// Loads test data from the problem set into two pass-by-reference sets.
//

void initSets(Set<string> & cities, Vector<Set<string> > & potentialHospLocs) {
    string strCities = "ABCDEF";
    for (char ch : strCities) {
        cities.add(string(1, ch));
    }
    
    Vector<string> vLoc;
    vLoc += "ABC";
    vLoc += "ACD";
    vLoc += "BF";
    vLoc += "CEF";
    
    for (int i = 0; i < vLoc.size(); i++) {
        Set<string> loc;
        for (char ch: vLoc[i]) {
            loc.add(string(1, ch));
        }
        potentialHospLocs += loc;
    }
}
