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

bool allCitiesCovered(Set<string> & citiesSoFar,
                      Set<string> & citiesToCover,
                      Vector<Set<string> > & pickedLocs,
                      Vector<Set<string> > & locs,
                      int numHospitals);

bool canOfferUniversalCoverage(Set<string> & cities,
                               Vector<Set<string> > & locations,
                               int numHospitals,
                               Vector<Set<string> > & result);

void initSets(Set<string> & cities, Vector<Set<string> > & locs);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    
    Set<string> cities;
    Vector<Set<string> > locations;
    initSets(cities, locations);
    
    for (int i = 2; i < 4; i++) {
        cout << "Can " << i << " hospitals provide health care for all" << endl;
        cout << cities.size() << " cities? " << boolalpha;
        
        Vector<Set<string> > oneSolution;
        bool result = canOfferUniversalCoverage(cities,
                                                locations,
                                                i,
                                                oneSolution);
        cout << result << endl << endl;
        if (result) {
            cout << "Here's one viable set of hospital locations:" << endl;
            cout << oneSolution << endl;
        }
    }

    return 0;
}

//
// Function: canOfferUniversalCoverage
// Usage: if (canOfferUniversalCoverage(cities, locations, 4, result);
// -------------------------------------------------------------------
// Given a set of cities, a list of what cities various hospitals can
// cover, and a number of hospitals, returns whether or not it's
// possible to provide coverage to all cities with the given number of
// hospitals.  If so, one specific way to do this is handed back in the
// result parameter.
//

bool canOfferUniversalCoverage(Set<string> & cities,
                               Vector<Set<string> > & locations,
                               int numHospitals,
                               Vector<Set<string> > & soln) {
    
    Set<string> citiesToCover = cities;
    Set<string> citiesSoFar;
    
    bool result = allCitiesCovered(citiesSoFar,
                                   citiesToCover,
                                   soln,
                                   locations,
                                   numHospitals);
    return result;
}

//
// Function: allCitiesCovered
// Usage: result = allCitiesCovered(citiesSoFar,
//                                  citiesToCover,
//                                  soln,
//                                  locations,
//                                  numHospitals);
// -----------------------------------------------
// Uses recursion with backtracking to discern what combination of
// hospital locations would provide coverage for a set of cities
// proximate to those locations.  The number of potential hospitals
// is constrained by a 'numHosp' parameter to reflects budgetary
// limits to the number of hospitals that may be built.
//
// If a solution is found, true is returned and the solution set
// is passed back to the caller in a 'pickedLocs' paramter.
//

bool allCitiesCovered(Set<string> & citiesSoFar,
                      Set<string> & citiesToCover,
                      Vector<Set<string> > & pickedLocs,
                      Vector<Set<string> > & locs,
                      int numHosp) {
    // base case
    
    int n = locs.size();                    // num hospitals to pick from
    int k = numHosp - pickedLocs.size();    // num hospitals still to pick
    
    // We need to pick k more hospitals but only k hospitals are left to pick
    // so we should stop now (n == k).
    //
    // We've picked the max allowed number of hospitals (k == 0).
    
    if ((n == k || k == 0)) {
        return (citiesToCover.size() == 0);
    }
    
    // recursive case
    
    for (int i = locs.size() - 1; i >= 0; i--) {
    
        // Start by selecting hospitals to consider from
        // the -end- of the vector of locations.
        //
        // This makes it easier on the for-loop if we later regret the
        // choice and have to add it back to the list.
        //
        // TODO: Wrapper the selection logic in a helper function.
        
        Set<string> hospCities = locs[i];
        pickedLocs.add(hospCities);
        locs.remove(i);
        for (string city: hospCities) {
            citiesSoFar.add(city);
            citiesToCover.remove(city);
        }
        
        if (allCitiesCovered(citiesSoFar,
                             citiesToCover,
                             pickedLocs,
                             locs,
                             numHosp)) {
            return true;
        }
        
        // Undo this choice of hospital locations since it didn't
        // result in a solve.
        //
        // TODO: Wrapper the de-selection logic in a helper function.
        
        locs.insert(i, hospCities);
        pickedLocs.remove(pickedLocs.size() - 1);
        for (string city: hospCities) {
            citiesSoFar.remove(city);
            citiesToCover.add(city);
        }
    }
    
    return false;   // Trigger recursive backtracking.
}

//
// Function: initSets
// Usage: initSets(cities, locations);
// -----------------------------------
// Loads test data from the problem set into two pass-by-reference sets.
//

void initSets(Set<string> & cities, Vector<Set<string> > & locations) {
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
        locations += loc;
    }
}
