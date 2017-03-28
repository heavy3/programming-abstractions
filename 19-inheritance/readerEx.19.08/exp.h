//
// exp.h
//
// This interface defines a class hierarchy for arithmetic expressions.
// It has been extended to be template based to support various numeric
// formats.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 08
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code mainly comes from Figure 19-11.
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/27/17
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)

#ifndef exp_h
#define exp_h

#include <string>
#include "map.h"
#include "tokenscanner.h"

// Forward reference

template <typename NumType>
class EvaluationContext;

// Type: ExpressionType
// --------------------
// This enumerated type is used to differentiate the three different expression
// types: CONSTANT, IDENTIFIER, and COMPOUND.

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };

// Class: Expression
// -----------------
// This class is used to mode a node in an expression tree.  Expression itself is an
// abstract class, which means that there are never any objects whose primary type is
// Expression.  All objects are instead created using one of the three concrete subclasses:
//
//    ConstantExp   -- a NumType constant
//    IdentifierExp -- a string representing an identifier
//    CompoundExp   -- two expressions combined by an operator

template <typename NumType>
class Expression {

public:

	// Constructor: Expression
	// -----------------------
	// Specifies the constructor for the base Expression class.  Each subclass constructor
	// will define its own constructor.

	Expression();

	// Destructor: ~Expression
	// Usage: delete exp;
	// ------------------
	// The destructor deallocates the storage for this expression.  This method must be
	// declared virtual to ensure that the correct subclass destructor is called when 
	// deleting an expression.
	//
	// We make this virtual so destructors are invoked in the proper order when subclassed
	// concrete objects go out of scope.

	virtual ~Expression();

	// Method: eval
	// Usage: NumType val = exp->eval(context);
	// ------------------------------------
	// Evaluates the expression and returns its value in the context of an encapsulated
	// symbol table which associates values to indentifiers which may show up as variables
	// in the expression.

	virtual NumType eval(EvaluationContext<NumType>& context) = 0;

	// Method: toString
	// Usage: string str = exp->toString();
	// ------------------------------------
	// Returns a string representation of this expression.

	virtual std::string toString() = 0;

	// Method: type
	// Usage: ExpressionType type = exp->getType();
	// --------------------------------------------
	// Returns the type of the expression, which must be one of the constants
	// CONSTANT, IDENTIFIER, or COMPOUND.

	virtual ExpressionType getType() = 0;

	// Getter methods for convenience
	// ------------------------------
	// The following methods get the fields of the appropriate subclass.  Calling
	// these methods on an object of the wrong subclass generates an error.

	virtual NumType getConstantValue();
	virtual std::string getIdentifierName();
	virtual std::string getOp();
	virtual Expression *getLHS();
	virtual Expression *getRHS();
    
    friend class ConstExp;
};

// Subclass: ConstantExp
// ---------------------
// This subclass represents a NumType constant.

template <typename NumType>
class ConstantExp: public Expression<NumType> {

public:
	// Constructor: ConstantExp
	// Usage: Expression* exp = new ConstantExp(value);
	// ------------------------------------------------
	// The constructor creates a new NumType constant expression.

	ConstantExp(NumType value);

	// Prototypes for virtual methods overridden in this class.

	virtual NumType eval(EvaluationContext<NumType>& context);
	virtual std::string toString();
	virtual ExpressionType getType();
	virtual NumType getConstantValue();

private:
	NumType value;
};

// Subclass: IdentifierExp
// -----------------------
// This subclass represents an identifier (variable name) within an 
// expression.

template <typename NumType>
class IdentifierExp: public Expression<NumType> {

public:

	// Constructor: IdentifierExp
	// Usage: Expression* exp = new IdentifierExp(name);
	// ------------------------------------------------
	// The constructor creates an identifier expression with the specified name.

    IdentifierExp(std::string name);

	// Prototypes for virtual methods overridden in this class.

	virtual NumType eval(EvaluationContext<NumType>& context);
	virtual std::string toString();
	virtual ExpressionType getType();
    virtual std::string getIdentifierName();

private:
	std::string name;
};

// Subclass: CompoundExp
// -----------------------
// This subclass represents an identifier (variable name) within an 
// expression.

template <typename NumType>
class CompoundExp: public Expression<NumType> {

public:
	
	// Constructor: CompoundExp
	// Usage: Expression* exp = new CompoundExp(op, lhs, rhs);
	// -------------------------------------------------------
	// The constructor creates a new compound expression composed of the operator
	// (op) and the left and right subexpression (lhs and rhs).

    CompoundExp(std::string op,
                Expression<NumType>* lhs,
                Expression<NumType>* rhs);

	// Prototypes for virtual methods overridden in this class.

	virtual ~CompoundExp();
	virtual NumType eval(EvaluationContext<NumType>& context);
	virtual std::string toString();
	virtual ExpressionType getType();
	virtual std::string getOp();
	virtual Expression<NumType>* getLHS();
	virtual Expression<NumType>* getRHS();

private:
	std::string op;                 // The operator string (+, -, *, /)
	Expression<NumType> *lhs, *rhs;	// The left and right subexpressions.
};

// Class: EvaluationContext
// ------------------------
// This class encapsulates the symbol table required for evaluation of
// generalized expressions (which may include variables).

template <typename NumType>
class EvaluationContext {
public:
	// Method: setValue
	// Usage: context.setValue(var, value);
	// ------------------------------------
	// Sets the value associated with the specified var.

	void setValue(std::string var, NumType value);

	// Method: getValue
	// Usage: context.getValue(var, value);
	// ------------------------------------
	// Sets the value associated with the specified var.

	NumType getValue(std::string var);

	// Method: isDefined
	// Usage: if (context.isDefined(var)) . . .
	// ----------------------------------------

	bool isDefined(std::string var);

private:
	Map<std::string, NumType> symbolTable;
};

#include "exp.cpp"
#endif // exp_h
