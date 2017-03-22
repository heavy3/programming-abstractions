/**********************************************************
 * File: HuffmanEncodingTest.cpp
 * Author: Daniel Jackoway (jackoway@cs.stanford.edu)
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * A testing harness and driver for the Huffman encoding
 * assignment.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include "console.h"
#include "simpio.h"
#include "strlib.h"
#include "bstream.h"
#include "HuffmanEncoding.h"
#include "ReferenceHuffmanEncoding.h"
#include "MemoryDiagnostics.h"
using namespace std;

/* Type: MenuEntry
 * Identifying numbers for the menu.	See displayMenu() for descriptions.
 */
enum MenuEntry {
	MANUAL_FREQUENCY_TESTS = 1,
	AUTOMATIC_FREQUENCY_TESTS,
	MANUAL_TREE_TESTS,
	AUTOMATIC_TREE_TESTS,
	MANUAL_ENCODING_TESTS,
	AUTOMATIC_ENCODING_TESTS,
	AUTOMATIC_COMPLETE_TESTS,
	COMPRESS,
	DECOMPRESS,
	COMPARE,
	QUIT,
};

/* Macro: checkCondition
 * ------------------------------------------------------------
 * A utility macro that makes it easier to diagnose and report
 * test failures.
 */
#define checkCondition(expr, reason) doCheckCondition((expr), (reason), #expr, __LINE__)

/* Macro: assertCondition
 * ------------------------------------------------------------
 * A utility macro that checks if a condition is true.	If so,
 * no diagnostic is printed.	If not, an error is reported.
 */
#define assertCondition(expr, reason) doAssertCondition((expr), (reason), #expr, __LINE__);

/* Function: doAssertCondition
 * ------------------------------------------------------------
 * Checks whether the given condition is true.	If not, prints
 * a diagnostic error message.
 */
void doAssertCondition(bool expr, string reason, string expression, int lineNumber) {
	if (!expr) {
		cout << "! FAIL: " << reason << endl;
		cout << "  Test failed on line " << lineNumber << " of HuffmanEncodingTest.cpp" << endl;
		cout << "  Specific expression: " << expression << endl;
		getLine("  Press ENTER to continue...");
	}
}

/* Function: docheckCondition
 * ------------------------------------------------------------
 * Checks whether the given condition is true.	If not, prints
 * a diagnostic error message.
 */
void doCheckCondition(bool expr, string reason, string expression, int lineNumber) {
	if (expr) {
		cout << "   PASS: " << reason << endl;
	} else {
		doAssertCondition(expr, reason, expression, lineNumber);
	}
}

/* Function: beginTest
 * ------------------------------------------------------------
 * Displays a message indicating that a group of tests has
 * started.
 */
void beginTest(string testName) {
	cout << "================== BEGIN: " << testName << "==================" << endl;
}

/* Function: endTest
 * ------------------------------------------------------------
 * Displays a message indicating that a group of tests has
 * started.
 */
void endTest(string testName) {
	cout << "=================== END: " << testName << "===================" << endl;
	getLine("Press ENTER to continue...");
}

/* Function: logInfo
 * ------------------------------------------------------------
 * Displays an informational message.
 */
void logInfo(string message) {
	cout << "	 info: " << message << endl;
}

/* Function: representationOf
 * ------------------------------------------------------------
 * A utility function that, given an ext_char, returns a string
 * representation of that ext_char.
 */
string representationOf(ext_char ch) {
	/* Build up a string representation of this character. */
		ostringstream representation;
		
		/* Pseudo-eof and "Not a character" map to themselves. */
		if (ch == PSEUDO_EOF) {
			representation << "EOF";
		} else if (ch == NOT_A_CHAR) {
			representation << "NAC";
		}
		/* Any character with a visible glyph gets displayed as-is. */
		else if (isgraph(char(ch))) {
			representation << char(ch);
		}
		/* Special cases for types of whitespace. */
		else if (ch == ' ') {
			representation << "\" \"";
	} else if (ch == '\t') {
		representation << "\\t";
	} else if (ch == '\n') {
		representation << "\\n";
	} else if (ch == '\r') {
		representation << "\\r";
	}
	/* Default to the hexadecimal representation. */
	else {
			representation << "0x" << hex << setw(2) << setfill('0') << (int)(unsigned char)ch;
		}
		
		return representation.str();
}


/* Function: validateFrequencyTable
 * --------------------------------------------------------
 * Checks that the generated frequency table for a given
 * stream is correct.
 */
void validateFrequencyTable(ibstream& stream, int length) {
	/* If the length is short, print what string we're testing. */
	const int kMaxDisplayLength = 40;
	if (length <= kMaxDisplayLength) {
		ostringstream contents;
		contents << stream.rdbuf();
		
		/* Seek back to the beginning. */
		stream.rewind();
		
		/* Print a nice message about what's happening. */
		logInfo("Testing string " + contents.str());
	}

	/* Obtain the frequency table using your code. */
	Map<ext_char, int> table = getFrequencyTable(stream);
	
	/* Confirm that the pseudo-eof character was added to the table. */
	checkCondition(table.containsKey(PSEUDO_EOF),
	               "Frequency table should contain PSEUDO_EOF");
	checkCondition(table[PSEUDO_EOF] == 1,
	               "Frequency table should record 1 instance of PSEUDO_EOF");
	
	/* Confirm that all frequencies are valid. */
	int totalFreq = 0;
	bool warnedAboutZero = false; // Prevents warning repeatedly times about zero elements
	
	foreach (ext_char key in table) {
		/* Update total count of the number of characters here. */
		totalFreq += table[key];

		/* Check that the frequencies are always positive. */
		assertCondition(table[key] >= 0, "You should never have negative frequencies. "
						+ representationOf(key) + " has negative value " + integerToString(table[key]));
		
		/* It's fine to have 0 entries in the table, but it can make life much harder later
		 * on.	We put checks in here to make sure that you're aware of this.
		 */
		if (table[key] == 0 && !warnedAboutZero) {
			logInfo("You have a key in your map with frequency 0 ("
					+ representationOf(key) + " = 0). Make sure not to put 0-frequency characters in your tree.");
			warnedAboutZero = true;
		}
	}
	
	/* Make sure that the total is correct. */
	checkCondition(length + 1 == totalFreq,
	               "The sum of all frequencies should be equal to the number of characters of input, plus 1 for PSEUDO_EOF");
	
	/* Check against our reference solution to make sure that all values match. */
	
	/* Seek back to the start of the stream. */
	stream.rewind();
	Map<ext_char, int> referenceTable = referenceGetFrequencyTable(stream);
	
	/* Check for matches in both directions. */
	foreach (ext_char key in referenceTable) {
		assertCondition(referenceTable[key] == table[key], representationOf(key) + " should be "
		                + integerToString(referenceTable[key]) + " but is " + integerToString(table[key]));
	}
	foreach (ext_char key in table) {
		assertCondition(referenceTable[key] == table[key], representationOf(key) + " should be "
		                + integerToString(referenceTable[key]) + " but is " + integerToString(table[key]));
	}
}

/* Function: validateFrequencyTableString
 * --------------------------------------------------------
 * Runs the frequency table test on data specified as a string.
 */
void validateFrequencyTableString(string input, string message) {
	logInfo(message);
	istringbstream stream (input);
	validateFrequencyTable(stream, input.length());
}

/* Function: testGetFrequencyTable
 * --------------------------------------------------------
 * Test code for getFrequencyTable	If you are failing these
 * tests, you likely have an error in your implementation.
 */
void testGetFrequencyTable() {
	beginTest("getFrequencyTable Tests");

	/* Check the frequency table code on a variety of prebuilt strings. */
	validateFrequencyTableString("a", "Testing correct table with no content");
	validateFrequencyTableString("aaaaaaa", "Testing correct table with only one letter");
	validateFrequencyTableString("The quick brown fox jumps over the lazy dog",
	                             "Testing correct table with all of the letters");
	validateFrequencyTableString("#66FF33, #CC0099; #FFFF33. #eaa6ea! #c0f7fe?",
	                             "Testing correct table with all of the letters");


	/* Test the code on a file of 10,000 random characters. */
	{
		logInfo("Testing on a file of 10,000 random bytes.");
		ifbstream stream("test/input/random_10k.test");
		validateFrequencyTable(stream, 10000);
	}
	
	endTest("getFrequencyTable Tests");
}

/* Function: manualGetFrequencyTable
 * --------------------------------------------------------
 * Testing code that lets you manually test your code for
 * getFrequencyTable.
 */
void manualGetFrequencyTable() {
	beginTest("Manual getFrequencyTable Test");
	cout << "Enter strings below to see the frequency table constructed by your" << endl;
	cout << "getFrequencyTable function.	Enter the empty string to quit." << endl;

	while (true) {
		/* Grab the line of text to analyze, stopping if it's empty. */
		string text = getLine("Text (enter to stop test): ");
		if (text == "") break;

		/* Build an istringstream holding the data. */
		istringstream source(text);
		Map<ext_char, int> frequencies = getFrequencyTable(source);
		
		/* Print the map. */
		foreach (ext_char ch in frequencies) {
			cout << setw(4) << representationOf(ch) << ": " << frequencies[ch] << endl;
		}
	}
}


/* Function: treeCost
 * --------------------------------------------------------
 * Computes the total number of bits required to build up
 * a complete encoding of the input given the specified
 * encoding tree.	 This function can be used to check
 * whether your encoding tree is correct.
 */
int treeCost(Node* root, int depth = 0) {
	/* We should never run this on a NULL tree. */
	assertCondition(root != NULL, "NULL tree!  This will almost certainly crash.");

	/* If this is a leaf node, the bits required is equal to the
	 * depth of this node times the number of characters of this
	 * size.
	 */
	if (root->zero == NULL && root->one == NULL)
		return root->weight * depth;
		
	/* Otherwise, find the cost of the zero and one subtrees, which are
	 * at depth one greater than the current depth.
	 */
	return treeCost(root->zero, depth + 1) + treeCost(root->one, depth + 1);
}

/* Function: recCheckTreeCorrectness
 * --------------------------------------------------------
 * Recursively checks the structure of an encoding tree to
 * confirm that it's correct.
 */
void recCheckTreeCorrectness(Node* root, Map<ext_char, int>& frequencies) {
	/* We should never be given a null tree. */
	assertCondition(root != NULL, "Encoding tree should be non-NULL.");
	
	/* Either both children are null or both children are non-NULL. */
	assertCondition((root->zero == NULL && root->one == NULL) ||
					(root->zero != NULL && root->one != NULL),
					"All nodes should either have 0 or 2 children.");
	
	/* If this node is internal, it should not hold a character. */
	assertCondition((root->character == NOT_A_CHAR) ==
					(root->zero != NULL && root->one != NULL),
					"All internal nodes should not store characters.");
									
	/* No two children should point to the same node. */
	assertCondition(root->zero == NULL || root->zero != root->one,
					"No internal node should have the same children on both sides.");
									
	/* If we have children, the weight of this node should be the sum of the weight
	 * of the children.
	 */
	assertCondition(root->zero == NULL || root->weight == root->zero->weight + root->one->weight,
					"Each interal node should have weight equal to the sum of its children.");
									
	/* If this is a leaf, it should be in the map. */
	assertCondition(root->character == NOT_A_CHAR ||
					frequencies.containsKey(root->character),
					"Character not present in the frequency table is in the encoding tree?");
																		
	/* If this is a leaf, the frequency should match what's expected. */
	assertCondition(root->character == NOT_A_CHAR || 
	                frequencies[root->character] == root->weight,
	                "Weight in the tree should match weight in the table.");
									
	/* Mutate the map by removing this character from it.	 This helps us detect
	 * duplicates.
	 */
	frequencies.remove(root->character);
									
	/* Recurse on children if we have them. */
	if (root->zero) recCheckTreeCorrectness(root->zero, frequencies);
	if (root->one)	recCheckTreeCorrectness(root->one,	frequencies);
									
}

/* Function: recCheckTreesEqual
 * --------------------------------------------------------
 * Recursively checks that two encoding trees are equal.
 * This is used to determine whether or not we get consistent
 * encoding trees from a single distribution.
 */
bool recCheckTreesEqual(Node* first, Node* second) {	
	/* Both trees should either be null or non-null. */
	assertCondition((first == NULL) == (second == NULL),
	                "Both trees must be null or non-null.");
	
	/* If either tree is null, stop. */
	if (first == NULL && second == NULL) return true;
	if (first == NULL || second == NULL) return false;
	
	/* Confirm weights and characters match. */
	assertCondition(first->weight == second->weight,
	                "Two encoding trees have different weights.");
	assertCondition(first->character == second->character,
	                "Two encoding trees have different characters.");
									
	/* Recursively check children.	Note the use of the non-short-circuiting
	 * AND here.
	 */
	return recCheckTreesEqual(first->zero, second->zero) &
	       recCheckTreesEqual(first->one,  second->one);
}

/* Function: testBuildEncodingTree
 * --------------------------------------------------------
 * Tests your buildEncodingTree function on a variety of
 * inputs.	If your code fails these tests, you probably
 * have an error in either your buildEncodingTree or
 * getFrequencyTable functions.
 */
void testBuildEncodingTree() {
	beginTest("buildEncodingTree tests");
	
	/* This first test builds an encoding tree when there are two nodes - the EOF marker
	 * and the character A.	 These checks are mostly structural.	If you are failing this
	 * test, it probably means that your code to merge nodes is not working correctly, or
	 * that you are setting up the nodes incorrectly.
	 */
	{
		logInfo("Checking for correct tree on a single letter");
		Map<ext_char, int> frequencies;
		frequencies[PSEUDO_EOF] = 1;
		frequencies['A'] = 1;
		Node *tree = buildEncodingTree(frequencies);
		
		/* Check the root node. */
		checkCondition(tree != NULL, "Tree should be nonempty.");
		checkCondition(tree->character == NOT_A_CHAR, "Tree root does not hold a character.");
		checkCondition(tree->zero != NULL && tree->one != NULL, "Tree should have two children.");
		checkCondition(tree->weight == 2, "Tree should have weight two (one from each child).");
		
		/* Now, go and check the children.	One child should be 'A' and the other should be EOF.
		 * We'll figure out which is which and act accordingly.
		 */
		checkCondition(tree->zero->character == PSEUDO_EOF || tree->one->character == PSEUDO_EOF,
		               "One child should be pseudo-eof.");
		checkCondition(tree->zero->character == 'A' || tree->one->character == 'A',
		               "One child should be A.");

	logInfo("If either of the previous tests failed, this is almost certainly going to crash.");	 
		Node*		aChild = (tree->zero->character == 'A'? tree->zero : tree->one);
		Node* eofChild = (tree->zero->character == 'A'? tree->one : tree->zero);
		
		/* Confirm that these nodes are valid. */
		checkCondition(aChild->zero == NULL && aChild->one == NULL,
		               "Singleton node for A should have no children.");
		checkCondition(aChild->weight == 1, "Singleton for A should have weight 1.");
		checkCondition(eofChild->zero == NULL && eofChild->one == NULL,
		               "Singleton node for EOF should have no children.");
		checkCondition(eofChild->weight == 1, "Singleton for EOF should have weight 1.");
		
		delete aChild;
		delete eofChild;
		delete tree;
	}
	
	/* This next test builds a much more complex string and then does a walk of the tree to
	 * confirm that basic invariants hold throughout.
	 */
	{
		/* No merges in this string ever produce a duplicate. */
		istringstream stream("ABBCCCCDDDDDDDDEEEEEEEEEEEEEEEE");
		Map<ext_char, int> frequencies = referenceGetFrequencyTable(stream);
		Node* tree = buildEncodingTree(frequencies);
		
		logInfo("About to recursively check the tree for correctness.");
		logInfo("If you don't see any messages here, it means that the tree");
		logInfo("is structurally correct (even if it's the wrong tree.)");
		recCheckTreeCorrectness(tree, frequencies);
		checkCondition(frequencies.isEmpty(), "All letters accounted for.");
		
		/* LEAK MEMORY here, since we don't know whether or not freeTree is correct. */
	}
	
	/* This next test builds a much more complex string and then does a walk of the tree to
	 * confirm that basic invariants hold throughout.
	 */
	{
		/* We will get all sorts of duplicates here. */
		istringstream stream("ABBCCCDDDDDEEEEEEEEFFFFFFFFFFFFF");
		Map<ext_char, int> frequencies = referenceGetFrequencyTable(stream);
		Node* tree = buildEncodingTree(frequencies);
		
		logInfo("About to recursively check the tree for correctness.");
		logInfo("If you don't see any messages here, it means that the tree");
		logInfo("is structurally correct (even if it's the wrong tree.)");
		recCheckTreeCorrectness(tree, frequencies);
		checkCondition(frequencies.isEmpty(), "All letters accounted for.");
		
		/* LEAK MEMORY here, since we don't know whether or not freeTree is correct. */
	}
	
	/* This next test computes how many bits your encoding tree will require for the
	 * overall construction.	Since there are many valid encoding trees for the same
	 * sets of data, we will check your tree against our reference implementation.
	 */
	{
		istringstream stream("ABBCCCCDDDDDDDDEEEEEEEEEEEEEEEE");
		Map<ext_char, int> frequencies = referenceGetFrequencyTable(stream);
		Node* expected = referenceBuildEncodingTree(frequencies);
		Node* theirs = buildEncodingTree(frequencies);
		checkCondition(treeCost(expected) == treeCost(theirs), "Trees should require proper number of bits for simple text input");
	}
	{
		istringstream stream("ABBCCCDDDDDEEEEEEEEFFFFFFFFFFFFF");
		Map<ext_char, int> frequencies = referenceGetFrequencyTable(stream);
		Node* expected = referenceBuildEncodingTree(frequencies);
		Node* theirs = buildEncodingTree(frequencies);
		checkCondition(treeCost(expected) == treeCost(theirs), "Trees should require proper number of bits for harder text input");
	}
	{
		istringstream stream("0123AABBCCDD");
		Map<ext_char, int> frequencies = referenceGetFrequencyTable(stream);
		Node* expected = referenceBuildEncodingTree(frequencies);
		Node* theirs = buildEncodingTree(frequencies);
		checkCondition(treeCost(expected) == treeCost(theirs), "Trees should require proper number of bits for even harder text input");
	}

	{
		ifbstream stream("test/input/random_10k.test");
		Map<ext_char, int> frequencies = referenceGetFrequencyTable(stream);
		Node* expected = referenceBuildEncodingTree(frequencies);
		Node* theirs = buildEncodingTree(frequencies);
		checkCondition(treeCost(expected) == treeCost(theirs), "Trees should require proper number of bits for random bytes input");
	}

	
	/* Now, do checks for memory leaks.	 We'll build some small trees and see what happens
	 * when we call freeTree.
	 */
	{
		/* Count how many total allocations and deallocations have been done right now. */
		long disparity = numAllocations() - numDeallocations();

		
		/* Allocate a node, set it up, and free it. */
		Node* freeMe = new Node;
		freeMe->zero = freeMe->one = NULL;
		freeMe->character = PSEUDO_EOF;
		freeMe->weight = 1;
        
		/* Free the node. */
		freeTree(freeMe);
        
		/* Confirm total disparity is the same as before. */
		checkCondition(numAllocations() - numDeallocations() == disparity,
		               "Number of allocations/deallocations matches for a single node.");
	}
	
	{
	/* Count how many total allocations and deallocations have been done right now. */
		long disparity = numAllocations() - numDeallocations();
		
        ifbstream stream("test/input/random_10k.test");
		Map<ext_char, int> frequencies = referenceGetFrequencyTable(stream);
        
		Node* tree = buildEncodingTree(frequencies);

		/* Free the node. */
		freeTree(tree);
        
        //disparity = numAllocations() - numDeallocations();
        //cout << "after free disparity: " << disparity << endl;
        

		/* Confirm total disparity is the same as before. */
		checkCondition(numAllocations() - numDeallocations() == disparity,
		               "Number of allocations/deallocations matches for a huge tree.");
	}

	/* This next step verifies that the encoding function creates the same tree each time.
	 * The input given has a lot of possible trees that could be built.
	 */
	{
		istringstream stream("0123AABBCCDD");
		Map<ext_char, int> frequencies = referenceGetFrequencyTable(stream);
		Node* first = buildEncodingTree(frequencies);
		Node* second = buildEncodingTree(frequencies);
		Node* third = buildEncodingTree(frequencies);		 
		checkCondition(recCheckTreesEqual(first, second), "Encoding trees should be the same.");
		checkCondition(recCheckTreesEqual(second, third), "Encoding trees should be the same.");
		checkCondition(recCheckTreesEqual(third, first),  "Encoding trees should be the same.");
	}

	endTest("buildEncodingTree tests");
}

/* Function: printEncodings
 * --------------------------------------------------------
 * Recursively walks over an encoding tree, printing out all
 * the encodings it contains.
 */
void printEncodings(Node* root, string code = "") {
	/* If the tree is empty, there is nothing to display. */
	if (root == NULL) return;
	
	/* If this node holds a character, print its encoding. */
	if (root->character != NOT_A_CHAR) {
		cout << setw(4) << representationOf(root->character) << ": " << code << endl;
	}
	/* Otherwise, recursively print out the encodings of its children. */
	else {
		printEncodings(root->zero, code + "0");
		printEncodings(root->one, code + "1");
	}
}

/* Function: manualBuildEncodingTree
 * --------------------------------------------------------
 * Allows you to manually test and explore the encoding
 * trees constructed by your buildEncodingTree function.
 */
void manualBuildEncodingTree() {
	beginTest("Manual buildEncodingTree Tests");
	cout << "You can enter strings below to call your buildEncodingTree function on them." << endl;
	cout << "We will display the generated prefix code for the text you've entered." << endl;
	while (true) {
		/* Get the test to try out, terminating if there's nothing. */
		string text = getLine("Text (enter to stop test): ");
		if (text == "") break;
		
		/* Take a snapshot of how many allocations have been done. */
		long difference = numAllocations() - numDeallocations();
		
		/* Wrap the text in an istringstream and process it. */
		istringstream source(text);
		Map<ext_char, int> table = getFrequencyTable(source);
		Node* tree = buildEncodingTree(table);
		printEncodings(tree);
		
		/* Free memory and report if there were any leaks. */
		freeTree(tree);
		difference = (numAllocations() - numDeallocations()) - difference;
		if (difference > 0) {
			cout << "Memory leak detected!	Just leaked " << difference << " node(s)." << endl;
		} else if (difference < 0) {
			cout << "Double free detected!	Freed " << -difference << " too many node(s)." << endl;
		}
	}
}

/* Function: testEncoding
 * --------------------------------------------------------
 * A function to check that the encodeFile function works
 * correctly.	 These tests look for things like the following:
 *
 *	 1. Is the length of the generated file correct?
 *	 2. Does encoding the same file multiple times produce
 *			the same file each time?
 *	 3. Does decoding the same file multiple times produce
 *			the original input?
 *
 * This code assumes that your getFrequencyTable and
 * buildEncodingTree functions are correct.	 If this is not
 * the case, these tests will not work correctly.
 */
void testEncoding() {
	beginTest("encodeFile / decodeFile Tests");
	
	/* Construct a list of test files to run your code on. */
	Vector<string> files;
	files += "singleChar", "nonRepeated", "alphaOnce", "allRepeated", "fibonacci", "poem", "allCharsOnce", "tomSawyer", "dikdik.jpg", "random";
	
	foreach (string file in files) {
		logInfo("Testing encoding and decoding on file test/encodeDecode/" + file);
	
		ifbstream input("test/encodeDecode/" + file);
		assertCondition(input.is_open(), ("Cannot open file test/encodeDecode/" + file + " for reading!"));
	
		/* Extract basic information about the file. */
		Map<ext_char, int> frequency = getFrequencyTable(input);
		input.rewind();
		
		/* Build the encoding tree. */
		Node* encodingTree = buildEncodingTree(frequency);
	
		/* Basic test: Let's confirm that the size of the generated
		 * file matches what the theory predicts it should be.	This is
		 * the number of bytes required to hold the given number of bits.
		 */
		int expectedBytes = (treeCost(encodingTree) + 7) / 8;
		
		{
			/* Encode the file once. */
			ostringbstream compressed;
			encodeFile(input, encodingTree, compressed);
			input.rewind();
			
			checkCondition(compressed.size() == expectedBytes,
			               "Expected " + integerToString(expectedBytes) + " bytes, "
			               "got " + integerToString(compressed.size()) + " bytes.");
		}
				
		/* Confirm that we get the same thing coming back as what we started with. */
		{
			ostringstream fileContents;
			fileContents << input.rdbuf();
			input.rewind();
		
			ostringbstream compressed;
			encodeFile(input, encodingTree, compressed);
			input.rewind();
			
			istringbstream toDecompress(compressed.str());
			ostringbstream decompressed;
			decodeFile(toDecompress, encodingTree, decompressed);
			checkCondition(fileContents.str() == decompressed.str(),
			               "Encoding then decoding should get back the original file.");
		}
	}
	
	endTest("encodeFile / decodeFile Tests");
}

/* Function: testCompleteStack
 * --------------------------------------------------------
 * This test will run your compress and decompress functions
 * in their entirety.	 If your previous tests are passing but
 * this test fails, it is likely that you have an error in the
 * glue code that connects everything together.
 */
void testCompleteStack() {
	beginTest("Complete Stack Tests");
	
	/* Construct a list of files to run your compression/decompression functions on. */
	Vector<string> files;
	files += "singleChar", "nonRepeated", "alphaOnce", "allRepeated", "fibonacci", "poem", "allCharsOnce", "tomSawyer", "dikdik.jpg", "random";
	
	foreach (string file in files) {
		logInfo("Testing compress and decompress on file test/encodeDecode/" + file);
		
		/* Take a snapshot of total memory usage. */
		long difference = numAllocations() - numDeallocations();
	
		/* Open the file to compress. */
		ifbstream input("test/encodeDecode/" + file);
		assertCondition(input.is_open(), ("Cannot open file test/encodeDecode/" + file + " for reading!"));
	
		/* Read the file into memory. */
		ostringstream originalData;
		originalData << input.rdbuf();
		input.rewind();
	
		/* Compress the file into an ostringbstream so that we hold it in RAM. */
		ostringbstream result;
		compress(input, result);
		
		/* Decompress the input from memory. */
		istringbstream compressedData(result.str());
		ostringbstream decompressedData;
		decompress(compressedData, decompressedData);
		
		/* Confirm that it matches. */
		checkCondition(originalData.str() == decompressedData.str(),
		               "Compressed/decompressed data matches.");
        cout << "numAllocs: " << numAllocations() << endl;
        cout << "numDeallocs: " << numDeallocations() << endl;
		checkCondition(numAllocations() - numDeallocations() == difference,
		               "No tree nodes leaked.");
	}
	
	endTest("Complete Stack Tests");
}

/* Function: printBits
 * --------------------------------------------------------
 * Given a string, prints the bits of that string one at a
 * time.
 */
void printBits(string str, int maxBits) {
	/* Wrap your string in an istringbstream so we can do bit-by-bit
	 * I/O.
	 */
	istringbstream stream(str);
	for (int i = 0; i < maxBits; i++) {
		int bit = stream.readBit();
		if (bit == EOF) break;
		cout << bit;
	}
	cout << endl;
}

/* Function: manualEncodeDecode
 * --------------------------------------------------------
 * Function to let you manually test running encode and
 * decode on a given string.
 */
void manualEncodeDecode() {
	beginTest("Manual encodeFile / decodeFile Tests");
	
	cout << "Enter text for us to encode and then decode" << endl;
	while (true) {
		string text = getLine("Text (enter to stop test): ");
		if (text == "") break;
		
		/* Build an istringstream holding the data. */
		istringbstream source(text);

		/* Construct the encoding tree for the text. */
		Map<ext_char, int> frequencies = getFrequencyTable(source);
		Node* tree = buildEncodingTree(frequencies);
		ostringbstream compressed;
		
		/* Encode the text. */
		source.rewind();
		encodeFile(source, tree, compressed);
		
		/* Print out the compressed representation. */
		cout << "Compressed representation: " << endl;
		printBits(compressed.str(), treeCost(tree));
		
		/* No need to generate new map/tree because we know we'd read same map that we wrote,
		 * meaning we'd also get the same tree (assuming tree function is deterministic.)
		 */
		istringbstream compressedIn(compressed.str());
		ostringstream unpacked;
		decodeFile(compressedIn, tree, unpacked);
		checkCondition(unpacked.str() == text, "Result of compressing and decompressing should be the original input.");
		
		/* Clean up memory. */
		freeTree(tree);
	}
}

/* Function: openFile
 * --------------------------------------------------------
 * Prompts the user for the name of a file to open, opening
 * the indicated stream object to that file.
 */
template <typename FileStream>
void openFile(FileStream& file, string prompt = "") {
	while (true) {
		string filename = getLine(prompt);
		file.open(filename.c_str());
		
		if (file.is_open()) return;
		
		cout << "Sorry, I couldn't open that file." << endl;
		file.clear();
	}
}

/* Function: runCompress
 * --------------------------------------------------------
 * Harness to run your compress function on actual files.
 */
void runCompress() {
	/* Open a file for reading. */
	ifbstream infile;
	openFile(infile, "File to compress: ");

	/* Open a file for writing. */
	ofbstream outfile;
	openFile(outfile, "Filename for compressed output: ");
		
	/* Compress the file. */
	cout << "Compressing... " << flush;
	compress(infile, outfile);
	cout << "done!" << endl << endl;
		
	/* Report statistics. */
	cout << "Original file size: " << infile.size() << "B" << endl;
	cout << "New file size:      " << outfile.size() << "B" << endl;
	cout << "Compression ratio:  " << double(outfile.size()) / infile.size() << endl << endl;
	getLine("Press ENTER to continue...");
}

/* Function: runDecompress
 * --------------------------------------------------------
 * Harness code to run your decompression routine. */
void runDecompress() {
	ifbstream infile;
	openFile(infile, "File to decompress: ");

	/* Get the name of the output file. */
	ofbstream outfile;
	openFile(outfile, "Name of file to write result: ");
		
	/* Decompress the file. */
	decompress(infile, outfile);
	cout << "Decompressed file written!" << endl;
	getLine("Press ENTER to continue...");
}

/* Function: compareFiles
 * --------------------------------------------------------
 * Compares two files byte-by-byte to determine whether or
 * not they are equivalent to one another.
 */
void compareFiles() {
	/* Get the two files to compare. */
	ifstream one, two;
	openFile(one, "First file to compare:  ");
	openFile(two, "Second file to compare: ");
	
	/* Read the contents of the files into a stringstream for
	 * comparison.
	 */
	ostringstream oneContents, twoContents;
	oneContents << one.rdbuf();
	twoContents << two.rdbuf();
	
	const string oneString = oneContents.str(), twoString = twoContents.str();
	
	/* Check lengths are the same. */
	if (oneString.length() != twoString.length()) {
		cout << "Files differ!" << endl;
		cout << "File one has length " << oneString.length() << "." << endl;
		cout << "File two has length " << twoString.length() << "." << endl;
	} else {
		/* Compare the two sequences to find a mismatch. */
		pair<string::const_iterator, string::const_iterator> diff =
			mismatch(oneString.begin(), oneString.end(), twoString.begin());
		if (diff.first != oneString.end()) {
			cout << "Files differ!" << endl;
			ptrdiff_t offset = diff.first - oneString.begin();
			cout << "Bytes differ at offset " << offset << "." << endl;
			cout << "File one has value " << representationOf(*diff.first) << endl;
			cout << "File two has value " << representationOf(*diff.second) << endl;
		} else {
			/* Files match! */
			cout << "Files match!" << endl;
		}
	}
	getLine("Press ENTER to continue...");
}

/* Function: displayMenu
 * --------------------------------------------------------
 * Displays the main menu of options.
 */
void displayMenu() {
	cout << "CS106B Huffman Encoding Testing Harness" << endl;
	cout << "=====================================" << endl; 
	cout << setw(2) << MANUAL_FREQUENCY_TESTS << ": Manually test getFrequencyTable" << endl;
	cout << setw(2) << AUTOMATIC_FREQUENCY_TESTS << ": Automatically test getFrequencyTable" << endl;
	cout << setw(2) << MANUAL_TREE_TESTS << ": Manually test buildEncodingTree" << endl;
	cout << setw(2) << AUTOMATIC_TREE_TESTS << ": Automatically test buildEncodingTree" << endl;
	cout << setw(2) << MANUAL_ENCODING_TESTS << ": Manually test encodeFile/decodeFile" << endl;
	cout << setw(2) << AUTOMATIC_ENCODING_TESTS << ": Automatically test encodeFile/decodeFile" << endl;
	cout << setw(2) << AUTOMATIC_COMPLETE_TESTS << ": Automatically test compress/decompress" << endl;
	cout << setw(2) << COMPRESS << ": Compress a file" << endl;
	cout << setw(2) << DECOMPRESS << ": Decompress a compressed file" << endl;
	cout << setw(2) << COMPARE << ": Compare two files for equality" << endl;
	cout << setw(2) << QUIT << ": Quit" << endl;
}

/* Function: testEnvironment
 * --------------------------------------------------------
 * Self-test to ensure that the environment has been set up correctly.  If this
 * test fails, contact Keith as soon as possible to let him know!
 */
void testEnvironment() {
  /* Confirm that char is unsigned. */
  if (std::numeric_limits<char>::is_signed) {
    cerr << "============= ENVIRONMENT ERROR =============" << endl;
    cerr << "Something is wrong with this project setup." << endl;
    cerr << "This is our fault!  Please contact the course" << endl;
    cerr << "staff as soon as possible to let them know" << endl;
    cerr << "that you're seeing this error." << endl;
    cerr << "=============================================" << endl;
    cerr << "Error: char is signed in encoding test."  << endl;
    cerr << "=============================================" << endl;
    getLine("Press ENTER to quit...");
    exit(-1);
  }
}

/* Displays the menu and drives the testing code. */
int main() {
  testEnvironment();
  testReferenceSolutionConfiguration();

	while (true) {
		displayMenu();
		
		switch (getInteger("Enter choice: ")) {
			case COMPRESS:
				runCompress();
				break;
			case DECOMPRESS:
				runDecompress();
				break;
			case AUTOMATIC_FREQUENCY_TESTS:
				testGetFrequencyTable();
				break;
			case MANUAL_FREQUENCY_TESTS:
				manualGetFrequencyTable();
				break;
			case AUTOMATIC_TREE_TESTS:
				testBuildEncodingTree();
				break;
			case MANUAL_TREE_TESTS:
				manualBuildEncodingTree();
				break;
			case MANUAL_ENCODING_TESTS:
				manualEncodeDecode();
				break;
			case AUTOMATIC_ENCODING_TESTS:
				testEncoding();
				break;
			case AUTOMATIC_COMPLETE_TESTS:
				testCompleteStack();
				break;
			case COMPARE:
				compareFiles();
				break;
			case QUIT:
				return 0;
			default:
				cout << "Sorry, but I don't know how to do that." << endl;
				break;
		}
	}
	return 0;
}

