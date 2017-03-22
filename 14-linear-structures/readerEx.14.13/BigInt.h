//
// BigInt.h
//
// This exports an interface for an extended-precision integer class
// called BigInt that uses linked lists for a very scalable representation
// of large, non-negative integers.
//
// TODO: Needs more testing.  (Very minimal implementation.)
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 13
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/5/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#ifndef BigInt_h
#define BigInt_h

#include <iostream>
#include <ostream>

class BigInt {
public:
    
    BigInt();
    BigInt(int num);
    BigInt(std::string str);
    
    // copy constructor and assignment operator (that perform deep copies)
    
    BigInt(const BigInt & src);
    BigInt & operator=(const BigInt & src);
    
    ~BigInt();
    
    std::string toString() const;
    BigInt operator+(const BigInt & bi2) const;
    BigInt operator*(const BigInt & bi2) const;
    
private:
    enum SignT {NEG = -1, POS = 1};
    
    struct Digit {
        int i;
        Digit *next;
    };
    
    int nDigits = 0;
    Digit *pd;
    SignT sign;
    
    void deepCopy(const BigInt & bi);
    void delDigits(Digit *pDigits);
    Digit * intToList(int num) const;
    Digit * strToList(std::string numStr) const;
    BigInt recursiveProd(BigInt n1, const BigInt & n2) const;
};

std::ostream & operator<<(std::ostream & os, BigInt & bi);

#endif // BigInt_h
