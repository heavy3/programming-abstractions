//
// HuffmanEncoding.cpp
//
// Implementation of the functions from HuffmanEncoding.h.
//
// --------------------------------------------------------------------------
// Assignment: 6 Huffman Encoding
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/230%20Assignment%206.pdf
//
// Function prototypes and descriptions provided through Stanford as part of the
// assignment package.
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/13/2017.
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//

#include "HuffmanEncoding.h"
#include "pqueue.h"

// Function: getFrequencyTable
// Usage: Map<ext_char, int> freq = getFrequencyTable(file);
// --------------------------------------------------------
// Given an input stream containing text, calculates the
// frequencies of each character within that text and stores
// the result as a Map from ext_chars to the number of times
// that the character appears.
//
// This function will also set the frequency of the PSEUDO_EOF
// character to be 1, which ensures that any future encoding
// tree built from these frequencies will have an encoding for
// the PSEUDO_EOF character.

Map<ext_char, int> getFrequencyTable(istream& file) {
    ext_char ch;
    Map<ext_char, int> map;
    
    while ((ch = file.get())!= EOF) {
        map[ch]++;
    }
    map[PSEUDO_EOF] = 1;
	
    return map;
}

// Function: buildEncodingTree
// Usage: Node* tree = buildEncodingTree(frequency);
// --------------------------------------------------------
// Given a map from extended characters to frequencies,
// constructs a Huffman encoding tree from those frequencies
// and returns a pointer to the root.
//
// This function can assume that there is always at least one
// entry in the map, since the PSEUDO_EOF character will always
// be present.

Node* buildEncodingTree(Map<ext_char, int>& frequencies) {
    Node* pTree = NULL;
    PriorityQueue<Node*> pq;
    
    // Add all the singleton nodes to the priority queue.
    // Each unique char from the input file will reside in
    // its own node with a frequency count.
    
    for (ext_char key: frequencies) {
        Node* pNode = new Node;
        
        pNode->character = key;
        pNode->weight = frequencies[key];
        
        pNode->zero = NULL;
        pNode->one  = NULL;
        
        // We desire least frequently occuring chars to be at
        // the head of the priority queue so they will be dequeued
        // first after this loop and thus land at the bottom of the
        // encoding tree.
        
        pq.enqueue(pNode, pNode->weight);
    }
    
    // Gradually assemble an optimal binary encoding tree (from the
    // bottom up) that associates a variable-length bit sequence for
    // each unique char encountered on the input based upon frequency
    // of occurence.
    //
    // The branching structure of the tree itself (and especially
    // the depth at which a char resides within the tree) determines
    // the specific binary encoding for that char.
    
    while (!pq.isEmpty()) {
        pTree = new Node;
        pTree->character = NOT_A_CHAR;
        
        Node* pChild0;
        Node* pChild1;
        
        switch (pq.size()) {
            case 1: {
                pChild0 = pq.dequeue();
                
                pTree->weight = pChild0->weight;
                pTree->zero = pChild0;
                pTree->one = NULL;
            }
            break;
                
            case 2:
            default: {
                pChild0 = pq.dequeue();
                pChild1 = pq.dequeue();
                
                pTree->weight = pChild0->weight + pChild1->weight;
                pTree->zero = pChild0;
                pTree->one = pChild1;
            }
            break;
        }
        if (pq.isEmpty()) break;
        
        // Add this sub-tree back into the queue to be joined to others
        // until we're down to one big tree and no subtrees or individual
        // child nodes to attach.
        
        pq.enqueue(pTree, pTree->weight);
    }
    return pTree;
}

// Function: freeTree
// Usage: freeTree(encodingTree);
// --------------------------------------------------------
// Deallocates all memory allocated for a given encoding
// tree.

void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->one);
    freeTree(root->zero);
    delete root;
    
}

// Function: buildEncodingMap
// Usage: buildEncodingMap(map, encodingTree);
// -------------------------------------------
// Builds a map between each character in the encoding tree to its
// binary encoding sequence (represented in string form).
//
// This is a wrapper script that setups up a recursive primitive
// that does the tree descent.

void buildEncodingMap(Map<ext_char, string>& map, const Node* tree) {
    recurseTree(map, "", tree);
}

// Function: recurseTree
// Usage: recurseTree(map, "", encodingTree);
// ------------------------------------------
// Adds the binary encoding string pattern for a given character to a map
// once the terminating node for that character is encountered in the
// encoding tree.

void recurseTree(Map<ext_char, string>& map, string soFar, const Node* tree) {
    if (tree == NULL) return;
    
    if (tree->character != NOT_A_CHAR) {
        map[tree->character] = soFar;
    }
    
    if (tree->zero != NULL) recurseTree(map, soFar + "0", tree->zero);
    if (tree->one  != NULL) recurseTree(map, soFar + "1", tree->one);
}

// Function: encodeFile
// Usage: encodeFile(source, encodingTree, output);
// --------------------------------------------------------
// Encodes the given file using the encoding specified by the
// given encoding tree, then writes the result one bit at a
// time to the specified output file.
//
// This function can assume the following:
//
//   - The encoding tree was constructed from the given file,
//     so every character appears somewhere in the encoding
//     tree.
//
//   - The output file already has the encoding table written
//     to it, and the file cursor is at the end of the file.
//     This means that you should just start writing the bits
//     without seeking the file anywhere.

void encodeFile(istream& infile, Node* encodingTree, obstream& outfile) {
    bool emptyFile = true;
    
    // Simplify look ups of a given character's encoding sequence.
    Map<ext_char, string> map;
    buildEncodingMap(map, encodingTree);
    
    ext_char ch;
    ext_char prevCh;
    while ((ch = infile.get()) != EOF) {
        emptyFile = false;
        if (map.containsKey(ch)) {
            string binStr = map[ch];
            for (int i = 0; i < binStr.length(); i++) {
                outfile.writeBit(binStr[i] - '0');
            }
        } else {
            error("encodeFile: No encoding for: " + string(1, ch));
        }
        prevCh = ch;
    }
    
    // Use a psuedo end-of-file character to mark the logical end
    // of the file.  Some filesystems may add padding bytes to the
    // file (which would otherwise confuse the decode logic).
    
    if (!emptyFile && ext_char(prevCh) != PSEUDO_EOF) {
        string eofStr = map[PSEUDO_EOF];
        for (int i = 0; i < eofStr.length(); i++) {
            outfile.writeBit(eofStr[i] - '0');
        }
    }

}

// Function: decodeFile
// Usage: decodeFile(encodedFile, encodingTree, resultFile);
// --------------------------------------------------------
// Decodes a file that has previously been encoded using the
// encodeFile function.  You can assume the following:
//
//   - The encoding table has already been read from the input
//     file, and the encoding tree parameter was constructed from
//     this encoding table.
//
//   - The output file is open and ready for writing.

void decodeFile(ibstream& infile, Node* encodingTree, ostream& file) {
    Node *pNode = encodingTree;
    
    if (encodingTree == NULL) return;
    
    int bit;
    ext_char ch;
    
    while ((bit = infile.readBit()) != EOF) {
        if (pNode == NULL) error("decodeFile: Unexpected null node.");
        
        // The bit tells us which branch of the parent node
        // to descend (i.e., the 'zero' or 'one' branch).
        
        Node *pChild = (bit == 0) ? pNode->zero : pNode->one;
        
        // Can't descend if the branch is null.
        if (pChild == NULL) error("decodeFile: Unexpected null child.");
        
        ch = pChild->character;
        if (ch == NOT_A_CHAR) {
            pNode = pChild;        // Not a leaf node so keep descending.
        } else {
            if (ch == PSEUDO_EOF) break;  // Don't emit our pseudo eof.
            file.put(char(ch));
            pNode = encodingTree;  // Reset for next decoding sequence.
        }
    }
}

// Function: writeFileHeader
// Usage: writeFileHeader(output, frequencies);
// --------------------------------------------------------
// Writes a table to the front of the specified output file
// that contains information about the frequencies of all of
// the letters in the input text.  This information can then
// be used to decompress input files once they've been
// compressed.
//
// This function is provided for you.  You are free to modify
// it if you see fit, but if you do you must also update the
// readFileHeader function defined below this one so that it
// can properly read the data back.

void writeFileHeader(obstream& outfile, Map<ext_char, int>& frequencies) {
    
	// The format we will use is the following:
	//
	// First number: Total number of characters whose frequency is being
	//               encoded.
	// An appropriate number of pairs of the form [char][frequency][space],
	// encoding the number of occurrences.
	//
	// No information about PSEUDO_EOF is written, since the frequency is
	// always 1.
	//
	 
	// Verify that we have PSEUDO_EOF somewhere in this mapping.//
	if (!frequencies.containsKey(PSEUDO_EOF)) {
		error("No PSEUDO_EOF defined.");
	}
	
	// Write how many encodings we're going to have.  Note the space after
	// this number to ensure that we can read it back correctly.
	//
	outfile << frequencies.size() - 1 << ' ';
	
	// Now, write the letter/frequency pairs.//
	foreach (ext_char ch in frequencies) {
		// Skip PSEUDO_EOF if we see it.//
		if (ch == PSEUDO_EOF) continue;
		
		// Write out the letter and its frequency.//
		outfile << char(ch) << frequencies[ch] << ' ';
	}
}

// Function: readFileHeader
// Usage: Map<ext_char, int> freq = writeFileHeader(input);
// --------------------------------------------------------
// Reads a table to the front of the specified input file
// that contains information about the frequencies of all of
// the letters in the input text.  This information can then
// be used to reconstruct the encoding tree for that file.
//
// This function is provided for you.  You are free to modify
// it if you see fit, but if you do you must also update the
// writeFileHeader function defined before this one so that it
// can properly write the data.

Map<ext_char, int> readFileHeader(ibstream& infile) {
    
	// This function inverts the mapping we wrote out in the
	// writeFileHeader function before.  If you make any
	// changes to that function, be sure to change this one
	// too!
	//
	Map<ext_char, int> result;
	
	// Read how many values we're going to read in.//
	int numValues;
	infile >> numValues;
	
	// Skip trailing whitespace.//
	infile.get();
	
	// Read those values in.//
	for (int i = 0; i < numValues; i++) {
		// Get the character we're going to read.//
		ext_char ch = infile.get();
		
		// Get the frequency.//
		int frequency;
		infile >> frequency;
		
		// Skip the space character.//
		infile.get();
		
		// Add this to the encoding table.//
		result[ch] = frequency;
	}
	
	// Add in 1 for PSEUDO_EOF.//
	result[PSEUDO_EOF] = 1;
	return result;
}

// Function: compress
// Usage: compress(infile, outfile);
// --------------------------------------------------------
// Main entry point for the Huffman compressor.  Compresses
// the file whose contents are specified by the input
// ibstream, then writes the result to outfile.  Your final
// task in this assignment will be to combine all of the
// previous functions together to implement this function,
// which should not require much logic of its own and should
// primarily be glue code.

void compress(ibstream& infile, obstream& outfile) {
	// TODO: Implement this!
}

// Function: decompress
// Usage: decompress(infile, outfile);
// --------------------------------------------------------
// Main entry point for the Huffman decompressor.
// Decompresses the file whose contents are specified by the
// input ibstream, then writes the decompressed version of
// the file to the stream specified by outfile.  Your final
// task in this assignment will be to combine all of the
// previous functions together to implement this function,
// which should not require much logic of its own and should
// primarily be glue code.

void decompress(ibstream& infile, ostream& outfile) {
	// TODO: Implement this!
}

