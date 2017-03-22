//
// rational.cpp
//
// This file implements a const correct Rational class.
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
// Created by Glenn Streiff on 2/19/16 and 7/20/16.
// Copyright © 2016 Glenn Streiff. All rights reserved. (derivative work)
//

#include <string>
#include <cstdlib>
#include "error.h"
#include "rational.h"
#include "strlib.h"
using namespace std;

// Function prototypes

int gcd(int x, int y);

//
// Implementation notes: Constructors
// ----------------------------------
// There are three constructors for the Rational class.  The default
// constructor creates a Rational with a zero value, the one-argument
// form converts an integer to a Rational, and the two-argument form allows
// specifying a fraction.  The constructors ensure that the following
// invariants are maintained:
//
// 1. The fraction is always reduced to lowest terms.
// 2. The denominator is always positive.
// 3. Zero is always represented as 0/1.
//

Rational::Rational() {
    num = 0;
    den = 1;
}

Rational::Rational(int n) {
    num = n;
    den = 1;
}

Rational::Rational(int x, int y) {
    if (y == 0) error("Rational: Division by zero");
    if (x == 0) {
        num = 0;
        den = 1;
    } else {
        int g = gcd(abs(x), abs(y));
        num = x / g;
        den = abs(y) / g;
        if (y < 0) num = -num;
    }
}

// Implementation of toString and the << operator

string Rational::toString() const {
    if (den == 1) {
        return integerToString(num);
    } else {
        return integerToString(num) + "/" + integerToString(den);
    }
}

ostream & operator<<(ostream & os, Rational r) {
    return os << r.toString();
}

// Implementation of shorthand assignment operators.

Rational & Rational::operator+=(const Rational & rhs) {
    return *this = *this + rhs;
}

Rational & Rational::operator-=(const Rational & rhs) {
    return *this = *this - rhs;
}

Rational & Rational::operator*=(const Rational & rhs) {
    return *this = *this * rhs;
}

Rational & Rational::operator/=(const Rational & rhs) {
    return *this = *this / rhs;
}

// Implementation of pre-increment and post-increment operators.

Rational & Rational::operator++() {
    *this = *this + Rational(1);    // ++r;
    return *this;
}

Rational Rational::operator++(int) {
    Rational rBefore = *this;
    *this = *this + Rational(1);    // r++;
    return rBefore;
}

Rational & Rational::operator--() {
    *this = *this - Rational(1);    // --r;
    return *this;
}

Rational Rational::operator--(int) {
    Rational rBefore = *this;
    *this = *this - Rational(1);    // r--;
    return rBefore;
}

//
// Implementation notes: arithmetic operators
// ------------------------------------------
// The implementation of the operators follows directly from the definitions.
//

Rational operator+(Rational r1, Rational r2) {
    /* a/b + c/d = (ad + cb) / bd */
    
    return Rational(r1.num * r2.den + r2.num * r1.den, r1.den * r2.den);
}

Rational operator-(Rational r1, Rational r2) {
    return Rational(r1.num * r2.den - r2.num * r1.den, r1.den * r2.den);
}

Rational operator*(Rational r1, Rational r2) {
    return Rational(r1.num * r2.num, r1.den * r2.den);
}

Rational operator/(Rational r1, Rational r2) {
    return Rational(r1.num * r2.den, r1.den * r2.num);
}

bool operator==(Rational r1, Rational r2) {
    return (r1.num == r2.num) && (r1.den == r2.den);
}

bool operator!=(Rational r1, Rational r2) {
    return !(r1 == r2);
}

bool operator<(Rational r1, Rational r2) {
    return r1.num/float(r1.den) < r2.num/float(r2.den);
}

bool operator<=(Rational r1, Rational r2) {
    return (r1 == r2) || (r1 < r2);
}

bool operator>(Rational r1, Rational r2) {
    return r1.num/float(r1.den) > r2.num/float(r2.den);
}

bool operator>=(Rational r1, Rational r2) {
    return (r1 == r2) || (r1 > r2);
}

//
// Implementation notes: gcd
// -------------------------
// This implementation uses Euclid's algorithm to calculate the greatest
// common divisor.
//

int gcd(int x, int y) {
    int r = x % y;
    while (r != 0) {
        x = y;
        y = r;
        r = x % y;
    }
    return y;
}
