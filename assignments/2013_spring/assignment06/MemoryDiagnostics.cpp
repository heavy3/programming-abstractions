/**********************************************************
 * File: MemoryDiagnostics.cpp
 *
 * Implementation of memory diagnostic functions.
 */

#include "MemoryDiagnostics.h"
#include "HuffmanTypes.h"

/* Global variables (ewww!) tracking total allocations. */
static long gTotalAllocs = 0;
static long gTotalFrees = 0;

/* Operators new and delete
 * Usage: Implicit
 * --------------------------------------------------------
 * This code is called whenever you allocate or deallocate
 * a node.	We use it to track total allocations and
 * deallocations.
 */
void* Node::operator new (size_t bytesNeeded) {
	++gTotalAllocs;
	return ::operator new(bytesNeeded);
}
void	Node::operator delete(void* toDelete) {
	++gTotalFrees;
	return ::operator delete(toDelete);
}


/* Function: numAllocations
 * Usage: long x = numAllocations();
 * --------------------------------------------------------
 * Returns how many total allocations of Nodes have occurred
 * throughout the program.
 */
long numAllocations() {
	return gTotalAllocs;
}

/* Function: numDeallocations
 * Usage: long x = numDeallocations();
 * --------------------------------------------------------
 * Returns how many total deallocations of Nodes have occurred
 * throughout the program.
 */
long numDeallocations() {
	return gTotalFrees;
}

/* Function: resetAllocationMetrics
 * Usage: resetAllocationMetrics();
 * --------------------------------
 * Resets the global variables used to track dynamic heap memory 
 * allocations for Nodes.
 */
void resetAllocationMetrics() {
    gTotalAllocs = 0;
    gTotalFrees = 0;
}
