//
// BigInt.cpp
//
// This file implements an extended-precision integer class called
// BigInt that relies upon a linked-list representation.
//
// TODO: Add support for negative numbers and additional operators beyond
//       + and *.
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

#include "BigInt.h"
#include <cmath>
#include "strlib.h"

static const int BIG_INT_RADIX = 10;

// Constructor: BigInt
// Usage: BigInt bi = new BigInt;
// ------------------------------
// Create a null big integer with no digits and no value.

BigInt::BigInt() {
    nDigits = 0;
    pd = NULL;
    sign = POS;
}

// Constructor: BigInt
// Usage: BigInt bi = new BigInt(42);
// ----------------------------------
// Create a big integer initialized from an integer value.
//
// For example, 42 becomes:
//
//    bi.pd -> 2 -> 4
//
// where the least significant digit (lsd) appears at the head of the list.

BigInt::BigInt(int num) {
    nDigits = integerToString(abs(num)).length();
    pd = intToList(num);
    sign = (num < 0) ? NEG : POS;
}

// Constructor: BigInt
// Usage: BigInt bi = new BigInt("42");
// ------------------------------------
// Create a big integer initialized from a string of decimal digits.
//
// For example, "42" becomes:
//
//    bi.pd -> 2 -> 4
//
// where the least significant digit (lsd) appears at the head of the list.

BigInt::BigInt(std::string numStr) {

    // TODO: Add defensive string trim and data validation.  Are all the
    //       string digits valid for the radix?
    
    switch(numStr[0]) {
        case '-':
            sign = NEG;
            numStr.erase(0, 1); // strip off the negative sign
            break;
            
        case '+':
            sign = POS;
            numStr.erase(0, 1); // strip off the plus sign
            break;
            
        default:
            sign = POS;         // assume no sign implies positive value
            break;
    }
    pd = strToList(numStr);     // represent the string as a linked list
    nDigits = numStr.length();
}

// Copy Constructor: BigInt
// Usage: BigInt bi = new BigInt(myBigInt);
// ----------------------------------------
// Allows you to bootstrap another big integer from an existing big integer.

BigInt::BigInt(const BigInt & biSrc) {
    deepCopy(biSrc);
}

// Assignment Operator: =
// Usage: BigInt newNum = someBigInt;
// ----------------------------------
// Overrides the assignment operator to work with extended precision integers.
// Assigns one big integer to another.  The left-hand side is the implied
// receiver and is implicitly passed in as the 'this' parameter.  The right-
// hand side is passed as a formal parameter.

BigInt & BigInt::operator=(const BigInt & rhs) {
    if (this != &rhs) {
        delDigits(this->pd);    // avoid mem leak; reclaim allocated heap
        deepCopy(rhs);
    }
    return *this;
}

// Destructor
// Usage: (typically implied)
// --------------------------
// When a big integer goes out of scope, the destructor is invoked to reclaim
// any associated heap memory that has been dynamically allocated.

BigInt::~BigInt() {
    delDigits(pd);
}

// Method: toString
// Usage: cout << bigInt.toString() << endl;
// -----------------------------------------
// Returns the a string representation of the underlying big integer.

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

// Operator: +
// Usage: BigInt sum = bigInt1 + bigInt2;
// --------------------------------------
// Allows two big integers to be added together and sum returned to the caller.
// Returns the sum as a new big integer without mutating the input operands.
//
// TODO: Add support for negative operands.

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
    
    // The operands can have varying lengths.  Sum up those digits
    // for which both numbers have values.
    
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
    
    // Continue with the summation (possibly including a carry-in from
    // the previous operation) for the remaining digits associated only
    // with the longer of the two operands.
    
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
    
    // Account for any final carry operation that might result
    // in another digit added to the representation.
    
    if (carry > 0) {
        psum->next = new Digit;
        psum = psum->next;
        psum->i = carry;
        psum->next = NULL;
        biSum.nDigits++;
    }
    return biSum;
}

// Operator: *
// Usage: BigInt product = bigInt1 * bigInt2;
// ------------------------------------------
// Uses a recursive strategy to return the product of two big integers.
//
// TODO: Add support for negative operands.

BigInt BigInt::operator*(const BigInt & bi2) const {
    BigInt bi1(*this);
    return recursiveProd(bi1, bi2);
}

// Method: recursiveProd
// Usage: BigInt product = recursiveProd(bigN1, bigN2)
// -------------------------------------------------------
// Returns the product of two extended precision integers according to the
// following recursive strategy:
//
// result = N1_least_significant_digit * bigN2
//        + N1_remaining_most_significant_digits * bigN2 * 10;
//
// where the first operand may be computed as the recursive base case through
// a simple for loop that simulates multiplication through repeated addition
// of the n2 term.
//
// The second operand takes the remaining (most significant digits) which have
// been reduced by 1 digit and recursively computes the resulting product.
//
// Finally, the two terms are added using the extended precision '+' operator
// to yield a big integer return value.
//
// TODO: Add support for negative operands.

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

// Method: deepCopy
// Usage: this->deepCopy(fromThisBigInt);
// --------------------------------------
// Performs a deep copy of one big integer to another.
// The target integer is implied to be the one pointed to by 'this'.

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

// Method: delDigits
// Usage: this->delDigits(pDigits);
// --------------------------------
// Frees the memory allocated to the linked list of extended precision digits
// headed by pDigits.

void BigInt::delDigits(Digit *pDigits) {
    Digit *pNext = pDigits;
    
    while (pNext != NULL) {
        Digit *pSave = pNext->next;
        delete pNext;
        pNext = pSave;
    }
}

// Method: intToList
// Usage: this->intToList(42)
// --------------------------
// Primitive which returns a pointer to a list-ized integer.
//
// For example, 42 becomes p -> 2 -> 4

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

// Method: strToList
// Usage: this->strToList("42")
// --------------------------
// Primitive which returns a pointer to a list-ized integer.
//
// For example, "42" becomes p -> 2 -> 4

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

// Insertion Operator: <<
// Usage: cout << myBigInt << endl;
// --------------------------------
// Overloads the insertion stream operator (<<) to support the
// output of extended precision integers.

std::ostream & operator<<(std::ostream & os, BigInt & bi) {
    os << bi.toString();
    return os;
}

