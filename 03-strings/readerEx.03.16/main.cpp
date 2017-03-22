//
// main.cpp
//
// This program converts English text into Obenglobish such that the
// string "ob" is inserted prior to voiced vowels within a given word.
//
// The following simplifying heuristics are used:
//
//     Don't tokenize on vowels that follow other vowels.
//     Don't tokenize on final e as it may be silent.
//
// For example:
//
//     obenglobish("english") returns "obenglobish"
//     obenglobish("amaze")   returns "obamobaze"
//
// TODO: Add test cases!  Some good test words:
//       english, hobnob, gooiest, amaze, rot, yoyo, happy, pay, sea
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 3, Exercise 16
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/7/15.
// Copyright © 2015 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

// Types and constants

enum ObTokenAction {
    prefixOB,
    nochange
};

// Function prototypes

string getCaseAwareOb(string & token);
bool hasTokens(string word);
bool hasTokenableTrailingY(size_t tokenLength, string word);
bool hasTrailingSilentE(size_t tokenLength, string word);
size_t getTokenLength(string word, ObTokenAction & action, bool yIsVowel=false);
size_t findNonconsecutiveVowel(string word, bool yIsVowel=false);
size_t findVowel(string word, bool yIsVowel=false);
bool isConsonant(char ch, bool yIsConsonant=true);
bool isTerminating(string word, char ch);
bool isVowel(char ch, bool yIsVowel=false);
string nextToken(string & word, ObTokenAction & action, bool yIsVowel=false);
string obenglobish(string word);
string translateLine(string line);

// Main program

int main(int argc, const char * argv[]) {
    string line;
    cout << "This program translates English to Obenglobish." << endl;
    
    do {
        cout << "Enter English text: ";
        getline(cin, line);
        string translation = translateLine(line);
        cout << "Obenglobish output: " << translation << endl;
    } while (line != "");
    
    return 0;
}

// Function definitions

//
// Function: translateLine
// Usage: string translation = translateLine(line);
// Courtesy: Eric Roberts
// -------------------------------------------------
// Translates each word in the line to Obenglobish, leaving all other
// characters unchanged.  The variable start keeps track of the index
// position at which the current word begins.  As a special case,
// the code sets start to -1 to indicate that the beginning of the
// current word has not yet been encountered.
//

string translateLine(string line) {
    string result;
    int start = -1;
    
    for (int i = 0; i < line.length(); i++) {
        char ch = line[i];
        if (isalpha(ch)) {
            if (start == -1) start = i;
        } else {
            if (start >= 0) {
                result += obenglobish(line.substr(start, i - start));
                start = -1;
            }
            result += ch;
        }
    }
    if (start >= 0) result += obenglobish(line.substr(start));
    return result;
}

//
// Function: obenglobish
// Usage: string translation = obenglobish(word);
// --------------------------------------------
// This function translates a word from English to Obenglobish using the rules
// specified in the tokenizing routine, getTokenLength().
//
// Generally speaking, words are tokenizied on voiced vowel boundaries
// with an "ob" prepended before vowels as in this example:
//
//  "english" --becomes--> engl + ish --> obengl + obish --> "obenglobish"
//
// The translated word is returned as the value of the function.
//
// With each token extracted, the original word is correspondingly shortened
// for subsequent tokenization until nothing is left.
//
// The nextToken routine performs this mutation and also informs the caller
// how to process the resulting token (i.e., by pre-pending "ob" to it or
// simply allowing the token to pass unchanged into the output string).
//

string obenglobish(string word) {
    string result;
    
    while (hasTokens(word)) {
        ObTokenAction action;
        string token = nextToken(word, action);
        //cout << "debug: token = " << token << endl;
        switch (action) {
            case prefixOB:
                {
                    string ob = getCaseAwareOb(token);
                    result += ob + token;
                }
                break;
            case nochange:
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
// Returns a single substring from the front of a word to be
// obenglobish-ified by the caller.  Also returns what processing
// action should be preformed by the caller.
//
// The original word is shortened by the chopped off token,
// making this function suitable for loops that iterate until the
// word has 0-length.
//

string nextToken(string & word, ObTokenAction & action, bool yIsVowel) {
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
// Returns the length of the next Obenglobish token (generally along
// a vowel or word boundary).
//
// Also returns the syntactic action the caller should perform with
// the associated token (i.e., pre-pend "ob" or no change).  This is
// accomplished with the pass-by-reference output variable, action.
//
// Ideally, we would tokenize at voiced vowels for fluent
// Obenglobish.  However, that requires phonetic sophistication
// beyond the scope of this problem.
//
// Instead we use these simplifying rules:
//
// 1. If a token starts with a vowel, stop on the next non-consecutive vowel
//    such that groups of neighboring vowels (and consonants) stay together:
//
//      e.g., oobber --> oobb + er (return 4 since 'e' is non-consecutive vowel)
//                       ----   ^
//            ob-oobb-ob-er
//
// 2. If a token starts with a consonant, stop on the next vowel.
//
//      e.g., play --> pl + ay (returns 2 since 'a' is a vowel)
//                     --   ^
//            pl-ob-ay
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
//            ob-abe
//
//    This is especially true for cosonant+e patterns unless we've devolved
//    into two-letter words like 'me' and 'be' which have voiced e's.
//
// 5. Treat trailing y's as voiced vowels (as in 'happy') but don't tokenize
//    when it neighbors another vowel with a single blended sound (as
//    in 'pay' or 'hey'.  (Hum, what about buoy?)
//

size_t getTokenLength(string word, ObTokenAction & action, bool yIsVowel) {
    size_t pos;
    size_t length = 0;
    
    if (isVowel(word[0], yIsVowel)) {
        //
        // Rule 1: Token starts with vowel, ends with next non-consec vowel.
        //
        pos = findNonconsecutiveVowel(word, yIsVowel);
        action = prefixOB;
        
    } else {
        //
        // Rule 2: Token starts with consonant, ends with next vowel. 
        //
        pos = findVowel(word, yIsVowel);
        action = nochange;
        
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
            // this more like a vowel.  Hint to the caller that "ob"
            // should be prefixed to the corresponding token.
            //
            action = prefixOB;
        }
    }
    return length;  /* can i have that day back? */
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
    //  Check for trailing-e.
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
    // Otherwise joy becomes j-ob-o-ob-y instead of the expected
    // j-ob-oy. :-/
    //
    
    if (isVowel(word[word.length() - 2])) {
        return false;
    }
    
    //
    // Turn 'appy' into 'app' + 'y' to yield ob-app-ob-y
    // so words like 'happy' become h-ob-app-ob-y not h-ob-appy.
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
// Function: getCaseAwareOb(string & token)
// Usage: string ob = getCaseAwareOb("English"); // returns "Ob"
// -------------------------------------------------------------
// Returns an "ob" string that matches the case of the input token and
// possibly mutates the case of the token itself so we get case-appropriate
// translations like:
//
//      English --> Obenglobish :-)
//
// instead of:
//
//      English --> obEnglobish :-(
//

string getCaseAwareOb(string & token) {
    string ob = "ob";
    
    if (token[0] == toupper(token[0])) {
        ob = "Ob";
        if (token.length() > 1) {
            if (token[1] == toupper(token[1])) {
                ob = "OB";
            }
        }
        if (ob == "Ob") {
            token[0] = tolower(token[0]);
        }
    }
    return ob;
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
// Usage: if (isTerminating(word, pos, ch)) { ... }
// ---------------------------------------------
// Returns true if the character at the specified position is
// at the end of a word matches the letter provided.
//

bool isTerminating(string word, char ch) {
    if (word.length() > 0) {
        char lastChar = word[word.length() - 1];
        return (lastChar == ch || lastChar == toupper(ch));
    } else {
        return false;
    }
}