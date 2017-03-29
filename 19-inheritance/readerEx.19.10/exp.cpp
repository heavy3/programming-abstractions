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
#include "error.h"
#include "exp.h"
#include "strlib.h"
using namespace std;

// Implementation notes: Expression
// --------------------------------
// The Expression class implements those methods that are not designated as pure virtual.

Expression::Expression() {
	// Empty
}

Expression::~Expression() {
	// Empty
}

int Expression::getConstantValue() {
	error("getConstantValue: Illegal expression type");
	return 0;
}

std::string Expression::getIdentifierName() {
	error("getIdentifierName: Illegal expression type");
	return "";
}

std::string Expression::getOp() {
	error("getOp: Illegal expression type");
	return NULL;
}

Expression* Expression::getLHS() {
	error("getLHS: Illegal expression type");
	return NULL;
}

Expression* Expression::getRHS() {
	error("getRHS: Illegal expression type");
	return NULL;
}

// Implementation notes: ConstantExp
// ---------------------------------
// The ConstantExp subclass represents an integer constant.  The eval method
// simply returns that value.

ConstantExp::ConstantExp(int value) {
	this->value = value;
}

int ConstantExp::eval(EvaluationContext& context) {
	return this->value;
}

std::string ConstantExp::toString() {
	return integerToString(value);
}

ExpressionType ConstantExp::getType() {
	return CONSTANT;
}

int ConstantExp::getConstantValue() {
	return value;
}

// Implementation notes: IdentifierExp
// -----------------------------------
// The IdentifierExp subclass represents a variable name.  The implementation
// of eval looks up that name in the evaluation context.

IdentifierExp::IdentifierExp(std::string name) {
	this->name = name;
}

int IdentifierExp::eval(EvaluationContext& context) {
	if (!context.isDefined(name)) error(name + " is undefined");
	return context.getValue(name);
}

std::string IdentifierExp::toString() {
	return name;
}

ExpressionType IdentifierExp::getType() {
	return IDENTIFIER;
}

std::string IdentifierExp::getIdentifierName() {
	return name;
}

// Implementation notes: CompoundExp
// -----------------------------------
// The CompoundExp subclass represents two expressions joined by an operator.

CompoundExp::CompoundExp(std::string op, Expression* lhs, Expression* rhs) {
	this->op = op;
	this->lhs = lhs;
	this->rhs = rhs;
}

CompoundExp::~CompoundExp() {
	delete lhs;
	delete rhs;
}

int CompoundExp::eval(EvaluationContext& context) {
	int right = rhs->eval(context);
	if (op == "=") {
		context.setValue(lhs->getIdentifierName(), right);
		return right;
	}
	int left = lhs->eval(context);
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
        if (right == 0) error("Division by 0");
        return left / right;
    }
    if (op == "%") {
        if (right == 0) error("Division by 0");
        return left % right;
    }
    error("Unknown operator: " + op);
	return 0;
}

std::string CompoundExp::toString() {
	return '(' + lhs->toString() + ' ' + op + ' ' + rhs->toString() + ')';
}

ExpressionType CompoundExp::getType() {
	return COMPOUND;
}

std::string CompoundExp::getOp() {
	return op;
}

Expression* CompoundExp::getLHS() {
	return lhs;
}

Expression* CompoundExp::getRHS() {
	return rhs;
}

// Implementation notes: EvaluationContext
// ---------------------------------------
// Thin wrapper for symbol table.

void EvaluationContext::setValue(std::string var, int value) {
	symbolTable.put(var, value);
}

int EvaluationContext::getValue(std::string var) {
	return symbolTable.get(var);
}

bool EvaluationContext::isDefined(std::string var) {
	return symbolTable.containsKey(var);
}
