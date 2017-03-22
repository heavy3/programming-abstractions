//
// pair.cpp
//
// This file implements the template Pair class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 3
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/5/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

template <typename T1, typename T2>
Pair<T1, T2>::Pair() { }

template <typename T1, typename T2>
Pair<T1, T2>::Pair(T1 v1, T2 v2) {
    this->v1 = v1;
    this->v2 = v2;
}

template <typename T1, typename T2>
T1 Pair<T1, T2>::first() {
    return v1;
}

template <typename T1, typename T2>
T2 Pair<T1, T2>::second() {
    return v2;
}
