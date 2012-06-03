#include "ParserExpression.h"
#include "../FormulaNodes/FormulaNode.h"

//ParserString

/**
 * Default constructor.
 */
ParserString::ParserString()
{
}

/**
 * Destructor.
 */
ParserString::~ParserString()
{
}

/**
 * Adds an expression corresponding to a node.
 * @param [in] expr The expression.
 * @param [in] node The node.
 */
void ParserString::Add(char* expr, FormulaNode* node)
{
	Add(string(expr), node);
}

/**
 * Adds an expression corresponding to a node.
 * @param [in] expr The expression.
 * @param [in] node The node.
 */
void ParserString::Add(string& expr, FormulaNode* node)
{
	HierarchyPos p;
	node->GetHierarchyPos(p);
	//annotate the string
	annotation.push_back(AnnotationPos(expression.length(), expr.length(), p));
	
	//add the expression
	expression += expr;
}

/**
 * = casting operator.
 * @param source Source for the parser string.
 */
void ParserString::operator=(const ParserString& source)
{
	expression = source.expression;
	annotation = source.annotation;
}

bool ParserString::operator==(const ParserString& expr)
{
	return expression == expr.expression && annotation == expr.annotation;
}

//ParserExpression

/**
 * Default constructor.
 */
ParserExpression::ParserExpression()
{
	solved = false;
}

ParserExpression::ParserExpression(const FormulaNode* node)
{
	node->GetHierarchyPos(pos);
}

void ParserExpression::UpdatePos(const FormulaNode* node) const
{
	pos.clear();
	node->GetHierarchyPos(pos);
}

/**
 * Destructor.
 */
ParserExpression::~ParserExpression()
{
}

/**
 * == casting operator.
 * @param expr The expression.
 * @return The result of the operation.
 */
bool ParserExpression::operator==(const ParserExpression& expr)
{
	return pos == expr.pos;
}

//RealParserExpression

RealParserExpression::RealParserExpression() : precision(0), exp(0)
{
}

RealParserExpression::RealParserExpression(FormulaNode* node, int _precision, int _exp) : ParserExpression(node), precision(_precision), exp(_exp)
{
}

bool RealParserExpression::operator==(const RealParserExpression& expr) const
{
	return pos == expr.pos && precision == expr.precision && exp == expr.exp;
}

//IntegerParserExpression

IntegerParserExpression::IntegerParserExpression() : notation(DECIMAL_NOTATION)
{
}

IntegerParserExpression::IntegerParserExpression(FormulaNode* node, ExpressionNotation _notation) : ParserExpression(node), notation(_notation)
{
}

bool IntegerParserExpression::operator==(const IntegerParserExpression& expr) const
{
	return pos == expr.pos && notation == expr.notation;
}

//RationalParserExpression

RationalParserExpression::RationalParserExpression()
{
}

RationalParserExpression::RationalParserExpression(FormulaNode* node, FractionType _fractionType) : ParserExpression(node), fractionType(_fractionType)
{
}

bool RationalParserExpression::operator==(const RationalParserExpression& expr) const
{
	return pos == expr.pos && fractionType == expr.fractionType;
}

//AutoParserExpression

AutoParserExpression::AutoParserExpression()
{
}

AutoParserExpression::AutoParserExpression(FormulaNode* node, int _precision, int _exp, ExpressionNotation _notation, FractionType _fractionType) : 
	ParserExpression(node), precision(_precision), exp(_exp), notation(_notation), fractionType(_fractionType)
{
}

bool AutoParserExpression::operator==(const AutoParserExpression& expr) const
{
	return pos == expr.pos && precision == expr.precision && exp == expr.exp;
}

bool AutoParserExpression::ToReal(RealParserExpression& expr)
{
	try
	{
		Real res = boost::get<Real>(result);
		expr.expression = expression;
		expr.exception = exception;
		expr.exp = exp;
		expr.pos = pos;
		expr.precision = precision;
		expr.result = res;
		expr.solved = solved;
		return true;
	}
	catch (boost::bad_get)
	{
	}
	
	return false;
}

bool AutoParserExpression::ToInteger(IntegerParserExpression& expr)
{
	try
	{
		Integer res = boost::get<Integer>(result);
		expr.expression = expression;
		expr.exception = exception;
		expr.pos = pos;
		expr.result = res;
		expr.notation = notation;
		expr.solved = solved;
		return true;
	}
	catch (boost::bad_get)
	{
	}
	
	return false;
}

bool AutoParserExpression::ToRational(RationalParserExpression& expr)
{
	try
	{
		Rational res = boost::get<Rational>(result);
		expr.expression = expression;
		expr.exception = exception;
		expr.pos = pos;
		expr.result = res;
		expr.fractionType = fractionType;
		expr.solved = solved;
		return true;
	}
	catch (boost::bad_get)
	{
	}
	
	return false;
}
