//
// main.cpp
//
// This program implements a function:
//
//      int countFifteens(Vector<Card> & cards);
//
// which takes a vector of Card values (see Exercise 6.2), and returns
// the number of ways you can make 15 from that collection of cards.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 8, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/9/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "Vector.h"
#include "Set.h"
#include "Card.h"

// Prototypes

int countFifteens(Vector<Card> & cards);
int countSubsetSumWays(Vector<Card> & cards, int target);

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 8.07\n";
const std::string DETAIL = "Cribbage Counting";
const std::string BANNER = HEADER + DETAIL;

// Main program

int main(int argc, char * argv[]) {
    std::cout << BANNER << std::endl << std::endl;
    
    
    Vector<Card> cribbageHand;
    //cribbageHand += Card("AD"), Card("5C"), Card("10S"), Card("4H"), Card("9C");
    cribbageHand += Card("5C"), Card("JC"), Card("5D"), Card("5H"), Card("5S");
    
    std::cout << "Number of 15's from cribbage hand " << cribbageHand;
    std::cout << " = " << countFifteens(cribbageHand) << std::endl;
    
    return 0;
}

//
// Function: countFifteens
// Usage: int countF = countFifteens(cribbageHand);
// ------------------------------------------------
// Takes a vector of Card values and returns the number of ways you can make 
// 15 from that collection of cards.
//

int countFifteens(Vector<Card> & cards) {
    return countSubsetSumWays(cards, 15);
}

//
// Function: countSubsetSumWays
// Usage: int count = countSubsetSumWays(set, targetSum);
// ------------------------------------------------------
// Returns the number of ways in which you can produce the target value
// by choosing a subset of the specified set.
//
// The key insight is count should be 1 when the target sum
// is at 0.  This might happen if empty set is passed in and sum is 0
// or if target has been reduced to 0 through recursion.
//

int countSubsetSumWays(Vector<Card> & hand, int target) {
    
    if (hand.isEmpty()) {
        return (target == 0) ? 1 : 0;
    } else {
        Card c = hand[0];
        int cVal = (c.getRank() >= 10) ? 10 : c.getRank();
        
        Vector<Card> rest = hand;
        rest.remove(0);

        return countSubsetSumWays(rest, target) +
               countSubsetSumWays(rest, target - cVal);
    }
}
