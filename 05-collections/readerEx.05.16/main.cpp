//
// main.cpp (This basically solves both 05.16 & 05.17)
//
// This program simulates servicing waiting customers across
// one or more lines or through a single line staffed with multiple
// checkers.
//
// For example,
//
//    ------------------------------------------------------------
//    Simulation results given the following conditions:
//    Simulation Time (s):      2000
//    Arrival Probability:         0.05
//    Minimum Service Time (s):    5
//    Maximum Service Time (s):   15
//    Clerks:                      1
//    Lines:                       1
//    Customers served:          116
//    Average waiting time (s):    8.80
//    Average queue length:        0.51
//
//    Simulation results given the following conditions:
//    Simulation Time (s):      2000
//    Arrival Probability:         0.10
//    Minimum Service Time (s):    5
//    Maximum Service Time (s):   15
//    Clerks:                      1
//    Lines:                       1
//    Customers served:          176
//    Average waiting time (s):   68.95
//    Average queue length:        6.49
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 5, Exercise 16 & 17
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/12/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "queue.h"
#include "random.h"
#include "vector.h"
using namespace std;

#define SAME_RANDOM_SEED_AS_TEXTBOOK

#ifdef SAME_RANDOM_SEED_AS_TEXTBOOK
#define RANDOM_SEED (1)                 // Use this if striving to match
#else                                   // published results.
#define RANDOM_SEED (int(time(NULL)))   // Use this for less deterministic
#endif                                  // initial conditions.

// Types and constants

const string LINE = string(60, '-') + '\n';
const int SIMULATION_TIME_SECS = 2000;

enum QueueModel {           // Allow simulations for these scenarios.
    ONE_LINE_ONE_CLERK,
    ONE_LINE_MANY_CLERKS
};

struct ClerkT {
    int estBusyTime;        // Estimated time to finish with current customer.
    //int customersServed;  // TODO: Add for per-clerk throughput stats.
};

struct SimCondT {           // Simulation conditions.
    double arrivalProbability;
    int minServiceTimeSecs;
    int maxServiceTimeSecs;
    int nClerks;
    QueueModel qModel;
    int simTimeSecs;
};

// Function prototypes

void banner();
bool clerkReady(ClerkT & clerk);
bool customerArrives(double probability);
bool customerWaiting(Queue<int> line);
int numLines(QueueModel qModel, int nClerks);
void runSimulation(SimCondT conditions, int & totalWaitTime,
                   int & totalLineLength, int & numCustomersServed);
void reportResults(SimCondT conditions, int totalWaitTime,
                   int totalLineLength, int numCustomersServed);
bool servicingCustomer(ClerkT clerk);
void serviceCustomer(ClerkT & clerk, int & numCustomersServed);
int shortestLine(Vector<Queue<int> > waitingLines);

// Main program

int main(int argc, char * argv[]) {
    banner();
    cout << LINE;
    
    // Metrics to compute in each simulation.
    
    int numCustomersServed;
    int totalLineLength;
    int totalWaitTime;
    
    // 
    // Set initial conditions to match published results in CS106B Reader:
    //
    // Programming Abstractions in C++ by Eric Roberts
    // Stanford University, Autumn Quarter 2012
    // Page 225.
    //
    // http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
    //
    
    SimCondT simCond;
    simCond.simTimeSecs = SIMULATION_TIME_SECS;
    simCond.minServiceTimeSecs = 5;  // These next two values seem undersized
    simCond.maxServiceTimeSecs = 15; // but left unchanged to match textbook.
    simCond.nClerks = 1;
    simCond.qModel = ONE_LINE_ONE_CLERK;
    
    simCond.arrivalProbability = 1/double(20);
    
    runSimulation(simCond, totalWaitTime, totalLineLength, numCustomersServed);
    reportResults(simCond, totalWaitTime, totalLineLength, numCustomersServed);
    
    // 
    // Double the arrival rate (1 customer every 10 seconds on average) 
    // to see average wait times increase from just under 10 seconds to more 
    // than 1 minute.
    //
    
    simCond.arrivalProbability = 1/double(10);
    
    runSimulation(simCond, totalWaitTime, totalLineLength, numCustomersServed);
    reportResults(simCond, totalWaitTime, totalLineLength, numCustomersServed);
    
    //
    // Q1: At that faster arrival rate, what works better?
    //
    //    1. opening up more lines, one clerk per line?
    // or 2. having more clerks service one longer line?
    //
    
    simCond.nClerks = 3;
    
    simCond.qModel = ONE_LINE_ONE_CLERK;
    runSimulation(simCond, totalWaitTime, totalLineLength, numCustomersServed);
    reportResults(simCond, totalWaitTime, totalLineLength, numCustomersServed);

    //
    // A1:
    //
    // The second option is the clear winner under these simulated conditions
    // with wait times ~15x shorter and queue length 1/3rd as long for same
    // number of customers.
    //
    
    simCond.qModel = ONE_LINE_MANY_CLERKS;
    runSimulation(simCond, totalWaitTime, totalLineLength, numCustomersServed);
    reportResults(simCond, totalWaitTime, totalLineLength, numCustomersServed);

    //
    // Q2: What happens if we increase the range of time it takes to complete
    //     a checkout from 5 - 15 seconds (blindingly fast, imo) to something 
    //     more tenable like 30 seconds to 3 minutes?
    //
    
    simCond.minServiceTimeSecs = 30;
    simCond.maxServiceTimeSecs = 180;
    simCond.nClerks = 15;
    
    simCond.qModel = ONE_LINE_ONE_CLERK;
    runSimulation(simCond, totalWaitTime, totalLineLength, numCustomersServed);
    reportResults(simCond, totalWaitTime, totalLineLength, numCustomersServed);

    //
    // A2:
    //
    // If number of clerks is left at 3, there is no clear advantage;
    // customers suffer at about the same level.  However if you up the number
    // of clerks to 15, then the multiple-clerks, one-line strategy is
    // dramatically better (e.g., 10x shorter wait times and line length 
    // almost 1/4th as long).
    //
    
    simCond.qModel = ONE_LINE_MANY_CLERKS;
    runSimulation(simCond, totalWaitTime, totalLineLength, numCustomersServed);
    reportResults(simCond, totalWaitTime, totalLineLength, numCustomersServed);

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
    cout << "This program can simulate servicing waiting customers across"
         << endl
         << "multiple lines or many clerks servicing one large line."
         << endl;
}

// 
// Function: runSimulation
// Usage: runSimulation(conditions, totalWait, totalLine, totalCustomers);
// -----------------------------------------------------------------------
// Models thoughput and wait times for customers in line for service across
// two different queuing scenarios:
//
//    1. One or more lines with one clerk per line.
//    2. Multiple clerks servicing one line.
//
// Summarizing metrics are conveyed back to the caller through reference
// variables.
//
// Assumptions: 
//
// 1. Arrivals occur randomly with equal probability in each unit of time.
// 2. Service time for each customer is uniformly distributed within a
//    minimum and maximum range specified in the simulation conditions.
//

void runSimulation(SimCondT sc, int & totalWaitTime,
                   int & totalLineLength, int & numCustomersServed) {
   
    // 
    // In one model, the number of lines equals the number of clerks.
    // In another, many clerks service one line.
    //
    int nLines = numLines(sc.qModel, sc.nClerks);

    Vector<Queue<int> > waitingLines(nLines);
    Vector<ClerkT> clerks(sc.nClerks);
    
    // Simulation metrics to calculate.
    
    totalWaitTime = 0;
    totalLineLength = 0;
    numCustomersServed = 0;
    
    setRandomSeed(RANDOM_SEED);
    
    // Iterate for a specified amount of time.
    
    for (int t = 0; t < sc.simTimeSecs; t++) {
        
        // Add arriving customer to waiting line.
        
        if (customerArrives(sc.arrivalProbability)) {
            int shortest = shortestLine(waitingLines);
            waitingLines[shortest].enqueue(t);
        }

        // Iterate across all clerks.
        
        for (int c = 0; c < clerks.size(); c++) {
            int line;
            if (sc.qModel == ONE_LINE_MANY_CLERKS) {
                line = 0;   // Only one line to choose from.
            } else {
                line = c;   // One clerk, one line.
            }
            
            if (clerkReady(clerks[c])) {
                if (customerWaiting(waitingLines[line])) {
                    
                    // Pull next customer off the waiting line.
                    
                    int startedWaiting = waitingLines[line].dequeue();
                    int stoppedWaiting = t;
                    
                    totalWaitTime += (stoppedWaiting - startedWaiting);
                    
                    // Calculate expected time to complete this checkout.
                    
                    clerks[c].estBusyTime = randomInteger(sc.minServiceTimeSecs,
                                  sc.maxServiceTimeSecs);
                }
            } else {
                serviceCustomer(clerks[c], numCustomersServed);
            }
            
            totalLineLength += waitingLines[line].size();
        }
    }
}

//
// Function: numLines
// Usage: int nLines = numLines(qModel, nClerks);
// ----------------------------------------------
// Return the number of waiting lines implied by the queuing model and number 
// of clerks present.
//

int numLines(QueueModel qModel, int nClerks) {
    int nLines;
    
    switch (qModel) {
        case ONE_LINE_MANY_CLERKS:
            nLines = 1;
            break;
            
        case ONE_LINE_ONE_CLERK:
        default:
            nLines = nClerks;
            break;
    }
    return nLines;
}

//
// Function: shortestLine
// Usage: int lineIndex = shortestLine(waitingLines);
// --------------------------------------------------
// Returns the line with the fewest number of waiting customers.
//

int shortestLine(Vector<Queue<int> > waitingLines) {
    
    if (waitingLines.size() == 1) {
        // Short-circuit return if only one line open, no matter how full.
        return 0;
    }
    
    // Which line has fewest customers, ignoring how full their cart?
    
    int shortest = 0;
    int shortestLength = -1;
    int nLines = waitingLines.size();
    
    for (int l = 0; l < nLines; l++) {
        int length = waitingLines[l].size();
        if (length) {
            if (shortestLength == -1 || length < shortestLength) {
                shortestLength = length;
                shortest = l;
            }
        } else {
            // Stop on first 0-length line.  Can't improve that.
            shortest = l;
            break;
        }
    }
    return shortest;
}

//
// Function: reportResults
// Usage: reportResults(conditions, totalWait, totalLine, totalCustomers);
// -----------------------------------------------------------------------
// Summarize the simulation results to the console.
//

void reportResults(SimCondT sc, int totalWaitTime, int totalLineLength,
                   int numCustomersServed) {
    
    int nLines = numLines(sc.qModel, sc.nClerks);
    cout << "Simulation results given the following conditions:"
         << endl;
    cout << fixed << setprecision(2);
    cout << "Simulation Time (s):      " << setw(4);
    cout << sc.simTimeSecs               << endl;
    cout << "Arrival Probability:      " << setw(7);
    cout << sc.arrivalProbability        << endl;
    cout << "Minimum Service Time (s): " << setw(4);
    cout << sc.minServiceTimeSecs        << endl;
    cout << "Maximum Service Time (s): " << setw(4);
    cout << sc.maxServiceTimeSecs        << endl;
    cout << "Clerks:                   " << setw(4);
    cout << sc.nClerks                   << endl;
    cout << "Lines:                    " << setw(4);
    cout << nLines                       << endl;
    cout << "Customers served:         " << setw(4)
         << numCustomersServed           << endl;
    cout << "Average waiting time (s): " << setw(7);
    if (numCustomersServed) {
         cout << double(totalWaitTime) / numCustomersServed      << endl;
    } else {
         cout << "     unable to compute (customers served = 0)" << endl;
    }
    cout << "Average queue length:     " << setw(7)
         << double(totalLineLength) / sc.simTimeSecs
         << endl << endl;
}

//
// Function: clerkReady
// Usage: if (clerkReady(clerk)) { ... }
// -------------------------------------
// Returns true is a given clerk is available to checkout the next waiting
// customer.
//

bool clerkReady(ClerkT & clerk) {
    return !servicingCustomer(clerk);
}

//
// Function: customerArrives
// Usage: if (customerArrives(.10)) { ... }
// ----------------------------------------
// Returns true if a customer arrives given a certain probability.
//

bool customerArrives(double probability) {
    return randomChance(probability);
}

//
// Function: customerWaiting
// Usage: if (customerWaiting(line)) { ... }
// -----------------------------------------
// Returns true if one or more customers are waiting in a given line.
//

bool customerWaiting(Queue<int> line) {
    return !line.isEmpty();
}

//
// Function: servicingCustomer
// Usage: if (servicingCustomer(clerk)) { ... }
// --------------------------------------------
// Returns true if a given checkout clerk is busy with a customer.
//

bool servicingCustomer(ClerkT clerk) {
    return (clerk.estBusyTime > 0);
}

//
// Function: serviceCustomer
// Usage: serviceCustomer(clerk, numCustomersServed);
// --------------------------------------------------
// Models the helping of a customer by a checkout clerk at a discrete
// time in the simulation.  If the clerk finishes with the customer in this
// call, a reference variable with a current count of all customers served
// so far is incremented.
//

void serviceCustomer(ClerkT & clerk, int & numCustomersServed) {
    if (clerk.estBusyTime > 0) {
        clerk.estBusyTime--;
        if (clerk.estBusyTime == 0) {
            // 
            // NB: We only update the count of customers served once they
            //     are fully checked out.  Ignore partial checkouts if the
            //     simulation ends in that state.
            //
            numCustomersServed++;   // TODO Could implement clerk-specific stats
                                    // here instead of aggregated count.
                                    // e.g., clerk.customersServed++;
        }
    }
}
