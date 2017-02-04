//
// calendar.h
//
// This file defines a simple date-related interface.  Specifically,
// it exports a Month type along with the functions:
//
//    int daysInMonth(Month month, int year);
//    bool isLeapYear(int year);
//    std::string monthToString(Month month);
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 2, Exercise 11
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 9/21/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#ifndef _calendar_h
#define _calendar_h

enum Month {
    JANUARY,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
};

//
// Function: daysInMonth
// Usage: int days = daysInMonth(Month month, int year)
// ----------------------------------------------------
// Returns the number of days in a given month, handling
// the special case posed by leap day for the month of
// February in leap years.
//

int daysInMonth(Month month, int year);

//
// Function: isLeapYear
// Usage: if (isLeapYear(year)) ...
// --------------------------------
// Returns true for years where a leap day should be added
// to the calendar.
//

bool isLeapYear(int year);

//
// Function: monthToString
// Usage: cout << monthString(JANUARY);
// ------------------------------------
// Stringizes the <code>Month</code> enumerated type.
//

std::string monthToString(Month month);

//
// Operator: ++ (pre-increment)
// Usage: Month m = JANUARY; ++m;
// ------------------------------
// Prefix operator overload for easy iteration across a range of months.
// Limitations: Does not support rollover from DECEMBER to JANUARY since
// that would defeat exit condition tests in a for-loop iterator.
//

Month & operator++(Month & month);

//
// Operator: ++ (post-increment)
// Usage: for (Month m = JANUARY; m <= DECEMBER; m++) {...}
// --------------------------------------------------------
// Post-increment operator overload for easy iteration across a range of months.
// Limitations: Does not support rollover from DECEMBER to JANUARY since
// that would defeat exit condition tests in a for-loop iterator.
//

Month operator++(Month & month, int);

#endif