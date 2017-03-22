//
// Card.h
//
// This file exports an interface for modeling a playing Card.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 12/5/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#ifndef Card_h
#define Card_h

#include <string>
#include <iostream>
#include "error.h"

extern const int MAX_RANK;  //  Highest rank.
extern const int NUM_SUIT;  //  Number of suits.

extern const int ACE;       //  Numeric values for face cards.
extern const int JACK;
extern const int QUEEN;
extern const int KING;

enum Suit {
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES
};

class Card {
public:
    
    //
    // Constructor: Card
    // Usage: Card empty;
    //        Card card(rank, suit);
    //        Card card("10S");     // 10 of spades.
    // ---------------------------------------------
    // Create a Card object.  The default constructor creates and empty card
    // that can be assigned a value later.
    //
    // The second constructor take separate parameters for rank (1 .. 13)
    // and suit (CLUBS, DIAMONDS, HEARTS, SPADES).
    //
    // The third constructor takes an abbreviated name in string form
    // (e.g., "AC", "2D").
    //
    
    Card();
    Card(std::string shortName);
    Card(int rank, Suit suit);
    
    //
    // Methods: getRank, getSuit
    // Usage: int rank = card.getRank();
    //        Suit suit = card.getSuit();
    // ----------------------------------
    // These methods return the rank and suit associated with a card.
    //
    
    int getRank();
    Suit getSuit();
    
    //
    // Method: decodeName
    // Usage: if (decodeName("AC", rank, suit)) { ... };
    // -------------------------------------------------
    // Returns true if the short-name string is successfully converted to
    // a valid rank and suit and Card state updated accordingly.
    //
    
    bool decodeName(std::string shortName, int & rank, Suit & suit);
    std::string toString();
    
private:
    int rank;
    Suit suit;
    
    //
    // Methods: rankToString, suitToString
    // Usage: string strRank = rankToString(1);     // strRank = "A";
    //        string strSuit = suitToString(CLUBS); // strSuit = "C";
    // --------------------------------------------------------------
    // Returns the string version of the rank or suit.
    //

    std::string rankToString(int rank);
    std::string suitToString(Suit suit);
    
    //
    // Methods: stringToRank, stringToSuit
    // Usage: int rank = stringToRank("A");     // rank = 1;
    //        Suit suit = stringToSuit("H");    // suit = HEADS;
    // ---------------------------------------------------------
    // Returns the rank or suit of a card given a string version of same.
    //
    
    int stringToRank(std::string strRank);
    Suit stringToSuit(std::string strSuit);
};

//
// Operator: <<
// Usage: cout << card;
// --------------------
// Sends a string version of a card to an output stream.
//

std::ostream & operator<<(std::ostream & os, Card d);

//
// Suffix operator: ++
// Usage: for (Suit s = CLUBS; s < SPADES; s++) { ... };
// -----------------------------------------------------
// Provides a suffix increment operator for the enumerated suit type.
//

Suit operator++(Suit & suit, int);

#endif // Card_h
