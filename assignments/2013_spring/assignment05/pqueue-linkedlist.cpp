//
// pqueue-linkedlist.cpp
//
// Implementation file for the LinkedListPriorityQueue class.
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
 
#include "pqueue-linkedlist.h"
#include "error.h"
#include "vector.h"

LinkedListPriorityQueue::LinkedListPriorityQueue() {
	head = NULL;
	count = 0;
}

LinkedListPriorityQueue::LinkedListPriorityQueue(
        const LinkedListPriorityQueue& src) {
    deepCopy(src);
}

LinkedListPriorityQueue& LinkedListPriorityQueue::operator=(
		const LinkedListPriorityQueue& src) {
	if (this != &src) {
		deleteList(head);
		deepCopy(src);
	}
	return *this;
}

LinkedListPriorityQueue::~LinkedListPriorityQueue() {
	deleteList(head);
}

int LinkedListPriorityQueue::size() const {
	return count;
}

bool LinkedListPriorityQueue::isEmpty() const {
	return (size() == 0);
}

void LinkedListPriorityQueue::enqueue(string value) {
	CellT* pNew = new CellT;
	pNew->value = value;
	pNew->next = NULL;
	insertCell(head, pNew);
	++count;
}

string LinkedListPriorityQueue::peek() const {
	if (head == NULL) error("peek: Cannot peek at an empty queue");
	return head->value;
}

string LinkedListPriorityQueue::dequeueMin() {
	string result;

	if (head == NULL) error("dequeueMin: Cannot dequeue from an empty queue");
	result = head->value;
	deleteCell(head);
	--count;
	return result;	
}

string LinkedListPriorityQueue::toString() const {
    Vector<string> result;
    LinkedListPriorityQueue pqCopy = *this;
    
    while (!pqCopy.isEmpty()) {
        result += pqCopy.dequeueMin();  // dequeue in priority order
    }
    return result.toString();
}

void LinkedListPriorityQueue::deepCopy(const LinkedListPriorityQueue& src) {
	head = NULL;
	count = src.count;
	copyList(src.head, head);
}

void LinkedListPriorityQueue::copyList(const CellT* pSrc, CellT*& pDst) {
	if (pSrc == NULL) return;
    
	CellT* pNew = new CellT;
	pNew->value = pSrc->value;
	pNew->next = NULL;          // wire out
	pDst = pNew;                // wire in

	copyList(pSrc->next, pDst->next);
}

void LinkedListPriorityQueue::deleteList(CellT*& pList) {
	if (pList == NULL) return;
	deleteList(pList->next);
	delete pList;
}

void LinkedListPriorityQueue::deleteCell(CellT*& pCell) {
	if (pCell == NULL) return;
	CellT* pSave = pCell;
	pCell = pCell->next;
	delete pSave;
}

void LinkedListPriorityQueue::insertCell(CellT*& pList, CellT* pNew) {
	if (pList == NULL || pNew->value < pList->value) {
		pNew->next = pList;     // wire out
		pList = pNew;           // wire in
	} else {
		insertCell(pList->next, pNew);
	}
}

ostream& operator<<(ostream& os, const LinkedListPriorityQueue& pq) {
    return os << pq.toString();
}
