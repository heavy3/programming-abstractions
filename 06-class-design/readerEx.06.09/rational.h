//
// rational.h
//
// This file exports a class interface which models rational
// numbers and related operations on those numbers.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 8
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This class extends the source code from Figure 6-07 in the reader.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 2/19/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#ifndef rational_h
#define rational_h

//
// Class: Rational
// ---------------
// The Rational class is used to represent rational numbers which are defined
// as the quotient of two integers.
//

class Rational {
    
public:
    
//
// Constructor: Rational
// Usage: Rational zero;
//        Rational num(n);
//        Rational r(num, den);
// ----------------------------
// Creates a Rational object.  The default constructor creates the rational
// number 0.  The single-argument constructor creates a rational equal to the
// specified integer, and the two-argument constructor creates a rational
// number corresponding to the fraction num/den.
//
    
    Rational();
    Rational(int n);
    Rational(int x, int y);
    
//
// Method: toString()
// Usage: string str = r.toString();
// ---------------------------------
// Returns the string representation of this rational number.
//
    
    std::string toString();

//
// Operator: +=, -=, *=, /=
// Usage: r1 += r2;
// ----------------
// Returns a reference to the lhs after adding r2.
//
    
    Rational & operator+=(const Rational & rhs);
    Rational & operator-=(const Rational & rhs);
    Rational & operator*=(const Rational & rhs);
    Rational & operator/=(const Rational & rhs);
    
//
// Operator: ++
// Usage: ++r;
// Usage: r++;
// ---------------------
// Overloads the pre and post-increment operators for rational numbers.
//

    Rational & operator++();    //  ++r
    Rational operator++(int);   //  r++

//
// Operator: --
// Usage: --r;
// Usage: r--;
// ---------------------
// Overloads the pre and post-decrement operators for rational numbers.
//

    Rational & operator--();    //  --r
    Rational operator--(int);   //  r--
    
// Declare the operator functions as friends.
    
    friend Rational operator+(Rational r1, Rational r2);
    friend Rational operator-(Rational r1, Rational r2);
    friend Rational operator*(Rational r1, Rational r2);
    friend Rational operator/(Rational r1, Rational r2);
    friend bool operator==(Rational r1, Rational r2);
    friend bool operator!=(Rational r1, Rational r2);
    friend bool operator<(Rational r1, Rational r2);
    friend bool operator<=(Rational r1, Rational r2);
    friend bool operator>(Rational r1, Rational r2);
    friend bool operator>=(Rational r1, Rational r2);
    
// Private instance data.
    
private:
    
    int num;
    int den;
    
};

//
// Operator: <<
// ------------
// Overloads the << operator so that it can display Rational values.
//

std::ostream & operator<<(std::ostream & os, Rational r);

//
// Operator: +
// Usage: r1 + r2
// --------------
// Overloads the + operator so that it can perform addition on two rational
// numbers.
//

Rational operator+(Rational r1, Rational r2);

//
// Operator: -
// Usage: r1 - r2
// --------------
// Overloads the - operator so it can perform substraction on two rational
// numbers.
//

Rational operator-(Rational r1, Rational r2);

//
// Operator: *
// Usage: r1 * r2
// --------------
// Overloads the * operator so that it performs multiplication on two rational
// numbers.
//

Rational operator*(Rational r1, Rational r2);

//
// Operator: /
// Usage: r1 / r2
// --------------
// Overloads the / operator so that it performs division on two rational
// numbers.
//

Rational operator/(Rational r1, Rational r2);

//
// Operator: ==
// Usage: r1 == r2
// ---------------
// Overloads the == operator so that two rational numbers can be compared
// for equality.
//

bool operator==(Rational r1, Rational r2);

//
// Operator: !=
// Usage: r1 != r2
// ---------------
// Overloads the != operator so that two rational numbers can be compared
// for inequality.
//

bool operator!=(Rational r1, Rational r2);

//
// Operator: <
// Usage: r1 < r2
// ---------------
// Overloads the < operator so that two rational numbers can be compared
// for less than.
//

bool operator<(Rational r1, Rational r2);

//
// Operator: <=
// Usage: r1 <= r2
// ---------------
// Overloads the <= operator so that two rational numbers can be compared
// for less than or equality.
//

bool operator<=(Rational r1, Rational r2);

//
// Operator: >
// Usage: r1 > r2
// ---------------
// Overloads the > operator so that two rational numbers can be compared
// for greater than.
//

bool operator>(Rational r1, Rational r2);

//
// Operator: >=
// Usage: r1 >= r2
// ---------------
// Overloads the >= operator so that two rational numbers can be compared
// for greater than or equality.
//

bool operator>=(Rational r1, Rational r2);

#endif // rational_h
