/******************************************************************************
 * File: TrailblazerTypes.h
 *
 * Fundamental types for the Trailblazer assignment.
 */
 
#ifndef TrailblazerTypes_Included
#define TrailblazerTypes_Included

/* Type: Loc
 *
 * A type representing a location in the world, represented as a pair of a row
 * and a column.
 */
struct Loc {
		int row;
		int col;
};

/* Utility function to construct a Loc from its location. */
Loc makeLoc(int row, int col);

/* Type: Color
 *
 * An enumerated type representing a color for a node during an execution of
 * Dijkstra's algorithm or A* search.
 */
enum Color {
		GRAY, YELLOW, GREEN
};

/* Type: Edge
 *
 * A type representing an edge in the grid, as encoded by its start and end node.
 * This type will be useful when implementing Kruskal's algorithm, though you may
 * also find it useful when writing Dijkstra's algorithm or A* search.
 */
struct Edge {
		Loc start;
		Loc end;
};

/* Utility function to create an Edge from its endpoints. */
Edge makeEdge(Loc start, Loc end);



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* The functions below this point are provided for convenience and are not
 * strictly necessary for your implementation.
 */
 
/* Comparison operators for Loc and Edge.	 Our Map and Set type cannot store
 * custom structs as keys (for a map) or values (for a set) unless they can be
 * compared with the relational operators.	You will probably not directly use
 * these in your solution, but you're welcome to do so if you find them useful.
 */
bool operator <	 (Loc lhs, Loc rhs);
bool operator >	 (Loc lhs, Loc rhs);
bool operator == (Loc lhs, Loc rhs);
bool operator != (Loc lhs, Loc rhs);
bool operator <= (Loc lhs, Loc rhs);
bool operator >= (Loc lhs, Loc rhs);

bool operator <	 (Edge lhs, Edge rhs);
bool operator >	 (Edge lhs, Edge rhs);
bool operator == (Edge lhs, Edge rhs);
bool operator != (Edge lhs, Edge rhs);
bool operator <= (Edge lhs, Edge rhs);
bool operator >= (Edge lhs, Edge rhs);

/* Hash function for Loc and Edge.	These functions are provided so that you can
 * store Locs and Edges in our HashMap or HashSet type, which require a hashCode
 * function to be available.	You will probably not directly use these in your
 * solution, but you're welcome to do so if you find them useful.
 */
int hashCode(Loc l);
int hashCode(Edge e);

#endif
