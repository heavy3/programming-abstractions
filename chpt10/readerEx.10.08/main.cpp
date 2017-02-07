//
// main.cpp
//
// This program implements a Quicksort that simply returns when the size
// of the input vector falls below a certain threshold. An insertion sort
// then picks up the task of sorting the elements into their final
// positions.  This works reasonably well since insertion sort runs in
// linear time on vectors that are mostly sorted.
//
// Notes
//
// At a threshold value of 20 items I'm seeing the following improvements at
// my larger datasets:
//
// Data Condition      Improvement of hybrid (qs+is) sort over quick sort
// ------------------- --------------------------------------------------
// ascending input:    35%
// random input:       15%
// descending input:   10%
//
// I'm not entirely happy with the run-to-run variability of my test
// harness with unchanged input.  I wouldn't bet the bank on these results.
// I'd need to get to an invariant baseline before fully vouching for the
// statistical confidence of these numbers.  Still, it makes sense that one
// would see the most improvement with ascending input since that is the
// O(n) sweet-spot of insertion sort.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 10, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// The QuickSort algorithm is the work of Sir Charles Antony Richard Hoare.
// The implementation here comes mostly from the course reader.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 5/11/16.
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
#define RANDOM_TRIAL    // Generate random data for each trial.
                        // Breaks ability to compare performance between
                        // two sort functions since input data are different
                        // but exposes sort to more variation on input.

#ifdef DEBUGGING        // Instrument debug output on console.
#define SHOW_UNSORTED   // Sanity check the data going into the sort fn.
#define SHOW_SORTED     // Sanity check the data coming from the sort fn.
#endif

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 10.08\n";
const std::string DETAIL = "Quicksort vs Hybrid QS + Insertion Sort.";
const std::string BANNER = HEADER + DETAIL;
const int THRESHOLD = 20;           // Use insertion sort (instead of quicksort)
                                    // when the number of elements in the input
                                    // vector is at or below this value.

const int NUM_TRIALS = 100;         // Number of trials to run with each input.


const int MIN_DATA_VAL = 0;         // Limits the minimum value in input data.
const int MAX_DATA_VAL = 9999;      // Limits the maximum value input data.

#if defined(SHOW_UNSORTED) || defined(SHOW_SORTED)
const int SMALLISH_DATASET = 50;    // In debug mode, these dump to the console.
#endif
const int MSEC_PER_SEC = 1000;      // Convert from seconds to milliseconds.
const double PERCENT_SORTED = 0.94; // Percentage of data in sorted order when
                                    // creating mostly-sorted input data sets.
const double PERCENT_DELTA  = 0.03; // Variability relative to MAX_DATA_VAL for
                                    // unsorted values in mostly-sorted data.
const int MAX_NUM_UNIQUE = MAX_DATA_VAL - MIN_DATA_VAL + 1;

// Types

enum SimulationT {
    GET_OVERHEAD,     // Don't run the sort, just calculate simulation overhead.
    PERFORM_SORT      // Perform the sort (includes simulation overhead).
};

enum InputCondT {
    ASCENDING,        // Input data already sorted.
    RANDOMIZE,        // Input data randomized.
    DESCENDING,       // Input data reverse sorted.
    LAST_CONDITION,   // Terminate post increment iterator.
    ASCENDISH,        // Input data mostly sorted in ascending order.
    SKIPPING,         // Unable generate data under current constraints.
};

typedef void(*pSortFn)(Vector<int>&);
typedef int(*pPivotFn)(Vector<int>&, int, int);

struct ResultT {
    int N;           // Size of input data.
    double msecHQS;  // Elapsed time to sort input with hybrid qs + insert sort.
    double msecQSMP; // Elapsed time to sort input with quickSort + medianPivot
    InputCondT cond; // Degree of randomness in generated input data.
    pSortFn fn;      // Function used to sort data.
    int numTrials;   // Number of trials conducted.
    int threshold;   // Quit QS when size of subvector falls below this.
};

typedef Vector<ResultT> Report; // Aggregates results for various sizes of input

// Prototypes

int fastPivot(Vector<int> & data, const int p1, const int p2);
int medianPivot(Vector<int> & data, const int p1, const int p2);
int partition(Vector<int> & data, const int p1, const int p2,
              const pPivotFn = fastPivot);
void insertionSort(Vector<int> & data);
void quickSortHybrid(Vector<int> & data);
void quickSortRHybrid(Vector<int> & data, const int p1, const int p2,
                      const pPivotFn pivotFn);
void quickSort(Vector<int> & data);
void quickSortMP(Vector<int> & data);
void quickSortR(Vector<int> & data, const int p1, const int p2,
                const pPivotFn = fastPivot);
void init(ResultT & result, const InputCondT cond, const int N);
void selectionSortN(Vector<int> & data, const int p1, const size_t n);
void selectionSortPart(Vector<int> & data, const int lh, const int rh);
ostream & operator<<(ostream & os, const Report & report);
InputCondT operator++(InputCondT & cond, int);
bool getInputData(Vector<int> & data, ResultT & result, const int N,
                  const InputCondT cond = RANDOMIZE);
void runSort(const pSortFn sortfn, Vector<int> & data, const int N,
             const InputCondT cond, ResultT & result);
double getSortTime(Vector<int> & data, const pSortFn sortfn,
                   InputCondT cond, const SimulationT simType = PERFORM_SORT);
ostream & operator<<(ostream & os, const InputCondT cond);
ostream & operator<<(ostream & os, const pSortFn sortfn);

// Main program

int main() {
    cout << BANNER << endl << endl;
    
    Vector<int> N;  // Predefined input data sizes.
    N += 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000;
    
    Vector<pSortFn> sortFunctions;

    sortFunctions += quickSortMP, quickSortHybrid;
    for (InputCondT cond = ASCENDING; cond <= DESCENDING; cond++) {
        Report report;
        for (int n: N) {
            ResultT result;
            init(result, cond, n);
            
            // Run sorting function against n-length vector of generated data.
            //
            // For small data sets00, this may entail repeating the sort
            // and computing average elapsed time and correcting for overhead.
            
            Vector<int> data;
            if (getInputData(data, result, n, cond)) {
                for (pSortFn sortFunction : sortFunctions) {
                    runSort(sortFunction, data, n, cond, result);
                }
                report.add(result);
            }
        }
        cout << report << endl;
    }
    return 0;
}

/*
 * Function: insertionSort
 * Usage: insertionSort(&data);
 * ----------------------------
 * Sorts a vector of integers that have been passed in by reference.
 *
 * Uses an Insertion Sort algorithm that iterates over the the vector
 * gradually mutating the contents into ascending order as follows:
 *
 *
 *  [ 56 | 25 | 37 | 58 | 95 | 19 | 73 | 30 ]
 *    --
 *      \_ in order
 *
 *  [ 25 | 56 | 37 | 58 | 95 | 19 | 73 | 30 ]
 *    --   --
 *           \_ in order
 *
 *  [ 25 | 37 | 56 | 58 | 95 | 19 | 73 | 30 ]
 *    --   --   --
 *                \_ in order
 *
 * Worst-case performance is O(N^2) when input is reversed sorted.
 * Base-case performance is O(N) when input is already sorted.
 *
 * This algorithm is suited to input that is partially sorted.
 *
 * Complexity Analysis
 * -------------------
 * Broadly speaking, we have a double loop structure with variable cost
 * contributed from inner loop comparisons plus a repositioning
 * vector mutation.
 *
 * Roughly, that gives a runtime of:
 *
 *  t(n) ~= (n-1) * (t_innerloop + t_mutation)
 *
 * t_innerloop  effort varies from 1 to (n-1) iterations depending upon
 *              structure of input data.
 *
 *              Worst case is repositioning final unsorted number from
 *              one end of the vector to the other (especially when data
 *              are in reversed order initially).
 *
 * t_mutation   effort to reposition a single number within the vector
 *              depends upon the underlying implementation of the data type.
 *
 *              In the best case, this would be constant time.
 *              In the worst case, it would be at most 2*(n-1) operations
 *              to remove an unsorted number and re-insert it
 *              in sorted order (especially at the beginning of the sorting
 *              activity).
 *
 * Assuming worst case for both inner loop actions, we get:
 *
 *  t(n) ~= (n-1) * (c2(n-1) + 2(n-1))
 *
 * Simplifying:
 *
 *  t(n) ~= (n-1) * (3n -1) = 3n^2 - 4n + 1 + c0
 *
 * Dropping non-dominant lower-order terms and constants, we get:
 *
 *  O(n^2)
 *
 * Interestingly, the qualitative analysis yields the same result even if
 * the vector mutation happens in constant time, since the inner loop
 * still dominates with an O(n) term.
 */

void insertionSort(Vector<int> & data) {
    for (int u = 1; u < data.size(); u++) {
        int k = data[u];
        int s = u - 1;
        while (s >= 0 && data[s] > k) {
            data[s+1] = data[s];
            data[s--] = k;
        }
    }
}

// Function: quickSortHybrid
// Usage: quickSortHybrid(&data);
// ------------------------------------------------------------
// Returns a sorted collection of integers in a pass-by-reference vector.
// This is a wrapper for the recursive quickSortRHybrid function.
//
// The idea is to use quick sort on large inputs to get the data mostly sorted.
// Then use insertion sort to finish the job since it runs in ~O(n) on
// mostly sorted data.
//
// For input vectors less than or equal in size to the threshold constant
// (see, THRESHOLD) insertion sort does the entire job.
//

void quickSortHybrid(Vector<int> & data) {
    int p1 = 0;
    int p2 = data.size() - 1;
    
    if ((p2 - p1) < THRESHOLD) {
        insertionSort(data);
    } else {
        quickSortRHybrid(data, p1, p2, medianPivot);
        insertionSort(data);
    }
}

void quickSortRHybrid(Vector<int> & data, const int p1, const int p2,
                      const pPivotFn pivotFn) {
    if ((p2 - p1) < THRESHOLD) return;
    if (p1 >= p2) return;
    int boundary = partition(data, p1, p2, pivotFn);
    quickSortRHybrid(data, p1, boundary - 1, pivotFn);
    quickSortRHybrid(data, boundary + 1, p2, pivotFn);
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
// items separated by a 'rightfully positioned' (sorted) value called the pivot.
//
// These groups are themselves subdivided into smaller groups with their own
// sorted pivot values until the entire collection consists of sorted pivot
// values.  Two indices mark the extent of the input vector under consideration
// for a given level of recursion.
//
// Performance
//
// Average runtime complexity is O(NlogN) when pivot values
// tend toward the median of the unsorted data and the input vector is then
// partitioned and evenly divided, log2(N) times.  In the recursive base case,
// the sub-vector consists of a sorted pivot singleton needing no
// further partitioning.
//
// At each level of recursion, an O(N) partition scheme determines
// the new pivot for the sub-vector and fixes it in sorted position.  The
// O(logN) subdivision cost and the O(N) partition cost at each level of
// recursion yield and overall complexity of O(NlogN) for non-denerate input.
//
// Worst-case runtime is O(N^2) and occurs when the input are pre-sorted.
// Instead of log2(N) subdivisions of the vector, you get N
// subdivisions for naive 1st-value pivots schemes.  Given that you still have
// O(N) work at each of those N subdivisions, the overall complexity is
// O(N^2).  The median-of-three pivot scheme implemented in medianPivot
// can address this issue for the ascending case and knock the complexity back
// down to O(NlogN) at scale.
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

// Function: init
// Usage: init(&result, dataConditioning, N);
// ------------------------------------------
// Returns an initialized result structure.
//

void init(ResultT & result, const InputCondT cond,
                const int N) {
    result.cond = cond;
    result.N = N;
    result.msecHQS = 0;
    result.msecQSMP = 0;
    result.numTrials = NUM_TRIALS;
    result.threshold = THRESHOLD;
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

void runSort(const pSortFn sortfn, Vector<int> & data, const int N,
             const InputCondT cond, ResultT & result) {

    // ------------------------------------------ //
    // Perform the sort and return elapsed time.  //
    // ------------------------------------------ //
    result.fn = sortfn;
    double totalSecs = getSortTime(data, sortfn, cond, PERFORM_SORT);

    // Correct for overhead incurred.
    
    double overheadSecs = getSortTime(data, sortfn, cond, GET_OVERHEAD);
    if (overheadSecs > totalSecs) {
        // Guard against negative elapsed time especially for small datasets
        // where either clock precision is an issue or runtime variability is
        // on the order of the total sort time (even with repeated trials).
        overheadSecs = 0;
    }
    double msecs = (totalSecs - overheadSecs) * double(MSEC_PER_SEC);
    if (sortfn == quickSortHybrid) {
        result.msecHQS = msecs;
    } else if (sortfn == quickSortMP) {
        result.msecQSMP = msecs;
    }
}

//
// Function: getSortTime
// Usage: elapsedSeconds = getSortTime(&inputData, quickSortHybrid);
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

double getSortTime(Vector<int> & data, const pSortFn sortfn,
                   InputCondT cond, const SimulationT simType) {
    int repeat = NUM_TRIALS;
    clock_t t0, tN;
    
    Vector<int> mutatedData;
    switch (simType) {
        case GET_OVERHEAD: {
                t0 = clock();   // cpu cycles since process started.
                while (repeat > 0) {
                    --repeat;
                    #ifdef RANDOM_TRIAL
                    ResultT dontcare;
                    if (cond == RANDOMIZE) {
                        getInputData(data, dontcare, data.size());
                    }
                    #endif
                    mutatedData.clear();
                    mutatedData += data;
                }
                tN = clock();   // cpu cycles since process started.
            }
            break;
            
        case PERFORM_SORT:
        default: {
                #ifdef SHOW_UNSORTED
                if (data.size() <= SMALLISH_DATASET) {
                    cout << "unsorted: " << data << endl;
                }
                #endif
            
                t0 = clock();
                while (repeat > 0) {
                    --repeat;
                    #ifdef RANDOM_TRIAL
                    ResultT dontcare;
                    if (cond == RANDOMIZE) {
                        getInputData(data, dontcare, data.size());
                    }
                    #endif
                    mutatedData.clear();
                    mutatedData += data;
                    // ================ //
                    // Perform the sort //
                    // ================ //
                    sortfn(mutatedData);
                }
                tN = clock();

                #ifdef SHOW_SORTED
                if (data.size() <= SMALLISH_DATASET) {
                    cout << "sorted:   " << mutatedData << endl;
                }
                #endif
            
                //data.clear();
                //data += mutatedData;
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
// Usage: getInputData(&data, &result, N);
//        getInputData(&data, &result, N, DESCENDING);
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

bool getInputData(Vector<int> & data, ResultT & result, const int N,
                  const InputCondT cond) {
    
    if (N > MAX_NUM_UNIQUE && cond != RANDOMIZE) {
        
        // Punt if constraints on min and max data value prevent us
        // from populating the input vector with the desired number
        // of N values.  This run will be marked as 'skipped' in the ouput
        // report.  For example, can't fill a vector of 10,000 numbers
        // in ascending order if allowable range is 0 - 999.
        
        result.cond = SKIPPING;
        return false;
    }
    
    data.clear();
    
    switch (cond) {
        case ASCENDING: // pre-sorted
            for (int n = MIN_DATA_VAL; n <= min(N-1, MAX_DATA_VAL); n++)
                data.add(n);
            break;
        
        case ASCENDISH: // mostly sorted
            for (int n = MIN_DATA_VAL; n <= min(N-1, MAX_DATA_VAL); n++) {
                if (randomChance(PERCENT_SORTED)) {
                    data.add(n);
                } else {
                    int n_maxrand = n + (PERCENT_DELTA) * (MAX_DATA_VAL);
                    int n_max = (n_maxrand <= MAX_DATA_VAL) ?
                                 n_maxrand : MAX_DATA_VAL;
                    data.add(randomInteger(n, n_max));
                }
            }
            break;
            
        case DESCENDING: // reverse sorted
            for (int n = min(N-1, MAX_DATA_VAL); n >= max(0, MIN_DATA_VAL); n--){
                data.add(n);
            }
            break;
            
        case RANDOMIZE: // random data in random positions within a range
        default:
            for (int n = 0; n < N; n++)
                data.add(randomInteger(MIN_DATA_VAL, MAX_DATA_VAL));
            break;
    }
    result.N = data.size();
    return true;
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
    os << "Random trials per N-sized dataset: " << report[0].numTrials << endl;
    os << "Threshold under which QS stops and defers to IS: ";
    os << report[0].threshold;
    os << endl << endl;
    os << "           |    Sort Time (msec)      " << endl;
    os << "-----------+--------------------------" << endl;
    os << " Data Size | QuickSort w/ | QuickSort " << endl;
    os << "     N     | median pivot |   Hybrid  " << endl;
    os << "-----------+--------------+-----------" << endl;
    
    for (ResultT rec : report) {
        os << right;
        os << setw(8)  << rec.N << "   |";
        os << setw(10) << fixed << setprecision(4) << rec.msecQSMP << "    | ";
        os << setw(8)  << fixed << setprecision(4) << rec.msecHQS << endl;
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
    if (sortfn == quickSortMP) {
        os << "quickSortMP";
    } else if (sortfn == quickSortHybrid){
        os << "quickSort hybrid";
    }else {
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