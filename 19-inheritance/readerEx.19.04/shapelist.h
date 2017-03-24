//
// shapelist.h
//
// This file defines a ShapeList class that maintains a list of shapes.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This content comes from Figure 19-18.
// --------------------------------------------------------------------------
//

#ifndef shapelist_h
#define shapelist_h

#include "vector.h"
#include "gwindow.h"
#include "shape.h"

// Class: ShapeList
// ----------------
// This class is a vector of shapes arranged from back to front.  The
// individual elements of the ShapeList are pointers to Shape objects.

class ShapeList: public Vector<Shape *> {
public:
    
    // Methods: moveToFront, moveToBack, moveForward, moveBackward
    // Usage: shapes.moveToFront(sp);
    //        shapes.moveToBack(sp);
    //        shapes.moveForward(sp);
    //        shapes.moveBackward(sp);
    // -------------------------------
    // Changes the position of sp in the ShapeList.  The first two methods
    // move the shape all the way to the specified end.  The last two move
    // it one position in the indicated direction, if possible.  The method
    // signals an error if sp is not in the ShapeList.
    
    void moveToFront(Shape *sp);
    void moveToBack(Shape *sp);
    void moveForward(Shape *sp);
    void moveBackward(Shape *sp);
    
    // Method: draw
    // Usage: shapes.draw(gw);
    // -----------------------
    // Draws the shapes in the ShapeList on the graphics window.  The shapes
    // are drawn from back to front, so that shapes closer to the front seem
    // to cover those further back.
    
    void draw(GWindow& gw) const;
    
    // Method: free
    // Usage: shapes.free();
    // ---------------------
    // Garbage collects any heap memory associated with the shape pointer
    // elements.
    
    void free();
    
private:
    
    // Method: find
    // Usage: int index = shapes.find(sp);
    // -----------------------------------
    // Returns the index corresponding to the location of the shape pointer
    // within the shape list.  If no such shape pointer exists, returns
    // npos.
    
    int find(Shape *sp) const;

};

#endif // shapelist_h
