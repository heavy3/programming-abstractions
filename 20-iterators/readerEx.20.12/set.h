//
// set.h
//
// This iterface exports the Set class, a collection for storing a set of
// distinct elements.
//
// It has been augmented to support an optimized implementation of the
// operator== method.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 12
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// The interface and implementation code comes from Figures 17-2 through 17-4.
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 4/6/17
//

#ifndef set_h
#define set_h

#include "map.h"
#include "vector.h"

#define FAST_EQUALITY_OPERATOR

// Class: Set<ValueType>
// ---------------------
// This template class stores a collection of distinct elements

template <typename ValueType>
class Set {
public:
    
    // Constructor: Set
    // Usage: Set<ValueType> set;
    // --------------------------
    // Initializes an empty set of the specified value type.
    Set();
    
    // Destructor: ~Set
    // ----------------
    // Frees any heap storage associated with set.
    ~Set();
    
    // Method: size
    // Usage: int count = set.size();
    // ------------------------------
    // Returns the number of elements in this set.
    
    int size() const;
    
    // Method: isEmpty
    // Usage: if (set.isEmpty()) . . .
    // -------------------------------
    // Returns true if this set contains no elements.
    
    bool isEmpty() const;
    
    // Method: add
    // Usage: set.add(value)
    // ---------------------
    // Adds an element to the set if it is not already there.
    
    void add(const ValueType& value);
    
    // Method: remove
    // Usage: set.remove(value)
    // ------------------------
    // Removes an element from the set.
    
    void remove(const ValueType& value);
    
    // Method: contains
    // Usage: if (set.contains(value)) . . .
    // -------------------------------------
    // Returns true if the specified value is in this set.
    
    bool contains(const ValueType& value) const;
    
    // Method: clear
    // Usage: set.clear()
    // ------------------
    // Removes all elements from this set.
    
    void clear();
    
    // Method: isSubsetOf
    // Usage: if (set.isSubsetOf(set2)) . . .
    // --------------------------------------
    // Implements the subset relation on sets.  It returns true if every
    // element of this set is contained in set2.
    
    bool isSubset(const Set& set2) const;
    
    // Operator: ==
    // Usage: set1 == set2
    // -------------------
    // Returns true if set1 and set2 contains the same elements.
    
    bool operator==(const Set& set2) const;
    
    // Operator: !=
    // Usage: set1 != set2
    // -------------------
    // Returns true if set1 and set2 are different.
    
    bool operator!=(const Set& set2) const;
    
    // Operator: +
    // Usage: set1 + set2
    //        set1 + value
    // -------------------
    // Returns the union of sets set1 and set2, which is the set of elements
    // that appear in at least one of the two sets.  The second form returns
    // the set formed by adding a single element.
    
    Set operator+(const Set& set2) const;
    Set operator+(const ValueType& value) const;
    
    // Operator: *
    // Usage: set1 * set2
    // ------------------
    // Returns the intersection of sets set1 and set2, which is the set of all
    // elements that appear in both.
    
    Set operator*(const Set& set2) const;
    
    // Operator: -
    // Usage: set1 - set2
    //        set1 - value
    // -------------------
    // Returns the difference of sets set1 and set2, which is all of the
    // elements that appear in set1 but not set2.  The second form returns
    // the set formed by adding a single element.
    
    Set operator-(const Set& set2) const;
    Set operator-(const ValueType& value) const;
    
    // Operator: +=
    // Usage: set1 += set2;
    //        set1 += value;
    // ---------------------
    // Adds all elements from set2 (or the single specified value) to set1.
    
    Set& operator+=(const Set& set2);
    Set& operator+=(const ValueType& value);
    
    // Operator: *=
    // Usage: set1 *= set2;
    // --------------------
    // Removes any elements from set1 that are not present in set2.
    
    Set& operator*=(const Set& set2);
    
    // Operaotr: -=
    // Usage: set1 -= set2;
    //        set1 -= value;
    // ---------------------
    // Removes all elements from set2 (or single value) from set1.
    
    Set& operator-=(const Set& set2);
    Set& operator-=(const ValueType& value);
    
private:
    
    // Notes on the representation
    // ---------------------------
    // This implementation of the Set class uses a map as its underlying data
    // structure.  The value field in the map is ignored, but is
    // declared as a bool to suggest the presence or absence of a value.
    // The fact that this calss is layered on top of an existing collection
    // makes it substantially easier to implement.
    
    // Instance variables
    
    Map<ValueType, bool> map;  // Map used to store the elements.
};

// Implementation notes: Set constructor and destructor
// ----------------------------------------------------
// The constructor and destructor are empty because the Map class manages the
// underlying representation.

template <typename ValueType>
Set<ValueType>::Set() {
    // Empty
}

template <typename ValueType>
Set<ValueType>::~Set() {
    // Empty
}

// Implementation notes: size, isEmpty, add, remove, contains, clear
// -----------------------------------------------------------------
// These methods forward their operation to the underlying Map object.

template <typename ValueType>
int Set<ValueType>::size() const {
    return map.size();
}

template <typename ValueType>
bool Set<ValueType>::isEmpty() const {
    return map.isEmpty();
}

template <typename ValueType>
void Set<ValueType>::add(const ValueType& value) {
    map[value] = true;
}

template <typename ValueType>
void Set<ValueType>::remove(const ValueType& value) {
    map.remove(value);
}

template <typename ValueType>
bool Set<ValueType>::contains(const ValueType& value) const {
    return map.containsKey(value);
}

template <typename ValueType>
void Set<ValueType>::clear() {
    map.clear();
}

// Implementation notes: isSubset
// ------------------------------
// This method simply checks to see whether each element of the current
// set is an element of the second.

template <typename ValueType>
bool Set<ValueType>::isSubset(const Set& set2) const {
    for (ValueType value: map) {
        if (!set2.contains(value)) return false;
    }
    return true;
}

// Implementation notes: operator==, operator!=
// --------------------------------------------
// These operators make use of the fact that two sets are equal only if
// each set is a subset of the other.

#ifdef FAST_EQUALITY_OPERATOR

// Optimized implementation.

template <typename ValueType>
bool Set<ValueType>::operator==(const Set& set2) const {
    bool result = true;
    
    typename Map<ValueType, bool>::iterator it1 = map.begin();
    typename Map<ValueType, bool>::iterator it2 = set2.map.begin();
    
    for (; it1 != map.end(); it1++, it2++) {
        if (it2 == set2.map.end() || *it1 != *it2) return false;
    }
    return result;
}

#else

// Original implementation.

template <typename ValueType>
bool Set<ValueType>::operator==(const Set& set2) const {
    return isSubset(set2) && set2.isSubset(*this);
}

#endif

template <typename ValueType>
bool Set<ValueType>::operator!=(const Set& set2) const {
    return !(*this == set2);
}

// Implementation notes: operator+
// -------------------------------
// The union operator copies the first set and then adds the elements
// from the second to it.

template <typename ValueType>
Set<ValueType> Set<ValueType>::operator+(const Set& set2) const {
    Set<ValueType> set = *this;
    for (ValueType value: set2.map) {
        set.add(value);
    }
    return set;
}

template <typename ValueType>
Set<ValueType> Set<ValueType>::operator+(const ValueType& value) const {
    Set<ValueType> set = *this;
    set.add(value);
    return set;
}

// Implementation notes: operator*
// -------------------------------
// The intersection operator adds elements to an empty set only if they
// appear in both sets.

template <typename ValueType>
Set<ValueType> Set<ValueType>::operator*(const Set& set2) const {
    Set<ValueType> set;
    for (ValueType value: map) {
        if (set2.contains(value)) set.add(value);
    }
    return set;
}

// Implementation notes: operator-
// -------------------------------
// The set difference operator adds elements from the first set only if
// they do -not- appear in the second.

template <typename ValueType>
Set<ValueType> Set<ValueType>::operator-(const Set& set2) const {
    Set<ValueType> set;
    for (ValueType value: map) {
        if (!set2.contains(value)) set.add(value);
    }
    return set;
}

template <typename ValueType>
Set<ValueType> Set<ValueType>::operator-(const ValueType& value) const {
    Set<ValueType> set = *this;
    set.remove(value);
    return set;
}

// Implementation notes: shorthand assignment operators
// ----------------------------------------------------
// These operators modify the current set but are otherwise similar to
// the operators that create new sets.  The only subtlety is that the
// intersection operator must create a vector of elements to remove to avoid
// changing the set while cycling through its elements.

template <typename ValueType>
Set<ValueType>& Set<ValueType>::operator+=(const Set& set2) {
    for (ValueType value: set2.map) {
        add(value);
    }
    return *this;
}

template <typename ValueType>
Set<ValueType>& Set<ValueType>::operator+=(const ValueType& value) {
    add(value);
    return *this;
}

template <typename ValueType>
Set<ValueType>& Set<ValueType>::operator*=(const Set& set2) {
    Vector<ValueType> toRemove;
    for (ValueType value: map) {
        if (!set2.contains(value)) toRemove.add(value);
    }
    for (ValueType value: toRemove) {
        remove(value);
    }
    return *this;
}

template <typename ValueType>
Set<ValueType>& Set<ValueType>::operator-=(const Set& set2) {
    for (ValueType value: set2.map) {
        remove(value);
    }
    return *this;
}

template <typename ValueType>
Set<ValueType>& Set<ValueType>::operator-=(const ValueType& value) {
    remove(value);
    return *this;
}

#endif // set_h
