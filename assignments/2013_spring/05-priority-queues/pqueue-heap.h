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
    
	// Constructs a new, empty priority queue backed by a binary heap.
    
	HeapPriorityQueue();
	
	// Cleans up all memory allocated by this priority queue.
    
	~HeapPriorityQueue();
	
	// Returns the number of elements in the priority queue.
    
	int size() const;
	
	// Returns whether or not the priority queue is empty.
    
	bool isEmpty() const;
	
	// Enqueues a new string into the priority queue in O(logN) time.
    
	void enqueue(const string value);
	
	// Returns, but does not remove, the lexicographically first string in the
    // priority queue.
    
	string peek() const;
	
	// Returns and removes the lexicographically first string in the
    // priority queue in O(logN) time.
    
	string dequeueMin();
    
    // Returns the string form of the queue object.
    
    string toString() const;

private:
    
    // Conceptually, queue data is managed as references to strings
    // in a binary tree.  In practice, those references reside compactly in
    // a heap-based array that grows as necessary.
    //
    // For example, the queue ["a", "c", "e", "g", "m"] could be represented
    // as:
    //
    //                       [a]
    //               +--------+-------+
    //              [c]              [g]
    //         +-----+-----+
    //        [e]         [m]
    //
    // but stored in an array of contiguous string references:
    //
    //                heapArray[0] -> "dummy entry"
    //                heapArray[1] -> "a"
    //                heapArray[2] -> "c"
    //                heapArray[3] -> "g"
    //                heapArray[4] -> "e"
    //                heapArray[5] -> "m"
    //
    //
    // (Normally, I'd use a vector for the underlying container, but
    // the assignment calls for an array implementation to give more opportunity
    // to manage memory directly.)
    //
    // You'll notice this is a 1-based system with a dummy string
    // occupying heapArray[0].  This simplifies calculation of parent
    // and child indices relative to each other.
    //
    // Enqueued items enter at the end of the array, equivalent to the last
    // row in the tree, and propagate up to sorted position (such that
    // every parent is lexigraphically less than its children).
    //
    // This implies the head of the queue is always at heapArray[1].
    //
    // The head is dequeued by swapping heapArray[1] with the last node
    // in the tree, adjusting the queue count, and then repositioning
    // the new head node into it's lexigraphically sorted order.
    //
    // Both operations happen in O(logN) time and require O(N) space overhead
    // for managing object references in the heap.
    
    static const int INIT_CAPACITY = 128;   // Initial size of heap array.
    
    // SHIFT_NUM is used as an arithmetic operator to quickly multiple
    // or divide a number by a power of 2.  Controls the rate of expansion
    // of the dynamic array used to store queue data when capacity has been
    // reached along with the calculation of parent and child node indices
    // relative to each other.  Hell will break loose if you change this. :-)
    //
    // TODO: Should probably decouple these two concepts
    //       since they are orthogonal.
    
    static const int SHIFT_NUM = 1;
    
    // Detect overflow and underflow situations.
    
    static const int MAX_PARENT_INDEX = INT_MAX >> SHIFT_NUM;
    static const int MAX_CHILD_INDEX = MAX_PARENT_INDEX << SHIFT_NUM;
    
    typedef string NodeT;   // Type for queue data payload.
                            // Could be generalized to a struct or class.
    
    // Instance data
    
    NodeT* heapArray;       // Points to dynamically allocated region where
                            // references to the queue data are stored.
    
    int rawCount;           // The total number of entries (including dummy item
                            // at heapArray[0]) are stored in the managed heap.
    
    int rawCapacity;        // Specifies number of references the heap array
                            // can hold.
    
    // Nulls out but does not garbage collect the binary heap memory.
    
    void clearHeap(const int numNodes, const int startNode = 0);
    
    // Effectively grows the size of the heap by 2x should capacity be
    // reached when enqueue rate exceeds dequeue rate.  Works transparently
    // to client.
    
    void expandCapacity();
    
    // The next two member functions reorder the specific line of nodes
    // in the tree after and enqueue or dequeue operation.  They embody
    // the heart of the O(logN) algorithm.
    
    // Reorders a child node up into proper heap-sort relative to a specific
    // chain of ancestor nodes.
    
    void heapSortNodeUp(int nodeIndex);
    
    // Reorders a parent node down into proper heap-sort order relative to
    // a specific chain of descendant nodes.
    
    void heapSortNodeDn(int nodeIndex);
    
    // Swap the contents of two nodes within the tree.  This effectively
    // allows for parent and child nodes to be reordered with respect to
    // each other.
    
    void swapNodes(const int nodeIndex1, const int nodeIndex2);
    
    // These next to member functions allow the calculation of parent node
    // index within the heap array from the child node index and vice versa.
    
    int pFc(const int childIndex) const;
    int cFp(const int parentIndex) const;
    
    // This calculates the 1-based row number of a given node from its heap
    // array index.
    
    int ordinalRow(const int nodeIndex) const;
    
    // When reording an out-of-position node downward in the tree, we
    // need to determine which (if any) of the child nodes has the smallest
    // lexigraphical string value.
    
    int smallestChild(const int parentIndex) const;
};

// Overloads the insertion operator to allow output of a string
// representation of the queue to an output stream.
    
ostream & operator<<(ostream & os, const HeapPriorityQueue& q);
    
#endif
