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


const int INIT_INDEX = 1;  // First useful index for 1-based indices.

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

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] heapArray;
}

int HeapPriorityQueue::size() const {
    return rawCount - INIT_INDEX;  // Return effective size.
}

bool HeapPriorityQueue::isEmpty() const {
    return (size() == 0);
}

void HeapPriorityQueue::enqueue(string value) {
    if (rawCount >= rawCapacity) expandCapacity();
    int nextNode = rawCount;  // rawCount serves as 1-based node index.
    heapArray[nextNode] = value;
    ++rawCount;
    heapSortNodeUp(nextNode);
}

string HeapPriorityQueue::peek() const {
    if (isEmpty()) error("HeapPriorityQueue: Unable to peek an empty queue");
    return heapArray[INIT_INDEX];
}

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
    
    // Give the client the tasty value it was wanting from the node
    // we just dequeued.
    
    return result;
}

string HeapPriorityQueue::toString() const {
    ostringstream os;
    os << '[' << endl;
    for (int i = 0; i < rawCount; i++) {
        os << "[" << i << "] " << heapArray[i] << endl;
    }
    os << ']' << endl;
    return os.str();
}

void HeapPriorityQueue::clearHeap(const int numNodes, const int startNode) {
    int normNodes = min(numNodes, rawCapacity - startNode);
    for (int i = startNode; i < normNodes; i++) {
        heapArray[i] = "";
    }
}

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

void HeapPriorityQueue::heapSortNodeUp(int childIndex) {
    int parentIndex = pFc(childIndex);
    
    if (childIndex <= INIT_INDEX ||
        heapArray[parentIndex] <= heapArray[childIndex]) {
        return;
    }
    swapNodes(childIndex, parentIndex);
    heapSortNodeUp(parentIndex);
}

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

int HeapPriorityQueue::pFc(const int childIndex) const {
    if (childIndex <= INIT_INDEX) return INIT_INDEX; // avoid underflow
    return childIndex >> SHIFT_NUM;  // parentIndex = childIndex/2
}

int HeapPriorityQueue::cFp(const int parentIndex) const {
    if (parentIndex > MAX_PARENT_INDEX) {
        error("HeapPriorityQueue: index overflow caught.");
    }
    return parentIndex << SHIFT_NUM; // childIndex = 2*parentIndex
}

void HeapPriorityQueue::swapNodes(const int node1, const int node2) {
    NodeT tmp;
    tmp = *(heapArray + node1);
    heapArray[node1] = heapArray[node2];
    heapArray[node2] = tmp;
}

int HeapPriorityQueue::ordinalRow(const int index) const {
    int ordRow = floor(log2(max(1, index))) + 1; // parent has 2 children max
    return ordRow;
}

int HeapPriorityQueue::smallestChild(const int parentIndex) const {
    
    int child1Index = cFp(parentIndex);
    int child2Index = child1Index + 1;
    
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

ostream & operator<<(ostream & os, const HeapPriorityQueue& q) {
    return os << q.toString();
}
