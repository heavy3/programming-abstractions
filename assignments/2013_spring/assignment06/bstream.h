/*
 * File: bstream.h
 * Last modified on Mon May 21 16:06:35 PST 2012 by htiek
 * ----------------------------------------------------------
 * Defines the ibstream and obstream classes which are basically
 * same as the ordinary istream and ostream classes, but add the 
 * functionality to read and write one bit at a time. 
 * 
 * The idea is that you can substitute an ibstream in place of an
 * istream and use the same operations (get, fail, >>, etc.) 
 * along with added member functions of readBit, rewind, and size.
 *
 * Similarly, the obstream can be used in place of ofstream, and has
 * same operations (put, fail, <<, etc.) along with additional
 * member functions writebit and size.
 *
 * There are two subclasses of ibstream: ifbstream and istringbstream,
 * which are similar to the ifstream and istringstream classes.	 The
 * obstream class similarly has ofbstream and ostringbstream as
 * subclasses.
 */

#ifndef _bstream_h
#define _bstream_h

#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
using namespace std;

/*
 * Class: ibstream
 * ---------------
 * Defines a class for reading files with all the functionality of istream
 * along with an added member function for reading a single bit and convenience
 * functions for rewinding the stream back to the beginning and getting the stream
 * size.
 *
 * You will probably not create instances of this class directly.	 Instead, you
 * will create ifbstreams or istringbstreams to read from files or string buffers.
 */
class ibstream: public istream {
public:
	/*
	 * Constructor: ibstream
	 * Usage: ibstream stream;
	 * -----------------------
	 * Initializes a new ibstream that is not attached to any source.	 You are
	 * unlikely to use this function directly.
	 */
	ibstream();	
	
	/*
	 * Member function: readBit
	 * Usage: bit = in.readBit();
	 * --------------------------
	 * Reads a single bit from the ibstream and returns 0 or 1 depending on
	 * the bit value.	 If the stream is exhausted, EOF (-1) is returned.
	 * Raises an error if this ibstream has not been properly opened.
	 */
	int readBit();
	
	/*
	 * Member function: rewind
	 * Usage: in.rewind();
	 * -------------------
	 * Rewinds the ibstream back to the beginning so that subsequent reads
	 * start again from the beginning.	Raises an error if this ibstream 
	 * has not been properly opened.
	 */
	void rewind();
	
	/*
	 * Member function: size
	 * Usage: sz = in.size();
	 * ----------------------
	 * Returns the size in bytes of the data attached to this stream.
	 * Raises an error if this ibstream has not been properly opened.
	 */
	long size();
	
	/*
	 * Member function: is_open()
	 * Usage: if (ibs.is_open()) { ... }
	 * ----------------------
	 * Returns whether or not this ibstream is opened.	This only has
	 * meaning if the ibstream is a file stream; otherwise it always
	 * returns true.
	 */
	virtual bool is_open();
	
private:
	int pos, curByte;
	streampos lastTell;
};


/*
 * Class: obstream
 * ---------------
 * Defines a class for writing files with all the functionality of ostream
 * along with an added member function for writing a single bit and a convenience
 * function for getting the stream size.
 *
 * You are unlikely to instantiate this class directly; instead, instantiate one
 * of the subclasses.
 */

class obstream: public ostream {
public:
	/*
	 * Constructor: obstream
	 * Usage: obstream outfile;
	 * ------------------------
	 * Initializes a new obstream that is not attached to any file.	 Use the
	 * open member function from ofstream to attach the stream to a file.
	 */
		obstream();

	/*
	 * Member function: writeBit
	 * Usage: out.writeBit(1);
	 * -----------------------
	 * Writes a single bit to the obstream.
	 * Raises an error if this ibstream has not been properly opened.
	 */
	void writeBit(int bit);
	
	/*
	 * Member function: size
	 * Usage: sz = in.size();
	 * ----------------------
	 * Returns the size in bytes of the file attached to this stream.
	 * Raises an error if this obstream has not been properly opened.
	 */
	long size();
	
	/*
	 * Member function: is_open()
	 * Usage: if (ibs.is_open()) { ... }
	 * ----------------------
	 * Returns whether or not this obstream is opened.	This only has
	 * meaning if the obstream is a file stream; otherwise it always
	 * returns true.
	 */
	virtual bool is_open();
	
private:
	int pos, curByte;
	streampos lastTell;
};

/*
 * Class: ifbstream
 * ---------------
 * A class for reading files in all of the usual ways, plus bit-by-bit.
 * You can treat this class like a normal ifstream, except that there is
 * extra support for bit-level operations.
 */
 
class ifbstream: public ibstream {
public:
	/*
	 * Constructor: ifbstream();
	 * Usage: ifbstream ifb;
	 * -------------------------
	 * Constructs a new ifbstream not attached to any file.	 You can
	 * open a file for reading using the .open() member functions.
	 */
	ifbstream();
	
	/*
	 * Constructor: ifbstream(const char* filename);
	 * Constructor: ifbstream(string filename);
	 * Usage: ifbstream ifb("filename");
	 * -------------------------
	 * Constructs a new ifbstream that reads the specified file, if
	 * it exists.	 If not, the stream enters an error state.
	 */
	ifbstream(const char* filename);
	ifbstream(string filename);
	
	/*
	 * Member function: open(const char* filename);
	 * Member function: open(string filename);
	 * Usage: ifb.open("my-file.txt");
	 * -------------------------
	 * Opens the specified file for reading.	If an error occurs, the
	 * stream enters a failure state, which can be detected by calling
	 * ifb.fail().
	 */
	void open(const char* filename);
	void open(string filename);
	
	/*
	 * Member function: is_open();
	 * Usage: if (ifb.is_open()) { ... }
	 * --------------------------
	 * Returns whether or not this ifbstream is connected to a file for
	 * reading.
	 */
	bool is_open();
	
	/*
	 * Member function: close();
	 * Usage: ifb.close();
	 * --------------------------
	 * Closes the currently-opened file, if the stream is open.	 If the
	 * stream is not open, puts the stream into a fail state.
	 */
	void close();
	
private:
	/* The actual file buffer which does reading and writing. */
	filebuf fb;
};

/*
 * Class: ofbstream
 * ---------------
 * A class for writing files in all of the usual ways, plus bit-by-bit.
 * You can treat this class like a normal ofstream, except that there is
 * extra support for bit-level operations.
 *
 * As a safety feature, you cannot use ofbstream to open files that end
 * in .h, .hh, .cpp, or .cc for writing, as this could very easily cause
 * you to destroy your source files.
 */

class ofbstream: public obstream {
public:
	/*
	 * Constructor: ofbstream();
	 * Usage: ofbstream ofb;
	 * -------------------------
	 * Constructs a new ofbstream not attached to any file.	 You can
	 * open a file for writing using the .open() member functions.
	 */
	ofbstream();
	
	/*
	 * Constructor: ofbstream(const char* filename);
	 * Constructor: ofbstream(string filename);
	 * Usage: ofbstream ofb("filename");
	 * -------------------------
	 * Constructs a new ofbstream that writes the specified file, if
	 * it exists.	 If not, the stream enters an error state.	Read
	 * the documentation on "open" for more details.
	 */
	ofbstream(const char* filename);
	ofbstream(string filename);
	
	/*
	 * Member function: open(const char* filename);
	 * Member function: open(string filename);
	 * Usage: ofb.open("my-file.txt");
	 * -------------------------
	 * Opens the specified file for writing.	If an error occurs, the
	 * stream enters a failure state, which can be detected by calling
	 * ifb.fail().	If an invalid filename is specified (for example,
	 * a source file), reports an error.
	 */
	void open(const char* filename);
	void open(string filename);
	
	/*
	 * Member function: is_open();
	 * Usage: if (ofb.is_open()) { ... }
	 * --------------------------
	 * Returns whether or not this ofbstream is connected to a file for
	 * reading.
	 */
	bool is_open();
	
	/*
	 * Member function: close();
	 * Usage: ifb.close();
	 * --------------------------
	 * Closes the currently-opened file, if the stream is open.	 If the
	 * stream is not open, puts the stream into a fail state.
	 */
	void close();

private:
	/* The actual file buffer which does reading and writing. */
	filebuf fb;
};

/*
 * Class: istringbstream
 * ---------------
 * A variant on C++'s istringstream class, which acts as a stream that
 * reads its data from a string.	This is mostly used by the testing
 * code to test your Huffman encoding without having to read or write
 * files on disk, but you can use it in your own testing if you would
 * like.
 */
class istringbstream: public ibstream {
public:
	/* Constructor: istringbstream(string s = "");
	 * Usage: istringbstream stream;
	 * --------------------------
	 * Constructs an istringbstream reading the specified string.
	 */
	istringbstream(string s = "");
	
	/* Member Function: str(string s);
	 * Usage: isb.str("This is some text!");
	 * ---------------------------
	 * Sets the underlying string of the istringbstream.
	 */
	void str(string s);	
private:
	/* The actual string buffer that does character storage. */
	stringbuf sb;
};

/*
 * Class: ostringbstream
 * ---------------
 * A variant on C++'s ostringstream class, which acts as a stream that
 * writes its data to a string.	 This is mostly used by the testing
 * code to test your Huffman encoding without having to read or write
 * files on disk, but you can use it in your own testing if you would
 * like.
 */
 
class ostringbstream: public obstream {
public:
	/* Constructor: ostringbstream();
	 * Usage: ostringbstream stream;
	 * --------------------------
	 * Constructs an ostringbstream.
	 */
	ostringbstream();
	
	/* Member function: string str();
	 * Usage: cout << osb.str() << endl;
	 * ----------------------------
	 * Retrieves the underlying string of the istringbstream.
	 */
	string str();
	
private:
	/* The actual string buffer that does character storage. */
	stringbuf sb;
};

#endif
