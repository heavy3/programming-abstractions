//
// Card.cpp
//
// This file implements the Card.h interface.
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

#include <sstream>
#include "Card.h"
#include "strlib.h"

extern const int MAX_RANK = 13;     // Highest rank.
extern const int NUM_SUIT =  4;     // Number of suits.

extern const int ACE      =  1;     // Numeric rank of facecards.
extern const int JACK     = 11;
extern const int QUEEN    = 12;
extern const int KING     = 13;

const std::string E_BAD_RANK_FACE =
                    "Card: Invalid rank.  Unknown face card.";
const std::string E_BAD_CARD_NAME =
                    "Card: Invalid card name.";
const std::string E_BAD_RANK =
                    "Card: Invalid rank.";
const std::string E_BAD_NUM_RANK =
                    "Card: Invalid numeric rank. Expect numbers from 2 to 10.";
const std::string E_BAD_SUIT =
                    "Card: Invalid suit. Expect 'C', 'D', 'H', 'S'";

//
// Implementation notes: Constructors
// ----------------------------------
// The default constructor does not initialize the instance variables; 
// it simply instantiates a card that can be assigned the value of another 
// card.
//
// The second constructor initializes rank and suit through parameters.
//
// The third constructor initializes the Card instance variables through
// a abbreviated string name that is decoded.
//
// Runtime errors are triggered if unrecognized or illegal values for the
// constructor initialization are passed in.
//

Card::Card() {
}

Card::Card(int theRank, Suit theSuit) {
    if ((1 <= theRank) && (theRank <= MAX_RANK)) {
        rank = theRank;
    } else {
        error(E_BAD_RANK);
    }
    suit = theSuit;
}

Card::Card(std::string shortName) {
    if (!decodeName(shortName, rank, suit)) {
        error(E_BAD_CARD_NAME);
    }
}

//
// Implementation notes: decodeName
// --------------------------------
// Decodes the rank and suit of a card given a string-ized short name.
// Some validation of the input string length provides some minimal 
// protection against bad data leading to invalidly defined cards.
//

bool Card::decodeName(std::string shortName, int & rank, Suit & suit) {
    int nameLen = shortName.length();
    if ((nameLen > 3) || (nameLen < 2)) {
        return false;
    }
    
    std::string strSuit = shortName.substr(nameLen - 1, 1);
    std::string strRank = shortName.erase(nameLen - 1, 1);
    
    suit = stringToSuit(strSuit);
    rank = stringToRank(strRank);
    
    return true;
}

//
// Implementation notes: Getters
// -----------------------------
// The getters set the private instance variables of a card.
// No setters are provided to ensure the Card object remains immutable.
//

Suit Card::getSuit() {
    return suit;
}

int Card::getRank() {
    return rank;
}

//
// Implementation notes: toString, rankToString, suitToString
// ----------------------------------------------------------
// Creates string version of a card, rank, or suit.
//

std::string Card::toString() {
    
    std::string rStr = rankToString(rank);
    std::string sStr = suitToString(suit);

    return rStr + sStr;
}

std::string Card::rankToString(int rank) {
    std::string rStr;
    switch (rank) {
        case ACE:
            rStr = "A";
            break;
            
        case JACK:
            rStr = "J";
            break;
            
        case QUEEN:
            rStr = "Q";
            break;
            
        case KING:
            rStr = "K";
            break;
            
        default:
            rStr = integerToString(rank);
            break;
    }
    return rStr;
}

std::string Card::suitToString(Suit suit) {
    std::string sStr;
    
    switch (suit) {
        case CLUBS:
            sStr = "C";
            break;
            
        case DIAMONDS:
            sStr = "D";
            break;
            
        case HEARTS:
            sStr = "H";
            break;
            
        case SPADES:
            sStr = "S";
            break;
            
        default:
            sStr = integerToString(suit);
            break;
    }
    return sStr;
}

//
// Implementation notes: stringToRank, stringToSuit
// ------------------------------------------------
// Converts string versions of rank and suit to integer and Suit enumerated
// type in support of the abbreviated string-name Card constructor.
// 
// Runtime errors occur if the input string does not map to a valid
// rank or suit.
//
// Lower-case input strings are normalized to upper-case.
//

int Card::stringToRank(std::string strRank) {
    
    if ((strRank.length() < 1) || (strRank.length() > 2)) {
        error(E_BAD_RANK);
    }
    
    int result = 0;
    if (isalpha(strRank[0])) {
        char faceCardRank = toupper(strRank[0]);
        switch (faceCardRank) {
            case 'A':
                result = 1;
                break;
                
            case 'J':
                result = 11;
                break;
                
            case 'Q':
                result = 12;
                break;
                
            case 'K':
                result = 13;
                break;
                
            default:
                error(E_BAD_RANK_FACE);
                break;
        }
    } else {
        if (isnumber(strRank[0])) {
            result = stringToInteger(strRank);
            if ((result < 2) or (result > 10)) {
                error(E_BAD_NUM_RANK);
            }
        } else {
            error(E_BAD_NUM_RANK);
        }
    }
    
    return result;
}

Suit Card::stringToSuit(std::string strSuit) {
    if (strSuit.length() != 1) {
        error(E_BAD_SUIT);
    }
    Suit result;
    
    char normalizedSuit = toupper(strSuit[0]);
    switch (normalizedSuit) {
        case 'C':
            result = CLUBS;
            break;
            
        case 'D':
            result = DIAMONDS;
            break;
            
        case 'H':
            result = HEARTS;
            break;
            
        case 'S':
            result = SPADES;
            break;
            
        default:
            result = HEARTS;    // Not used; squelching compiler warning.
            error(E_BAD_SUIT);
            break;
    }
    return result;
} // end Class


// Free functions

//
// Implementation notes: operator<<
// --------------------------------
// This is a free function for allowing convient insertion of Card 
// objects in an output stream.
//

std::ostream & operator<<(std::ostream & output, Card d) {
    output << d.toString();
    return output;
}

//
// Implementation notes: operator++
// --------------------------------
// This is a free function for allowing convenient iteration over the 
// enumerated Suit type.
//

Suit operator++(Suit & suit, int) {
    Suit oldSuit = suit;
    suit = Suit(suit + 1);
    return oldSuit;
}