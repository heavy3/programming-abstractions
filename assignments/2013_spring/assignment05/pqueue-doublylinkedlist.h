//
// pqueue-doublylinkedlist.h
//
// --------------------------------------------------------------------------
// Assignment: 5.3 Doubly Linked List Priority Queue
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

#ifndef PQueue_DoublyLinkedList_Included
#define PQueue_DoublyLinkedList_Included

#include <string>
#include <ostream>
using namespace std;

//
// A class representing a priority queue backed by an unsorted,
// doubly-linked list.
//

class DoublyLinkedListPriorityQueue {
public:
	// Constructs a new, empty priority queue backed by an unsorted doubly-linked list.
	DoublyLinkedListPriorityQueue();
    
	// Constructs a new, priority queue from an existing priority queue.
	DoublyLinkedListPriorityQueue(const DoublyLinkedListPriorityQueue& src);
    
	// Assignment operator.
	DoublyLinkedListPriorityQueue& operator=(const DoublyLinkedListPriorityQueue& src);
	
	// Cleans up all memory allocated by this priority queue.
	~DoublyLinkedListPriorityQueue();
	
	// Returns the number of elements in the priority queue.
	int size() const;
	
	// Returns whether or not the priority queue is empty.
	bool isEmpty() const;
	
	// Enqueues a new string into the priority queue.
	void enqueue(string value);
	
	// Returns, but does not remove, the lexicographically first string in the
	// priority queue.
	string peek();
	
	// Returns and removes the lexicographically first string in the
	// priority queue.
	string dequeueMin();
    
	// Return the string representation of priority queue.
	string toString() const;

private:
	struct CellT {
		string value;
		CellT* next;
		CellT* prev;
	};
	CellT* head;
	int count;

	void deepCopy(const DoublyLinkedListPriorityQueue& src);
	void deleteList(CellT*& pList);
	void deleteCell(CellT*& pCell);
	void prependCell(CellT* pNew);
    CellT* findFront();
};

// Returns a reference to an output stream.
ostream& operator<<(ostream& os, const DoublyLinkedListPriorityQueue& pq);

#endif
