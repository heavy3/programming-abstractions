//
// main.cpp
//
// This program modifies the Quicksort algorithm so, instead of picking
// the first element as the pivot, to chosing the median of the first,
// middle, and last elements as the pivot.
//
// Notes
//
// The median pivot strategy addresses the quadratic penalty seen with simpler
// pivots against pre-sorted ascending data while keeping the overall
// performance comparable for random input.  However, it doesn't seem to be a
// silver bullet for all cases.  Pre-sorted descending input still
// exhibits quardratic runtime cost at scale, though even here, I'm seeing
// a 4x improvement relative to a naive '1st-value' pivot scheme.
//
// When I'm debugging performance issues, I end up temporarily
// adding a bunch of inline counters that I later rip out once I get my
// answers.  It'd be nice if there were a clean way to leave this is in without
// a bunch of #ifdefs running around.  I did play with some of the Xcode
// profiling, though I don't yet have the fu to whisper it into replacing my
// clunky but useful profiling counters.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 10, Exercise 6
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// The QuickSort algorithm is the work of Sir Charles Antony Richard Hoare.
// The implementation here comes directly from the course reader.
//
// I've augmented the partition routine to take a pivot function as a 
// parameter so various strategies can be explored, the median-of-three 
// pivot among these.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 5/10/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
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

#ifdef DEBUGGING        // Instrument debug output on console.
#define SHOW_UNSORTED   // Sanity check the data going into the sort fn.
#define SHOW_SORTED     // Sanity check the data coming from the sort fn.
#endif

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 10.06\n";
const std::string DETAIL = "Quicksort with basic and median pivots";
const std::string BANNER = HEADER + DETAIL;

const int MIN_DATA_VAL = 0;         // Limits the minimum value in input data.
const int MAX_DATA_VAL = 9999;      // Limits the maximum value input data.

#if defined(SHOW_UNSORTED) || defined(SHOW_SORTED)
const int SMALLISH_DATASET = 50;    // In debug mode, these dump to the console.
#endif
const int NUM_TRIALS = 10;          // Number of trials to run with each input.
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

enum InputCondT {
    ASCENDING,        // Input data already sorted.
    ASCENDISH,        // Input data mostly sorted in ascending order.
    RANDOMIZE,        // Input data randomized.
    DESCENDING,       // Input data reverse sorted.
    LAST_CONDITION,   // Terminate post increment iterator.
    SKIPPING,         // Unable generate data under current constraints.
};

typedef void(*pSortFn)(Vector<int>&);
typedef int(*pPivotFn)(Vector<int>&, int, int);

struct ResultT {
    int N;           // Size of input data.
    double msecQS;   // Elapsed time to sort input with quickSort
    double msecQSMP; // Elapsed time to sort input with quickSort + medianPivot
    InputCondT cond; // Degree of randomness in generated input data.
    pSortFn fn;      // Function used to sort data.
    int numTrials;   // Number of trials conducted.
};

typedef Vector<ResultT> Report; // Aggregates results for various sizes of input

// Prototypes

int fastPivot(Vector<int> & data, const int p1, const int p2);
int medianPivot(Vector<int> & data, const int p1, const int p2);
int partition(Vector<int> & data, const int p1, const int p2,
              const pPivotFn = fastPivot);
void quickSort(Vector<int> & data);
void quickSortMP(Vector<int> & data);
void quickSortR(Vector<int> & data, const int p1, const int p2,
                const pPivotFn = fastPivot);
ostream & operator<<(ostream & os, const Report & report);
InputCondT operator++(InputCondT & cond, int);
void getInputData(Vector<int> & items, const int N,
                  const InputCondT cond = RANDOMIZE);
void runSort(const int N, ResultT & results, const InputCondT cond,
             const pSortFn sortfn);
double getSortTime(Vector<int> & data, const pSortFn sortfn,
                   const SimulationT simType = PERFORM_SORT);
ostream & operator<<(ostream & os, const InputCondT cond);
ostream & operator<<(ostream & os, const pSortFn sortfn);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    Vector<int> N;  // Predefined input data sizes.
    N += 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000;
    
    Vector<pSortFn> sortFunctions;

    sortFunctions += quickSort, quickSortMP;
    for (InputCondT cond = ASCENDING; cond <= DESCENDING; cond++) {
        Report report;
        for (int n: N) {
            ResultT result;
            
            // Run sorting function against n-length vector of generated data.
            //
            // For small data sets00, this may entail repeating the sort
            // and computing average elapsed time and correcting for overhead.
            
            for (pSortFn sortFunction : sortFunctions) {
                runSort(n, result, cond, sortFunction);
            }
            if (result.cond != SKIPPING) report.add(result);
        }
        cout << report << endl;
    }
    return 0;
}

// Function: quickSort, quickSortMP
// Usage: quickSort(&data);
//        quickSortMP(&data);
//
// Attribution: Programming Abstractions in C++ by Eric Roberts
// ------------------------------------------------------------
// Returns a sorted collection of integers in a pass-by-reference vector.
// This is a wrapper for the recursive quickSortR function.
//
// quickSort   uses the first item at the low index as the pivot
// quickSortMP uses median of the first, middle, & high index values as a pivot
//

void quickSort(Vector<int> & data) {
    quickSortR(data, 0, data.size() - 1);
}

void quickSortMP(Vector<int> & data) {
    quickSortR(data, 0, data.size() - 1, medianPivot);
}

// Function: quickSortR
// Usage: quickSortR(&data, firstIndex, lastIndex);
//        quickSortR(&data, firstIndex, lastIndex, pivotFunction);
// Attribution: Programming Abstraction in C++ by Eric Roberts
// ----------------------------------------------------------------
// Returns a sorted collection of integers in a pass-by-reference vector.
//
// Employs Tony Hoare's classic algorithm for sorting elements by first
// partitioning the input into two rougly sorted groups of smaller and larger
// items separated by a single sorted value called the pivot.
//
// These groups are themselves subdivided into smaller groups with their own
// sorted pivot values until the entire collection is sorted.  Two indices
// mark the extent of the input vector under consideration the current level
// of recursion.
//
// If one had a priori knowledge of the input data, you would pick the
// median value as your pivot to get the best performance from this algorithm.
// As it is, the pivot selection defaults to the item pointed to
// by the first managed index.
//
// An optional pivot function parameter may be passed in for alternate pivot
// selection strategies.
//

void quickSortR(Vector<int> & data, const int p1, const int p2,
                const pPivotFn pivotFn) {
    
    if (p1 >= p2) return;
    int boundary = partition(data, p1, p2, pivotFn);
    quickSortR(data, p1, boundary - 1, pivotFn);
    quickSortR(data, boundary + 1, p2, pivotFn);
}

// Function: partition
// Usage: int boundary = partition(&data, p1, p2)
//        int boundary = partition(&data, p1, p2, pivotFn);
// Attribution: Programming Abstrations in C++ by Eric Roberts
// -----------------------------------------------------------
// This functions returns a partially sorted region of a vector
// of integers.  A pivot value is selected within the region bounded
// by two vector indices.  Then lefthand and righthand indices advance
// toward each other, swapping data items that are smaller or
// larger than the pivot value.  Once the two indices meet, the pivot
// value is positioned
//

int partition(Vector<int> & data, const int p1, const int p2,
              const pPivotFn pivotFn) {
    
    int pivot = pivotFn(data, p1, p2);
    int lh = p1 + 1;
    int rh = p2;
    while (true) {
        while (lh < rh && data[rh] >= pivot) rh--;
        while (lh < rh && data[lh] < pivot) lh++;
        if (lh == rh) break;
        int tmp = data[lh];     // NB: This section is heavily exercised when
        data[lh] = data[rh];    //     input data are presented in descending
        data[rh] = tmp;         //     order.
    }
    if (data[lh] >= pivot) return p1;
    data[p1] = data[lh];
    data[lh] = pivot;
    return lh;
}

// Function: partitionTR
// Usage: int boundary = partitionTR(&data, l, r, pivotFn);
// Attribution: Quick Sort YouTube by Tim Roughgarden
//              https://www.youtube.com/watch?v=6UHCG64tHgo
// --------------------------------------------------------
// Parititions a vector of integers about a pivot point determined
// by a function specified though a parameter.
//
// I coded this up off of Tim's interesting youtube lecture to explore
// how it would vary with regard to the descending input data case I'm find
// vexing.  Since the gist of the video is instructional ease (as opposed
// to optimization), it's not surprising the performance is only comparable
// (if a tad slower) than the implementation presented in Robert's text.

int partitionTR(Vector<int> & A, int l, int r, pPivotFn pivotFn) {
    int p = pivotFn(A, l, r);
    int i = l + 1;
    int tmp;
    for (int j = i; j <= r; j++) {
        if (A[j] < p) {
            tmp = A[i];
            A[i] = A[j];
            A[j] = tmp;
            ++i;
        }
    }
    // Swap pivot value into position.
    tmp = A[i-1];
    A[i-1] = A[l];
    A[l] = tmp;
    return i-1;
}

// Function: pivot
// Usage: int pivot = fastPivot(&data, p1, p2);
//        int pivot = medianPivot(&data, p1, p2);
// -----------------------------------------------
// Mutates the data vector so the pivot value resides in data[p1]
// and returns that as the function value.
//
// fastPivot uses data[p1] as the pivot value.
//
// medianPivot takes the median of the fist, middle, and last
// values relative to the p1 and p2 indices.
//

int fastPivot(Vector<int> & data, const int p1, const int p2) {
    return data[p1];
}

// Function: medianPivot
// Usage: int boundary = medianPivot(&data, p1, p2);
// -------------------------------------------------
// Returns a pivot point which is the median of the left, middle, and right
// terms of an input vector.
//

int medianPivot(Vector<int> & data, const int p1, const int p2) {
    int medIndex;
    int mid = (p1 + p2) / 2;
    
    // Find median of three values: data[p1], data[mid], versus data[p2]
    // 6 cases to consider.
    
    if (data[p1] < data[mid]) {
        if (data[mid] < data[p2]) {
            medIndex = mid;
        } else if (data[p1] < data[p2]) {
            medIndex = p2;
        } else {
            medIndex = p1;
        }
    } else {
        if (data[p1] < data[p2]) {
            medIndex = p1;
        } else if (data[mid] < data[p2]) {
            medIndex = p2;
        } else {
            medIndex = mid;
        }
    }
    
    // Caller expects pivot value at data[p1].
    // Swap it into position.
    
    if (medIndex != p1) {
        int tmp = data[p1];
        data[p1] = data[medIndex];
        data[medIndex] = tmp;
    }
    return data[p1];
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

void runSort(const int N, ResultT & result, const InputCondT cond,
             const pSortFn sortfn) {
    
    if (N > MAX_NUM_UNIQUE && cond != RANDOMIZE) {
        
        // Punt if constraints on min and max data value prevent us
        // from populating the input vector with the desired number
        // of N values.  This run will be marked as 'skipped' in the ouput
        // report.  For example, can't fill a vector of 10,000 numbers
        // in ascending order if allowable range is 0 - 999.
        
        result.cond = SKIPPING;
        result.N = N;
        if (sortfn == quickSort) {
            result.msecQS = 0;
        } else if (sortfn == quickSortMP) {
            result.msecQSMP = 0;
        }
        result.fn = sortfn;
        result.numTrials = 0;
        return;
    }
    
    Vector<int> data;
    getInputData(data, N, cond);
    
    #ifdef SHOW_UNSORTED
    if (data.size() <= SMALLISH_DATASET) {
        cout << "unsorted: " << data << endl;
    }
    #endif
    
    // ------------------------------------------ //
    // Perform the sort and return elapsed time.  //
    // ------------------------------------------ //
    
    double totalSecs = getSortTime(data, sortfn, PERFORM_SORT);
    
    #ifdef SHOW_SORTED
    if (data.size() <= SMALLISH_DATASET) {
        cout << "sorted:   " << data << endl;
    }
    #endif
    
    // Correct for overhead incurred.
    
    double overheadSecs = getSortTime(data, sortfn, GET_OVERHEAD);
    if (overheadSecs > totalSecs) {
        // Guard against negative elapsed time especially for small datasets
        // where either clock precision is an issue or runtime variability is
        // on the order of the total sort time (even with repeated trials).
        overheadSecs = 0;
    }
    
    double msecs = (totalSecs - overheadSecs) * double(MSEC_PER_SEC);
    
    if (sortfn == quickSort) {
        result.msecQS = msecs;
    } else if (sortfn == quickSortMP) {
        result.msecQSMP = msecs;
    }
    
    result.N = data.size();
    result.cond = cond;
    result.fn = sortfn;
    result.numTrials = NUM_TRIALS;
}

//
// Function: getSortTime
// Usage: elapsedSeconds = getSortTime(&inputData, quickSort);
//        elapsedSeconds = getSortTime(&inputData, quickSortMP, GET_OVERHEAD);
// ---------------------------------------------------------------------------
// Returns the elapsed time in seconds for sorting an input vector.
//
// Can be configured to return the overhead associated with with
// running the simulation over multiple repetitions.
//
// This function is influenced by the constant, NUM_TRIALS, which controls
// how many repetitions of a sort to make.  Having NUM_TRIALS > 1 improves
// the metric calculations for small datasets.
//

double getSortTime(Vector<int> & data, const pSortFn sortfn, const SimulationT simType) {
    int repeat = NUM_TRIALS;
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
                    // ================ //
                    // Perform the sort //
                    // ================ //
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
    double elapsedCpuClocks = double(tN - t0) / NUM_TRIALS;
    return elapsedCpuClocks / double(CLOCKS_PER_SEC);
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

void getInputData(Vector<int> & items, const int N, const InputCondT cond){
    items.clear();
    
    switch (cond) {
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
}

//
// Operator: <<
// Usage: cout << report << endl;
// ------------------------------
// Displays formatted results from multiple runs of a sort function under
// conditions of increasing scale.  Input data condition is also reflected.
//

ostream & operator<<(ostream & os, const Report & report) {
    os << "Input data are in " << report[0].cond << " order." << endl;
    os << "Random trials per N-sized dataset: " << report[0].numTrials;
    os << endl << endl;
    os << "           |    Sort Time (msec)      " << endl;
    os << "-----------+--------------------------" << endl;
    os << " Data Size | QuickSort | QuickSort w/ " << endl;
    os << "     N     |           | median pivot " << endl;
    os << "-----------+-----------+--------------" << endl;
    
    for (ResultT rec : report) {
        os << right;
        os << setw(8) << rec.N << "   |";
        os << setw(9) << fixed << setprecision(4) << rec.msecQS << "  | ";
        os << setw(9) << fixed << setprecision(4) << rec.msecQSMP << endl;
    }
    return os;
}

//
// Operator: <<
// Usage: cout << inputConditions << endl;
// ---------------------------------------
// Displays the string equivalents for the enumerated InputCondT type
// which characterizes the input data.
//

ostream & operator<<(ostream & os, const InputCondT cond) {
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

ostream & operator<<(ostream & os, const pSortFn sortfn) {
    if (sortfn == quickSort) {
        os << "quickSort";
    } else if (sortfn == quickSortMP) {
        os << "quickSortMP";
    } else {
        os << "unknown";
    }
    return os;
}

//
// Operator: ++ (InputCondT post-increment)
// Usage: for (InputCondT cond = ASCENDING; cond <= DESCENDING; cond++) {...}
// --------------------------------------------------------------------------
// Defines a post-increment operator for the input data conditioning type,
// InputCondT.
//

InputCondT operator++(InputCondT & cond, int) {
    InputCondT old = cond;
    if (cond < LAST_CONDITION) cond = InputCondT(cond + 1);
    return old;
}