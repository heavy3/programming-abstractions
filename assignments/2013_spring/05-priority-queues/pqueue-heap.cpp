//
// pqueue-heap.cpp
//
// Implementation file for the HeapPriorityQueue class.
//
// --------------------------------------------------------------------------
// Assignment: 5.4 Binary Heap Priority Queue
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/180%20Assignment%205.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/11/2017.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//
 
#include <cmath>            // For min()
#include <sstream>          // For toString()
#include "pqueue-heap.h"
#include "error.h"

static const int INIT_INDEX = 1;  // First useful index for 1-based indices.

// Constructor: HeapPriorityQueue
// Usage: HeapPriorityQueue queue = new HeapPriorityQueue;
// -------------------------------------------------------
// Allocates space for the dynamic array of strings managed by the queue from
// the heap.  The size of the array is initially fixed based upon the
// value of the constant INIT_CAPACITY, though enquing will trigger
// migration to a larger arry if the existing capacity is reached.
//
// Generally items in the array are accessed through a 1-based index
// scheme which simplifies the calculation of algorithmically important
// child and parent node indices.  An INIT_INDEX constant provides
// the offset from a dummy node occupying slot 0.
//
HeapPriorityQueue::HeapPriorityQueue() {
    rawCapacity = INIT_CAPACITY;
    heapArray = new NodeT[rawCapacity];
    
    // We're using 1-based indexing into the heapArray to simplify the
    // calculation of parent-child indices.
    //
    // Pad the unused slot at the head of the array with a dummy value
    // for debug purposes.
    
    heapArray[0] = "dummy";
    rawCount = INIT_INDEX;  // Bias past the dummy entry to first usable slot.
}

// Constructor: ~HeapPriorityQueue
// Usage: Usually implicity
// -------------------------------
// Frees any dynamically allocated memory once the priority queue goes
// out of scope.

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] heapArray;
}

// Method: size
// Usage: q.size();
// ----------------
// Returns the number of strings stored in the array.  It ignores the
// dummy string in slot 0.

int HeapPriorityQueue::size() const {
    return rawCount - INIT_INDEX;  // Return effective size.
}

// Method: isEmpty
// Usage: if (q.isEmpty()) . . .
// -----------------------------
// Returns true the queue is functionally empty.

bool HeapPriorityQueue::isEmpty() const {
    return (size() == 0);
}

// Method: enqueue
// Usage: q.enqueue("some string");
// --------------------------------
// Adds a new value to the queue such that it resides within the binary
// heap in sorted order relative to some parent node.
//
// Initially the value is stored at the end of the array, but then
// is compared against parent nodes and repositioned upward in the
// tree until it is just below a parent node with a higher priority
// string (i.e., lexigraphically smaller).
//
// The overall count of items in the array is incremented.
// Should the array fill up, a new array is allocated to accommodate
// double the existing capacity.
//
// An error is thrown if an attempt is made to dequeue an empty queue.

void HeapPriorityQueue::enqueue(string value) {
    if (rawCount >= rawCapacity) expandCapacity();
    int nextNode = rawCount;  // rawCount serves as 1-based node index.
    heapArray[nextNode] = value;
    ++rawCount;
    heapSortNodeUp(nextNode);
}

// Method: peek
// Usage: q.peek();
// ----------------
// Returns the value of the highest priority string, but does not remove
// it from the queue itself.  In this scheme, the highest priority string
// always occupies the first effective slot in the array using a 1-based
// index.
//
// An error is thrown if an attempt is made to peek at an empty queue.

string HeapPriorityQueue::peek() const {
    if (isEmpty()) error("HeapPriorityQueue: Unable to peek an empty queue");
    return heapArray[INIT_INDEX];
}

// Method: dequeueMin
// Usage: string str = q.dequeueMin();
// -----------------------------------
// Returns the value of the highest priority string while also removing it from
// the queue itself.  In this scheme, the highest priority string always
// occupies the first effective slot in the array using a 1-based index.
//
// An error is thrown if an attempt is made to dequeue an empty queue.

string HeapPriorityQueue::dequeueMin() {
    if (isEmpty()) error("HeapPriorityQueue: Unable to dequeue an empty queue");
    
    // Highest priority item is always sitting at heapArray[1]
    
    string result = heapArray[INIT_INDEX];
    
    // We have no more use of it though since we harvested the value.
    // Consign it to the ash heap by swapping it for the last-most item.
    
    int theSize = size();
    swapNodes(INIT_INDEX, theSize);
    
    // Account for having one-less item in our queue.
    
    rawCount = max(INIT_INDEX, rawCount - 1);  // keep dummy node @ heapArray[0]
    
    // Re-sort the heap since the child node we just moved to the top
    // is not likely in proper order relative to the remaining nodes.
    // This takes at most O(logN) time.
    
    heapSortNodeDn(INIT_INDEX);
    
    // Return string from dequeued node.
    
    return result;
}

// Method: toString
// Usage: q.toString();
// --------------------
// Returns a string representation of the queue in heap-order (as opposed
// to strict priority order).

string HeapPriorityQueue::toString() const {
    ostringstream os;
    os << '[' << endl;
    for (int i = 0; i < rawCount; i++) {
        os << "[" << i << "] " << heapArray[i] << endl;
    }
    os << ']' << endl;
    return os.str();
}

// Method: clearHeap
// Usage: clearHeap()
// ------------------
// Nulls out the contants of dynamic heap memory.

void HeapPriorityQueue::clearHeap(const int numNodes, const int startNode) {
    int normNodes = min(numNodes, rawCapacity - startNode);
    for (int i = startNode; i < normNodes; i++) {
        heapArray[i] = "";
    }
}

// Method: expandCapacity
// Usage: q.expandCapacity();
// --------------------------
// Effectively doubles the size of heap memory allocated to the dynamic
// array of strings should that array ever fill.
//
// A space twice the size is allocated and the contents copied
// therein before the old space is reclaimed.

void HeapPriorityQueue::expandCapacity() {
    if (rawCount > MAX_PARENT_INDEX) {
        
        // We wouldn't be able to index into this expanded area
        // given the current datatype we're using to do the indexing.
        
        error("HeapPriorityQueue: maximum capacity reached.");
    }

    int biggerCapacity = rawCapacity << SHIFT_NUM;  // Fast mul by 2.
    NodeT* biggerArray = new NodeT[biggerCapacity];
    for (int i = 0; i < rawCount; i++) {
        biggerArray[i] = heapArray[i];
    }
    delete[] heapArray;
    heapArray = biggerArray;
    rawCapacity = biggerCapacity;
}

// Method: heapSortNodeUp
// Usage: heapSortNodeUp(childIndex);
// ----------------------------------
// Causes a child node at some lower position in the binary tree to bubble
// up to it's sorted position such that it is smaller in value that
// all it's child nodes.

void HeapPriorityQueue::heapSortNodeUp(int childIndex) {
    int parentIndex = pFc(childIndex);
    
    if (childIndex <= INIT_INDEX ||
        heapArray[parentIndex] <= heapArray[childIndex]) {
        return;
    }
    swapNodes(childIndex, parentIndex);
    heapSortNodeUp(parentIndex);
}

// Method: heapSortNodeDn
// Usage: heapSortNodeDn(parentIndex);
// ----------------------------------
// Causes a parent node at some higher position in the binary tree to bubble
// down to it's sorted position such that it lower in value than all it's
// child nodes.

void HeapPriorityQueue::heapSortNodeDn(int parentIndex) {
    
    // fectch index of smallest-valued child directly below parent
    
    int smallestChildIndex = smallestChild(parentIndex);
    
    // base case: parent already in sorted position
    //            has no children with smaller values
    
    if (parentIndex == smallestChildIndex) {
        return;
    }
    
    // recursive case:
    //
    // swap parent with smallest-valued child.
    // heap sort from new, lower position in the tree
    
    swapNodes(smallestChildIndex, parentIndex);
    heapSortNodeDn(smallestChildIndex);
}

// Method: swapNodes
// Usage: swapNodes(n1, n2);
// -------------------------
// Swaps the contents of the two nodes indexed by n1 and n2.

void HeapPriorityQueue::swapNodes(const int node1, const int node2) {
    NodeT tmp;
    tmp = *(heapArray + node1);
    heapArray[node1] = heapArray[node2];
    heapArray[node2] = tmp;
}

// Method: pFc
// Usage: int parentIndex = pFc(childIndex);
// -----------------------------------------
// Calculates the index of a parent node relative to a child node according
// to the following forumula:
//
//     parentIndex = childIndex / 2;

int HeapPriorityQueue::pFc(const int childIndex) const {
    if (childIndex <= INIT_INDEX) return INIT_INDEX; // avoid underflow
    return childIndex >> SHIFT_NUM;  // parentIndex = childIndex/2
}

// Method: cFp
// Usage: int childIndex = pFc(parentIndex);
// -----------------------------------------
// Calculates the index of a child node relative to a parent node according
// to the following forumula:
//
//     childIndex = parentIndex * 2;

int HeapPriorityQueue::cFp(const int parentIndex) const {
    if (parentIndex > MAX_PARENT_INDEX) {
        error("HeapPriorityQueue: index overflow caught.");
    }
    return parentIndex << SHIFT_NUM; // childIndex = 2*parentIndex
}

// Method: smallestChild
// Usage: int index = smallestChild(someIndex);
// --------------------------------------------
// Returns the index of the lexigraphically smallest child string relative
// to some parent node string.  If neither child is smaller, then the index
// of the parent is itself returned as an indication to the client that the
// parent has no children.

int HeapPriorityQueue::smallestChild(const int parentIndex) const {
    
    int child1Index = cFp(parentIndex); // index = parentIndex * 2
    int child2Index = child1Index + 1;  // adjacent children of the same parent
    
    if (child1Index > size()) {
        // no children
        return parentIndex;
    } else {
        if (child2Index > size()) {
            // one child
            if (heapArray[child1Index] < heapArray[parentIndex]) {
                return child1Index;
            } else {
                return parentIndex;
            }
        } else {
            // two children
            if (heapArray[child1Index] < heapArray[child2Index]) {
                if (heapArray[child1Index] < heapArray[parentIndex]) {
                    return child1Index;
                } else {
                    return parentIndex;
                }
            } else {
                if (heapArray[child2Index] < heapArray[parentIndex]) {
                    return child2Index;
                } else {
                    return parentIndex;
                }
            }
        }
    }
}

// Method: ordinalRow
// Usage: int row = ordinalRow(nodeIndex);
// ---------------------------------------
// Returns the 1-based row that a given node occupies in the tree
// from it's index in the heap array.
//
// For example, given the following binary heap:
//
//                       [a]             row 1
//               +--------+-------+
//              [c]              [g]     row 2
//         +-----+-----+
//        [e]         [m]                row 3
//
//
// stored as an array of contiguous string references:
//
//                heapArray[1] -> "a"
//                heapArray[2] -> "c"
//                heapArray[3] -> "g"
//                heapArray[4] -> "e"
//                heapArray[5] -> "m"
//
// ordinalRow(4) returns 3 since the reference to the "e" string
// resides on the 3rd row of the binary tree.
//

int HeapPriorityQueue::ordinalRow(const int index) const {
    int ordRow = floor(log2(max(1, index))) + 1; // parent has 2 children max
    return ordRow;
}

// Operator: <<
// Usage: cout << queue << endl;
// -----------------------------
// Overloads the insertion operator to support display of priority queueus.
// Returns a reference to an output stream so the operator can be chained
// as input to another downstream insertion.

ostream & operator<<(ostream & os, const HeapPriorityQueue& q) {
    return os << q.toString();
}
