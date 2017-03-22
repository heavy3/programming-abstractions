//
// InverseGenetics.cpp
//
// This program enumerates all the valid RNA encodings for a given protein 
// sequence.
//
// Proteins are comprised of sequences of amino acids which are,
// themselves, comprised of specific 3-nucleotide sequences, called codons.
// The RNA nucleotides are the smallest building blocks for proteins in this
// model and are represented with the letters A, C, G, U.
//
// The same amino acid may be encoded from different nucleotide
// sequences.  This bit of natural redundancy yields a one-to-many
// mapping between a given protein and the underlying RNA that
// could synthesize that protein.
//
// For example:
//
//     amino acid  symbol   equivalent codons
//     ----------  ------   -----------------
//     asparagine     N     {AAU, AAC}
//          ?         W     {UGG}
//
// So the following protein sequence could be encoded two ways:
//
//     'NW' == AAU-UGG or AAC-UGG
//
// Example output:
//
//     CS106B Programming Abstractions: Assignment 3.3
//     Inverse Genetics
//
//     Enter a sequence of amino acids to encode.
//
//     Use one or more of the following letters: ACDEFGHIKLMNPQRSTVWY;
//     and limit the string length to: 10
//
//     sequence: kws
//
//     That protein could be encoded by any of these 12 RNA strands:
//     ------------------------------------------------------------
//      1. AAA-UGG-AGC
//      2. AAA-UGG-AGU
//      3. AAA-UGG-UCA
//      4. AAA-UGG-UCC
//      5. AAA-UGG-UCG
//      6. AAA-UGG-UCU
//      7. AAG-UGG-AGC
//      8. AAG-UGG-AGU
//      9. AAG-UGG-UCA
//     10. AAG-UGG-UCC
//     11. AAG-UGG-UCG
//     12. AAG-UGG-UCU
//
// --------------------------------------------------------------------------
// Assignment: 3, Part 3 Inverse Genetics
// Course: CS106B "Programming Abstractions in C++"
// Provider: Stanford University (SCPD)
// Instructor: Keith Schwarz
// Date: Spring 2013
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/handouts/110%20Assignment%203.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/7/2017.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include "simpio.h"
#include "map.h"
#include "set.h"
using namespace std;

// Constants

const string HEADER = "CS106B Programming Abstractions: Assignment 3.3\n";
const string DETAIL = "Inverse Genetics";
const string BANNER = HEADER + DETAIL;

const string SEQ_PROMPT = "Enter a sequence of amino acids to encode.";
const string SEQ_CHR = "ACDEFGHIKLMNPQRSTVWY;";
const int    SEQ_LEN = 10;

const string CHR_HELP = "Use one or more of the following letters: ";
const string LEN_HELP = "and limit the string length to: ";

const string CODON_FILE = "codons.txt";

const bool PRETTY_PRINT = true; // delimit codons by a '-' for readability

typedef Map<char, Set<string> > CodonMapT;

// Prototypes

string getProteinSeq(const string & prompt,
                     const string & chrGuidance,
                     const string & lenGuidance,
                     const string & validChars,
                     int maxLen);
bool initCodonMap(CodonMapT & condonMap, const string & codonFileName);
bool isValidSeq(const string & seq, int maxLen, const string & validCh);
void listAllRNAStrandsFor(string protein, const CodonMapT & codonMap);
void permuteCodons(Set<string> & rnaSeqs,
                   string soFar,
                   string rest,
                   const CodonMapT & codonMap);

// Main program

int main(int argc, char * argv[]) {
    cout << BANNER << endl << endl;
    
    CodonMapT codonMap;
    
    if (initCodonMap(codonMap, CODON_FILE)) {
        string protein = getProteinSeq(SEQ_PROMPT,
                                       CHR_HELP,
                                       LEN_HELP,
                                       SEQ_CHR,
                                       SEQ_LEN);
        listAllRNAStrandsFor(protein, codonMap);
    }
    return 0;
}

//
// Function: getProteinSeq
// Usage: string protein = getProteinSeq(prompt, chHelp, lenHelp, validCh, maxLen);
// --------------------------------------------------------------------------------
// Prompts user to enter a protein sequence encodes as a series of letters
// that correspond to specific amino acids.  Returns a valid sequence to the
// client.
//

string getProteinSeq(const string & prompt,
                     const string & chHelp,
                     const string & lenHelp,
                     const string & validCh,
                     int maxLen) {
    string result;
    
    while (true) {
        cout << prompt << endl << endl;
        cout << chHelp << validCh << endl;
        cout << lenHelp << maxLen << endl << endl;
        result = getLine("sequence: ");
	    if (isValidSeq(result, maxLen, validCh)) break;
    }
    return result;
}

// Function: initCodonMap
// Usage: if (initCodonMap(codonMap, "codons.txt")) ...
// ----------------------------------------------------
// Initializes a map of amino acids to codon sequences.  Returns the map
// as a pass-by-reference parameter.  The overall function returns a boolean
// false if the file was not found.
//
// The map is bootstrapped from data in a text file structured like this:
//
//     codons.txt
//     +----------------------
//     |UUU F
//     |UUC F
//     |UUA L
//     |UUG L
//     |..
//
// with codon on the left and amino acid character on the right.
//
// The same amino acid may be associated with multiple codons, suggesting a
// set as a reasonable container for the value portion of the key/value map:
//
//     codonMap = {
//         A:{"GCA", "GCC", "GCG", "GCU"},
//         C:{"UGC", "UGU"},
//         D:{"GAC", "GAU"},
//         E:{"GAA", "GAG"},
//         F:{"UUC", "UUU"},
//         ..
//     }
//

bool initCodonMap(CodonMapT & codonMap, const string & filename) {
    ifstream fs;
    bool result = false;
    
    fs.open(filename.c_str());
    if (fs) {
        string line;
        while (getline(fs, line)) {
            // TODO: More data validation here. :-)
            string codon = line.substr(0, 3);
            char aminoAcid = line[4];
            codonMap[aminoAcid].add(codon);
        }
        result = true;
        fs.close();
    } else {
        cerr << "Unable to initialize the amino acid to codon mapping." << endl;
        error("Unable to open file: " + filename);
    }
    return result;
}

//
// Function: isValidSeq
// Usage: if (isValidSeq("LMAADE", SEQ_LEN, SEQ_CHR)) . . .
// --------------------------------------------------------
// Returns true if the input sequence string is valid with regard to
// length and symbol content.
//

bool isValidSeq(const string & seq, int maxLen, const string & validCh) {
    if (seq.length() > maxLen) return false;
    for (int i = 0; i < seq.length(); i++) {
        char seqCh = toupper(seq[i]);
        int pos = validCh.find(seqCh);
        if (pos == string::npos) return false;
    } 
    return true;
}

//
// Function: listAllRNAStrandsFor
// Usage: listAllRNAStrandsFor("kws", codonMap);
// --------------------------------------------------
// Lists all the nucleotide encodings for a given protein and codon map.
// The codon map associates a single character in the protein string to
// one-or-more nucleotide sequences.
//
// This is mainly a wrapper for the recursive permuteCodons function.
//

void listAllRNAStrandsFor(string protein, const CodonMapT & codonMap) {
    Set<string> rnaStrands;
    
    permuteCodons(rnaStrands, "", protein, codonMap);
    
    cout << endl;
    switch (rnaStrands.size()) {
        case 0:
            cout << "No RNA strands found for that input." << endl;
            break;
        
        case 1:
            cout << "That protein could be encoded by this RNA strand:" << endl;
            cout << string(60, '-') << endl;
            break;
            
        default:
            cout << "That protein could be encoded by any of these " ;
            cout << rnaStrands.size() << " RNA strands: " << endl;
            cout << string(60, '-') << endl;
            break;
    }

    int count = 0;
    for (string strand : rnaStrands) {
        cout << setw(4) << right << ++count << ". " << strand << endl;
    }
}

//
// Function: permuteCodons
// Usage: permuteCodons(rnaStrands, "", protein, codonMap);
// --------------------------------------------------------
// This recursive function gathers up all the unique codon permutations
// for a given input protein and stores them as RNA strands in a set passed
// back to the caller.
//
// It uses a map of amino acid letters to codon encodings to expand
// a protein string into equivalent nucelotide sequences.
//
// To build up the strands, we iterate over the input protein string
// and permute the codons associated with each letter/amino acid
// in the protein.
//

void permuteCodons(Set<string> & rnaStrands,
                   string soFar,
                   string rest,
                   const CodonMapT & map) {
    // base case
    if (rest == "") {
        if (!rnaStrands.contains(soFar)) {
            rnaStrands.add(soFar);
        }
        return;
    }

    // recursive case
    char key = toupper(rest[0]);
    if (map.containsKey(key)) {
        for (string codon : map[key]) {
            string remaining = rest.substr(1);
            string next = soFar + codon;
            if (PRETTY_PRINT && remaining != "") {
                next = soFar + codon + "-" ;
            }
            permuteCodons(rnaStrands, next, remaining, map);
        }
    } else {
        error("permuteCodons: Encountered unknown amino acid on input: "
              + string(1, key));
    }
}
