//
// IntArray.cpp
//
// This file implements a simple, heap-based integer array class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 12, Exercise 4
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 7/12/16.
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

#include "IntArray.h"

const std::string IntArray::E_OUT_OF_BOUNDS = "Array index is outside the bounds of the array.";

IntArray::IntArray(int n) {
    capacity = n;
    array = new int[capacity];
    for (int i = 0; i < capacity; i++) array[i] = 0;
}

IntArray::~IntArray() {
    delete[] array;
}

int IntArray::size() const {
    return capacity;
}

int IntArray::get(int k) const {
    if (k >= capacity) error(E_OUT_OF_BOUNDS);
    return array[k];
}

void IntArray::put(int k, int value) {
    if (k >= capacity) error(E_OUT_OF_BOUNDS);
    array[k] = value;
}
