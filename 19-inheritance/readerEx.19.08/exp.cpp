//
// exp.cpp
//
// This file implements the exp.h interface.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 07
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code mainly comes from Figure 19-13.
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/27/17
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)

#include <string>
#include <ostream>
#include "error.h"
#include "exp.h"
#include "strlib.h"
using namespace std;

// Implementation notes: Expression
// --------------------------------
// The Expression class implements those methods that are not designated as pure virtual.

template <typename NumType>
Expression<NumType>::Expression() {
	// Empty
}

template <typename NumType>
Expression<NumType>::~Expression() {
	// Empty
}

template <typename NumType>
NumType Expression<NumType>::getConstantValue() {
	error("getConstantValue: Illegal expression type");
	return 0;
}

template <typename NumType>
std::string Expression<NumType>::getIdentifierName() {
	error("getIdentifierName: Illegal expression type");
	return "";
}

template <typename NumType>
std::string Expression<NumType>::getOp() {
	error("getOp: Illegal expression type");
	return NULL;
}

template <typename NumType>
Expression<NumType>* Expression<NumType>::getLHS() {
	error("getLHS: Illegal expression type");
	return NULL;
}

template <typename NumType>
Expression<NumType>* Expression<NumType>::getRHS() {
	error("getRHS: Illegal expression type");
	return NULL;
}

// Implementation notes: ConstantExp
// ---------------------------------
// The ConstantExp subclass represents an NumType constant.  The eval method
// simply returns that value.

template <typename NumType>
ConstantExp<NumType>::ConstantExp(NumType value) {
	this->value = value;
}

template <typename NumType>
NumType ConstantExp<NumType>::eval(EvaluationContext<NumType>& context) {
	return this->value;
}

template <typename NumType>
std::string ConstantExp<NumType>::toString() {
    ostringstream os;
    os << value;
	return os.str();
}

template <typename NumType>
ExpressionType ConstantExp<NumType>::getType() {
	return CONSTANT;
}

template <typename NumType>
NumType ConstantExp<NumType>::getConstantValue() {
	return value;
}

// Implementation notes: IdentifierExp
// -----------------------------------
// The IdentifierExp subclass represents a variable name.  The implementation
// of eval looks up that name in the evaluation context.

template <typename NumType>
IdentifierExp<NumType>::IdentifierExp(std::string name) {
	this->name = name;
}

template <typename NumType>
NumType IdentifierExp<NumType>::eval(EvaluationContext<NumType>& context) {
	if (!context.isDefined(name)) error(name + " is undefined");
	return context.getValue(name);
}

template <typename NumType>
std::string IdentifierExp<NumType>::toString() {
	return name;
}

template <typename NumType>
ExpressionType IdentifierExp<NumType>::getType() {
	return IDENTIFIER;
}

template <typename NumType>
std::string IdentifierExp<NumType>::getIdentifierName() {
	return name;
}

// Implementation notes: CompoundExp
// -----------------------------------
// The CompoundExp subclass represents two expressions joined by an operator.

template <typename NumType>
CompoundExp<NumType>::CompoundExp(std::string op,
                                  Expression<NumType>* lhs,
                                  Expression<NumType>* rhs) {
	this->op = op;
	this->lhs = lhs;
	this->rhs = rhs;
}

template <typename NumType>
CompoundExp<NumType>::~CompoundExp() {
	delete lhs;
	delete rhs;
}

template <typename NumType>
NumType CompoundExp<NumType>::eval(EvaluationContext<NumType>& context) {
	NumType right = rhs->eval(context);
	if (op == "=") {
		context.setValue(lhs->getIdentifierName(), right);
		return right;
	}
	NumType left = lhs->eval(context);
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
        if (right == 0) error("Division by 0");
        return left / right;
    }
#if 0   // Should use runtime type identification here to enable for
        // ints and longs (but not floating point).
    if (op == "%") {
        if (right == 0) error("Division by 0");
        return left % right;
    }
#endif
    error("Unknown operator: " + op);
	return 0;
}

template <typename NumType>
std::string CompoundExp<NumType>::toString() {
	return '(' + lhs->toString() + ' ' + op + ' ' + rhs->toString() + ')';
}

template <typename NumType>
ExpressionType CompoundExp<NumType>::getType() {
	return COMPOUND;
}

template <typename NumType>
std::string CompoundExp<NumType>::getOp() {
	return op;
}

template <typename NumType>
Expression<NumType>* CompoundExp<NumType>::getLHS() {
	return lhs;
}

template <typename NumType>
Expression<NumType>* CompoundExp<NumType>::getRHS() {
	return rhs;
}

// Implementation notes: EvaluationContext
// ---------------------------------------
// Thin wrapper for symbol table.

template <typename NumType>
void EvaluationContext<NumType>::setValue(std::string var, NumType value) {
	symbolTable.put(var, value);
}

template <typename NumType>
NumType EvaluationContext<NumType>::getValue(std::string var) {
	return symbolTable.get(var);
}

template <typename NumType>
bool EvaluationContext<NumType>::isDefined(std::string var) {
	return symbolTable.containsKey(var);
}
