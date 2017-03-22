/*************************************************************************
 * File: ReferenceHuffmanEncoding.h
 *
 * Header file containing links into our reference implementations used
 * during testing.  You should not modify this header file, nor should
 * you use these functions in your code; they're for our testing harness
 * only.
 */

#include "HuffmanEncoding.h"

/* Reference solution for getFrequencyTable. */
Map<ext_char, int> referenceGetFrequencyTable(istream & file);

/* Reference solution for buildEncodingTree. */
Node *referenceBuildEncodingTree(Map<ext_char, int> frequencies);

/* Function used to confirm that the reference solution has been configured
 * correctly.  You should never need to call this function on your own; it's
 * here to make sure that we didn't mess up the project when we created it.
 */
void testReferenceSolutionConfiguration();
