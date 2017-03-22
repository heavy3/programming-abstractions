//
// pqueue-doublylinkedlist.cpp
//
// Implementation file for the DoublyLinkedListPriorityQueue class.
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

#include "pqueue-doublylinkedlist.h"
#include "error.h"
#include "vector.h"

DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
    head = NULL;
    count = 0;
}

DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue(
    const DoublyLinkedListPriorityQueue& src) {
    deepCopy(src);
}

DoublyLinkedListPriorityQueue& DoublyLinkedListPriorityQueue::operator=(
    const DoublyLinkedListPriorityQueue& src) {
    if (this != &src) {
        deleteList(head);
        deepCopy(src);
    }
    return *this;
}

DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
    deleteList(head);
}

int DoublyLinkedListPriorityQueue::size() const {
    return count;
}

bool DoublyLinkedListPriorityQueue::isEmpty() const {
    return (size() == 0);
}

void DoublyLinkedListPriorityQueue::enqueue(string value) {
    CellT* pNew = new CellT;
    pNew->value = value;
    pNew->next = NULL;
    pNew->prev = NULL;
    
    prependCell(pNew);
    ++count;
}

string DoublyLinkedListPriorityQueue::peek() {

    if (head == NULL) error("peek: Cannot peek at an empty queue");
    
    CellT* front = findFront();
    return front->value;
}

string DoublyLinkedListPriorityQueue::dequeueMin() {
    string result;
    
    if (head == NULL) error("dequeueMin: Cannot dequeue from an empty queue");
    CellT* front = findFront();
    result = front->value;
    deleteCell(front);
    --count;
    if (count == 0) head = NULL;
    return result;
}

string DoublyLinkedListPriorityQueue::toString() const {
    Vector<string> result;
    DoublyLinkedListPriorityQueue pqCopy = *this;
    
    while (!pqCopy.isEmpty()) {
        result += pqCopy.dequeueMin();  // dequeue in priority order
    }
    return result.toString();
}

void DoublyLinkedListPriorityQueue::deepCopy(
        const DoublyLinkedListPriorityQueue& src) {
    
    head = NULL;
    count = src.count;
    
    if (src.head != NULL) {
        head = new CellT;
        CellT* prev = NULL;
        CellT* nextSrc = src.head;
        CellT* nextDst = head;
        while (nextSrc != NULL) {
            nextDst->value = nextSrc->value;
            nextDst->prev = prev;
            if (nextSrc->next == NULL) {
                nextDst->next = NULL;
                break;
            } else {
                prev = nextDst;
                nextDst->next = new CellT;
                nextSrc = nextSrc->next;
                nextDst = nextDst->next;
            }
        }
        
    }
}

void DoublyLinkedListPriorityQueue::deleteList(CellT*& pList) {
    if (pList == NULL) return;
    deleteList(pList->next);
    delete pList;
}

void DoublyLinkedListPriorityQueue::deleteCell(CellT*& pDelCell) {
    if (pDelCell == NULL) return;
    
    // wire the upstream past the thing we're deleting
    CellT* prevCell = pDelCell->prev;
    if (prevCell == NULL) {
        head = pDelCell->next;
    } else {
        prevCell->next = pDelCell->next;
    }
    
    // wire the downstream cell back to the upstream
    CellT* nextCell = pDelCell->next;
    if (nextCell != NULL) {
        nextCell->prev = pDelCell->prev;
    }
    
    delete pDelCell;
}

void DoublyLinkedListPriorityQueue::prependCell(CellT* pNew) {
    pNew->next = head;
    pNew->prev = NULL;  // always NULL since we're at the head
    if (head != NULL) head->prev = pNew;
    head = pNew;
}

DoublyLinkedListPriorityQueue::CellT* DoublyLinkedListPriorityQueue::findFront() {
    CellT* front = head;
    
    if (head == NULL) return front;
        
    string minValue = head->value;
    
    CellT* pCell = head;
    while (pCell != NULL) {
        if (pCell->value < minValue) {
            minValue = pCell->value;
            front = pCell;
        }
        pCell = pCell->next;
    }
    return front;
}

ostream& operator<<(ostream& os, const DoublyLinkedListPriorityQueue& pq) {
    return os << pq.toString();
}
