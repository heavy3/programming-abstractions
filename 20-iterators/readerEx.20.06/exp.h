//
// exp.h
//
// This interface defines a class hierarchy for arithmetic expressions.
// It has been extended to support evaluation of single-variable expressions
// for specific values of the variable through a functor.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 20, Exercise 06
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This code mainly comes from Figure 19-11.
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/31/17 and 4/1/17
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)


#ifndef exp_h
#define exp_h

#include <string>
#include <iostream>
#include <cmath>
#include "map.h"
#include "set.h"
#include "tokenscanner.h"

typedef double (*doubleFn)(double);

// Forward reference

class EvaluationContext;

// Type: ExpressionType
// --------------------
// This enumerated type is used to differentiate the three different expression
// types: CONSTANT, IDENTIFIER, and COMPOUND.

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND, FUNCTION };

// Class: Expression
// -----------------
// This class is used to model a node in an expression tree.  Expression itself
// is an abstract class, which means that there are never any objects whose
// primary type is Expression.  All objects are instead created using one of the
// three concrete subclasses:
//
//    ConstantExp   -- an integer constant
//    IdentifierExp -- a string representing an identifier
//    CompoundExp   -- two expressions combined by an operator

class Expression {

public:

	// Constructor: Expression
	// -----------------------
	// Specifies the constructor for the base Expression class.  Each subclass
    // constructor will define its own constructor.

	Expression();

	// Destructor: ~Expression
	// Usage: delete exp;
	// ------------------
	// The destructor deallocates the storage for this expression.  This method
    // must be declared virtual to ensure that the correct subclass destructor
    // is called when deleting an expression.
	//
	// We make this virtual so destructors are invoked in the proper order when
    // subclassed concrete objects go out of scope.

	virtual ~Expression();

	// Method: eval
	// Usage: double val = exp->eval(context);
	// ------------------------------------
	// Evaluates the expression and returns its value in the context of an
    // encapsulated symbol table which associates values to indentifiers which
    // may show up as variables in the expression.

	virtual double eval(EvaluationContext& context) = 0;

	// Method: toString
	// Usage: string str = exp->toString();
	// ------------------------------------
	// Returns a string representation of this expression.

	virtual std::string toString() = 0;

	// Method: type
	// Usage: ExpressionType type = exp->getType();
	// --------------------------------------------
	// Returns the type of the expression, which must be one of the constants
	// CONSTANT, IDENTIFIER, COMPUND, or FUNCTION.

	virtual ExpressionType getType() = 0;

	// Getter methods for convenience
	// ------------------------------
	// The following methods get the fields of the appropriate subclass.
    // Calling these methods on an object of the wrong subclass generates an
    // error.

	virtual double getConstantValue();
	virtual std::string getIdentifierName();
	virtual std::string getOp();
	virtual Expression *getLHS();
	virtual Expression *getRHS();
    virtual Expression *getFnArg();
    virtual std::string getFnName();
    virtual void getVariables(Set<std::string>& varnames);
};

// Subclass: ConstantExp
// ---------------------
// This subclass represents an integer constant.

class ConstantExp: public Expression {

public:
	// Constructor: ConstantExp
	// Usage: Expression* exp = new ConstantExp(value);
	// ------------------------------------------------
	// The constructor creates a new integer constant expression.

	ConstantExp(double value);

	// Prototypes for virtual methods overridden in this class.

	virtual double eval(EvaluationContext& context);
	virtual std::string toString();
	virtual ExpressionType getType();
	virtual double getConstantValue();

private:
	double value;
};

// Subclass: IdentifierExp
// -----------------------
// This subclass represents an identifier (variable name) within an 
// expression.

class IdentifierExp: public Expression {

public:

	// Constructor: IdentifierExp
	// Usage: Expression* exp = new IdentifierExp(name);
	// ------------------------------------------------
	// The constructor creates an identifier expression with the specified name.

    IdentifierExp(std::string name);

	// Prototypes for virtual methods overridden in this class.

	virtual double eval(EvaluationContext& context);
	virtual std::string toString();
	virtual ExpressionType getType();
    virtual std::string getIdentifierName();
    virtual void getVariables(Set<std::string>& varnames);

private:
	std::string name;
};

// Implementation notes: FunctionExp
// -----------------------------------
// The FunctionExp subclass represents a function name.  The implementation
// of eval looks up that and evaluates the function with the current argument.

class FunctionExp: public Expression {
    
public:
    // Constructor: FunctionExp
    // Usage: Expression* exp = new FunctionExp(fnName, fnArg);
    // --------------------------------------------------------
    // The constructor creates a function expression with the given name
    // and argument expression.
    
    FunctionExp(std::string fnName, Expression* fnExpArg);
    virtual ~FunctionExp();
    virtual double eval(EvaluationContext& context);
    virtual std::string toString();
    virtual ExpressionType getType();
    virtual std::string getFnName();
    virtual Expression* getFnArg();
    virtual void getVariables(Set<std::string>& varnames);
    
private:
    std::string fnName;
    Expression* fnArg;
};

// Subclass: CompoundExp
// -----------------------
// This subclass represents an identifier (variable name) within an 
// expression.

class CompoundExp: public Expression {

public:
	
	// Constructor: CompoundExp
	// Usage: Expression* exp = new CompoundExp(op, lhs, rhs);
	// -------------------------------------------------------
	// The constructor creates a new compound expression composed of the
    // operator (op) and the left and right subexpression (lhs and rhs).

    CompoundExp(std::string op, Expression* lhs, Expression* rhs);

	// Prototypes for virtual methods overridden in this class.

	virtual ~CompoundExp();
	virtual double eval(EvaluationContext& context);
	virtual std::string toString();
	virtual ExpressionType getType();
	virtual std::string getOp();
	virtual Expression* getLHS();
	virtual Expression* getRHS();
    virtual void getVariables(Set<std::string>& varnames);

private:
	std::string op;			// The operator string (+, -, *, /)
	Expression *lhs, *rhs;	// The left and right subexpressions.
};

// Class: EvaluationContext
// ------------------------
// This class encapsulates the symbol table required for evaluation of
// generalized expressions (which may include variables).

class EvaluationContext {
public:
    EvaluationContext();
    EvaluationContext(Map<std::string, doubleFn> fnTable);
    
	// Method: setValue
	// Usage: context.setValue(var, value);
	// ------------------------------------
	// Sets the value associated with the specified var.

	void setValue(std::string var, double value);
    void setFnPtr(std::string fnName, doubleFn fnPtr);

	// Method: getValue
	// Usage: context.getValue(var);
	// -----------------------------
	// Gets the value associated with the specified var.

	double getValue(std::string var);
    
    // Method: getFnPtr
    // Usage: context.getFnPtr(fnName);
    // --------------------------------
    // Gets the function pointer associated with the specified function name.
    
    doubleFn getFnPtr(std::string fnName);
    const Map<std::string, doubleFn>& getFnTable();

	// Method: isDefined
	// Usage: if (context.isDefined(var)) . . .
	// ----------------------------------------

	bool isDefined(std::string var);
    bool isDefinedFn(std::string fnName);

private:
	Map<std::string, double> symbolTable;
    Map<std::string, doubleFn> fnTable;
};

// Class: ExpressionFunction
// -------------------------
// Turns an expression string into a function object that maps one double
// representing the value of a variable to its evaluated result.

class ExpressionFunction {
public:
    ExpressionFunction(std::string expStr);
    ~ExpressionFunction();
    
    // Operator: ()
    // Usage: ExpressionFunction fn("2 * x + 3");
    //        cout << fn(7) << endl; // should equal 17
    // ------------------------------------------------
    // Overrides the call operator so the functor may be evaluated for different
    // values of the independent variable.  Expressions should only have at most
    // one such variable, otherwise an error will be through.
    
    double operator()(double varValue);

private:
    Expression* exp;            // Points to expression tree.
    EvaluationContext context;  // Provides symbol and function table mappings.
    Set<std::string> varnames;  // Set of names teased out from the expression.
};
#endif // exp_h
