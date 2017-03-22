// TODO: FIX ME!  Does not handle delta between dates correctly.
//
// calendar.cpp
//
// This file implements the calendar.h interface.
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

#include <sstream>
#include <iostream>
#include <string>
#include "calendar.h"

// Constants and types

// 
// Establish epoch reference date for serializing time.
//
// !! The month and day must start at the beginning of the year
//    or else the toEpochDay() member function will fail as
//    currently implemented.
//

extern const int EPOCH_YEAR = 1900;
extern const Month EPOCH_MONTH = JANUARY;
extern const int EPOCH_DAY = 1;
const int AVG_DAYS_PER_YEAR = 365;
const int AVG_DAYS_PER_MONTH = AVG_DAYS_PER_YEAR / 12;

//
// Implementation notes: Constructors
// ----------------------------------
//

Date::Date() {
    initDate(EPOCH_DAY, EPOCH_MONTH, EPOCH_YEAR);
}

Date::Date(Month theMonth, int theDay, int theYear) {
    initDate(theDay, theMonth, theYear);
}

Date::Date(int theDay, Month theMonth, int theYear) {
    initDate(theDay, theMonth, theYear);
}

void Date::initDate(int theDay, Month theMonth, int theYear) {
    if (theDay <= 0 || theDay > 31 || theDay > daysInMonth(theMonth, theYear)) {
        error("Date: Day value out of range for month and year specified.");
    }
    year = theYear;
    month = theMonth;
    day = theDay;
}

int Date::getDay() { return day; }

Month Date::getMonth() { return month; }

int Date::getYear() { return year; }

std::string Date::toString() const {
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
// String format is specified with the MonthFormat enumerated type.
//
// MonthFormat  Output
// -----------  ------
// UPPERCASE    "JANUARY" (default)
// ABBREVATED   "Jan"
//
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

bool Date::operator==(const Date & date) const {
    return (year == date.year && month == date.month && day == date.day);
}

bool Date::operator!=(const Date & date) const {
    return !(*this == date);
}

bool Date::operator<(const Date & date) const {
    return  (year <  date.year ||
            (year == date.year && month < date.month) ||
            (year == date.year && month == date.month && day < date.day));
}

bool Date::operator<=(const Date & date) const {
    return (*this < date || *this == date);
}

bool Date::operator>(const Date & date) const {
    return !(*this <= date);
}

bool Date::operator>=(const Date & date) const {
    return (*this > date || *this == date);
}

//
// Implementation notes: << (insertion operator)
// ---------------------------------------------
// This function implements an overloaded insertion operator for displaying
// Date objects through output streams.
//
// Date is passed in as a reference parameter to avoid invoking the copy
// method.  Since the contents of date are not modified, it is designated
// as a const.
//

std::ostream & operator<<(std::ostream & output, const Date & date) {
    return output << date.toString();
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

Month operator++(Month & month, int) {
    Month old = month;
    month = Month((month + 1) % 12);
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
    month = Month((month + 1) % 12);
    return month;
}

//
// Implementation notes: -- (post-decrement operator)
// --------------------------------------------------
// Overloaded post-decrement operator for easy iteration over a range of Months.
//
// NB: By convention, C++ differentiates suffix operators from
//     prefix operators with a dummy int as the second parameter in the function
//     signature.
//

Month operator--(Month & month, int) {
    Month old = month;
    if ((month - 1) < 0) {
        month = Month((month - 1 + 12));
    } else {
        month = Month(month - 1);
    }
    return old;
}

//
// Implementation notes: -- (pre-decrement operator)
// -------------------------------------------------
// Overloaded pre-decrement operator for iteration across a range
// of months.
//

Month & operator--(Month & month) {
    if ((month - 1) < 0) {
        month = Month((month - 1 + 12));
    } else {
        month = Month(month - 1);
    }
    return month;
}

//
// Implementation notes: toEpochDay
// --------------------------------
// Return the number of days between a date of interest and a reference
// date (the epoch).
//
// Overall strategy is to compute the number of whole years and
// months between the date and the epoch date and then add on the number
// of days from the beginning of the month.
//
// Dates prior to the epoch are represented as a negative number of days.
//

int toEpochDay(Date date) {
    
    Date epochDate(EPOCH_DAY, EPOCH_MONTH, EPOCH_YEAR);
    
    int epochDays = 0;
    int leapDays = 0;  // The leap days in the full year intervals
                       // between the date of interest and the epoch year.
    
    int startYr = EPOCH_YEAR;
    int stopYr = date.getYear();
    Month startMonth = EPOCH_MONTH;
    Month stopMonth = date.getMonth();
    int startDay = EPOCH_DAY;
    int stopDay= date.getDay();
    
    if (date < epochDate) {
        startYr = date.getYear() + 1;   // Don't count partial year as year.
        stopYr = EPOCH_YEAR;
        startMonth = date.getMonth();
        stopMonth = DECEMBER;       // Assumes epoch Month and Day are Jan 1.
        startDay = date.getDay();
        stopDay = 31;
    }
    
    //
    // Count number of leap days over half-open interval:
    //
    //  [startYr, stopYr)
    //
    //
    
    for (int yr = startYr; yr < stopYr; yr++) {
        if (isLeapYear(yr)) {
            leapDays++;
        }
    }
    epochDays = (stopYr - startYr) * 365 + leapDays;
    
    //
    // Add days for each month from beginning of year to just
    // prior to the terminal month (will account for leap Month as necessary).
    //
    
    for (Month month = startMonth; month < stopMonth; month++) {
        if (date < epochDate) {
            epochDays += daysInMonth(month, stopYr - 1);
        } else {
            epochDays += daysInMonth(month, stopYr);
        }
    }
    
    //
    // Add days from beginning of terminal month.
    //
    
    if (date < epochDate) {
        epochDays += stopDay - startDay + 1;
    } else {
        epochDays += stopDay - startDay;
    }
    
    if (date < epochDate) {
        epochDays = -epochDays;
    }
    
    return epochDays;
}

//
// Implementation notes: toDate
// ----------------------------
// Converts epoch days (number of days relative to the epoch) to a
// Date object (with field values for day, month, and year).
//

Date toDate(int epochDays) {
    
    int approxYears  = abs(epochDays / AVG_DAYS_PER_YEAR);
    int fracYears    = abs(epochDays % AVG_DAYS_PER_YEAR);
    
    int approxMonths = fracYears / AVG_DAYS_PER_MONTH;
    int approxDays   = fracYears % AVG_DAYS_PER_MONTH;
    
    int delta = 0;
    
    if (epochDays > 0)
        delta = 1;
    else if (epochDays < 0)
        delta = -1;
    
    int year = EPOCH_YEAR;
    int month = EPOCH_MONTH;
    int day = EPOCH_DAY;
    
    for (int tallyDays = 0; abs(tallyDays) < abs(epochDays); tallyDays += delta) {
        for (int tallyYears = 0; tallyYears < approxYears; tallyYears++) {
            year = EPOCH_YEAR + (tallyYears * delta);
            if (isLeapYear(year)) {
                tallyDays += 366;
            } else {
                tallyDays += 365;
            }
        }
        if (abs(tallyDays) >= abs(epochDays)) {
            break;
        }
        
        for (int tallyMonths = 0; tallyMonths < approxMonths; tallyMonths++) {
            month = EPOCH_MONTH + (tallyMonths * delta);
            if (month < 0) {
                month += 12;
            }
            tallyDays += daysInMonth(Month(month), year) * delta;
        }
        if (abs(tallyDays) >= abs(epochDays)) {
            break;
        }
        
        tallyDays += approxDays * delta;
        day += approxDays * delta;
        if (abs(tallyDays) >= abs(epochDays)) {
            break;
        }
    }
    
    Date date(day, Month(month), year);
    return date;
}