//
// shape.h
//
// This program extends the Shape class from the course reader to include a
// contains() member function.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file extends the shape interface from Figure 19-4.
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/23/17.
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//

#ifndef shape_h
#define shape_h

#include <string>
#include "gwindow.h"

static const double PIXEL_PROX = 5; // Proximity for hit detection with lines.
                                    // Text wanted this to be 0.5. Too small!

class Shape {
public:
    void setLocation(double x, double y);
    void move(double x, double y);
    void setColor(std::string color);
    virtual void draw(GWindow& gw) = 0;
    virtual bool contains(double x, double y, double pixelProx) const = 0;
    
protected:
    Shape();
    Shape(double x, double y, std::string color = "BLACK");
    std::string color;
    double x, y;
};

class Line: public Shape {
public:
    Line(double x1,
         double y1,
         double x2,
         double y2,
         std::string color = "BLACK");
    
    virtual void draw(GWindow& gw);
    virtual bool contains(double x,
                          double y,
                          double pixelProx = PIXEL_PROX) const;
    
private:
    double distToLine(double x, double y) const;
    double dx;
    double dy;
};

class Rect: public Shape {
public:
    Rect(double x1,
         double y1,
         double width,
         double height,
         std::string color = "BLACK");
    virtual void draw(GWindow& gw);
    virtual bool contains(double x, double y, double pixelProx = 0) const;
    
private:
    double width, height;
};

class Square: public Rect {
public:
    Square(double x, double y, double size, std::string color = "BLACK");
};

class Oval: public Shape {
public:
    Oval(double x,
         double y, double width,
         double height,
         std::string color = "BLACK");
    virtual void draw(GWindow& gw);
    virtual bool contains(double x, double y, double pixelProx = 0) const;
    
private:
    double width, height;
};

class Circle: public Oval {
public:
    Circle(double x, double y, double radius, std::string color = "BLACK");
};

#endif // shape_h

