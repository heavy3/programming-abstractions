//
// BigInt.cpp
//
// This file implements an extended-precision integer class called
// BigInt that relies upon a linked-list representation.
//
// TODO: Add support for negative operands.
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

#include "BigInt.h"
#include <cmath>
#include "strlib.h"

static const int BIG_INT_RADIX = 10;

BigInt::BigInt() {
    nDigits = 0;
    pd = NULL;
    sign = POS;
}

BigInt::BigInt(int num) {
    nDigits = integerToString(abs(num)).length();
    pd = intToList(num);
    sign = (num < 0) ? NEG : POS;
}

BigInt::BigInt(std::string numStr) {
    // TODO: Add defensive string trim and data validation.
    switch(numStr[0]) {
        case '-':
            sign = NEG;
            numStr.erase(0, 1);
            break;
            
        case '+':
            sign = POS;
            numStr.erase(0, 1);
            break;
            
        default:
            sign = POS;
            break;
    }
    pd = strToList(numStr);
    nDigits = numStr.length();
}

BigInt::BigInt(const BigInt & biSrc) {
    deepCopy(biSrc);
}

BigInt & BigInt::operator=(const BigInt & rhs) {
    if (this != &rhs) {
        delDigits(this->pd);    // avoid mem leak; reclaim allocated heap
        deepCopy(rhs);
    }
    return *this;
}

BigInt::~BigInt() {
    delDigits(pd);
}

std::string BigInt::toString() const {
    BigInt::Digit *pDigit = this->pd;
    std::string result;
    
    if (pDigit == NULL) return result;
    
    while (pDigit != NULL) {
        result = char((pDigit->i) + '0') + result;
        pDigit = pDigit->next;
    }
    if (sign == NEG) {
        result.insert(0, 1, '-');
    }
    return result;
}

BigInt BigInt::operator+(const BigInt & bi2) const {
    BigInt bi1 = *this;
    BigInt biSum(0);
    BigInt bigNum;
    BigInt smlNum;
    
    if (bi1.nDigits > bi2.nDigits) {
        bigNum = bi1;
        smlNum = bi2;
        biSum.nDigits = bi1.nDigits;
    } else {
        bigNum = bi2;
        smlNum = bi1;
        biSum.nDigits = bi2.nDigits;
    }
    
    Digit *psum = biSum.pd;
    Digit *pbig = bigNum.pd;
    Digit *psml = smlNum.pd;
    int carry = 0;
    
    for (int i = 0; i < smlNum.nDigits; i++) {
        int sum = pbig->i + psml->i + carry;
        psum->i = sum % BIG_INT_RADIX;
        psum->next = NULL;
        carry = (sum > (BIG_INT_RADIX - 1)) ? 1 : 0;
        
        pbig = pbig->next;
        psml = psml->next;
        
        if (psml == NULL) break;
        psum->next = new Digit;
        psum = psum->next;
    }
    
    for (int i = smlNum.nDigits; i < bigNum.nDigits; i++) {
        psum->next = new Digit;
        psum = psum->next;
        
        int sum = pbig->i + carry;
        psum->i = sum % BIG_INT_RADIX;
        psum->next = NULL;
        carry = (sum > (BIG_INT_RADIX - 1)) ? 1 : 0;
        
        pbig = pbig->next;
        
        if (pbig == NULL) break;
        psum->next = new Digit;
    }
    
    if (carry > 0) {
        psum->next = new Digit;
        psum = psum->next;
        psum->i = carry;
        psum->next = NULL;
        biSum.nDigits++;
    }
    return biSum;
}

BigInt BigInt::operator*(const BigInt & bi2) const {
    BigInt bi1(*this);
    return recursiveProd(bi1, bi2);
}

BigInt BigInt::recursiveProd(BigInt n1, const BigInt & n2) const {
    BigInt result(0);
    if (n1.nDigits <= 1) {
        if (n1.pd != NULL) {
            for (int i = 0; i < n1.pd->i; i++) {
                result = result + n2;   // multiply is repeated addition
            }
        }
        return result;
    }

    BigInt lsd(n1.pd->i);
    if (n1.pd != NULL) {
        n1.nDigits--;
        n1.pd = n1.pd->next;
    }
    
    BigInt msd(n1);
    BigInt lsdResult = recursiveProd(lsd, n2);
    BigInt msdResult = recursiveProd(msd, n2);
    
    // result = lsdResult + 10 * msdResult
    
    result = lsdResult;
    for (int i = 0; i < BIG_INT_RADIX; i++) {
        result = result + msdResult;
    }
    return result;
}

void BigInt::deepCopy(const BigInt & biSrc) {
    nDigits = biSrc.nDigits;
    sign = biSrc.sign;
    pd = NULL;
    if (biSrc.pd == NULL) return;
    
    pd = new Digit;
    
    Digit * pDst = pd;
    Digit * pSrc = biSrc.pd;

    while (true) {
        pDst->i = pSrc->i;
        pSrc = pSrc->next;
        if (pSrc != NULL) {
            pDst->next = new Digit;
            pDst = pDst->next;
        } else {
            pDst->next = NULL;
            break;
        }
    }
}

void BigInt::delDigits(Digit *pDigits) {
    Digit *pNext = pDigits;
    
    while (pNext != NULL) {
        Digit *pSave = pNext->next;
        delete pNext;
        pNext = pSave;
    }
}

BigInt::Digit * BigInt::intToList(int num) const {
    BigInt::Digit * pTail = new BigInt::Digit;
    pTail->next = NULL;

    if (abs(num) < BIG_INT_RADIX) {
        pTail->i = num;
        return pTail;
    }
    
    int lsd = num % BIG_INT_RADIX;
    pTail->i = lsd;
    
    int msd = num / BIG_INT_RADIX;
    pTail->next = intToList(msd);
    
    return pTail;
}

BigInt::Digit * BigInt::strToList(std::string numStr) const {
    BigInt::Digit * pTail = new BigInt::Digit;
    pTail->next = NULL;
    
    if (numStr.length() == 0) return NULL;
    
    if (numStr.length() == 1) {
        pTail->i = (numStr[0] - '0');
        return pTail;
    }
    
    int lastPos = numStr.length() - 1;
    char lsd = numStr[lastPos];
    pTail->i = (lsd - '0');
    
    pTail->next = strToList(numStr.erase(lastPos, 1));
    return pTail;
}

std::ostream & operator<<(std::ostream & os, BigInt & bi) {
    os << bi.toString();
    return os;
}

