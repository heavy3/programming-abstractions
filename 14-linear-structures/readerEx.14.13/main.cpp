//
// main.cpp
//
// This program builds a table of factorials to illustrate the use of an
// extended precision class called BigInt which uses a linked-list to
// represent large, non-negative integers.
//
// Motivation
//
// How many unique orderings are there in a deck of 52 cards?
//
// If you've been exposed to some discrete mathematics or some stat,
// you might respond:
//
//    52!
//
// But just -how- big is this number and how might you actually go about
// computing it?
//
// This program tells you the answer:
//
// 52! = 80658175170943878571660636856403766975289505440883277824000000000000
//
// That's a big number, and you can't generally compute it directly with the
// built-in data types offered up by most computing platforms without quickly
// succumbing to an overflow situation.
//
// Extended precision to the rescue!
//
// It turns out we can model large integers as linked-lists of a bazillion
// individual digits.  The only issue is the effort to craft member functions
// and operators that understand how to mathematically manipulate numbers with
// this representation.
//
// In the current program, we take a stab at two very simple operators,
// addition and multiplication.  Furthermore, we restrict our fu to just
// positive numbers since that's all we really need to get at the tantalizing
// question of 52!.
//
// Example output:
//
//    CS106B Programming Abstractions in C++: Ex 14.13
//    Extended precision BigInt Class
//
//    Factorials of n
//    ------------------------------------------------------------------------
//     n  n!
//    ------------------------------------------------------------------------
//     0  1
//     1  1
//     2  2
//     3  6
//     4  24
//     5  120
//     6  720
//     7  5040
//     8  40320
//     9  362880
//    10  3628800
//    11  39916800
//    12  479001600
//    13  6227020800
//    14  87178291200
//    15  1307674368000
//    16  20922789888000
//    17  355687428096000
//    18  6402373705728000
//    19  121645100408832000
//    20  2432902008176640000
//    21  51090942171709440000
//    22  1124000727777607680000
//    23  25852016738884976640000
//    24  620448401733239439360000
//    25  15511210043330985984000000
//    26  403291461126605635584000000
//    27  10888869450418352160768000000
//    28  304888344611713860501504000000
//    29  8841761993739701954543616000000
//    30  265252859812191058636308480000000
//    31  8222838654177922817725562880000000
//    32  263130836933693530167218012160000000
//    33  8683317618811886495518194401280000000
//    34  295232799039604140847618609643520000000
//    35  10333147966386144929666651337523200000000
//    36  371993326789901217467999448150835200000000
//    37  13763753091226345046315979581580902400000000
//    38  523022617466601111760007224100074291200000000
//    39  20397882081197443358640281739902897356800000000
//    40  815915283247897734345611269596115894272000000000
//    41  33452526613163807108170062053440751665152000000000
//    42  1405006117752879898543142606244511569936384000000000
//    43  60415263063373835637355132068513997507264512000000000
//    44  2658271574788448768043625811014615890319638528000000000
//    45  119622220865480194561963161495657715064383733760000000000
//    46  5502622159812088949850305428800254892961651752960000000000
//    47  258623241511168180642964355153611979969197632389120000000000
//    48  12413915592536072670862289047373375038521486354677760000000000
//    49  608281864034267560872252163321295376887552831379210240000000000
//    50  30414093201713378043612608166064768844377641568960512000000000000
//    51  1551118753287382280224243016469303211063259720016986112000000000000
//    52  80658175170943878571660636856403766975289505440883277824000000000000
//    ------------------------------------------------------------------------
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

#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include "error.h"
#include "BigInt.h"

using namespace std;

// Constants

const std::string HEADER = "CS106B Programming Abstractions in C++: Ex 14.13\n";
const std::string DETAIL = "Extended precision BigInt Class";
const std::string BANNER = HEADER + DETAIL;
const int MAX_N = 52;

// Prototypes

void showFactorialTable(int maxN);

// Main program

int main() {
    cout << BANNER << endl << endl;
    showFactorialTable(MAX_N);
    return 0;
}

// Function: showFactorialTable
// Usage: showFactorialTable(52);
// ------------------------------
// Will produce a table of factorials of n from 0 to some max specified value.

void showFactorialTable(int maxN) {
    cout << "Factorials of n" << endl;
    cout << std::string(72, '-') << endl;
    cout << " n  n!" << endl;
    cout << std::string(72, '-') << endl;
    
    int seed = 1;
    BigInt prevfact(seed);
    
    // Use extended precision operands and operators to avoid overflow
    // when computing the factorial of ridiculously large integers.
    
    for (int n = 0; n <= MAX_N; n++) {
        BigInt fact = prevfact * BigInt(max(1, n));
        cout << setw(2) << right << n << "  " << fact << endl;
        prevfact = fact;
    }
    cout << std::string(72, '-') << endl;
}
