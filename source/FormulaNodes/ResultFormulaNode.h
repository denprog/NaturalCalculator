#ifndef RESULTFORMULANODE_H
#define RESULTFORMULANODE_H

#include "GroupFormulaNode.h"
#include "../ParserThread/ParserExpression.h"

class ResultFormulaNode : public GroupFormulaNode
{
public:
	ResultFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, FormulaNode* expressionNode);
	virtual ~ResultFormulaNode();

public:
	virtual void Remake();
	
private:
	//int precision;
	int exp;
	//ParserExpression parserExpression;
	HierarchyPos expressionPos;
};

#endif
