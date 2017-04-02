//
// plot.h
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 06
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// Most of this code comes from Figure 20-2
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/31/17
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//
#ifndef plot_h
#define plot_h

#include "gwindow.h"
#include "exp.h"

typedef double(*doubleFn)(double);

// Function prototypes

// Function pointer version.

void plot(GWindow& gw,
          ExpressionFunction fn,
          double minX,
          double maxX,
          double minY,
          double maxY);

void plot(GWindow& gw,
          ExpressionFunction fn,
          double minX,
          double maxX);

void reasonableYInterval(double& minY,
                         double& maxY,
                         double minX,
                         double maxX,
                         ExpressionFunction& fn);
#endif // plot_h
