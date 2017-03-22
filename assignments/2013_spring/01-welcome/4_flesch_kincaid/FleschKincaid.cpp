//
//
// FleschKincaid.cpp
//
// This program calculates the approximate Flesch-Kincaid Grade-level
// readability metric for an input file of text specified at the console
// according to the following formula:
//
// Grade = C0 + C1(num words / num sentences) + C2(num syllables / num words)
//
//     where C0 = -15.59, C1 = 0.39, C2 = 11.8
//
// For example:
//
//     ----------------------------------------------------
//     Enter a file to analyze: TomSawyer.txt
//     syllables: 94385
//     words: 69218
//     sentences: 4961
//     grade: 5.94181
//
// Generally speaking, the results of this program are within 5% of the
// provided reference solution.
//
// --------------------------------------------------------------------------
// Assignment: 1, Part 4 Flesch-Kincaid Grade-level Readability
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/050%20Assignment%201.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/18/2015.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

#define DEBUGGING false // Set this to true to see tokenized output.

// Types and constants


// Input text is decomposed into tokens, some of which are actionable.
// Use enumeration to define those actions.

enum TokenAction {
    no_action,
    delimit_syllable    // We delimit syllables on voiced vowel boundaries.
};

// Provide the end-of-sentence detector some processing hints.

enum EOSHint {
    none,
    end_of_line,
};

// Function prototypes

void analyzeFile(ifstream & in,
                 int & syllables,
                 int & words,
                 int & sentences,
                 bool debugging = DEBUGGING);
string analyzeLine(string line,
                   int & sentenceCount,
                   int & syllableCount,
                   int & words);
size_t findNonconsecutiveVowel(string word, bool yIsVowel = false);
size_t findVowel(string word, bool yIsVowel=false);
bool getInputStream(ifstream & in, string prompt);
size_t getTokenLength(string word, TokenAction & action, bool yIsVowel = false);
float gradeLevel(int syllables, int words, int sentences);
bool hasTokens(string word);
bool hasTokenableTrailingY(size_t tokenLength, string word);
bool hasTrailingSilentE(size_t tokenLength, string word);
bool isConsonant(char ch, bool yIsConsonant = true);
bool isendofsentence(char ch, EOSHint = none);
bool isterminator(char ch);
bool isTerminating(string word, char ch, bool ignoreCase = true);
bool isVowel(char ch, bool yIsVowel = false);
string nextToken(string & word, TokenAction & action, bool yIsVowel = false);
bool openFile(ifstream & in, string fileName);
bool pickFile(string prompt, string & fileName);
void reportResults(float grade, int syllables, int words, int sentences);
string syllablize(string word, int & syllables);
string tolower(string in);

// Main program

int main(int argc, const char * argv[]) {
    ifstream in;
    int syllables = 0;
    int words = 0;
    int sentences = 0;
    float grade;
    
    cout << "This program reads a text file and computes the Flesch-Kincaid\n"
         << "grade-level readability metric based upon the content.\n" << endl;
    
    if (getInputStream(in, "Enter a file to analyze: ")) {
        analyzeFile(in, syllables, words, sentences);
        grade = gradeLevel(syllables, words, sentences);
        reportResults(grade, syllables, words, sentences);
    }
    return 0;
}

// Function definitions

//
// Function: getInputStream
// Usage: getInputStream(in, "Enter a file name: ");
// -------------------------------------------------
// Returns true if user enters a filename which can be opened.  Otherwise
// it continues to prompt the user until they enter a valid filename or an
// empty filename (by hitting 'Enter').  In the case of the later, false is
// returned.
//

bool getInputStream(ifstream & in, string prompt) {
    string fileName;
    
    while (true) {
        if (pickFile(prompt, fileName)) {
            if (openFile(in, fileName)) break;
        } else {
            cout << "No file selected.  Goodbye.";
            return false;
        }
    }
    return true;
}

//
// Function: reportResults
// Usage: reportResults(grade, numSyllables, numWords, numSentences);
// ---------------------------------------------------------------
// Reports low-level metrics and overall grade-level to the console.
//

void reportResults(float grade, int syllables, int words, int sentences) {
    cout << "syllables: " << syllables << endl;
    cout << "words: " << words << endl;
    cout << "sentences: " << sentences << endl;
    cout << "grade: " << grade << endl;
}

//
// Function: analyzeFile
// Usage: analyzeFile(in, syllables, words, sentences); { ... }
// ------------------------------------------------------------
// Returns the number of syllables, words, and sentences associated
// with text read from an input stream of an opened file.
//

void analyzeFile(ifstream & in,
                 int & syllables,
                 int & words,
                 int & sentences,
                 bool debugging) {
    string line;
    string syllablizedLine;
    
    while (true) {
        getline(in, line);
        if (in.fail()) {
            break;
        }
        syllablizedLine = analyzeLine(line, syllables, words, sentences);
        if (debugging) {
            cout << syllablizedLine << endl;
        }
    }
}

//
// Function: gradeLevel
// Usage: float G = gradeLevel(numSyllables, numWords, numSentences);
// ------------------------------------------------------------------
// Computes the approximate grade level required by the reader to
// comprehend a given body of English text according to the Flesch-Kincaid
// Grade-Level formula.
//

float gradeLevel(int syllables, int words, int sentences) {
    const float C0 = -15.59;
    const float C1 = 0.39;
    const float C2 = 11.8;
    float g;
    
    // Guard against divide by 0.
    
    words = (words == 0) ? 1 : words;
    sentences = (sentences == 0) ? 1 : sentences;
    
    g = C0 + C1 * words / float(sentences) + C2 * syllables / float(words);
    return g;
}

//
// Function: pickFile
// Usage: if (pickFile("Enter a file to process: ", fileName)) {...}
// -----------------------------------------------------------------
// Prompts user for the name of a file to process.  Returns true if a
// non-empty filename is entered, otherwise returns false, signaling
// the user's desire to not pick a file.
//

bool pickFile(string prompt, string& fileName) {
    
    cout << prompt;
    getline(cin, fileName);
    return (fileName != "");
}

//
// Function: openFile
// Usage: bool opened = openFile(in, fileName);
// --------------------------------------------
// Returns true and readies the input filestream for reading if the named file
// is successfully opened.  Otherwise false is return and related message is
// written to the error console.
//

bool openFile(ifstream& in, string fileName) {
    bool result;
    
    in.open(fileName.c_str());
    if (in.fail()) {
        cerr << "Unable to open " << fileName << endl;
        in.clear();
        result = false;
    } else {
        result = true;
    }
    return result;
}

//
// Function: analyzeLine
// Usage: string syllableDelimitedLine = analyzeLine(line, syl, wrd, sent);
// Credits: Eric Roberts (largely derived from lineToPigLatin() in
//                        "Programming Abstractions in C++, Chpt 3, pg 153)
// --------------------------------------------------------------------------
// Decomposes each word in a line into syllable-delimited text, leaving all
// other characters unchanged.  The variable, start, keeps track of the index
// position at which the current word begins.  As a special case,
// the code sets start to -1 to indicate that the beginning of the
// current word has not yet been encountered.
//

string analyzeLine(string line, int& syllables, int& words, int& sentences) {
    string result;
    int start = -1;
    char ch;
    
    for (int i = 0; i < line.length(); i++) {
        ch = line[i];
        if (isendofsentence(ch)) {
            sentences++;
        }
        if (isalpha(ch)) {
            if (start == -1) start = i;
        } else {
            if (start >= 0) {
                result += syllablize(line.substr(start, i - start), syllables);
                words++;
                start = -1;
            }
            result += ch;
        }
    }
    if (isendofsentence(ch, end_of_line)) {
        sentences++;
    }
    if (start >= 0) result += syllablize(line.substr(start), syllables);
    return result;
}

//
// Function: isendofsentence
// Usage: if (isendofsentence(ch)) sentenceCount++;
// ------------------------------------------------------------
// Returns true if an end-of-sentence pattern is detected, typically across
// consecutive calls.
//
// It'd be nice if you could simply detect the end of a sentence by the presence
// of a period.  But you really don't know if you've hit the end of a sentence
// until a new one starts, especially with literary flourishes such as:
//
// "You said what?!"
// "And you are...?"
//
// where /multiple/ puntuation marks may terminate a sentence.
//
// This implies the need for stateful examination of the input stream so we have
// adequate context to discern when one sentence ends and another beings.
//
// This function enters into a "seeking" state when a period, question mark, or
// exclamation point is detected and stays there, through subsequent calls,
// until a non-terminating character (such as a space, double quote, or
// alphabetic character) is passed in, signaling the boundary between two
// sentences, allowing us to assert and end-of-sentence condition.
//

bool isendofsentence(char ch, EOSHint hint) {
    static int terminators = 0;
    
    // Sentence terminators at the end of a line correlate strongly to
    // end of sentence.  Don't bother with state machine.
    
    if ((hint == end_of_line) && (terminators > 0)) {
        terminators = 0;
        return true;
    }
    
    // Enter the state of seeking the end of sentence now that we've
    // hit at least one sentence terminator (e.g., [. ? !])
    
    if (isterminator(ch)) {
        terminators++;      // Normalize consecutive terminators instead of
                            // counting each as the end of a new sentence.
        return false;
    }
    
    if (terminators == 0) {
        
        // This character is considered part of the current sentence since
        // we haven't hit any terminating punctuation yet.
        
        return false;
    }
    
    // This character likely delimits two sentences so reset state and assert
    // end-of-sentence.
    
    terminators = 0;
    return true;
}

//
// Function: isterminator
// Usage: if (isterminator(".")) {...}
// -----------------------------------
// Return true if the character passed in matches a period, exclamation mark,
// or question mark.  Returns false otherwise.
//

bool isterminator(char ch) {
    bool result = false;
    
    switch (ch) {
        case '.': case '!': case '?':
            result = true;
            break;
            
        default:
            break;
    }
    return result;
}

//
// Function: syllablize
// Usage: string translation = syllablize(word);
// --------------------------------------------
// This function approximately splits English text into syllable fragments using
// the rules specified in the tokenizing routine, getTokenLength().
//
// Generally speaking, words are tokenizied on voiced vowel boundaries.
//
// With each token extracted, the original word is correspondingly shortened
// for subsequent tokenization until nothing is left.
//
// The nextToken routine performs this mutation and also informs the caller
// how to process the resulting token.
//

string syllablize(string word, int& syllables) {
    string result;
    
    while (hasTokens(word)) {
        TokenAction action;
        string token = nextToken(word, action);
        //cout << "debug: token = " << token << endl;
        switch (action) {
            case delimit_syllable:
            {
                string delim = "~";
                syllables++;
                result += delim + token;
            }
                break;
            case no_action:
            default:
                result += token;
        }
    }
    return result;
}

//
// Function: hasTokens
// Usage: if (hasTokens(word)) { ... }
// -----------------------------------
// Returns true if a given word has characters to parse.
//

bool hasTokens(string word) {
    return (word.length() > 0);
}

//
// Function: nextToken
// Usage: string token = nextToken(word, action);
// ----------------------------------------------
// Returns a single syllable substring from the front of a word; also
// returns what processing action should be preformed, if any, by the caller.
//
// The original word is shortened by the chopped off token,
// making this function suitable for loops that iterate until the
// word has 0-length.
//

string nextToken(string& word, TokenAction& action, bool yIsVowel) {
    string token;
    size_t length = getTokenLength(word, action, yIsVowel);
    
    token = word.substr(0, length);
    word.erase(0, token.length());
    
    return token;
}

//
// Function: getTokenLength
// Usage: size_t n = getTokenLength(word, action); token = word.substr(0, n);
// --------------------------------------------------------------------------
// Returns the length of the next token (generally along a vowel or word
// boundary).
//
// Ideally, we would tokenize at voiced vowels.  However, that requires phonetic
// sophistication beyond the scope of this problem.
//
// We use these simplifying rules:
//
// 1. If a token starts with a vowel, stop on the next non-consecutive vowel
//    such that groups of neighboring vowels (and consonants) stay together:
//
//      e.g., oobber --> oobb + er (return 4 since 'e' is non-consecutive vowel)
//                       ----   ^
//
// 2. If a token starts with a consonant, stop on the next vowel.
//
//      e.g., play --> pl + ay (returns 2 since 'a' is a vowel)
//                     --   ^
//
// 3. If we get to the end of the string without satisfying any of the
//    conditions above, assume the token is the complete word.
//
// Exceptions to the rules include these two wrinkles:
//
// 4. Assume 'e' at the end of a word is generally silent.
//
//      e.g., abe --> abe  (not ab + e)  (returns 3, exception to Rule 1)
//                    ---^
//
//    This is especially true for cosonant+e patterns unless we've devolved
//    into two-letter words like 'me' and 'be' which have voiced e's.
//
// 5. Treat trailing y's as voiced vowels (as in 'happy') but don't tokenize
//    when it neighbors another vowel with a single blended sound (as
//    in 'pay' or 'hey'.  (Hum, what about buoy?)
//

size_t getTokenLength(string word, TokenAction& action, bool yIsVowel) {
    size_t pos;
    size_t length = 0;
    
    if (isVowel(word[0], yIsVowel)) {
        
        // Rule 1: Token starts with vowel, ends with next non-consec vowel.
        
        pos = findNonconsecutiveVowel(word, yIsVowel);
        action = delimit_syllable;  // Use voiced vowel sounds as syllables.
        
    } else {
        //
        // Rule 2: Token starts with consonant, stops with next vowel.
        //
        // e.g., For a word like "program", search until "o" is found at
        //       position 2, giving a token length of 2, corresponding to "pr":
        //
        //       program = pr + ogram
        //
        //       (A subsequent call will detect "ogr" under Rule 1 as the
        //       second token and mark it as a syllable delimiter:
        //
        //       program = pr ~ ogr
        //
        //       This is contrary to the expected tokenization:
        //
        //       pro + gram
        //
        //       but the dark secret is I wrote this tokenizer initially for
        //       an Obenglobish translator that benefitted from having tokens
        //       /begin/ with vowels rather than /end/ with them, such that
        //       the "ob" sound could be easily pre-pendended to the token as in
        //
        //       pr-obogr-obam :-)
        //
        pos = findVowel(word, yIsVowel);
        action = no_action;
    }
    
    if (pos != string::npos) {
        //
        // Found a valid token delimiter.
        //
        // Since pos is a 0-based index to the first character of the /next/
        // token, we can also use it as the length of the current token.
        //
        length = pos;
        
    } else {
        //
        // Fell off the end of string before finding token delimiter.
        //
        // Rule 3: Assume token is just the word itself by setting token length
        //         to word length.
        //
        length = word.length();
    }
    
    //
    // Post-process the candidate token length a tad for trailing silent-e and
    // voiced trailing-y scenarios.
    //
    
    if (hasTrailingSilentE(length, word)) {
        //
        // Rule 4
        //
        // Normally we'd tokenize on a vowel.  Reattach the silent-e
        // to the token.
        //
        length = word.length();
        
    } else if (hasTokenableTrailingY(length, word)) {
        //
        // Rule 5
        //
        // Chop off trailing-y from this token since it is probably voiced.
        //
        length--;
        
    } else {
        if ((word == "y") || (word == "Y")) {
            //
            // Assume we're down to a final voiced trailing-y.  Treat
            // this more like a vowel.
            //
            action = delimit_syllable;
        }
    }
    return length;  // can i have that day back?
}

//
// Function: hasTrailingSilentE
// Usage: if (hasTrailingSilentE(tokenLength, word)) { ... }
// ---------------------------------------------------------
// Returns true if a word includes a silent-e at the end.
//

bool hasTrailingSilentE(size_t tokenLength, string word) {
    
    //
    // Ignore atomic case where current word length is nothing or just a
    // single letter.
    //
    // We're already at the smallest word size so trust that earlier
    // invocations have already handled the silent-e scenario (since that
    // should only surface in the context of non-trivially sized words).
    //
    // Also, this guard logic ensures we can safely perform a consonant+e
    // test below.
    //
    
    if (word.length() <= 1) {
        return false;
    }
    
    //
    // Check for trailing-e.
    //
    
    if (!isTerminating(word, 'e')) {
        return false;
    }
    
    //
    // In the silent-e scenario, the token will be the same
    // as the word except missing an 'e' at the end.  Test for this
    // by normalizing both strings.
    //
    
    string normalizedToken = word.substr(0, tokenLength) + 'e';
    string normalizedWord  = word.substr(0, word.length() - 1) + 'e';
    
    if (normalizedToken != normalizedWord) {
        return false;
    }
    
    //
    // Silent-e generally occurs with consonant+e.  Fail otherwise.
    //
    
    if (!isConsonant(word[word.length() - 2])) {
        return false;
    }
    
    //
    // Ignore two-letter words like 'me' and 'be'.
    //
    // TODO: Implement this as a count of non-consecutive vowels to get
    //       at better heuristic of voiced versus unvoiced.
    //
    
    if (word.length() == 2) {
        return false;
    }
    
    //
    // Um, words that have an 'e' at the end as their /only/ vowel should not
    // be considered unvoiced.  Otherwise "the" was getting analyzed as having
    // no syllables. :-/
    //
    // TODO: Replace this expedient hard-code with something more algorithmic.
    //
    
    if (tolower(word) == "the") {
        return false;
    }
    
    return true;
}

//
// Function: hasTokenableTrailingY
// Usage: if (hasTokenableTrailingY(tokenLength, word)) tokenLength--;
// -------------------------------------------------------------------
// Returns true if a word ending in y, should be split
// into tokens at the y-boundary, since trailing-y often sounds like a
// voiced vowel (as in 'happy').
//
// Implementaion is complicated by the need to distinguish interior
// y's that come at the end of substring tokens and y's that really
// come at the end of the word we're tokenizing.
//
// Also, some words ending in vowel+y combinations (like 'boy' or 'pay')
// should /not/ be tokenized since the last two letters form a single
// sound.  Contrast this to 'happy' where the y is voiced separately
// from neighboring letters and is a good candidate for tokenizing.
//

bool hasTokenableTrailingY(size_t tokenLength, string word) {
    
    //
    // Ignore atomic case where current token is empty or just a single letter.
    // We're already at the smallest token size so nothing to split apart.
    //
    // Also, this guard logic allows safe vowel+y pattern testing later.
    //
    
    if (tokenLength <= 1) {
        return false;
    }
    
    //
    // Ignore case where current token is a substring of the word
    // we're tokenizing.
    //
    // We want to tokenize along the trailing-y of the /word/,
    // not some interior-y that happens to be at the end of a
    // substring token.  (Leading or interior y's are better treated
    // as consonants, unlike trailing y's that are more like voiced
    // vowels at the ends of words (e.g., 'yoyo' versus 'happy').
    //
    
    if (tokenLength < word.length()) {
        return false;
    }
    
    //
    // Word should end in y.
    //
    
    if (!isTerminating(word, 'y')) {
        return false;
    }
    
    //
    // Ignore case where word ends in a vowel+y combination,
    // as in 'joy', since the 'oy' represents a single sound.
    // Don't split on the y-boundary.
    //
    
    if (isVowel(word[word.length() - 2])) {
        return false;
    }
    
    //
    // Turn 'icky' into 'ick' + 'y'.
    //
    
    return true;
}

//
// Function: findNonconsecutiveVowel
// Usage: size_t pos findNonconsecutiveVowel(word);
// ------------------------------------------------
// Returns the 0-based index location of the first non-consecutive
// vowel in word.  A non-consecutive vowel is one that follows a consonant.
// Returns string::npos otherwise.
//

size_t findNonconsecutiveVowel(string word, bool yIsVowel) {
    size_t pos = string::npos;
    bool ignoreVowels = true;   // Ignore consecutive vowels.
    
    for (int i = 0; i < word.length(); i++) {
        bool yIsConsonant = !yIsVowel;
        if (isConsonant(word[i], yIsConsonant)) {
            ignoreVowels = false;
        } else {
            if (!ignoreVowels) {
                pos = i;
                break;
            }
        }
    }
    return pos;
}

//
// Function: findVowel
// Usage: size_t pos = findVowel(word);
// ------------------------------------
// Returns the 0-based index location of the first vowel found in a word.
// Returns string::npos otherwise.
//

size_t findVowel(string word, bool yIsVowel) {
    size_t pos = string::npos;
    
    for (int i = 0; i < word.length(); i++) {
        if (isVowel(word[i], yIsVowel)) {
            pos = i;
            break;
        }
    }
    return pos;
}

//
// Function: isVowel
// Usage: if (isVowel(ch)) ...
// ------------------------------
// Returns true if the character ch is a vowel (or vowel-like with 'y').
//

bool isVowel(char ch, bool yIsVowel) {
    string vowels = "aeiouAEIOU";
    if (yIsVowel) vowels += "yY";
    return (vowels.find(ch) != string::npos);
}

//
// Function: isConsonant
// Usage: if (isConsonant(ch)) ...
// -------------------------------
// Returns true if the character ch is a vowel (or vowel-like with 'y').
//

bool isConsonant(char ch, bool yIsConsonant) {
    bool yIsVowel = !yIsConsonant;
    return !isVowel(ch, yIsVowel);
}

//
// Function: isTerminating
// Usage: if (isTerminating(word, ch)) { ... }
// ---------------------------------------------
// Returns true if the character specified matches the character at the end
// of the word provided.
//

bool isTerminating(string word, char ch, bool ignoreCase) {
    if (word.length() > 0) {
        char lastChar = word[word.length() - 1];
        if (ignoreCase) {
            return (lastChar == ch || lastChar == toupper(ch));
        } else {
            return (lastChar == ch);
        }
    } else {
        return false;
    }
}

//
// Function: tolower
// Usage: string str = tolower("STRING"); // returns "string";
// -----------------------------------------------------------
// Returns a lower-case version of the input string as a new string.
//

string tolower(string in) {
    string result;
    
    for (int i = 0; i < in.length(); i++) {
        result += tolower(in[i]);
    }
    return result;
}
