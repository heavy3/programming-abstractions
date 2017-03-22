//
// Boggle.cpp
//
// This program plays a game of Boggle.  At the core is a function:
//
//      void findBoggleWords(const Grid<char> & board,
//                           const Lexicon & english,
//                           Set<string> & wordsFound);
//
// which finds all the legal words on the board that appear in an English
// lexicon and adds those words to the set wordsFound.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 9, Exercise 16
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 4/18/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "error.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "set.h"
#include "grid.h"
#include "lexicon.h"
#include "random.h"
using namespace std;

// Types

//
// Type: Player
// ------------
// This type identifies valid players as human or computer.
//

enum Player { HUMAN, COMPUTER };

//
// Type: Move
// ----------
// Moves are designated by a symbol and a position.
//

struct Move {
    Set<std::string> wordsFound;
};

//
// Type: Path
// ----------
// Identifies an ordered sequence of adjacent Boggle squares.
//

typedef Vector<int> Path;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 9.16\n";
const std::string DETAIL = "Boggle";
const std::string BANNER = HEADER + DETAIL;
const std::string WELCOME = "Welcome to Boggle, where Scrabble meets Tic-Tac-Toe\n";
const std::string LANGUAGE = "English";
const std::string LEXICON_FILE = "EnglishWords.dat";
const std::string VOWELS = "aeiou";
const std::string CONSONANTS = "bcdfghjklmnpqrstvwxyz";
const std::string ALPHABET = VOWELS + CONSONANTS;
const std::string TEXTGAME = "XCERIMGANOMLGZRP";    // Game in text book.

const Player STARTING_PLAYER = HUMAN;

const int LENGTH = 4;                         // Grid dimension; side length.
const int MAX_CELL = LENGTH * LENGTH;         // Limit for iterators.
const int MIN_LENGTH = 3;                     // Smallest legal word.
const int MAX_LENGTH = MAX_CELL;              // Longest legal word.

// Prototypes

ostream & operator<<(ostream & os, Grid<char> & gameGrid);

//
// Class: Boggle
// -------------
// This class implements a character-graphics Boggle game.
//

class Boggle {

public:
    
//
// Constructor: Boggle
// Usage: Boggle game(firstPlayer, englishLexicon);
// ------------------------------------------------
// Construct a boggle game object.
//
    
    Boggle(const Player firstPlayer, Lexicon & lexicon,
           std::string initStr = "") {
        
        startPlayer = firstPlayer;
        grid.resize(LENGTH, LENGTH);
        turnCount = 0;
        bogglex = lexicon;
    }
    
//
// Method: play
// Usage: game.play();
// -------------------
// Plays a game between a computer and a human.
//
    
    void play(std::string initStr = "") {
        initGame(initStr);
        while (!gameIsOver()) {
            Move move;
            if (getCurrentPlayer() == HUMAN) {
                move = getUserMove();
            } else {
                move = getComputerMove();
            }
            makeMove(move);
            displayMove(move);
            switchTurn();
        }
        announceResult();
    }
    
private:
    
//
// Method: initGame
// Usage: initGame();
// ------------------
// Sets a game to an initial state of play.
//
    
    void initGame(std::string initStr) {
        whoseTurn = startPlayer;
        fillGrid(initStr);
        explain();
        findBoggleWords(grid, bogglex, gridWords);
    }

//
// Method: explain
// Usage: game.explain();
// ----------------------
// This method explains the rules of the game to the user.
//
    
    void explain() {
        cout << WELCOME << endl;
        cout << "Compete to create " << LANGUAGE << " words from a ";
        cout << LENGTH << "x" << LENGTH << " grid" << endl;
        cout << "of characters by stringing together sequences of "<< endl;
        cout << "adjacent letters. Words must be at least " << MIN_LENGTH << endl;
        cout << "letters long and you cannot revisit the same letter." << endl;
        cout << endl;
        cout << "The game grid looks like this: " << endl << endl;
        cout << grid << endl;
    }
    
//
// Method: fillGrid
// Usage: fillGrid("abcdefghijklmnop");
// ------------------------------------
// Fills the boggle grid with characters from the input string in row-major
// order.
//
// Mutates the private instance variable, grid.
//
    
    void fillGrid(std::string initStr) {
        if (initStr.empty()) {
            fillGridRandomly();
        } else {
            //
            // Populate the game grid to a specific state.
            //
            if (initStr.length() != MAX_CELL) {
                error("initGame: Incorrect length for initStr.");
            }
            for (int r = 0; r < LENGTH; r++) {
                for (int c = 0; c < LENGTH; c++) {
                    int index = r * LENGTH + c;
                    grid.set(r, c, tolower(initStr[index]));
                }
            }
        }
    }
    
//
// Method: fillGridRandomly
// Usage: fillGridRandomly()
// -------------------------
// Populates the boggle grid with randomly selected vowels and consonants.
// TODO: This could be more Boggle-like by modeling the letter frequencies
//       according to actual Boggle dice.
//
    
    void fillGridRandomly() {
    
        Vector<char> vowels;
        int numConsonants = MAX_CELL - LENGTH;
        Vector<char> consonants;
        Vector<char> letters;
        
        //
        // Ensure we have at least one row's worth of vowels in the grid
        // to keep the game-play interesting.
        //
        // Load up a vector with random vowels.
        //
        
        for (int i = 0; i < LENGTH; i++) {
            int vIndex = randomInteger(0, VOWELS.length() - 1);
            char v = VOWELS[vIndex];
            vowels.add(v);
        }
        
        //
        // Load up a vector with random consonants.
        //
        
        for (int i = 0; i < numConsonants; i++) {
            int cIndex = randomInteger(0, CONSONANTS.length() - 1);
            char c = CONSONANTS[cIndex];
            consonants.add(c);
        }
        
        //
        // Merge the two vectors such that consonants and vowels are
        // spread out and we don't get vowels grouped in one area
        // in the final arrangement.
        //
        
        for (int r = 0; r < LENGTH; r++) {
            for (int c = 0; c < LENGTH; c++) {
                if (c % LENGTH == 0) {
                    int lastV = vowels.size() - 1;
                    letters += vowels[lastV];
                    vowels.remove(lastV);
                } else {
                    int lastC = consonants.size() - 1;
                    letters += consonants[lastC];
                    consonants.remove(lastC);
                }
            }
        }
        
        //
        // Populate the game grid in a random way from the list
        // of pre-selected consonants and vowels.
        //
        
        for (int r = 0; r < LENGTH; r++) {
            for (int c = 0; c < LENGTH; c++) {
                int index = randomInteger(0, letters.size() - 1);
                grid.set(r, c, tolower(letters[index]));
                letters.remove(index);
            }
        }
    }
    
//
// Method: findBoggleWords
// Usage: findBoggleWords(grid, lexicon, wordsFound);
// --------------------------------------------------
// Returns the set of all boggle words for a given grid of characters.
//
// This is a wrapper for the recursive walkBoggleGraph method
// which returns the words associated with all the paths
// radiating from a single start location.
//
// Therefore, to get all the words in the grid, the wrapper
// iterates over all the cells in the grid and passes a unique
// trail head
//
    
    void findBoggleWords(Grid<char> & board, const Lexicon & lex,
                         Set<std::string> & wordsFound) {
        
        for (int cellNum = 1; cellNum <= MAX_CELL; cellNum++) {
            
            Set<int> visited;
            visited.add(cellNum);
            
            std::string wordlet = string(1, getCh(board, cellNum));
            
            Path head;
            head.add(cellNum);
            
            walkBoggleGraph(head, 
                        board, 
                        lex, 
                        wordsFound, 
                        wordlet, 
                        visited,
                        MIN_LENGTH, 
                        MAX_LENGTH);
        }
    }
    
//
// Method: walkBoggleGraph
// Usage: bool wordInPath = walkBoggleGraph(path, board, lexicon, wordsFound,
//                               wordlet, visitedCells, minLength, maxLength);
// ---------------------------------------------------------------------------
// Returns all the words found along the directed acyclic paths that radiate 
// out from a single cell within the Boggle grid.
//
// A word is considered valid if it exists within a lexicon and is neither
// too short nor too long.
//
// The initial call to this method is made from a wrapper function
// which establishes the trail head and seeds the wordlet and visited nodes
// parameters accordingly.
// 
// Subsequent calls amount to recursive explorations of adjacent character 
// nodes that advance the path and allow found letters to be appended to
// a burgeoning word prefix termed a 'wordlet'.
//
// Progress halts down a specific path when:
//
//  1. the wordlet fails to prefix-match against any words in the lexicon
//  2. all positions in the grid have been traversed (without doubling back)
//  3. the maximum valid word length has been reached
//
// Generally 2 and 3 will happen at the same time given the rules of Boggle.
//
// It is possible to get the same word from two different paths.  Duplicates
// are prevented by using a set for collecting words found.
//
// A path is simply a vector of cell number integers.  The first entry is
// the starting position of the path.
//
// Care has been taken to keep looking for words down a path even if one word
// has already been found.  Sometimes larger words are extensions of smaller
// words.
//
    
    bool walkBoggleGraph(Path path, 
                         Grid<char> & board,
                         const Lexicon & lex, 
                         Set<std::string> & wordsFound,
                         std::string wordlet, 
                         Set<int> visited,
                         int min, 
                         int max, 
                         bool foundOne = false) {
        // 
        // Found the end of a word.  Add word to the collection.
        //
        
        if (wordlet.length() >= min && lex.contains(wordlet)) {
            wordsFound += wordlet;
            foundOne = true;        // There may be multiple words on a path.
        }
        
        //
        // Base case: reached word length limit or ran out of path to explore
        //
        // Return true if any words were found relative to the beginning of
        // this specific path.
        //
        // This formulation also ensures we continue exploring even if we've
        // already found the end of a word along this path.
        //
        // A longer word that includes a shorter word as a prefix may still
        // lie ahead provided we haven't traversed the entire grid
        // (e.g., 'program' vs 'programming').
        //
        
        if (wordlet.length() >= max || path.size() == MAX_CELL) {
            return foundOne;
        }
        
        // Recursive case:
        //
        // Search for an n-length word in the grid from a starting position 
        // and an adjacent sequence of n-1 characters.
        //
        
        int tail = path.size() - 1;
        int here = path[tail];                        // Our current location.
        
        for (int adjNode : getAdjacents(here)) {      // Explore adjacencies.
            
            if (visited.contains(adjNode)) continue;  // Keep path acyclic.

            visited.add(adjNode);                // Update where we've been.
            path += adjNode;                     // Update the directed path.
            wordlet += getCh(board, adjNode);    // Grow word prefix.
            
            if (lex.containsPrefix(wordlet) &&
                walkBoggleGraph(path, board, lex, wordsFound, wordlet,
                                visited, min, max, foundOne)) {
                return true;
            }
            
            // 
            // Backtracking case:
            //
            // Discard the current path since no words found.
            // Rollback one step so we're positioned to try a different 
            // adjacency in the next iteration.
            //
            
            foundOne = false;
            wordlet.erase(wordlet.size() - 1);
            path.remove(path.size() - 1);
            visited.remove(adjNode);
        }
        
        // Exit if all possible paths from a given starting position
        // have been explored.
        //
        // Otherwise, trigger backtracking to an earlier context if we're
        // on a wordless path at the moment.

        return false;
    }
    
//
// Method: getAdjacents
// Usage: Vector<int> cells = getAdjacents(cell);
// ----------------------------------------------------------------------
// Returns a vector of integers representing adjacent cells to the one
// passed in.  Cell numbers are assumed to be 1-based.
//
    
    Vector<int> getAdjacents(int cell) {
        Vector<int> adjacents;
        
        int row = (cell - 1) / LENGTH;
        int col = (cell - 1) % LENGTH;
        
        for (int r = row - 1; r <= (row + 1); r++) {
            for (int c = col - 1; c <= (col + 1); c++) {
                if  (r == row && c == col) continue;
                if (grid.inBounds(r, c)) {
                    adjacents.add(getCellNum(r, c));
                }
            }
        }
        return adjacents;
    }
    
//
// Method: getRC
// Usage: getRC(cell, row, col);
// -----------------------------
// Returns the row and col corresponding to a linearized cell number.
//
    
    void getRC(int cellNum, int & row, int & col) {
        row = (cellNum - 1) / LENGTH;
        col = (cellNum - 1) % LENGTH;
    }
    
//
// Method: getCellNum
// Usage: int cellNum = getCellNum(row, col);
// ---------------------------------------
// Returns linear cell number given row and col.
//
    
    int getCellNum(int row, int col) {
        return row * LENGTH + col + 1;
    }
    
//
// Method: getCh
// Usage: char ch = getCh(grid, cellNum);
// --------------------------------------
// Returns the character associated with a given cell.
//
    
    char getCh(Grid<char> & board, int cellNum) {
        int r, c;
        getRC(cellNum, r, c);
        return board[r][c];
    }
    
//
// Method: displayMove
// Usage: displayMove(move);
// -------------------------
// Updates the display with the results of a player's move.
//
    
    void displayMove(Move move) {
        int i = 0;
        if (whoseTurn == HUMAN) {
            cout << endl;
            if (move.wordsFound.size() > 0) {
                cout << "You found " << move.wordsFound.size() << " word";
                if (move.wordsFound.size() != 1) {
                    cout << "s";
                }
                cout << "." << endl;
                cout << string(50, '-') << endl;
                for (std::string word : move.wordsFound) {
                    cout << setw(2) << left << ++i << " " << word << endl;
                }
            } else {
                cout << "You found no words. :-/" << endl;
            }
        } else {
            if (move.wordsFound.size() > 0) {
                cout << "I found " << move.wordsFound.size() << " word";
                if (move.wordsFound.size() != 1) {
                    cout << "s";
                }
                cout << "." << endl;
                cout << string(50, '-') << endl;
                for (std::string word : move.wordsFound) {
                    cout << setw(2) << left << ++i << " " << word << endl;
                }
            } else {
                cout << "I found no words. :-/" << endl;
            }
        }
        cout << endl;
    }
    
//
// Method: getUserMove
// Usage: Move move = getUserMove();
// ----------------------------------
// Asks the user to enter a list of words they see in the boggle grid.
//
    
    Move getUserMove() {
        Move move;
        cout << "Your turn.  Enter one word per line.";
        cout << endl;
        while (true) {
            std::string word = toLowerCase(getLine("Word: "));
            if (word.empty()) break;
            if (word.length() >= MIN_LENGTH && word.length() <= MAX_LENGTH) {
                if (bogglex.contains(word)) {
                    if (contains(word)) {
                        move.wordsFound += word;
                    } else {
                        cout << "That word isn't there, yo." << endl;
                    }
                } else {
                    cout << "Um, is that a word? :-/" << endl;
                }
            } else {
                cout << "Words must be between " << MIN_LENGTH << " and ";
                cout << MAX_LENGTH << " characters long." << endl;
            }
        }
        return move;
    }
    
//
// Method: contains
// Usage: if (contains("word")) { . . . }
// --------------------------------------
// Returns true if a given string exists within the boggle grid as a series
// of adjacent characters.
//
    bool contains(std::string str) {
        return gridWords.contains(str);
    }
    
//
// Method: announceResult
// Usage: announceResult();
// ------------------------
// This method announces the final result of the game.
//
    
    void announceResult() {
        if (turnCount < 2) {
            /* quit */;
        } else if (numComputerWords > numHumanWords) {
            cout << "I win." << endl;
        } else if (numHumanWords > numComputerWords) {
            //
            // TODO: This will never happen unless we introduce expertise-level
            //       to the computer's turn.  Otherwise the best you can hope
            //       for is a tie under the current implementation.
            //       Skynet, anyone?
            //
            cout << "You win." << endl;
        } else {
            cout << "It's a tie." << endl;
        }
    }
    
//
// Method: getCurrentPlayer
// Usage: if (getCurrentPlayer() == HUMAN) . . .
// ---------------------------------------------
// Returns the player currently selected to take a turn.
//
    
    Player getCurrentPlayer() {
        return whoseTurn;
    }
    
//
// Method: switchTurn
// Usage: switchTurn();
// --------------------
// Selects the next player to take a turn.
//
    
    void switchTurn() {
        whoseTurn = opponent(whoseTurn);
    }

//
// Method: makeMove
// Usage: makeMove(move);
// ----------------------
// Updates the game state by making the given move.
//
// In Boggle, the notion of move making is a bit nebulous.  The state of the
// board does not change, per se.  So I'm going to highjack this general
// routine called by the game engine to update the score of the respective
// player.
//
    
    void makeMove(Move move) {
        ++turnCount;
        if (whoseTurn == COMPUTER) {
            numComputerWords = move.wordsFound.size();
        } else {
            numHumanWords = move.wordsFound.size();
        }
    }
    
//
// Method: getComputerMove
// Usage: int nTaken = getComputerMove();
// --------------------------------------
// Figures out what move is best for the computer player and returns that.
//
    
    Move getComputerMove() {
        Move move;
        
        move.wordsFound = gridWords;
        numComputerWords = move.wordsFound.size();
        return move;
    }
    
//
// Method: gameIsOver
// Usage: if (gameIsOver()) . . .
// ------------------------------
// Returns true if the game is over.
//
    
    bool gameIsOver() {
        return turnCount == 2;
    }
    
//
// Method: opponent
// Usage: Player other = opponent(player);
// ---------------------------------------
// Returns the opponent of the player.
//
// The opponent of the computer is the human player and vice versa.
//
    
    Player opponent(Player player) {
        return (player == HUMAN) ? COMPUTER : HUMAN;
    }
    
// Instance variables
    
    Player whoseTurn;      // Identifies which player moves next.
    Player startPlayer;    // Identifies player who starts the game.
    int numHumanWords;     // Number of validated words from human player.
    int numComputerWords;  // Number of validated words from computer.
    int turnCount;         // Detect when game is over (two turns taken).
    Lexicon bogglex;       // Lexicon for identifying valid words.
    Grid<char> grid;       // The field of play.
    Set<std::string> gridWords; // All words found in the grid.
};

// Free functions

//
// Operator: <<
// Usage: cout << game << endl;
// ----------------------------
// Displays the boggle grid to the output stream.
//

ostream & operator<<(ostream & os, Grid<char>  & gameGrid) {
    for (int r = 0; r < gameGrid.nRows; r++) {
        for (int c = 0; c < gameGrid.nCols; c++) {
            os << setw(2) << gameGrid[r][c] << " ";
            if ((c + 1) % LENGTH == 0) {
                if ((r + 1) < LENGTH) {
                    os << endl << "---";
                    for (int i = 1; i < LENGTH; i++) {
                        os << "+---";
                    }
                    os << endl;
                }
            } else {
                os << "|" ;
            }
        }
    }
    os << endl;
    return os;
}

// Main program

int main() {
    cout << BANNER << endl << endl;
    Lexicon english("EnglishWords.dat");
    Boggle game(STARTING_PLAYER, english);
    
    game.play(TEXTGAME);    // Play game from the textbook.

    bool playAgain = true;
    do {
        std::string response = toLowerCase(getLine("Play again? ([y],n): "));
        if (response != "n" && response != "no") {
            Boggle game(STARTING_PLAYER, english);
            game.play();
        } else {
            playAgain = false;
            cout << "Goodbye" << endl;
        }
    } while (playAgain);

    return 0;
}