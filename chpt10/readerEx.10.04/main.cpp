//
// main.cpp
//
// This program implements an O(N) algorithm for sorting an integer array
// where all the values fall within the range 0 to 9999.
//
// Suppose you know that all the values in an integer array fall into the
// range 0 to 9999. Show that it is possible to write a O(N) algorithm to sort
// arrays with this restriction. Implement your algorithm and evaluate its
// performance by taking empirical measurements using the strategy outlined in
// exercise 3. Explain why the performance of the algorithm is so bad for
// small values of N.
//
// Notes
//
// My first attempt at mutating the input data to create a sort index
// resulted in a mess.  Then I backed up and created a linear-ish
// sort using the map abstract data type, solving the more general
// case of dynamic data range on the input though missing the linear
// performance requirement for large N.  I then realized the constraint of
// a fixed data range in the problem statement implied I should use a fixed
// length frequency vector in lieu of the (less performant) map.  So
// now I'm getting the desired linear behavior for large N.
//
// For fun, I added a '#define COMPARE_SORTS' to trigger a comparison of
// runtimes across my two new sorts (linearSort and mapSort) and my insertion
// sort from an earlier problem.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 10, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 5/3/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <ostream>
#include <ctime>
#include <cmath>
#include "vector.h"
#include "random.h"
#include "map.h"
#include "error.h"
using namespace std;

//#define DEBUGGING     // Uncomment this for more diagnostic info.
#define ENABLE_TRIALS   // Improve accuracy of runtime calc for small data sets.
#define COMPARE_SORTS   // Integrate results from multiple sort functions.

#ifdef DEBUGGING        // Instrument debug output on console.
#define SHOW_UNSORTED   // Sanity check the data going into the sort fn.
#define SHOW_SORTED     // Sanity check the data coming from the sort fn.
#undef ENABLE_TRIALS    // Disable repeated trials when debugging.
#endif

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 10.04\n";
#ifdef COMPARE_SORTS
const std::string DETAIL = "Multi-sort runtime comparison for fixed range input: 0 - 9999";
#else
const std::string DETAIL = "O(N-ish) sort of integers of known range: 0 - 9999";
#endif
const std::string BANNER = HEADER + DETAIL;

const int MIN_DATA_VAL = 0;         // Limits the minimum value in input data.
const int MAX_DATA_VAL = 9999;      // Limits the maximum value input data.

#if defined(SHOW_UNSORTED) || defined(SHOW_SORTED)
const int SMALLISH_DATASET = 25;    // In debug mode, these dump to the console.
#endif
#ifdef ENABLE_TRIALS
const int MAX_REPS     = 10000;     // Number of trials for data set with N = 1.
#endif
const int MSEC_PER_SEC = 1000;      // Convert from seconds to milliseconds.
const double PERCENT_SORTED = 0.94; // Percentage of data in sorted order when
                                    // creating mostly-sorted input data sets.
const double PERCENT_DELTA  = 0.03; // Variability relative to MAX_DATA_VAL for
                                    // unsorted values in mostly-sorted data.
const int MAX_NUM_UNIQUE = MAX_DATA_VAL - MIN_DATA_VAL + 1;

// Types

enum SimulationT {
    GET_OVERHEAD,   // Don't run the sort.  Just calculate simulation overhead.
    PERFORM_SORT    // Perform the sort (includes simulation overhead).
};

enum ConditionT {
    ASCENDING,        // Input data already sorted.
    ASCENDISH,        // Input data mostly sorted in ascending order.
    RANDOMIZE,        // Input data randomized.
    DESCENDING,       // Input data reverse sorted.
    LAST_CONDITION,   // Terminate post increment iterator.
    SKIPPING,         // Unable generate data under current constraints.
};

typedef void(*pSortFn)(Vector<int>&);

struct ResultT {
    int N;           // Size of input data.
    double timeMsec; // Elasped time to sort input data (factors out overhead).
    ConditionT cond; // Degree of randomness in generated input data.
    pSortFn fn;      // Function used to sort data.
};

typedef Vector<ResultT> Report; // Aggregates results for various sizes of input


// Prototypes

void insertionSort(Vector<int> & data);
void mapSort(Vector<int> & data);
void linearSort(Vector<int> & data);
ostream & operator<<(ostream & os, const Report & report);
ConditionT operator++(ConditionT & cond, int);
int getNumTrials(int N);
void getInputData(Vector<int> & items, const int N, const ConditionT condition = RANDOMIZE);
void runSort(const int N, ResultT & result, const ConditionT cond,
             void (sortfn)(Vector<int> & data));
double getSortTime(Vector<int> & data, void (sortfn)(Vector<int> & data),
                   SimulationT simType = PERFORM_SORT);
ostream & operator<<(ostream & os, const ConditionT cond);
ostream & operator<<(ostream & os, pSortFn fn);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    Vector<int> N;  // Predefined input data sizes.
    N += 10, 50, 100, 500, 1000, 5000, 10000;
    
    Vector<pSortFn> sortFunctions;
#ifdef COMPARE_SORTS
    sortFunctions += linearSort, mapSort, insertionSort;
#else
    sortFunctions += linearSort;
#endif
    
    for (ConditionT cond = ASCENDING; cond <= DESCENDING; cond++) {
        Report report;
        for (int n: N) {
            ResultT result;
            
            // Run sorting function against n-length vector of generated data.
            //
            // For small data sets00, this may entail repeating the sort
            // and computing average elapsed time and correcting for overhead.
            
            for (pSortFn sortFunction : sortFunctions) {
                runSort(n, result, cond, sortFunction);
                report.add(result);
            }
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

void runSort(const int N, ResultT & result, const ConditionT cond,
             void (sortfn)(Vector<int> &)) {
    
    if (N > MAX_NUM_UNIQUE && cond != RANDOMIZE) {
        
        // Punt if constraints on min and max data value prevent us
        // from populating the input vector with the desired number
        // of N values.  This run will be marked as 'skipped' in the ouput
        // report.  For example, can't fill a vector of 10,000 numbers
        // in ascending order if allowable range is 0 - 999.
        
        result.cond = SKIPPING;
        result.N = N;
        result.timeMsec = 0.0;
        result.fn = sortfn;
        return;
    }
    
    Vector<int> data;
    getInputData(data, N, cond);
    
#ifdef SHOW_UNSORTED
    if (data.size() <= SMALLISH_DATASET) {
        cout << "unsorted: " << data << endl;
    }
#endif
    
    // Perform the sort and return elapsed time.
    double totalSecs = getSortTime(data, sortfn, PERFORM_SORT);
    
#ifdef SHOW_SORTED
    if (data.size() <= SMALLISH_DATASET) {
        cout << "sorted:   " << data << endl;
    }
#endif
    
    // Correct for overhead incurred.
    double overheadSecs = getSortTime(data, sortfn, GET_OVERHEAD);
    result.timeMsec = (totalSecs - overheadSecs) * double(MSEC_PER_SEC);
    result.N = data.size();
    result.cond = cond;
    result.fn = sortfn;
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
    int repeat = getNumTrials(data.size());
    int repeatSave = repeat;
    clock_t t0, tN;
    
    Vector<int> mutatedData;
    switch (simType) {
        case GET_OVERHEAD: {
                t0 = clock();   // cpu cycles since process started.
                while (repeat > 0) {
                    --repeat;
                    mutatedData.clear();
                    mutatedData += data;
                }
                tN = clock();   // cpu cycles since process started.
            }
            break;
            
        case PERFORM_SORT:
        default: {
                t0 = clock();
                while (repeat > 0) {
                    --repeat;
                    mutatedData.clear();
                    mutatedData += data;
                    sortfn(mutatedData);
                }
                tN = clock();
                data.clear();
                data += mutatedData;
            }
            break;
    }
    if (tN < t0) {
        // Debugging stack corruption makes you do crazy things.
        error("getSortTime: clock() error.  Negative elapsed time. :-/");
    }
    double elapsedCpuClocks = double(tN - t0) / repeatSave;
    return elapsedCpuClocks / double(CLOCKS_PER_SEC);
}

//
// Function: getNumTrials
// Usage: int numRepetitions = getNumTrials(inputdata.size());
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

int getNumTrials(int N) {
    int numTrials = 1;
    
#ifdef ENABLE_TRIALS
    if ((N > 0) && (N < MAX_REPS)) numTrials = MAX_REPS / N;
#endif
    
    return numTrials;
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
// from MIN_DATA_VAL to MAX_DATA_VAL is returned.
//
// Under ASCENDING conditions, a sorted-ascending order vector is returned.
// Under ASCENDISH conditions, a mostly sorted vector is returned.
// Under DESCENDING conditions, a sorted-descending order vector is returned.
//

void getInputData(Vector<int> & items, const int N, const ConditionT condition){
    items.clear();
    
    switch (condition) {
        case ASCENDING: // pre-sorted
            for (int n = MIN_DATA_VAL; n <= min(N-1, MAX_DATA_VAL); n++)
                items.add(n);
            break;
        
        case ASCENDISH: // mostly sorted
            for (int n = MIN_DATA_VAL; n <= min(N-1, MAX_DATA_VAL); n++) {
                if (randomChance(PERCENT_SORTED)) {
                    items.add(n);
                } else {
                    int n_maxrand = n + (PERCENT_DELTA) * (MAX_DATA_VAL);
                    int n_max = (n_maxrand <= MAX_DATA_VAL) ?
                                 n_maxrand : MAX_DATA_VAL;
                    items.add(randomInteger(n, n_max));
                }
            }
            break;
            
        case DESCENDING: // reverse sorted
            for (int n = min(N-1, MAX_DATA_VAL); n >= max(0, MIN_DATA_VAL); n--){
                items.add(n);
            }
            break;
            
        case RANDOMIZE: // random data in random positions within a range
        default:
            for (int n = 0; n < N; n++)
                items.add(randomInteger(MIN_DATA_VAL, MAX_DATA_VAL));
            break;
    }
#ifdef DEBUGGING
    cout << "getInputData() size = " << items.size() << endl;
#endif
}

// Function: linearSort
// Usage: linearSort(&data);
// -------------------------
// Returns a sorted collection of integers in a pass-by-reference vector.
//
// Iterate over the input data, using the values as indices into a
// (potentially sparse) frequency-of-occurrence vector.
// This sorts the data while accounting for duplicate values on the input.
//
// Repopulate the input vector with sorted data by rediscovering that data
// in order within the frequency vector, using an inner loop to
// output the correct number of duplicates of a given value.
//
// Complexity Analysis
// -------------------
// Running time is ~ t(2N), or O(N)
//      Populating the frequency-of-occurrence vector takes runtime of t(N).
//      Iterating over the intrinsically sorted frequency vector and
//      overwriting the input vector with sorted data also takes t(N).
//
// Memory usage is O(C), where C is expected range in value across the input.
//
// Performance is generally linear for various N-sized data sets.
// However, per-item overhead increases for relatively small N since
// the frequency vector becomes correspondingly sparse but still requires
// full traversal at any N-sized input.
//
// The capacity of the frequency vector is a function of the expected range in
// values on the input (as opposed to the the number of items in the input).

void linearSort(Vector<int> & items) {
    Vector<int> freq(MAX_NUM_UNIQUE, 0);
    
    for (int item: items) {
        
        // Guard against indexing beyond the bounds of the frequency
        // vector if some out-of-range input data shows up.
        // This should probably be an exception once I learn about those. :P
        
        if (item >= MAX_NUM_UNIQUE) {
            ostringstream oss;
            oss << "(" << item << " >= " << MAX_NUM_UNIQUE << ")";
            error("linearSort(): item >= MAX_NUM_UNIQUE " + oss.str());
        }
        freq[item] = freq[item] + 1;
    };

    int si = 0;
    for (int i = 0; i < MAX_NUM_UNIQUE; i++) {
        for (int f = 0; f < freq[i]; f++) {
            // Overwrite input vector with sorted data.
            items[si++] = i;
        }
    }
}

//
// Function: mapSort
// Usage: mapSort(&data);
// ----------------------
// My stab at an O(n) sort which returns a collection of sorted integers in a
// pass-by-reference vector.
//
// (I'm probably cheating by using the map data type. :P
// My earlier attempt to use the input data itself to directly calculate a
// proximate sorted index devolved to a freakish mess.)
//
// The current algorithm iterates over the unsorted input, building a map
// of unique data values and their frequency of occurance.
//
// A subsequent iteration of the map overwrites the unsorted input vector
// with sorted items (including any duplicates items).
//
// Complexity Analysis
// -------------------
// If map populating a map is O(N), then building up the frequency
// map is also O(N).
//
// The map traversal is implemented as a double for-loop which, upon
// cursory analysis, might suggest O(N^2) operations.  However
// the inner loop merely iterates over the constant number of duplicates
// of a given input value.  The total number of operations across
// the two loops is still O(N) (um, assuming map traversal is O(N)).
//
// This yields ~ 2N operations which would still be O(N).
//

void mapSort(Vector<int> & items) {
    Map<int, int> freq; // Map item value to item frequency in the input data.
    
    for (int item : items) {
        freq[item] += 1;
    }
    
    // Rely upon map iterator to return items in sorted order.
    // Use frequency count to drive inner loop.
    
    int si = 0;
    for (int sortedItem : freq) {
        for (int i = 0; i < freq[sortedItem]; ++i) {
            items[si++] = sortedItem;
        }
    }
}

//
// Function: insertionSort
// Usage: insertionSort(&data);
// ----------------------------
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
//       As the sorted region becomes large, one could employ binary search to
//       to find the insertion point therein, yielding O(logN) behavior for that
//       portion of the algorithm over the O(n) flip-flop positioning strategy.
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
// Operator: <<
// Usage: cout << report << endl;
// ------------------------------
// Displays formatted results from multiple runs of a sort function under
// conditions of increasing scale.  Input data condition is also reflected.
//

ostream & operator<<(ostream & os, const Report & report) {
    int prevN = report[0].N;
#ifdef COMPARE_SORTS
    os << "    N    |  Time (msec)  | Input Data         |  Sort Function" << endl;
    os << "---------+---------------+--------------------+-------------------" << endl;
#else
    os << "    N    |  Time (msec)  | Input Data         " << endl;
    os << "---------+---------------+--------------------" << endl;
#endif
    for (ResultT rec : report) {
#ifdef COMPARE_SORTS
        if (prevN != rec.N) {
            prevN = rec.N;
            os << "---------+---------------+--------------------+-------------------" << endl;
        }
#endif
        os << right;
        os << setw(7)  << rec.N << "  |";
        os << setw(14) << fixed << setprecision(5) << rec.timeMsec;
        os << " | " << setw(18) << left << rec.cond;
#ifdef COMPARE_SORTS
        os << " |  " << setw(14) << left << rec.fn;
#endif
        os << endl;
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

ostream & operator<<(ostream & os, const ConditionT cond) {
    switch (cond) {
        case ASCENDING:
            os << "ascending";
            break;
            
        case ASCENDISH:
            os << "mostly ascending";
            break;
            
        case DESCENDING:
            os << "descending";
            break;
            
        case SKIPPING:
            os << "skipping";
            break;
            
        case RANDOMIZE:
        default:
            os << "random";
            break;
    }
    return os;
}

//
// Operator: <<
// Usage: cout << sortfn << endl;
// ------------------------------
// Displays the string equivalents for the specified sort function.
//

ostream & operator<<(ostream & os, pSortFn fn) {
    if (fn == linearSort) {
        os << "linearSort";
    } else if (fn == mapSort) {
        os << "mapSort";
    } else if (fn == insertionSort) {
        os << "insertionSort";
    } else {
        os << "unknown";
    }
    return os;
}

//
// Operator: ++ (ConditionT post-increment)
// Usage: for (ConditionT cond = ASCENDING; cond <= DESCENDING; cond++) {...}
// --------------------------------------------------------------------------
// Defines a post-increment operator for the input data conditioning type,
// ConditionT.
//

ConditionT operator++(ConditionT & cond, int) {
    ConditionT old = cond;
    if (cond < LAST_CONDITION) cond = ConditionT(cond + 1);
    return old;
}