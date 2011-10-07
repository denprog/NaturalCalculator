#ifndef PARSEREXPRESSION_H
#define PARSEREXPRESSION_H

#include <string>
#include <boost/variant.hpp>
#include <boost/any.hpp>
#include <map>
#include "Integer.h"
#include "Real.h"
#include "Rational.h"
#include "../Editor/CaretState.h"

using namespace std;
using namespace BigNumbersParser;

class FormulaNode;

enum ExpressionType
{
	AUTO = 1, 
	INTEGER, 
	REAL, 
	RATIONAL
};

enum ExpressionNotation
{
	BINARY_NOTATION = 1, 
	OCTAL_NOTATION, 
	DECIMAL_NOTATION, 
	HEXADECIMAL_NOTATION
};

enum FractionType
{
	PROPER_FRACTION = 1, 
	IMPROPER_FRACTION
};

struct ParserString
{
	ParserString();
	~ParserString();

	void Add(char* expr, FormulaNode* node);
	void Add(string& expr, FormulaNode* node);

	void operator=(const ParserString& source);
		
	struct AnnotationPos
	{
		AnnotationPos(int _pos, int _length, HierarchyPos _hierarchyPos) : pos(_pos), length(_length), hierarchyPos(_hierarchyPos)
		{
		}
		
		int pos;
		int length;
		HierarchyPos hierarchyPos;
	};
	
	string expression; //expression to be solved
	vector<AnnotationPos> annotation; //for having possibility of finding error's position
};

struct ParserExpression
{
	ParserExpression();
	ParserExpression(FormulaNode* node);
	~ParserExpression();
	
	bool operator==(const ParserExpression& expr);
	
	HierarchyPos pos; //position of the node, which created this object
	ParserException exception; //a possible solving exception
	ParserString expression;
	bool solved;
};

struct RealParserExpression : ParserExpression
{
	RealParserExpression();
	RealParserExpression(FormulaNode* node, int _precision, int _exp);

	bool operator==(const RealParserExpression& expr) const;
	
	int precision;
	int exp;
	
	Real result;
};

struct IntegerParserExpression : ParserExpression
{
	IntegerParserExpression();
	IntegerParserExpression(FormulaNode* node, ExpressionNotation _notation);

	bool operator==(const IntegerParserExpression& expr) const;
	
	ExpressionNotation notation;
	
	Integer result;
};

struct RationalParserExpression : ParserExpression
{
	RationalParserExpression();
	RationalParserExpression(FormulaNode* node, FractionType _fractionType);

	bool operator==(const RationalParserExpression& expr) const;
	
	FractionType fractionType;
	
	Rational result;
};

struct AutoParserExpression : ParserExpression
{
	AutoParserExpression();
	AutoParserExpression(FormulaNode* node, int _precision, int _exp);

	bool operator==(const AutoParserExpression& expr) const;

	bool ToReal(RealParserExpression& expr);
	
	int precision;
	int exp;
	
	boost::variant<Integer, Real, Rational> result;
};

//typedef boost::variant<RealParserExpression, IntegerParserExpression, AutoParserExpression> ParserExpressionVar;

struct ParserExpressionVariant
{
	ParserExpressionVariant(AutoParserExpression& expr)
	{
		var = expr;
	}

	ParserExpressionVariant(RealParserExpression& expr)
	{
		var = expr;
	}
	
	ParserExpressionVariant(IntegerParserExpression& expr)
	{
		var = expr;
	}

	ParserExpressionVariant(RationalParserExpression& expr)
	{
		var = expr;
	}

	bool operator==(const ParserExpressionVariant& _var) const
	{
		return var == _var.var;
	}
	
	struct ParserStringVisitor : boost::static_visitor<const ParserString*>
	{
		const ParserString* operator()(RealParserExpression const& expr) const
		{
			return &expr.expression;
		}
		
		const ParserString* operator()(IntegerParserExpression const& expr) const
		{
			return &expr.expression;
		}

		const ParserString* operator()(RationalParserExpression const& expr) const
		{
			return &expr.expression;
		}

		const ParserString* operator()(AutoParserExpression const& expr) const
		{
			return &expr.expression;
		}
	};

	struct SolvedVisitor : boost::static_visitor<const bool*>
	{
		const bool* operator()(RealParserExpression const& expr) const
		{
			return &expr.solved;
		}
		
		const bool* operator()(IntegerParserExpression const& expr) const
		{
			return &expr.solved;
		}

		const bool* operator()(RationalParserExpression const& expr) const
		{
			return &expr.solved;
		}

		const bool* operator()(AutoParserExpression const& expr) const
		{
			return &expr.solved;
		}
	};
	
	ParserString* GetExpression()
	{
		return (ParserString*)boost::apply_visitor(ParserStringVisitor(), var);
	}

	bool* GetSolved()
	{
		return (bool*)boost::apply_visitor(ParserStringVisitor(), var);
	}
	
	boost::variant<RealParserExpression, IntegerParserExpression, RationalParserExpression, AutoParserExpression> var;
};

#endif
