//
// pqueue-linkedlist.h
//
// --------------------------------------------------------------------------
// Assignment: 5.2 Linked List Priority Queue
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/180%20Assignment%205.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/10/2017.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#ifndef PQueue_LinkedList_Included
#define PQueue_LinkedList_Included

#include <string>
#include <ostream>
using namespace std;

//
// A class representing a priority queue backed by a sorted,
// singly-linked list.
//

class LinkedListPriorityQueue {
public:
	// Constructs a new, empty priority queue backed by a sorted linked list.
	LinkedListPriorityQueue();
    
	// Constructs a new, priority queue from an existing priority queue.
	LinkedListPriorityQueue(const LinkedListPriorityQueue& src);
    
	// Assignment operator.
	LinkedListPriorityQueue& operator=(const LinkedListPriorityQueue& src);
	
	// Cleans up all memory allocated by this priority queue.
	~LinkedListPriorityQueue();
	
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
	struct CellT {
		string value;
		CellT* next;
	};
	CellT* head;
	int count;

	void copyList(const CellT* pSrc, CellT*& pDst);
	void deepCopy(const LinkedListPriorityQueue& src);
	void deleteList(CellT*& pList);
	void deleteCell(CellT*& pCell);
	void insertCell(CellT*& pList, CellT* pNew);
};

// Returns a reference to an output stream.
ostream& operator<<(ostream& os, const LinkedListPriorityQueue& pq);

#endif
