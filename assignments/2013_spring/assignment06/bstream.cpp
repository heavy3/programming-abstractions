 /*
 * File: bstream.cpp
 * -----------------
 * Implementation of ibstream and obstream classes.	 These classes are patterned after
 * (and, in fact, inherit from) the standard ifstream and ofstream classes.	 Please
 * see bstream.h for information about how a client properly uses these classes.
 *
 * Last modified on Mon May 21 19:50:00 PST 2012 by htiek
 */
#include "bstream.h"
#include "error.h"
#include "strlib.h"
#include <iostream>

static const int NUM_BITS_IN_BYTE = 8;

inline int GetNthBit(int n, int fromByte) { return ((fromByte & (1 << n)) != 0); }
inline void SetNthBit(int n, int & inByte) { inByte |= (1 << n); }


/* Constructor ibstream::ibstream
 * ------------------------------
 * Each ibstream tracks 3 integers as private data.	 
 * "lastTell" is streampos of the last byte that was read (this is used
 * to detect when other non-readBit activity has changed the tell)
 * "curByte" contains contents of byte currently being read
 * "pos" is the bit position within curByte that is next to read
 * We set initial state for lastTell and curByte to 0, then pos is
 * set at 8 so that next readBit will trigger a fresh read.
 */
ibstream::ibstream() : istream(NULL), lastTell(0), curByte(0), pos(NUM_BITS_IN_BYTE) {}

/* Member function ibstream::readBit
 * ---------------------------------
 * If bits remain in curByte, retrieve next and increment pos
 * Else if end of curByte (or some other read happened), then read next byte
 * and start reading from bit position 0 of that byte.
 * If read byte from file at EOF, return EOF.
 */
int ibstream::readBit() {
	if (!is_open()) error("Cannot read a bit from a stream that is not open.");
	
	// if just finished bits from curByte or if data read from stream after last readBit()
	if (lastTell != tellg() || pos == NUM_BITS_IN_BYTE) { 
		if ((curByte = get()) == EOF) // read next single byte from file
			return EOF;						
		pos = 0; // start reading from first bit of new byte
			lastTell = tellg();
	}
	int result = GetNthBit(pos, curByte);
	pos++; // advance bit position for next call to readBit
	return result;
}

/* Member function ibstream::rewind
 * ---------------------------------
 * Simply seeks back to beginning of file, so reading begins again
 * from start.
 */
void ibstream::rewind() {
	if (!is_open()) error("Cannot rewind stream which is not open.");
	clear();
	seekg(0, ios::beg);
}

/* Member function ibstream::size
 * ------------------------------
 * Seek to file end and use tell to retrieve position.
 * In order to not disrupt reading, we also record cur streampos and
 * re-seek to there before returning.
 */
long ibstream::size() {
	if (!is_open()) error("Cannot get size of stream which is not open.");
	clear();					// clear any error state
	streampos cur = tellg();	// save current streampos
	seekg(0, ios::end);			// seek to end
	streampos end = tellg();	// get offset
	seekg(cur);					// seek back to original pos
	return long(end);
}

/* Member function ibstream::is_open
 * -------------------------------------------
 * Default implementation of is_open has the stream always
 * open.	Subclasses can customize this if they'd like.
 */
bool ibstream::is_open() {
	return true;
}

/* Constructor obstream::obstream
 * ----------------------------------
 * Each obstream tracks 3 integers as private data.	 
 * "lastTell" is streampos of the last byte that was written (this is used
 * to detect when other non-writeBit activity has changed the tell)
 * "curByte" contains contents of byte currently being written
 * "pos" is the bit position within curByte that is next to write
 * We set initial state for lastTell and curByte to 0, then pos is
 * set at 8 so that next writeBit will start a new byte.
 */
obstream::obstream() : ostream(NULL), lastTell(0), curByte(0), pos(NUM_BITS_IN_BYTE) {}

/* Member function obstream::writeBit
 * ----------------------------------
 * If bits remain to be written in curByte, add bit into byte and increment pos
 * Else if end of curByte (or some other write happened), then start a fresh
 * byte at position 0.
 * We write the byte out for each bit (backing up to overwrite as needed), rather
 * than waiting for 8 bits.	 This is because the client might make
 * 3 writeBit calls and then start using << so we can't wait til full-byte
 * boundary to flush any partial-byte bits.
 */
void obstream::writeBit(int bit) {
	if (bit != 0 && bit != 1) error("writeBit expects argument which can be only 0 or 1.");
	if (!is_open()) error("Cannot writeBit to stream which is not open.");
	
		// if just filled curByte or if data written to stream after last writeBit()
	if (lastTell != tellp() || pos == NUM_BITS_IN_BYTE) { 
		curByte = 0; // zero out byte for next writes
		pos = 0;	// start writing to first bit of new byte	
	}
		
	if (bit) // only need to change if bit needs to be 1 (byte starts already zeroed)
		SetNthBit(pos, curByte);
		
	if (pos == 0 || bit) { // only write if first bit in byte or changing 0 to 1
			if (pos != 0) seekp(-1, ios::cur); // back up to overwite if pos > 0
			put(curByte);
	}
			
	pos++; // advance to next bit position for next write
	lastTell = tellp();
}


/* Member function obstream::size
 * ------------------------------
 * Seek to file end and use tell to retrieve position.
 * In order to not disrupt writing, we also record cur streampos and
 * re-seek to there before returning.
 */
long obstream::size() {
	if (!is_open()) error("Cannot get size of stream which is not open.");
	clear();					// clear any error state
	streampos cur = tellp();	// save current streampos
	seekp(0, ios::end);			// seek to end
	streampos end = tellp();	// get offset
	seekp(cur);					// seek back to original pos
	return long(end);
}

/* Member function obstream::is_open
 * -------------------------------------------
 * Default implementation of is_open has the stream always
 * open.	Subclasses can customize this if they'd like.
 */
bool obstream::is_open() {
	return true;
}

/* Constructor ifbstream::ifbstream
 * -------------------------------------------
 * Wires up the stream class so that it knows to read data
 * from disk.
 */
ifbstream::ifbstream() {
	init(&fb);
}

/* Constructor ifbstream::ifbstream
 * -------------------------------------------
 * Wires up the stream class so that it knows to read data
 * from disk, then opens the given file.
 */
ifbstream::ifbstream(const char* filename) {
	init(&fb);
	open(filename);
}
ifbstream::ifbstream(string filename) {
	init(&fb);
	open(filename);
}

/* Member function ifbstream::open
 * -------------------------------------------
 * Attempts to open the specified file, failing if unable
 * to do so.
 */
void ifbstream::open(const char* filename) {
	if (!fb.open(filename, ios::in | ios::binary))
		setstate(ios::failbit);
}
void ifbstream::open(string filename) {
	open(filename.c_str());
}

/* Member function ifbstream::is_open
 * -------------------------------------------
 * Determines whether the file stream is open.
 */
bool ifbstream::is_open() {
	return fb.is_open();
}

/* Member function ifbstream::close
 * -------------------------------------------
 * Closes the file stream, if one is open.
 */
void ifbstream::close() {
	if (!fb.close())
		setstate(ios::failbit);
}

/* Constructor ofbstream::ofbstream
 * -------------------------------------------
 * Wires up the stream class so that it knows to write data
 * to disk.
 */
ofbstream::ofbstream() {
	init(&fb);
}

/* Constructor ofbstream::ofbstream
 * -------------------------------------------
 * Wires up the stream class so that it knows to write data
 * to disk, then opens the given file.
 */
ofbstream::ofbstream(const char* filename) {
	init(&fb);
	open(filename);
}
ofbstream::ofbstream(string filename) {
	init(&fb);
	open(filename);
}

/* Member function ofbstream::open
 * -------------------------------------------
 * Attempts to open the specified file, failing if unable
 * to do so.
 */
void ofbstream::open(const char* filename) {
	/* Confirm we aren't about to do something that could potentially be a
	 * Very Bad Idea.
	 */
	if (endsWith(filename, ".cpp") || endsWith(filename, ".h") ||
			endsWith(filename, ".hh") || endsWith(filename, ".cc")) {
		cerr << "It is potentially extremely dangerous to write to file "
				 << filename << ", because that might be your own source code.	"
				 << "We're explicitly disallowing this operation.	 Please choose a "
				 << "different filename." << endl;
		setstate(ios::failbit);
		
	} else {
		if (!fb.open(filename, ios::out | ios::binary))
			setstate(ios::failbit);
	}
}
void ofbstream::open(string filename) {
	open(filename.c_str());
}

/* Member function ofbstream::is_open
 * -------------------------------------------
 * Determines whether the file stream is open.
 */
bool ofbstream::is_open() {
	return fb.is_open();
}

/* Member function ofbstream::close
 * -------------------------------------------
 * Closes the given file.
 */
void ofbstream::close() {
	if (!fb.close())
		setstate(ios::failbit);
}

/* Constructor istringbstream::istringbstream
 * -------------------------------------------
 * Sets the stream to use the string buffer, then sets
 * the initial string to the specified value.
 */
istringbstream::istringbstream(string s) {
	init(&sb);
	sb.str(s);
}

/* Member function istringbstream::str
 * -------------------------------------------
 * Sets the underlying string in the buffer to the
 * specified string.
 */
void istringbstream::str(string s) {
	sb.str(s);
}

/* Member function ostringbstream::ostringbstream
 * -------------------------------------------
 * Sets the stream to use the string buffer.
 */
ostringbstream::ostringbstream() {
	init(&sb);
}

/* Member function ostringbstream::str
 * -------------------------------------------
 * Retrives the underlying string data.
 */
string ostringbstream::str() {
	return sb.str();
}
