//
// pqueue-vector.cpp
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

#include "pqueue-vector.h"
#include "error.h"

VectorPriorityQueue::VectorPriorityQueue() {
	front = npos;
}

VectorPriorityQueue::VectorPriorityQueue(const VectorPriorityQueue& src) {
    deepCopy(src);
}

VectorPriorityQueue& VectorPriorityQueue::operator=(
                        const VectorPriorityQueue& src) {
    if (this != &src) {
        // NB: No dynamic memory to free since none explicitly allocated.
        deepCopy(src);
    }
    return *this;
}

VectorPriorityQueue::~VectorPriorityQueue() {
}

int VectorPriorityQueue::size() const {
    return v.size();
}

bool VectorPriorityQueue::isEmpty() const {
	return (v.size() == 0);
}

void VectorPriorityQueue::enqueue(string value) {
	v.add(value);
    front = findFront();
}

string VectorPriorityQueue::peek() const {
	if (v.size() == 0) error("peek: Cannot peek at an empty queue");
	return v.get(front);
}

string VectorPriorityQueue::dequeueMin() {
	if (front == npos) error("dequeueMin: Cannot dequeue from an empty queue");

	string result = v.get(front);
	v.remove(front);

	// recalculate the front of the queue
	front = (v.isEmpty()) ? npos : findFront();
	return result;
}

string VectorPriorityQueue::toString() const {
    VectorPriorityQueue vpqCopy = *this;
    Vector<string> result;
    
    while (!vpqCopy.isEmpty()) {
        result += vpqCopy.dequeueMin(); // dequeue in priority order
    }
    return result.toString();
}

int VectorPriorityQueue::findFront() const {
	string minStr = v[0];
	int front = 0;

	for (int i = 1; i < v.size(); i++) {
		if (v[i] < minStr) {
			minStr = v[i];
			front = i;
		}						
	}
	return front;
}

void VectorPriorityQueue::deepCopy(const VectorPriorityQueue& src) {
    front = src.front;
    v = src.v;
}

ostream& operator<<(ostream& os, const VectorPriorityQueue& pq) {
    return os << pq.toString();
}
