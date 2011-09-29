#include "ParserExpression.h"
#include "../FormulaNodes/FormulaNode.h"

ParserExpression::ParserExpression()
{
}

ParserExpression::ParserExpression(FormulaNode* node, int _precision) : precision(_precision)
{
	node->GetHierarchyPos(pos);
}

ParserExpression::~ParserExpression()
{
}

void ParserExpression::Add(char* expr, FormulaNode* node)
{
	Add(string(expr), node);
}

void ParserExpression::Add(string& expr, FormulaNode* node)
{
	HierarchyPos p;
	node->GetHierarchyPos(p);
	//annotate the string
	annotation.push_back(AnnotationPos(expression.length(), expr.length(), p));
	
	//add the expression
	expression += expr;
}

bool ParserExpression::operator==(const HierarchyPos& _pos)
{
	return pos == _pos;
}
