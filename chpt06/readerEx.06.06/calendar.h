// TODO: FIX ME!  Does not handle delta between dates correctly.
//
// calendar.h
//
// This file specifies an interface for Calendar-related types and functions.
// Extensions to the interface for this exercise include:
//
//  • The insertion operator <<.
//  • The relational operators ==, !=, <, <=, >, and >=
//  • The expression date + n, which returns the date n days after date
//  • The expression date - n, which returns the date n days before date
//  • The expression d1 - d2, which returns how many days separate d1 and d2
//  • The shorthand assignment operators += and -= with an integer on the right
//  • The ++ and -- operators in both their prefix and suffix form.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 6
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 12/17/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#ifndef calendar_h
#define calendar_h

#include "error.h"

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

enum MonthFormat {
    UPPERCASE,
    ABBREVIATED
};

extern const int   EPOCH_YEAR;
extern const Month EPOCH_MONTH;
extern const int   EPOCH_DAY;

//
// Class: Date
// ---------------
// This class implements a calendar and date model.
//

class Date {

public:
    
//
// Constructor: Date
// Usage: Date date;
//        Date date(month, day, year);
//        Date date(day, month, year);
// -----------------------------------------------
// Create a date object on the stack.
//
// Default constructor defaults to JANUARY 1, 1900.
// Month is specified with an enumerated type exported by the interface.
//
    
    Date();
    Date(Month theMonth, int theDay, int theYear);
    Date(int theDay, Month theMonth, int theYear);
    
//
// Accessors: getDay, getMonth, getYear
// Usage: int day = date.getDay();
//        Month month = date.getMonth();
//        int year = date.getYear();
//
    
    int getDay();
    Month getMonth();
    int getYear();
    std::string toString() const;

    bool operator==(const Date & date) const;
    bool operator!=(const Date & date) const;
    bool operator<(const Date & date) const;
    bool operator<=(const Date & date) const;
    bool operator>(const Date & date) const;
    bool operator>=(const Date & date) const;
    Date operator+(int nDays);

private:
    
// Instance variables
    
    int year;
    Month month;    // JANUARY - DECEMBER
    int day;        // 1 - 31
    
    int epochDay;   // Number of days since epoch.
    
// Private methods
    
    void initDate(int day, Month month, int year);
    
}; // end Date

// Free functions

//
// Operator: <<
// Usage: std::cout << dateObj;
// ----------------------------
// Returns a reference to an output stream after redirecting a Date object,
// formatted as a string, to that stream.
// 
// This must be declared as a free function (and not a member function) since
// the left operand, a reference to an output stream, is not an instance of 
// Date.
//

std::ostream & operator<<(std::ostream & output, const Date & date);

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

std::string monthToString(Month month, MonthFormat format = UPPERCASE);

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
// Post-increment operator overloaded for easy iteration across a range of 
// months.
//
// Limitations: Does not support rollover from DECEMBER to JANUARY since
// that would defeat exit condition tests in a for-loop iterator.
//

Month operator++(Month & month, int);

Date toDate(int epochDay);

int toEpochDay(Date date);

#endif // calendar_h
