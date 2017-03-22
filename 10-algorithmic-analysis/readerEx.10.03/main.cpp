//
// main.cpp
//
// This program implements a function that keeps track of the elapsed 
// time as it executes an insertion sort on various sized datasets.
//
// The program produces a table of observed run times for datasets
// of a given dataset size, N.  Output looks like this:
//
//      N     |   Time (msec)
// -----------+-----------------
//        10  |       0.00074
//        50  |       0.00688
//        ..  |       ..
//    100000  |      43.76030
//
// This was an interesting problem.  Initially I was vexed that my
// implementation didn't approach the efficiency implied by the runtimes
// published in the text.
//
// Then I realized the published metrics could only be achieved by
// conditioning the input data to be mostly sorted in the first place.
//
// How else could you explain scaling the data set size by 10x but only
// increase runtime by 30% to 50%?  For an algorithm that is purportedly
// O(N^2) in the worst case?  Instead of seeing the expected 100x longer
// runtimes, the text publishes 1.3x - 1.5x increased runtimes.  Clearly
// the input was massaged to be nearly best-case (or mostly pre-sorted to
// begin with) to yield such impressive numbers.
//
// Once I understood that, I included a 'mostly-sorted' data set for one of
// my test runs and got numbers that approached the published results.
//
// I also exploited the notion of function-as-data by treating the
// insertion sort itself as just another parameter passed into the
// test harness for conducting repeated trials.
//
// This should simplify reusing the harness for comparing other sort
// implementations.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 10, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/29/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include "vector.h"
#include "random.h"
#include "error.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 10.03\n";
const std::string DETAIL = "Runtime performance of Insertion Sort";
const std::string BANNER = HEADER + DETAIL;
const int MAX_REPS = 10000;
const int MSEC_PER_SEC = 1000;

// Types

enum SimulationT {
    GET_OVERHEAD,   // Don't run the sort.  Just calculate simulation overhead.
    PERFORM_SORT    // Perform the sort (includes simulation overhead).
};

enum ConditionT {
    ASCENDING,        // Input data already sorted.
    ASCENDISH,        // Input data mostly sorted in ascending order.
    RANDOMIZE,        // Input data randomized in position and value.
    DESCENDING,       // Input data reverse sorted.
    LAST_CONDITION
};

struct ResultT {
    int N;           // Size of input data.
    double timeMsec; // Elasped time to sort input data (factors out overhead).
    ConditionT cond; // Degree of randomness in generated input data.
};

typedef Vector<ResultT> Report; // Aggregates results for various sizes of input

// Prototypes

void insertionSort(Vector<int> & data);
std::string toString(Vector<int> & data, int s);
ostream & operator<<(ostream & os, const Report & report);
ConditionT operator++(ConditionT & cond, int);
int getSimulationReps(int N);
void getInputData(Vector<int> & items, int N, ConditionT condition = RANDOMIZE);
void runSort(int N, ResultT & results, ConditionT cond,
     void (sortfn)(Vector<int> & data));
double getSortTime(Vector<int> & data, void (sortfn)(Vector<int> & data),
     SimulationT simType = PERFORM_SORT);
ostream & operator<<(ostream & os, ConditionT cond);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    Vector<int> N;  // Predefined input data sizes.
    N += 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000;
    
    for (ConditionT cond = ASCENDING; cond <= DESCENDING; cond++) {
        Report report;
        for (int n : N) {
            ResultT result;
            
            // Run sorting function against n-length vector of generated data.
            //
            // For small data sets, this may entail repeating the sort
            // and computing average elapsed time and correcting for overhead.
            
            runSort(n, result, cond, insertionSort);
            report.add(result);
        }
        cout << report << endl;
    }
    return 0;
}

//
// Function: runSort
// Usage: runSort(N, &result, RANDOMIZE);
// ------------------------------------
// Runs a sorting simulation for an input data set of size N items.
// The input data are generated as a side-effect of the simulation.
//
// Results are reported through a pass-by-reference result record.
//
// The degree of randomness seen in the input data may be specified
// by an optional condition variable.
//

void runSort(int N, ResultT & results, ConditionT cond, void (sortfn)(Vector<int> &)) {
    Vector<int> data;
    getInputData(data, N, cond);
    
    // Perform the sort and return elapsed time.
    
    double totalSecs = getSortTime(data, sortfn, PERFORM_SORT);
    
    double overheadSecs = getSortTime(data, sortfn, GET_OVERHEAD);
    results.timeMsec = abs(totalSecs - overheadSecs) * double(MSEC_PER_SEC);
    results.N = N;
    results.cond = cond;
}

//
// Function: getSortTime
// Usage: elapsedSeconds = getSortTime(&inputData);
//        elapsedSeconds = getSortTime(&inputData, GET_OVERHEAD);
// --------------------------------------------------------------
// Returns the elapsed time in seconds for sorting an input vector.
//
// Can be configured to return the overhead associated with with
// running the simulation over multiple repetitions.
//

double getSortTime(Vector<int> & data, void (sortfn)(Vector<int> &), SimulationT simType) {
    int repeat = getSimulationReps(data.size());
    int repeatSave = repeat;
    Vector<int> mutableData;
    clock_t t0, tN;
    
    switch (simType) {
        case GET_OVERHEAD: {
                t0 = clock();
                while (repeat > 0) {
                    --repeat;
                    mutableData = data;
                }
                tN = clock();
            }
            break;
            
        case PERFORM_SORT:
        default: {
                t0 = clock();
                while (repeat > 0) {
                    --repeat;
                    mutableData = data;
                    sortfn(mutableData);
                }
                tN = clock();
                data = mutableData; // Pass sorted data back to the caller.
            }
            break;
    }
    double elapsedCpuClocks = double(tN - t0)/repeatSave;
    return elapsedCpuClocks/double(CLOCKS_PER_SEC);
}

//
// Function: getSimulationReps
// Usage: int numRepetitions = getSimulationReps(inputdata.size());
// ----------------------------------------------------------------
// Returns a repetition count for looping through multiple runs of identical
// but short-lived processing steps.
//
// Handy for benchmarking under small input data conditions.
//
// When the constant MAX_REPS = 10000, this routine returns the following
// repetitions for the following values of N:
//
//      N =     10 reps = 1000
//      N =     50 reps = 200
//      N =    100 reps = 100
//      N =    500 reps = 20
//      N =   1000 reps = 10
//      N =   5000 reps = 2
//      N =  10000 reps = 1
//      N =  50000 reps = 1
//      N = 100000 reps = 1
//
// For large data sets, we don't need multiple simulation runs, so the
// repetition value steps down accordingly.
//

int getSimulationReps(int N) {
    int reps = 1;
    
    if ((N > 0) && (N < MAX_REPS)) reps = MAX_REPS / N;
    return reps;
}

//
// Function: getInputData
// Usage: getInputData(&data, N);
//        getInputData(&data, N, DESCENDING);
// ------------------------------------------
// Populates a pass-by-reference vector with N integers generated
// under client-specified conditions.
//
// By default, a vector of random integers ranging in value
// from 0 to N is returned.
//
// Under ASCENDING conditions, a sorted-ascending order vector is returned.
// Under ASCENDISH conditions, a mostly sorted (99%) vector is returned.
// Under DESCENDING conditions, a sorted-descending order vector is returned.
//

void getInputData(Vector<int> & items, int N, ConditionT condition) {
    items.clear();
    switch (condition) {
        case ASCENDING: // pre-sorted
            for (int n = 0; n < N; n++) items.add(n);
            break;
        
        case ASCENDISH: // mostly sorted
            for (int n = 0; n < N; n++) {
                if (randomChance(0.94)) {
                    items.add(n);
                } else {
                    items.add(randomInteger(n, n + 0.03 * (N-1)));
                }
            }
            break;
            
        case DESCENDING: // reverse sorted
            for (int n = N-1; n >= 0; n--) items.add(n);
            break;
            
        case RANDOMIZE: // random data in random positions
        default:
            for (int n = 0; n < N; n++) items.add(randomInteger(0, N-1));
            break;
    }
}

//
// Function: sort
// Usage: sort(&data);
// -------------------
// Sorts a pass-by-reference vector of integers using Insertion Sort.
//
// The algorithm partitions the input vector into sorted and unsorted
// regions.
//
// Initially, only the 0th element is considered sorted.
//
// As the algorithm proceeds, the sorted region at the head of the vector grows
// while the unsorted tail becomes vanishingly small.
//
// An outter loop passes the nearest unsorted item to an inner loop
// which flip-flops that item into position within the sorted
// region.
//
// With each iteration of the outter loop, the sorted region grows by one
// while the unsorted region deminishes by one.
//
// Graphically, the agorithm looks like this:
//
//      [ 56 | 25 37 58 95 19 73 30 ] Initial conditions.
//          \_ sorted
//
//      [ 25 56 | 37 58 95 19 73 30 ] 1st pass
//
//      [ 25 37 56 | 58 95 19 73 30 ] 2nd pass
//
// Complexity: O(N^2) worst case
// -----------------------------
// The outter loop is linearly sensitive to the size of the input vector.
// The inner loop is similarly sensitive in the worst case of reverse ordered
// input data since unsorted values at the end of the vector are repositioned
// to the head of the vector with O(n) operations.  This yields and overall
// worst case complexity bounded by O(N^2).
//
// TODO: Optimization
//
// As the sorted region becomes large, one could employ binary search to
// to find the insertion point therein, yielding O(logN) behavior for that
// portion of the algorithm over the O(n) flip-flop positioning strategy.
//

void insertionSort(Vector<int> & items) {
    for (int u = 1; u < items.size(); u++) {
        int k = items[u];
        int s = u - 1;
        while (s >= 0 && items[s] > k) {
            items[s+1] = items[s];
            items[s--] = k;
        }
    }
}

//
// Function: toString
// Usage: cout << toString(numVector, partionIndex);
// -------------------------------------------------
// Display a vector of integers with a vertical partition separating the
// sorted portion of the vector from the unsorted.
//

std::string toString(Vector<int> & data, int s) {
    ostringstream oss;
    
    oss << "[";
    for (int i = 0; i < data.size(); i++) {
        oss << setw(3) << data[i] << " ";
        if (i == s) oss << "|";
    }
    oss << "]";
    return oss.str();
}

//
// Operator: <<
// Usage: cout << report << endl;
// ------------------------------
// Displays formatted results from multiple runs of a sort function under
// conditions of increasing scale.  Input data condition is also reflected.
//

ostream & operator<<(ostream & os, const Report & report) {
    os << "    N    |  Time (msec)  | Input Data" << endl;
    os << "---------+---------------+-----------------" << endl;
    for (ResultT rec : report) {
        os << right;
        os << setw(7)  << rec.N << "  |";
        os << setw(14) << fixed << setprecision(5) << rec.timeMsec;
        os << " | " << setw(14) << left << rec.cond  << endl;
    }
    return os;
}

//
// Operator: <<
// Usage: cout << inputConditions << endl;
// ---------------------------------------
// Displays the string equivalents for the enumerated ConditionT type
// which characterizes the input data.
//

ostream & operator<<(ostream & os, ConditionT cond) {
    switch (cond) {
        case ASCENDING:
            os << "sorted ascending";
            break;
            
        case ASCENDISH:
            os << "mostly ascending";
            break;
            
        case DESCENDING:
            os << "sorted descending";
            break;
            
        case RANDOMIZE:
        default:
            os << "randomized";
            break;
    }
    return os;
}

//
// Operator: ++ (ConditionT post-increment)
// Usage: for (ConditionT cond = ASCENDING; cond <= DESCENDING; cond++) { . . . }
// --------------------------------------------------------------------
// Defines a post-increment operator for the input data conditioning type,
// ConditionT.
//

ConditionT operator++(ConditionT & cond, int) {
    ConditionT old = cond;
    if (cond < LAST_CONDITION) cond = ConditionT(cond + 1);
    return old;
}