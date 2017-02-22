//
// RandomWriter.cpp
//
// This program builds a Markov model to generate random text based upon
// a frequency analysis of an input text.
//
// For a given language, certain character sequences are more likely than
// others.  In english, for example, the letter 'q' is often followed by 'u'
// as in the word, quick.
//
// A useful abstraction for capturing the sequence frequency of a given
// input text is the Markov model.
//
// Building such a model allows us to start with a seed string and then
// rely upon the model to supply the next most frequently occuring character
// to append to the output sequence.  The new output is then fed back into
// somethe model to generate subsequent output until some specified length of
// desired content is reached.
//
// Also at play is the notion of Markov 'order' which specifies how much
// sequence history should be factored in when predicting the most likely
// next character.  An order-1 model predicts the next character based upon
// only a single character sequence, whereas an order-2 model would look at
// all two-character sequences on the input text to predict the likeliest
// next character.
//
// For example:
//
//    Enter the source text: Hamlet.txt
//    Enter the Markov order [1-10]: 10
//                    [March within.]
//    What thou hast done,- must send thee hence
//    And how, and who, what means, and where 'tis fine,
//    Make choice of whom your wisest friends
//    And curd, like eager droppings into milk,
//
// --------------------------------------------------------------------------
// Assignment: 2, Part 2 Random Writer
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/090%20Assignment%202.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 11/23/2015.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include "filelib.h"
#include "simpio.h"
#include "map.h"
#include "vector.h"
#include "error.h"
#include "random.h"
using namespace std;

// Constants and types

const string PROMPT_INPUT_TEXT = "Enter the source text: ";
const string PROMPT_MARKOV_ORDER = "Enter the Markov order [1-10]: ";
const string E_BAD_MARKOV_ORDER = "That value is out of range.";
const int MIN_MARKOV_ORDER = 1;
const int MAX_MARKOV_ORDER = 10;
const int DFLT_NUM_CHARS = 2000;

const string PROCESSING = "Processing file...";
typedef Map<string,Vector<char> > MarkovModel_T;

// Function prototypes

void getMarkovInput(string & filename, ifstream & ifs, int & markovOrder);
string getMarkovSeed(MarkovModel_T & model);
void buildMarkovModel(ifstream & ifs, int markovOrder, MarkovModel_T & model);
void markovWriter(MarkovModel_T model, string seed, int nChar = DFLT_NUM_CHARS);

int main(int argc, char ** argv) {
    string filename;
    ifstream ifs;
    int markovOrder;
    MarkovModel_T model;
    
    getMarkovInput(filename, ifs, markovOrder);
    buildMarkovModel(ifs, markovOrder, model);
    string seed = getMarkovSeed(model);
    markovWriter(model, seed);
    return 0;
}

//
// Function: getMarkovInput
// Usage: getMarkovInput(filename, ifs, markovOrder);
// --------------------------------------------------
// Prompts the user for an input filename containing text to analyze
// along with the desired Markov order.  Requires user to enter an
// order which is within a min and max range specified by the
// integer constants:
//
//    MIN_MARKOV_ORDER
//    MAX_MARKOV_ORDER
//
// Attempts to open the file and associate it with an input stream which
// is returned as a reference variable to the caller along with the filename
// and Markov order.
//

void getMarkovInput(string & filename, ifstream & ifs, int & markovOrder) {
    filename = promptUserForFile(ifs, PROMPT_INPUT_TEXT);
    markovOrder = 0;
    
    while (true) {
        markovOrder = getInteger(PROMPT_MARKOV_ORDER);
        if ((markovOrder < MIN_MARKOV_ORDER) ||
            (markovOrder > MAX_MARKOV_ORDER)) {
            cout << E_BAD_MARKOV_ORDER << endl;
        } else {
            break;
        }
    }
}

string getMarkovSeed(MarkovModel_T & model) {
    int size = 0;
    string seed;
    
    //
    // Return most frequently occurring seed.
    // This will be the one associated with the longest vector of characters.
    //

    for (string word : model) {
        if (model[word].size() > size) {
            seed = word;
            size = model[word].size();
        }
    }
    return seed;
}

//
// Function: buildMarkovModel
// Usage: model = buildMarkovModel(ifs, order);
// --------------------------------------------
// Parses source text from the input file stream, building up a Markov model
// of specified order that associates a string-snippet of fixed length (equal
// to the order of the model) to a Vector of characters most likely to follow
// that snippet based upon frequency analysis of the input text.
//

void buildMarkovModel(ifstream & ifs, int markovOrder, MarkovModel_T & model) {
    if (markovOrder < 1) return;
    
    char ch;
    string key;
    while (ifs.get(ch)) {
        if (key.length() == markovOrder) {
            model[key].add(ch);
            key = key.substr(1) + ch;   // Advance to next key string.
        } else {
            key += ch;                  // Build up first key string.
        }
    }
}

//
// Function: markovWriter
// Usage: markovWriter(model, seed);
//        markovWriter(model, seed, numChars);
// -------------------------------------------
// Writes randomly generate text to the console based upon a given
// Markov model of some source text, an input seed string, and an
// optional text length given in characters.
//
// If text length is not provided, the writer defaults to the value
// specified in the constant:
//
//    DFLT_NUM_CHARS
//
// The seed string should be a key in the Markov map, otherwise
// no output will be produced since subsequent characters to output
// cannot be derived from the model.
//

void markovWriter(MarkovModel_T model, string seed, int textLength) {
    Vector<char> nextCharList;
    char nextChar;
    string currSeed = seed;
    
    if (model.size()) {
        cout << currSeed;
        for (int i = 0; i < textLength; i++) {
            if (model.containsKey(currSeed)) {
                nextCharList = model[currSeed];
                int len = nextCharList.size();
                if (len > 0) {
                    nextChar = nextCharList[randomInteger(0, len - 1)];
                    cout << nextChar;
                    currSeed = currSeed.substr(1) + nextChar;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    }
}
