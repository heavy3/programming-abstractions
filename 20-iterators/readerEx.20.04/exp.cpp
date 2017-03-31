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
// The Expression class implements those methods that are not designated as
// pure virtual.

Expression::Expression() {
	// Empty
}

Expression::~Expression() {
	// Empty
}

double Expression::getConstantValue() {
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

Expression* Expression::getFnArg() {
    error("getFnArg: Illegal expression type");
    return NULL;
}

std::string Expression::getFnName() {
    error("getFnName: Illegal expression type");
    return "";
}

// Implementation notes: ConstantExp
// ---------------------------------
// The ConstantExp subclass represents an integer constant.  The eval method
// simply returns that value.

ConstantExp::ConstantExp(double value) {
	this->value = value;
}

double ConstantExp::eval(EvaluationContext& context) {
	return this->value;
}

std::string ConstantExp::toString() {
	return integerToString(value);
}

ExpressionType ConstantExp::getType() {
	return CONSTANT;
}

double ConstantExp::getConstantValue() {
	return value;
}

// Implementation notes: IdentifierExp
// -----------------------------------
// The IdentifierExp subclass represents a variable name.  The implementation
// of eval looks up that name in the evaluation context.

IdentifierExp::IdentifierExp(std::string name) {
	this->name = name;
}

double IdentifierExp::eval(EvaluationContext& context) {
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

// Implementation notes: FunctionExp
// -----------------------------------
// The FunctionExp subclass represents a function name.  The implementation
// of eval looks up that and evaluates the function with the current argument.

FunctionExp::FunctionExp(std::string fnName, Expression* fnArg) {
    this->fnName = fnName;
    this->fnArg = fnArg;
}

FunctionExp::~FunctionExp() {
    delete fnArg;
}

double FunctionExp::eval(EvaluationContext& context) {
    if (!context.isDefinedFn(fnName)) error(fnName + " is undefined");
    double argVal = fnArg->eval(context);
    doubleFn fn = context.getFnPtr(fnName);
    return fn(argVal);
}

std::string FunctionExp::toString() {
    return fnName + "(" + fnArg->toString() + ")";
}

ExpressionType FunctionExp::getType() {
    return FUNCTION;
}

std::string FunctionExp::getFnName() {
    return fnName;
}

Expression* FunctionExp::getFnArg() {
    return fnArg;
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

double CompoundExp::eval(EvaluationContext& context) {
	double right = rhs->eval(context);
	if (op == "=") {
		context.setValue(lhs->getIdentifierName(), right);
		return right;
	}
	double left = lhs->eval(context);
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
        if (right == 0) error("Division by 0");
        return left / right;
    }
    if (op == "%") {
        error("CompoundExp::eval: % operator not supported for type double");
        //if (right == 0) error("Division by 0");
        //return left % right;
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
// Thin wrapper for symbol table and function table.

EvaluationContext::EvaluationContext(Map<std::string, doubleFn> table):
        fnTable(table) {
    // Empty
}

void EvaluationContext::setValue(std::string var, double value) {
	symbolTable.put(var, value);
}

void EvaluationContext::setFnPtr(std::string fnName, doubleFn fnPtr) {
    fnTable.put(fnName, fnPtr);
}

double EvaluationContext::getValue(std::string var) {
	return symbolTable.get(var);
}

doubleFn EvaluationContext::getFnPtr(std::string fnName) {
    return fnTable.get(fnName);
}

bool EvaluationContext::isDefined(std::string var) {
	return symbolTable.containsKey(var);
}

bool EvaluationContext::isDefinedFn(std::string fnName) {
    return fnTable.containsKey(fnName);
}
