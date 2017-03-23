//
// shape.h
//
// This program extends the Shape class from the course reader to include a
// square subclass.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 2
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

class Shape {
public:
    void setLocation(double x, double y);
    void move(double x, double y);
    void setColor(std::string color);
    virtual void draw(GWindow& gw) = 0;
    
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
    
private:
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
    
private:
    double width, height;
};

class Square: public Rect {
public:
    Square(double x, double y, double size, std::string color = "BLACK");
    
private:
    double size;
};

class Oval: public Shape {
public:
    Oval(double x,
         double y, double width,
         double height,
         std::string color = "BLACK");
    virtual void draw(GWindow& gw);

private:
    double width, height;
};
#endif // shape_h
