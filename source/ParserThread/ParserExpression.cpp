#include "ParserExpression.h"
#include "../FormulaNodes/FormulaNode.h"

//ParserString

ParserString::ParserString()
{
}

ParserString::~ParserString()
{
}

void ParserString::Add(char* expr, FormulaNode* node)
{
	Add(string(expr), node);
}

void ParserString::Add(string& expr, FormulaNode* node)
{
	HierarchyPos p;
	node->GetHierarchyPos(p);
	//annotate the string
	annotation.push_back(AnnotationPos(expression.length(), expr.length(), p));
	
	//add the expression
	expression += expr;
}

void ParserString::operator=(const ParserString& source)
{
	expression = source.expression;
	annotation = source.annotation;
}

//ParserExpression

ParserExpression::ParserExpression()
{
	//precision = 0;
	//exp = 0;
	//type = AUTO;
	solved = false;
}

ParserExpression::ParserExpression(FormulaNode* node)
{
}

//ParserExpression::ParserExpression(FormulaNode* node, ExpressionType _type, int _precision, int _exp) : type(_type)//, precision(_precision), exp(_exp)
//{
//	node->GetHierarchyPos(pos);
//	SetParam("precision", _precision);
//	SetParam("exp", _exp);
//	solved = false;
//}

ParserExpression::~ParserExpression()
{
}

bool ParserExpression::operator==(const ParserExpression& expr)
{
	//return pos == expr.pos && precision == expr.precision && type == expr.type;
	//for (ParamsIter iter = params.begin(); iter != params.end(); ++iter)
	//{
	//	if ((*iter).second != expr.params[(*iter).first])
	//		return false;
	//}
	
	return pos == expr.pos;
}

//void ParserExpression::SetParam(char* name, int p)
//{
//	params[name] = p;
//}
//
//template<>
//int ParserExpression::GetParam<int>(char* name)
//{
//	return any_cast<int>(params[name]);
//}

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

AutoParserExpression::AutoParserExpression(FormulaNode* node, int _precision, int _exp) : ParserExpression(node), precision(_precision), exp(_exp)
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
