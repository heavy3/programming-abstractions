//
// main.cpp
//
// This program uses recursion with backtracking to solve the
// Instant Insanity(TM) puzzle.
//
// The puzzle consists of four cubes whose faces are each painted with one of
// the colors red, blue, green, and white, represented in the rest of this
// problem by their initial letter.
//
// A solve entails arranging the cubes in a row (or in a column) such that no
// color is duplicated on a side.
//
// For example, in two dimensions, the cubes could be represented as follows:
//
//
//        Unsolved                 Solved                Solution
//            ---                     ---             Representation
//   Cube 1 / G /                   / g /
//   --- --- --- ---         --- --- --- ---
//  [ W | B | B | R ]       [ W | B | B | R ]       [ W | B | B | R ] Cube 1
//   --- --- --- ---         --- --- --- ---        [ G | R | W | G ] Cube 2
//          \ G \             |   | \ g \ |         [ B | G | R | W ] Cube 3
//            ---             |   |   --- |         [ R | W | G | B ] Cube 4
//                            |   |       |
//            ---             |   |   --- |         Colors stacked along the
//   Cube 2 / R /             v   v / b / v         vertical do not repeat for
//   --- --- --- ---         --- --- --- ---        a given face.
//  [ W | W | G | B ]       [ G | R | W | G ]
//   --- --- --- ---         --- --- --- ---
//          \ G \             |   | \ w \ |
//            ---             |   |   --- |
//                            |   |       |
//            ---             |   |   --- |
//   Cube 3 / W /             v   v / r / v
//   --- --- --- ---         --- --- --- ---
//  [ R | R | W | B ]       [ B | G | R | W ]
//   --- --- --- ---         --- --- --- ---
//          \ G \             |   | \ w \ |
//            ---             |   |   --- |
//                            |   |       |
//            ---             |   |   --- |
//   Cube 4 / B /             v   v / r / v
//   --- --- --- ---         --- --- --- ---
//  [ G | R | R | R ]       [ R | W | G | B ]
//   --- --- --- ---         --- --- --- ---  * Lower case letters are
//          \ W \                   \*r \       irrelevant to the solution since
//            ---                     ---       they are obscured when cubes are
//                                              stacked.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/24/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "Vector.h"
#include "Set.h"
#include "error.h"
using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.07\n";
const std::string DETAIL = "Instant Insanity";
const std::string BANNER = HEADER + DETAIL;
const int NUM_ORTHOG_FACES = 4; /* # of faces orthogonal to axis of rotation. */
                                
// Types

enum ColorT {       // Cube face colors
    R,              // red             
    G,              // green           
    B,              // blue            
    W,              // white           
};

enum FaceT {        // Enumerate faces of a cube.
    FRONT,
    TOP,
    BACK,
    BOTTOM,
    LEFT,
    RIGHT,
    LAST_FACE
};

enum AxisT {        // Orients a reference side along an axis.
    POS_X,          // +x axis
    NEG_X,          // -x axis
    POS_Y,          // +y axis
    NEG_Y,          // -y axis
    POS_Z,          // +z axis
    NEG_Z,          // -z axis
    LAST_AXIS       // Halts post-increment operator (during iteration).
};

typedef Vector<ColorT> CubeT;   // A cube is a vector of face colors.

// Function prototypes

bool solve(Vector<CubeT> & cubes);
bool solveN(Vector<CubeT> & solved, Vector<CubeT> & unsolved);
bool permute(CubeT & cube, AxisT & axis, int & rotateCount);
bool isSolved(const Vector<CubeT> & cubes);
bool isSolved(Vector<CubeT> cubes, CubeT newCube);
bool isSolvedFace(const Vector<CubeT> & cubes, FaceT f);
void rotate90(CubeT & cube, AxisT axis);
void rotateNeg90(CubeT & cube, AxisT axis);
char getColorCh(ColorT c);
Set<ColorT> getFaceSet(const Vector<CubeT> & cubes, FaceT face);
Vector<ColorT> getFaceColors(const Vector<CubeT> & c, FaceT f);
ostream & operator<<(ostream & os, const Vector<CubeT> & cubes);
ostream & operator<<(ostream & os, const Vector<ColorT> & colors);
ostream & operator<<(ostream & os, ColorT c);
FaceT operator++(FaceT & f, int);
FaceT & operator++(FaceT & f);
AxisT operator++(AxisT & a, int);

// Main program

int main(int argc, char * argv[]) {
    
    cout << BANNER << endl << endl;
    CubeT c0, c1, c2, c3;
    
    // Statically initialize scrambled cube configurations.

#if 1   // Case 1: Input configuration given in text book.
    c0 += B, G, W, G, B, R; // front, top, back, bottom, left, right
    c1 += G, R, W, G, W, B;
    c2 += W, W, R, G, R, B;
    c3 += R, B, G, W, R, R;
#endif
    
#if 0   // Case 2: Already in solved order; should require no backtracking.
    c0 += G, B, R, W, W, R; // front, top, back, bottom, left, right
    c1 += B, R, W, B, W, G;
    c2 += W, G, G, R, B, B;
    c3 += R, W, B, G, R, R; // http://www.winning-moves.com/instantinsanitysolution/
#endif
    
#if 0   // Case 3: No solution.
    c0 += W, W, R, W, G, B; // front, top, back, bottom, left, right
    c1 += W, R, B, R, B, G;
    c2 += W, R, B, R, G, B;
    c3 += W, R, B, G, G, R;
#endif
    
#if 0   // Case 4: No solution.
    c0 += R, B, W, B, R, G; // front, top, back, bottom, left, right
    c1 += G, R, B, R, W, G;
    c2 += G, R, B, R, W, G;
    c3 += R, G, W, G, B, R;
#endif
    
#if 0   // Case 5
    c0 += W, B, R, R, B, G; // front, top, back, bottom, left, right
    c1 += B, G, B, W, R, W;
    c2 += G, R, W, B, G, R;
    c3 += G, R, W, R, G, B;
#endif

#if 0   // Case 6
    c0 += W, R, B, R, R, G; // front, top, back, bottom, left, right
    c1 += W, W, G, R, R, B;
    c2 += B, G, W, G, B, R;
    c3 += W, W, G, B, G, R;
#endif
    
    Vector<CubeT> cubes;
    cubes += c0, c1, c2, c3;
    Vector<CubeT> saved(cubes);

    if (solve(cubes)) {
        cout << "Colored cubes start in this configuration:" << endl;
        cout << saved << endl;
        cout << "Solution:" << endl << cubes << endl;
    } else {
        cout << "No solution found for these cubes:" << endl << cubes << endl;
    }
    return 0;
}

//
// Function: solve
// Usage: if(solve(cubes)) { . . . }
// ---------------------------------
// Returns true if four colored cubes can be arranged side-by-side such that
// no color repeats on a side.
//
// This is a wrapper for the recursive solveN function.
//
// Here, we set up two buckets, one for the currently unsolved cubes and
// another for the cubes we have solved so far.  Initially the unsolved
// bucket is full and the solved bucket is empty.
//

bool solve(Vector<CubeT> & cubes) {
    Vector<CubeT> solved, unsolved;
    unsolved = cubes;
    
    if  (solveN(solved, unsolved)) {
        cubes = solved;
        return true;
    }
    return false;
}

//
// Function: solveN
// Usage: if (solveN(solvedCubes, unsolvedCubes)) { . . . }
// --------------------------------------------------------
// Returns true if a collection of scrambled cubes can be arranged into
// a solved state wherein no color is repeated along a side.  The solved
// state is passed back by reference.
//
// This algorithm uses recursion with backtracking to progressively
// arrange cubes into a solution vector, one-by-one, minimally permuting
// the orientation of each candidate cube with respect to the subset of cubes
// already considered solved.  
//
// Finding a proximately acceptable orientation of the nth candidate cube allows
// us to tentatively place it with the other solved cubes, reducing
// the size of the unsolved input by one.  We attempt to arrange the
// smaller unsolved collection through a recursive call to solveN.
//
// If permutations of the remaining cubes get us to a solution, the recursion
// bottoms out when the unsolved collection is empty and we'll
// unwind the recursion to return a complete vector of solved cubes to the 
// caller.
//
// If permutation of remaining cubes /doesn't/ get us to a solution, then
// we'll fail out of a permuting loop and trigger backtracking to an earlier
// invocation of solveN where we then remove from the solved collection the last
// tentatively placed cube for more permuting, or, failing that, even more
// backing tracking, or, failing that, returning false to the caller to
// signify no solution found for the collection of input cubes (and remaining 
// philosophically detached from the larger issue of wasted compute power that 
// could have been applied to nobler pursuits like drawing rainbows).
//
// TODO: It might be fun to craft a graph-theoretic solution (that recurses
//       through various disjoint sub-graphs) for a more efficient algorithm.
//

bool solveN(Vector<CubeT> & solved, Vector<CubeT> & unsolved) {
    
    // base case: no unsolved cubes remain
    
    if (unsolved.size() == 0) return true;
    
    // recursive case
    //
    // 1. pull nth cube from unsolved collection, leaving n-1 unsolved cubes
    //
    // 2. if nth cube happens to be in a solved state with regard
    //    to other solved cubes, 
    //
    //      optimistically add nth cube to solved cubes
    //      if a recursive call to solveN with n-1 unsolved cubes is successful
    //          claim victory, return solution :-)
    //      otherwise, optimisim was unwarranted
    //      remove it from the solved cubes (so it can be permuted more)
    // 
    // 3. permute nthCube through next orientation (out of 24 possible!)
    //    loop back to 2
    //
    // 4. if we exhaust all permutes of nth cube
    //          return it to the unsolved collection
    //          return false
    //              if the collection of solved cubes is empty,
    //                  fail out of the routine entirely
    //                  no solution is possible :-/
    //              else
    //                  backtrack to an earlier invocation of solvN
    //                  we need to try re-permuting one of the solved cubes
    //

    AxisT axis = POS_X;  // Seed the cube permutation process w/ some state.
    int orthogRotations = NUM_ORTHOG_FACES;
    
    CubeT nthCube = unsolved[unsolved.size() - 1];
    unsolved.remove(unsolved.size() -1);
    do {
        if (isSolved(solved, nthCube)) {
            solved.insert(0, nthCube);
            if (solveN(solved, unsolved)) {
                return true;
            }
            solved.remove(0);
        }
     } while (permute(nthCube, axis, orthogRotations));
    unsolved += nthCube;
    return false;
}

//
// Function: permute
// Usage: AxisT axis = POS_X; int rotateCount = 4;
//        while (permute(cube, axis, rotateCount)) { . . . }
// ---------------------------------------------------------
// Returns true if a cube has yet to complete an entire walk of all 24 
// possible orientations.
//
// A face may be oriented along one of six axes (counting positive and negative
// axes separatately).  For each axis, four 90-degree rotations can be made.
// This yields 24 unique orientations to iterate across over multiple calls
// to this function.
//
// State is maintained by three pass-by-reference parameters embodying the
// cube, the current axis of rotation, and the number of rotations left to make
// for the current axis.
//
// The reference face is the right side (along the positive x axis).
//
// IMPORTANT
// ---------
// To effect a complete walk of all orientations, it is important to initialize
// the axis to POS_X and the rotation count to 4.  Thereafter, axis and rotation
// count are managed for you by the function.
//
// TODO: With more fu, there's likely a way to turn this into a proper iterator.
//

bool permute(CubeT & cube, AxisT & axis, int & rotateCount) {
    bool keepPermuting = true;
    int maxCount = NUM_ORTHOG_FACES;  // Max # of 90' rotations about an axis.
    
    rotate90(cube, axis);
    rotateCount--;
    
    switch (axis) {
        case POS_X:
            if (rotateCount <= 0) {    // Done with POS_X. Position reference
                rotateCount = maxCount;// face along NEG_X axis and get ready
                rotate90(cube, POS_Z); // for four more rotations about the
                rotate90(cube, POS_Z); // new axis starting with next call.
                axis = NEG_X;
            }
            break;
            
        case NEG_X:
            if (rotateCount <= 0) {    // Done with NEG_X. Position ref face
                rotateCount = maxCount;// along POS_Y in prep for next call.
                rotateNeg90(cube, POS_Z);
                axis = POS_Y;
            }
            break;
          
        case POS_Y:
            if (rotateCount <= 0) {    // Done with POS_Y. Position ref face
                rotateCount = maxCount;// along NEG_Y in prep for next call.
                rotate90(cube, POS_X);
                rotate90(cube, POS_X);
                axis = NEG_Y;
            }
            break;
        
        case NEG_Y:
            if (rotateCount <= 0) {    // Done with NEG_Y. Position ref face
                rotateCount = maxCount;// along POS_Z in prep for next call.
                rotateNeg90(cube, POS_X);
                axis = POS_Z;
            }
            break;
            
        case POS_Z:
            if (rotateCount <= 0) {    // Done with POS_Z. Position ref face
                rotateCount = maxCount;// along NEG_Z in prep for next call.
                rotate90(cube, POS_Y);
                rotate90(cube, POS_Y);
                axis = NEG_Z;
            }
            break;
            
        case NEG_Z:
            if (rotateCount <= 0) {    // Done with permuted walk of cube
                                       // through all 24 unique orientations.
                rotateNeg90(cube, POS_Y);
                rotateNeg90(cube, POS_X);   // Return to initial orientation.
                axis = LAST_AXIS;
            }
            break;
            
        default:
            keepPermuting = false;
            break;
    }
    return keepPermuting;
}

//
// Function: isSolved
// Usage: if (isSolved(cubes)) { . . . }
// Usage: if (isSolved(cubes, testCube)) { . . .}
// ----------------------------------------------
// Returns true if the vector of passed in cubes is in a solved state.
//
// Returns true if vector of cubes plus another cube outside the collection
// are in a solved state.
//

bool isSolved(const Vector<CubeT> & cubes) {
    for (FaceT f = FRONT; f <= BOTTOM; f++) {
        if (!isSolvedFace(cubes, f)) {
            return false;
        }
    }
    return true;
}

bool isSolved(Vector<CubeT> cubes, CubeT newCube) {
    cubes.insert(0, newCube);
    return isSolved(cubes);
}

//
// Function: isSolvedFace
// Usage: if (isSolvedFace(cubes, cubeFace)) { . . . }
// ---------------------------------------------------
// Returns true if a given face (e.g. TOP) has no repeating colors across all
// the cubes in the collection.
//
// Uses set membership to determine the number of unique colors along a face.
//

bool isSolvedFace(const Vector<CubeT> & cubes, FaceT f) {
    int nCubes = cubes.size();
    if (nCubes > 0) {
        
        //
        // Gather the set of colors for this face across all the
        // cubes in the collection.
        //
        
        Set<ColorT> colorSet = getFaceSet(cubes, f);
        
        // 
        // If the size of the color set equals the number of cubes
        // then we're in a solved state (for this face at least)
        // since we've inferred no duplicate colors.
        //
        
        return colorSet.size() == nCubes;
    }
    return true; // A collection of 0 cubes is always in a solved state. :-)
}

//
// Function: getFaceSet
// Usage: Set<ColorT> faceColorSet = getFaceSet(cubes, face);
// ----------------------------------------------------------
// Returns a set of colors associated with a given face for a vector of cubes.
//

Set<ColorT> getFaceSet(const Vector<CubeT> & cubes, FaceT face) {
    Set<ColorT> result;
    
    Vector<ColorT> faceColors = getFaceColors(cubes, face);
    for (ColorT color : faceColors) {
        result += color;
    }
    return result;
}

//
// Function: getFaceColors
// Usage: Vector<ColorT> faceColors = getFaceColors(cubes, face);
// --------------------------------------------------------------
// Returns a vector of colors associated with a given face across a collection
// of cubes.
//

Vector<ColorT> getFaceColors(const Vector<CubeT> & cubes, FaceT f) {
    Vector<ColorT> result;
    
    for (CubeT c: cubes) {
        result += c[f];
    }
    return result;
}

//
// Function: rotate90
// Usage: rotate90(cube, axis);
// --------------------------------------
// Rotate a cube 90 degrees about an axis according to right-hand rule.
//

void rotate90(CubeT & cube, AxisT axis) {
    CubeT temp(cube);
    
    switch (axis) {
        case POS_X:
        case NEG_X: {
            cube[FRONT]  = temp[TOP];
            cube[TOP]    = temp[BACK];
            cube[BACK]   = temp[BOTTOM];
            cube[BOTTOM] = temp[FRONT];
            break;
            }
            
        case POS_Y:
        case NEG_Y: {
            cube[TOP]    = temp[LEFT];
            cube[LEFT]   = temp[BOTTOM];
            cube[BOTTOM] = temp[RIGHT];
            cube[RIGHT]  = temp[TOP];
            break;
            }
            
        case POS_Z:
        case NEG_Z: {
            cube[FRONT]  = temp[LEFT];
            cube[LEFT]   = temp[BACK];
            cube[BACK]   = temp[RIGHT];
            cube[RIGHT]  = temp[FRONT];
            break;
            }
            
        case LAST_AXIS:
        default:
            break;
    }
}

//
// Function: rotateNeg90
// Usage: rotateNeg90(cube, axis);
// --------------------------------------
// Rotate a cube -90 degrees about an axis according to right-hand rule.
//

void rotateNeg90(CubeT & cube, AxisT axis) {
    CubeT temp(cube);
    
    switch (axis) {
        case POS_X:
        case NEG_X: {
            cube[FRONT]  = temp[BOTTOM];
            cube[TOP]    = temp[FRONT];
            cube[BACK]   = temp[TOP];
            cube[BOTTOM] = temp[BACK];
        }
            break;
            
        case POS_Y:
        case NEG_Y: {
            cube[TOP]    = temp[RIGHT];
            cube[LEFT]   = temp[TOP];
            cube[BOTTOM] = temp[LEFT];
            cube[RIGHT]  = temp[BOTTOM];
        }
            break;
            
        case POS_Z:
        case NEG_Z: {
            cube[FRONT]  = temp[RIGHT];
            cube[LEFT]   = temp[FRONT];
            cube[BACK]   = temp[LEFT];
            cube[RIGHT]  = temp[BACK];
        }
            break;
            
        case LAST_AXIS:
        default:
            break;
    }
}

//
// Function: getColorCh
// Usage: char colorCh = getColorCh(faceColor);
// --------------------------------------------
// Returns the character representation of a color.
//

char getColorCh(ColorT c) {
    char result;

    switch (c) {
        case R: result = 'R';
            break;
        
        case G: result = 'G';
            break;
            
        case B: result = 'B';
            break;
            
        case W: result = 'W';
            break;
            
        default: result = '?';
            break;
    }
    return result;
}

//
// Operator: <<
// Usage: cout << cubes << endl;
// -----------------------------
// Displays a character representation of a collection of cubes on an
// output stream.
//

ostream & operator<<(ostream & os, const Vector<CubeT> & cubes) {
    for (CubeT c : cubes) {
        os << c ;
    }
    return os;
}

//
// Operator: << (stream insertion)
// Usage: cout << cube << endl;
// -------------------------------
// Displays a 3D cube as an unfolded 2D representation on an output stream.
//
// For example:
//
//            ---
//          / W /
//   --- --- --- ---
//  [ R | R | W | B ]  In my world, the wings are actually left and right faces
//   --- --- --- ---   that get occluded when the cubes are stacked together.
//          \ G \
//            ---
//

ostream & operator<<(ostream & os, const CubeT & c) {
    ColorT T, B, L, R, f, b;
    
    f = c[FRONT];
    T = c[TOP];
    b = c[BACK];
    B = c[BOTTOM];
    L = c[LEFT];
    R = c[RIGHT];
    
    os << "           ---"                        << endl;
    os << "         / " << R << " /"              << endl;
    os << " --- --- --- ---"                      << endl;
    os << "| " << b << " ";
    os << "| " << B << " ";
    os << "| " << f << " ";
    os << "| " << T << " |"                       << endl;
    os << " --- --- --- ---"                      << endl;
    os << "         \\ " << L << " \\"            << endl;
    os << "           ---"                        << endl;
    
    return os;
}

//
// Operator: <<
// Usage: cout << color << endl;
// -----------------------------
// Displays the character representation of a color on an output stream.
//

ostream & operator<<(ostream & os, ColorT c) {
    os << getColorCh(c);
    return os;
}

//
// Operator: ++ (post-increment)
// Usage: for (FaceT f = TOP; f < LAST_FACE; f++) { . . . }
// --------------------------------------------------------
// Returns the current value of a cube face variable and then advances that
// variable to the next face in the order established by the FaceT enumerated
// type.
//

FaceT operator++(FaceT & f, int) {
    FaceT old = f;
    if (f < LAST_FACE) {
        f = FaceT(f + 1);
    }
    return old;
}

//
// Operator: ++ (pre-increment)
// Usage: FaceT next = ++currentFace;
// ----------------------------------
// Returns the next face of a cube relative to the order established in the
// FaceT enumerated type.
//

FaceT & operator++(FaceT & f) {
    if (f < LAST_FACE) {
        f = FaceT(f + 1);
    }
    return f;
}

//
// Operator: ++ (post-increment)
// Usage: for (AxisT axis = POS_X; axis < LAST_AXIS; axis++) { . . . }
// -------------------------------------------------------------------
// Returns the current value of a cube-relative axis and then advances
// it to the next axis in the order established by the AxisT enumerated type.
//
// This is useful when iteratively rotating a cube about all three-dimensionas
// of a coordinate system.
//

AxisT operator++(AxisT & ax, int) {
    AxisT old = ax;
    if (ax < LAST_AXIS) {
        ax = AxisT(ax + 1);
    }
    return old;
}