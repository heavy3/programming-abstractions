//
// pqueue-vector.h
//
// --------------------------------------------------------------------------
// Assignment: 5.1 Vector Priority Queue
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/180%20Assignment%205.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/9/2017.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#ifndef PQueue_Vector_Included
#define PQueue_Vector_Included

#include <string>
#include <ostream>
#include "vector.h"
using namespace std;

//
// A class representing a priority queue backed by an unsorted Vector.
//

class VectorPriorityQueue {
public:
	// Constructs a new, empty priority queue backed by a vector.
	VectorPriorityQueue();
    
	// Constructs a new, priority queue from an existing priority queue.
	VectorPriorityQueue(const VectorPriorityQueue& src);
    
	// Assignment operator.
	VectorPriorityQueue& operator=(const VectorPriorityQueue& src);
	
	// Cleans up all memory allocated by this priority queue.
	~VectorPriorityQueue();
	
	// Returns the number of elements in the priority queue.
	int size() const;
	
	// Returns whether or not the priority queue is empty.
	bool isEmpty() const;
	
	// Enqueues a new string into the priority queue.
	void enqueue(string value);
	
	// Returns, but does not remove, the lexicographically first string in the
	// priority queue.
	string peek() const;
	
	// Returns and removes the lexicographically first string in the
	// priority queue.
	string dequeueMin();
    
	// Return the string representation of priority queue.
	string toString() const;

private:
    
    static const int npos = -1;
	Vector<string> v;
	int front;
    
    void deepCopy(const VectorPriorityQueue& src);
    int findFront() const;
};


// Returns a reference to an output stream.
ostream& operator<<(ostream& os, const VectorPriorityQueue& pq);

#endif
