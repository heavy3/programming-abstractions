//
// sort.cpp
//
// This file implements a polymorphic Quicksort for all base types that 
// support the standard relational operators.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file templatizes code from Figure 10-9.
// --------------------------------------------------------------------------
//

// Implementation notes: partition
// -------------------------------
// This function selects a 'pivot' element from the input vector then mutates 
// the vector so all elements smaller than the pivot migrate to the low end of 
// the vector while elements equal to or greater than the pivot migrate to the
// high end of the vector, leaving the pivot element in sorted position.
//
// The index of the sorted pivot element is returned to the caller.
//

template <typename ValueType>
int partition(Vector<ValueType> & vec, int start, int finish) {
    ValueType pivot = vec[start];
    int lh = start + 1;
    int rh = finish;
    while (true) {
        while (lh < rh && vec[rh] >= pivot) rh--;
        while (lh < rh && vec[lh] < pivot) lh++;
        if (lh == rh) break;
        ValueType tmp = vec[lh];
        vec[lh] = vec[rh];
        vec[rh] = tmp;
    }
    if (vec[lh] >= pivot) return start;
    vec[start] = vec[lh];
    vec[lh] = pivot;
    return lh;
}

// Implementation notes: quicksort
// -------------------------------
// This function sorts the elements of a vector that fall within 
// the inclusive bounds of start and finish indices.
//
// The algorithm works by recursively partitioning the input vector
// about a sorted pivot element.  
//
// By applying that same strategy to increasingly smaller sub-vectors that 
// fall to the left and right of the pivot boundary, we eventually
// attain all elementes sorted between the initial start and finish 
// indices.

template <typename ValueType>
void quicksort(Vector<ValueType> & vec, int start, int finish) {
    if (start >= finish) return;
    int boundary = partition(vec, start, finish);
    quicksort(vec, start, boundary - 1);
    quicksort(vec, boundary + 1, finish);
}

// Implementation notes: sort
// --------------------------
// sort is a wrapper function for quicksort

template <typename ValueType>
void sort(Vector<ValueType> & vec) {
    quicksort(vec, 0, vec.size() - 1);
}
