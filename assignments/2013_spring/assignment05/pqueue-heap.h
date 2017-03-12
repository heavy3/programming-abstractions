//
// pqueue-heap.h
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

#ifndef PQueue_Heap_Included
#define PQueue_Heap_Included

#include <string>

using namespace std;

// A class representing a priority queue backed by a binary heap.

class HeapPriorityQueue {
public:
	// Constructs a new, empty priority queue backed by a binary heap. //
	HeapPriorityQueue();
	
	// Cleans up all memory allocated by this priority queue. 
	~HeapPriorityQueue();
	
	// Returns the number of elements in the priority queue. 
	int size() const;
	
	// Returns whether or not the priority queue is empty. 
	bool isEmpty() const;
	
	// Enqueues a new string into the priority queue. 
	void enqueue(const string value);
	
	// Returns, but does not remove, the lexicographically first string in the
    // priority queue.
	 
	string peek() const;
	
	// Returns and removes the lexicographically first string in the
    // priority queue.
	 
	string dequeueMin();
    
    // Returns the string form of the queue object.
    
    string toString() const;

private:
    static const int INIT_CAPACITY = 128;
    static const int SHIFT_NUM = 1;   // Fast int multiply or divide by 2.

    static const int MAX_PARENT_INDEX = INT_MAX >> SHIFT_NUM;        // div by 2
    static const int MAX_CHILD_INDEX = MAX_PARENT_INDEX << SHIFT_NUM;// mul by 2
    
    typedef string NodeT;
    
    // Instance data
    
    NodeT* heapArray;       // Assignment specifies array instead of vector.
    int rawCount;
    int rawCapacity;
    
    void clearHeap(const int numNodes, const int startNode = 0);
    void expandCapacity();
    void heapSortNodeUp(int nodeIndex);
    void heapSortNodeDn(int nodeIndex);
    int pFc(const int childIndex) const;
    int cFp(const int parentIndex) const;
    void swapNodes(const int nodeIndex1, const int nodeIndex2);
    int ordinalRow(const int nodeIndex) const;
    int smallestChild(const int parentIndex) const;
};

ostream & operator<<(ostream & os, const HeapPriorityQueue& q);
    
#endif
