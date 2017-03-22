//
// calendar.cpp
//
// This file implements the calendar.h interface.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 5
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 12/16/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <sstream>
#include <iostream>
#include <string>
#include "calendar.h"

Date::Date() {
    year = 1900;
    month = JANUARY;
    day = 1;
}

Date::Date(Month theMonth, int theDay, int theYear) {
    year = theYear;
    month = theMonth;
    day = theDay;
}

Date::Date(int theDay, Month theMonth, int theYear) {
    year = theYear;
    month = theMonth;
    day = theDay;
}

int Date::getDay() { return day; }

Month Date::getMonth() { return month; }

int Date::getYear() { return year; }

std::string Date::toString() {
    std::ostringstream oss;
    // dd-mmm-yyyy
    oss << day << "-" << monthToString(month, ABBREVIATED) << "-" << year;
    return oss.str();
}

//
// Implementation notes: daysInMonth
// ---------------------------------
// Returns days in a month and handling the special case
// of adding a day in February on leap years.
//

int daysInMonth(Month month, int year) {
    switch (month) {
        case APRIL: case JUNE: case SEPTEMBER: case NOVEMBER:
            return 30;
        case FEBRUARY:
            return (isLeapYear(year)) ? 29 : 28;
        default:
            return 31;
    }
}

//
// Implementation notes: isLeapYear
// --------------------------------
// Returns true for leap years according to the aglorithm:
//
// A year is a leap year if it is ...
//
// 	divisible by 4 but not 100
//  OR
// 	is divisible by 400
//

bool isLeapYear(int year) {
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

//
// Implementation notes: monthToString
// ------------------------------------
// Returns a string representation of the enumerated Month type.
//

std::string monthToString(Month month, MonthFormat format) {
    switch (format) {
        case ABBREVIATED:
            switch (month) {
                case JANUARY:
                    return "Jan";
                case FEBRUARY:
                    return "Feb";
                case MARCH:
                    return "Mar";
                case APRIL:
                    return "Apr";
                case MAY:
                    return "May";
                case JUNE:
                    return "Jun";
                case JULY:
                    return "Jul";
                case AUGUST:
                    return "Aug";
                case SEPTEMBER:
                    return "Sep";
                case OCTOBER:
                    return "Oct";
                case NOVEMBER:
                    return "Nov";
                case DECEMBER:
                    return "Dec";
                default:
                    return "???";
            }
            break;
            
        case UPPERCASE:
        default:
            switch (month) {
                case JANUARY:
                    return "JANUARY";
                case FEBRUARY:
                    return "FEBRUARY";
                case MARCH:
                    return "MARCH";
                case APRIL:
                    return "APRIL";
                case MAY:
                    return "MAY";
                case JUNE:
                    return "JUNE";
                case JULY:
                    return "JULY";
                case AUGUST:
                    return "AUGUST";
                case SEPTEMBER:
                    return "SEPTEMBER";
                case OCTOBER:
                    return "OCTOBER";
                case NOVEMBER:
                    return "NOVEMBER";
                case DECEMBER:
                    return "DECEMBER";
                default:
                    return "UNKNOWN_MONTH";
            }
            break;
    }
}

//
// Implementation notes: ++ (post-increment operator)
// --------------------------------------------------
// Overloaded post-increment operator for easy iteration over a range of Months.
//
// NB: By convention, C++ differentiates suffix operators from
//     prefix operators with a dummy int as the second parameter in the function
//     signature.
//
// The reasonable expectation that the operator support rollover from JANUARY to
// DECEMBER is complicated by a lack of elegant C++ fu and a greater desire that
// that expressions like month <= DECEMBER work as expected in for-loop
// iterators; with a naive rollover implementation, the test condition will
// always fail when month rolls over to JANUARY (which is always less than
// DECEMBER).
//
// Advice on Stack Overflow suggests stuffing the enum values in a vector
// and iterating over that:
//
// http://stackoverflow.com/questions/261963/how-can-i-iterate-over-an-enum
//
// I may try that when I have more c++ skills.
//

Month operator++(Month & month, int) {
    Month old = month;
    month = Month(month + 1);  // Intentionally not rolling over from DEC to JAN
    return old;
}

//
// Implementation notes: ++ (pre-increment operator)
// -------------------------------------------------
// Overloaded pre-increment operator overload for easy iteration across a range
// of months.
//
// Does not support rollover from DECEMBER to JANUARY.  See notes in
// post-increment operator overload comment header.
//

Month & operator++(Month & month) {
    month = Month(month + 1);
    return month;
}