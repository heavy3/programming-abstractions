/**********************************************
 * File: HuffmanTypes.h
 *
 * A file containing the types used by the Huffman
 * encoding program.
 */
 
#ifndef HuffmanTypes_Included
#define HuffmanTypes_Included

#include <stddef.h>

/* Type: ext_char
 * A type representing a character, a pseudo-eof, or nothing. */
typedef int ext_char;

/* Constant: PSEUDO_EOF
 * A constant representing the PSEUDO_EOF marker that you will
 * write at the end of your Huffman-encoded file.
 */
const ext_char PSEUDO_EOF = 256;

/* Constant: NOT_A_CHAR
 * A constant representing an extended character that does not
 * actually hold a value.	 When you are constructing your Huffman
 * encoding tree, you should set the characters in each internal
 * node to this value to explicitly mark that they are not being
 * used.
 */
const ext_char NOT_A_CHAR = 257;

/* Type: Node
 * A node inside a Huffman encoding tree.	 Each node stores four
 * values - the character stored here (or NOT_A_CHAR if the value
 * is not a character), pointers to the 0 and 1 subtrees, and the
 * weight of the tree.
 *
 * You are free to add fields to this class, but please *do not*
 * modify the existing fields here.	 Our testing code relies on
 * these fields being here in order to function correctly, and
 * Very Bad Things will happen if you change or reorder them.
 */
struct Node {
	/* The character represented here.	This can also be PSEUDO_EOF
	 * if the nodes represents the pseudo-eof character, and should be
	 * set to NOT_A_CHARACTER for each internal node.
	 */
	ext_char character;
	
	/* A pointer to the 0 subtree. */
	Node *zero;

	/* A pointer to the 1 subtree. */
	Node *one;
	
	/* The weight of this node, which is the combined weight of all
	 * characters at or below this node.
	 */
	int weight;
	
	/* The following code is used to instrument the Node type so that we can
	 * check whether or not you are leaking memory.	 You don't need to worry
	 * about how this works or even what a void pointer is; this is just extra
	 * code to track allocations and deallocations.
	 */
	void* operator new	 (size_t bytesNeeded);
	void	operator delete(void* toDelete);
};

#endif
