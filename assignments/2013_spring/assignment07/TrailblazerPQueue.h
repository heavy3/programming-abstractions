/******************************************************************************
 * File: TrailblazerPQueue.h
 *
 * A priority queue type that supports the normal priority queue operations,
 * plus a final operation, decreaseKey.  The decreaseKey operation takes as
 * input an element in the priority queue and a new priority for that element,
 * then reduces the element's priority to be equal to the new priority.  This
 * makes it possible to implement Dijkstra's algorithm and A* search more
 * efficiently.
 */
 
#ifndef TrailblazerPQueue_Included
#define TrailblazerPQueue_Included

#include <map>     // Use std::multimap type as our backing.
#include "error.h"

template <typename ElemType>
class TrailblazerPQueue {
public:
	/* Function: enqueue
	 * Usage: pq.enqueue(myElem, 137.0);
	 * ------------------------------------------------------
	 * Inserts the given element into the priority queue with
	 * the indicated priority.  This element must not already
	 * be inside the priority queue; adding an element that
	 * has already been added will cause an error.  However,
	 * it is acceptable for multiple elements to have the
	 * same priority.
	 */
	void enqueue(ElemType elem, double priority);
	
	/* Function: dequeueMin
	 * Usage: ElemType e = pq.dequeueMin();
	 * ------------------------------------------------------
	 * Removes and returns the element with the lowest
	 * priority.  If the priority queue is empty, this causes
	 * an error.
	 */
	ElemType dequeueMin();
	
	/* Function: decreaseKey
	 * Usage: pq.decreaseKey(elem, 1.0);
	 * -----------------------------------------------------------
	 * Reduces the priority of the given element to the specified
	 * value.  The element must exist in the priority queue, and
	 * the new priority must not exceed the existing priority.
	 */
	void decreaseKey(ElemType elem, double newPriority);
	
	/* Function: isEmpty
	 * Usage: if (pq.isEmpty()) { ... } 
	 * -----------------------------------------------------------
	 * Returns whether the priority queue is empty.
	 */
	bool isEmpty();
	
	/* Function: size
	 * Usage: int elems = pq.size();
	 * -----------------------------------------------------------
	 * Returns the number of elements in the priority queue.
	 */
	int size();

private:
	/* Internally, our priority queue is implemented as two maps:
	 * one map from priorities to the list of elements with that
	 * priority, and one map from elements to their priorities.
	 * The priority -> element map is used to actually implement
	 * the priority queue, while the element -> priority map is
	 * used to implement decrease-key efficiently.
	 */
	std::multimap<double, ElemType> priorityQueue;
	std::map<ElemType, double> elemToPriority;
	 
	/* Utility function that compares two objects only using <. */
	bool equal(ElemType one, ElemType two) {
		return !(one < two || two < one);
	}
};

/* * * * * Implementation Below This Point * * * * */
template <typename ElemType>
void TrailblazerPQueue<ElemType>::enqueue(ElemType elem, double priority) {
	/* The priority must not be NaN; that breaks the total ordering on
	 * doubles that is needed by the std::map.
	 */
	if (!(priority == priority)) {
		error("Attempted to use NaN as a priority.");
	}

	/* We do not allow duplicate elements - that's the entire point
	 * of having decreaseKey!
	 */
	if (elemToPriority.insert(std::make_pair(elem, priority)).second == false) {
		error("Duplicate element in priority queue.");
	}
	priorityQueue.insert(std::make_pair(priority, elem));
}

template <typename ElemType>
bool TrailblazerPQueue<ElemType>::isEmpty() {
	return priorityQueue.empty();
}

template <typename ElemType>
int TrailblazerPQueue<ElemType>::size() {
	return int(priorityQueue.size());
}

template <typename ElemType>
ElemType TrailblazerPQueue<ElemType>::dequeueMin() {
	if (isEmpty()) {
		error("Attempted to dequeue from an empty priority queue.");
	}

	/* First, find the key with the lowest overall priority by obtaining
	 * an iterator to the first element.
	 */
	typename std::multimap<double, ElemType>::iterator toRemove =
		priorityQueue.begin();
	
	/* Cache the value to return. */
	ElemType result = toRemove->second;
	
	/* Remove this element from the priority queue. */
	priorityQueue.erase(toRemove);
	
	/* Remove this element from the inverse mapping. */
	if (!elemToPriority.erase(result)) {
		error("Could not remove element from inverse mapping, because it's not there.");
	}
	
	return result;
}

template <typename ElemType>
void TrailblazerPQueue<ElemType>::decreaseKey(ElemType elem, double newPriority) {
	/* The priority must not be NaN; that breaks the total ordering on
	 * doubles that is needed by the std::map.
	 */
	if (!(newPriority == newPriority)) {
		error("Attempted to use NaN as a priority.");
	}

	/* Confirm this element is here in the first place. */
	typename std::map<ElemType, double>::iterator itr = elemToPriority.find(elem);
	if (itr == elemToPriority.end()) {
		error("Cannot call decrease-key on an element not in the priority queue.");
	}
	
	/* Confirm that we're not *increasing* the key. */
	if (newPriority > itr->second) {
		error("Cannot use decrease-key to increase a key.");
	}
	
	/* Now, find the entry in the priority queue corresponding to this element. */
	typedef typename std::multimap<double, ElemType>::iterator Iterator;
	Iterator pItr = priorityQueue.find(itr->second);

	/* Walk forward until we find the key/value pair we're looking for. */
	while (!equal(pItr->second, elem)) {
		pItr++;
	}
		
	/* Remove this key/value pair and reinsert it. */
	priorityQueue.erase(pItr);
	priorityQueue.insert(std::make_pair(newPriority, elem));

	/* Finally, update the inverse table to reflect the new priority. */
	itr->second = newPriority;
}

#endif



