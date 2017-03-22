/******************************************************************************
 * File: TrailblazerGraphics.h
 *
 * Functions to draw in the graphics display
 */

#ifndef TrailblazerGraphics_Included
#define TrailblazerGraphics_Included

#include "TrailblazerTypes.h"
#include "grid.h"

/* Function: colorCell
 *
 * Highlights the specified cell in the world the given color, which must be
 * either GRAY, YELLOW, or GREEN.
 */
void colorCell(Grid<double>& world, Loc loc, Color locColor);

#endif
