//
// main.cpp
//
// This program uses a Card class to create a deck of cards.
//
// For example:
//
//    CS106B Programming Abstractions in C++: Chpt 6, Ex 2
//    Print a deck of playing cards modeled with a Class
//
//    AC 2C 3C 4C 5C 6C 7C 8C 9C 10C JC QC KC
//    AD 2D 3D 4D 5D 6D 7D 8D 9D 10D JD QD KD
//    AH 2H 3H 4H 5H 6H 7H 8H 9H 10H JH QH KH
//    AS 2S 3S 4S 5S 6S 7S 8S 9S 10S JS QS KS
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 6, Exercise 2
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// Portions of the test driver come from the course reader.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 12/5/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "Card.h"

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 6.2";
const std::string DETAIL1 = "Print playing cards modeled with a Class\n";
const std::string DETAIL2 = "Exercising 'short name' Card constructor:\n";

int main(int argc, char * argv[]) {
    
    std::cout << HEADER << std::endl;
    std::cout << DETAIL1 << std::endl;
    
    for (Suit suit = CLUBS; suit <= SPADES; suit++) {
        for (int rank = ACE; rank <= KING; rank++) {
            Card myCard = Card(rank, suit);
            std::cout << " " << myCard;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    // Test out "short name" constructor.
    
    std::cout << DETAIL2 << std::endl;
    
    std::cout << Card("AC") << std::endl;
    std::cout << Card("2d") << std::endl;
    std::cout << Card("10H") << std::endl;
    std::cout << Card("ks") << std::endl;
    
    // This next card is illegal. It should be "AC". Expecting runtime error.
    // Error: Card: Invalid numeric rank. Expect numbers from 2 to 10.
    
    std::cout << Card("1C") << std::endl;
    
    return 0;
}