//
// grid.cpp
//
// This files implements a Grid class.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 11
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 3/3/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

template <typename ValueType>
Grid<ValueType>::Grid() {
    pv = new Vector<ValueType>;
    rows = 0;
    cols = 0;
}

template <typename ValueType>
Grid<ValueType>::Grid(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    pv = new Vector<ValueType>(rows * cols);
}

template <typename ValueType>
Grid<ValueType>::Grid(const Grid<ValueType> & src) {
    deepCopy(src);
}

template <typename ValueType>
Grid<ValueType> & Grid<ValueType>::operator=(const Grid<ValueType> & src) {
    if (this != &src) {
        delete pv;
        deepCopy(src);
    }
    return *this;
}

template <typename ValueType>
Grid<ValueType>::~Grid() {
    delete pv;
}
    
template <typename ValueType>
int Grid<ValueType>::numRows() const {
    return rows;
}

template <typename ValueType>
int Grid<ValueType>::numCols() const {
    return cols;
}

template <typename ValueType>
bool Grid<ValueType>::inBounds(int row, int col) const {
    return (row <= rows && col <= cols);
}

template <typename ValueType>
ValueType Grid<ValueType>::get(int row, int col) const {
    if (!inBounds(row, col))
        error("Grid<ValueType>::get: Grid[row][col] out of bounds");

    int vIndex = getIndex(row, col);
    return pv->get(vIndex);
}

template <typename ValueType>
void Grid<ValueType>::set(int row, int col, ValueType val) {
    if (!inBounds(row, col))
        error("Grid<ValueType>::set: Grid[row][col] out of bounds");

    int vIndex = getIndex(row, col);
    pv->set(vIndex, val);
}

template <typename ValueType>
int Grid<ValueType>::getIndex(int row, int col) const {
    return row * cols + col;
}

template <typename ValueType>
void Grid<ValueType>::resize(int rows, int cols) {
    if ( (rows * cols) != (this->rows * this->cols) ) {
        delete pv;
        pv = new Vector<ValueType>(rows * cols);
    } else {
        ValueType dummy;
        for (int i = 0; i < rows * cols; i++) {
            pv->set(i, dummy);
        }
    }
    this->rows = rows;
    this->cols = cols;
}

template <typename ValueType>
std::string Grid<ValueType>::toString() const {
    ostringstream os;
    for (int r = 0; r < this->rows; r++) {
        for (int c = 0; c < this->cols; c++) {
            os << setw(3) << right << this->get(r, c);
        }
        os << endl;
    }
    return os.str();
}

template <typename ValueType>
ostream & operator<<(ostream & os, const Grid<ValueType> & g) {
    return os << g.toString();
}

template <typename ValueType>
void Grid<ValueType>::deepCopy(const Grid<ValueType> & g) {
    int n = g.rows * g.cols;
    rows = g.rows;
    cols = g.cols;
    pv = new Vector<ValueType>(n);
    *pv = *(g.pv);  // Leverage deep copy inherent to Vector data type.
    
    //for (int r = 0; r < g.rows; r++) {
    //   for (int c = 0; c < g.cols; c++) {
    //       this->set(r, c, g.get(r, c));
    //   }
    //}
}
